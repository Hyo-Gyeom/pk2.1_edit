namespace City_hp_up
{
    const int KEY = pk::hash("기교내구상승");  

    class cityInfo
    {
        int city_id = -1;
        bool has_hp_boost = false;
        int base_max_hp = 0; // ?? 도시의 원래 최대 내구도를 저장
    };

    class Main
    {
        array<cityInfo> _cityInfos;

        Main()
        {
            pk::bind(102, pk::trigger102_t(Init));
            pk::bind(105, pk::trigger105_t(SaveData));
        }

        void Init()
        {
            LoadData(); // 저장된 내구도 상승 기록을 불러옴
            auto current_cities = pk::list_to_array(pk::get_city_list());

            for (int i = 0; i < int(current_cities.length); i++)
            {
                pk::city@ city = current_cities[i];
                pk::building@ building = pk::city_to_building(city);
                pk::force@ force = pk::get_force(building.get_force_id());
                
                if (!pk::is_alive(building) || !pk::is_alive(force))
                    continue;

                // ?? 1. 저장된 cityInfo를 찾습니다.
                cityInfo@ info = null;
                for (int j = 0; j < _cityInfos.length; j++)
                {
                    if (_cityInfos[j].city_id == city.get_id())
                    {
                        info = _cityInfos[j];
                        break;
                    }
                }
                
                bool already_recorded = (info != null);
                
                // ?? 2. Base HP를 결정하고 city.max_hp를 초기화합니다.
                int current_base_hp = city.max_hp;
                
                if (already_recorded && info.base_max_hp > 0)
                {
                    // 저장된 Base HP가 있으면 그 값을 내구도 재계산의 기준으로 사용
                    current_base_hp = info.base_max_hp;
                }
                
                // 내구도를 기본 내구도 값으로 되돌려 세력 변경 시 재상승 문제를 방지합니다.
                city.max_hp = current_base_hp; 

                
                // ?? 3. 현재 세력의 기술에 맞춰 내구도를 계산 및 적용합니다.
                float temp_hp = float(city.max_hp);
                bool applied = false;

                if (pk::has_tech(force, 기교_공병육성)) // 외부 상수 참조
                {
                    temp_hp += 300;
                    applied = true;
                }
                
                if (pk::has_tech(force, 기교_시설강화)) // 외부 상수 참조
                {
                    temp_hp += 300;
                    applied = true;
                }
                if (pk::has_tech(force, 기교_방어강화)) // 외부 상수 참조
                {
                    temp_hp += 300;
                    applied = true;
                }

		if (pk::has_tech(force, 기교_석조건축)) // 외부 상수 참조
                {
                    temp_hp += 1000;
                    applied = true;
                }
		
                // 계산된 값을 적용하고 상한치를 처리합니다.
                city.max_hp = int(temp_hp + 0.5f); // 반올림 적용
                
                if (city.max_hp > 9999)
                {
                    city.max_hp = 9999; // ? 내구도 상한치 적용
                }

                // ?? 4. 정보 기록 또는 업데이트
                if (!already_recorded)
                {
                    // 최초 적용: 기본 내구도와 함께 기록합니다.
                    cityInfo new_info;
                    new_info.city_id = city.get_id();
                    new_info.has_hp_boost = applied;
                    new_info.base_max_hp = current_base_hp; // 원래 값 저장
                    _cityInfos.insertLast(new_info);
                }
                else
                {
                    // 이미 기록된 경우, base_max_hp가 0이었다면 현재 max_hp를 저장합니다.
                    if (info.base_max_hp == 0)
                        info.base_max_hp = current_base_hp;
                        
                    // 현재 기술 적용 여부만 업데이트 (선택 사항)
                    info.has_hp_boost = applied;
                }
            }
        }

        void LoadData()
        {
            // 배열을 비움
            while (_cityInfos.length > 0)
            {
                _cityInfos.removeLast();
            }

            int count = int(pk::load(KEY, 0, 0));
            int li = 1;
            for (int i = 0; i < count; i++)
            {
                cityInfo ui;
                ui.city_id = int(pk::load(KEY, li++, 0));
                ui.has_hp_boost = (int(pk::load(KEY, li++, 0)) == 1);
                ui.base_max_hp = int(pk::load(KEY, li++, 0)); // ?? base_max_hp 로드
                _cityInfos.insertLast(ui);
            }
        }

        void SaveData(int file_id)
        {
            // 게임 내 현재 도시 목록 기준으로 데이터 업데이트 (도시 해산 대비)
            array<cityInfo> temp_infos;
            auto current_cities = pk::list_to_array(pk::get_city_list());

            for (int i = 0; i < int(current_cities.length); i++)
            {
                pk::city@ city = current_cities[i];
                cityInfo info;
                info.city_id = city.get_id();
                
                // 기존 _cityInfos에서 정보 찾아와서 업데이트
                for (int j = 0; j < _cityInfos.length; j++)
                {
                    if (_cityInfos[j].city_id == city.get_id())
                    {
                        info.has_hp_boost = _cityInfos[j].has_hp_boost;
                        info.base_max_hp = _cityInfos[j].base_max_hp; // ?? base_max_hp 저장
                        break;
                    }
                }
                temp_infos.insertLast(info);
            }
            _cityInfos = temp_infos;

            // 데이터 저장
            pk::store(KEY, 0, _cityInfos.length);
            int si = 1;
            for (int i = 0; i < _cityInfos.length; i++)
            {
                cityInfo@ ui = _cityInfos[i];
                pk::store(KEY, si++, ui.city_id);
                pk::store(KEY, si++, ui.has_hp_boost ? 1 : 0);
                pk::store(KEY, si++, ui.base_max_hp); // ?? base_max_hp 저장
            }
        }
    }
    Main main;
}