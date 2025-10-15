namespace City_hp_up
{
    const int KEY = pk::hash("�ⱳ�������");  

    class cityInfo
    {
        int city_id = -1;
        bool has_hp_boost = false;
        int base_max_hp = 0; // ?? ������ ���� �ִ� �������� ����
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
            LoadData(); // ����� ������ ��� ����� �ҷ���
            auto current_cities = pk::list_to_array(pk::get_city_list());

            for (int i = 0; i < int(current_cities.length); i++)
            {
                pk::city@ city = current_cities[i];
                pk::building@ building = pk::city_to_building(city);
                pk::force@ force = pk::get_force(building.get_force_id());
                
                if (!pk::is_alive(building) || !pk::is_alive(force))
                    continue;

                // ?? 1. ����� cityInfo�� ã���ϴ�.
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
                
                // ?? 2. Base HP�� �����ϰ� city.max_hp�� �ʱ�ȭ�մϴ�.
                int current_base_hp = city.max_hp;
                
                if (already_recorded && info.base_max_hp > 0)
                {
                    // ����� Base HP�� ������ �� ���� ������ ������ �������� ���
                    current_base_hp = info.base_max_hp;
                }
                
                // �������� �⺻ ������ ������ �ǵ��� ���� ���� �� ���� ������ �����մϴ�.
                city.max_hp = current_base_hp; 

                
                // ?? 3. ���� ������ ����� ���� �������� ��� �� �����մϴ�.
                float temp_hp = float(city.max_hp);
                bool applied = false;

                if (pk::has_tech(force, �ⱳ_��������)) // �ܺ� ��� ����
                {
                    temp_hp += 300;
                    applied = true;
                }
                
                if (pk::has_tech(force, �ⱳ_�ü���ȭ)) // �ܺ� ��� ����
                {
                    temp_hp += 300;
                    applied = true;
                }
                if (pk::has_tech(force, �ⱳ_��ȭ)) // �ܺ� ��� ����
                {
                    temp_hp += 300;
                    applied = true;
                }

		if (pk::has_tech(force, �ⱳ_��������)) // �ܺ� ��� ����
                {
                    temp_hp += 1000;
                    applied = true;
                }
		
                // ���� ���� �����ϰ� ����ġ�� ó���մϴ�.
                city.max_hp = int(temp_hp + 0.5f); // �ݿø� ����
                
                if (city.max_hp > 9999)
                {
                    city.max_hp = 9999; // ? ������ ����ġ ����
                }

                // ?? 4. ���� ��� �Ǵ� ������Ʈ
                if (!already_recorded)
                {
                    // ���� ����: �⺻ �������� �Բ� ����մϴ�.
                    cityInfo new_info;
                    new_info.city_id = city.get_id();
                    new_info.has_hp_boost = applied;
                    new_info.base_max_hp = current_base_hp; // ���� �� ����
                    _cityInfos.insertLast(new_info);
                }
                else
                {
                    // �̹� ��ϵ� ���, base_max_hp�� 0�̾��ٸ� ���� max_hp�� �����մϴ�.
                    if (info.base_max_hp == 0)
                        info.base_max_hp = current_base_hp;
                        
                    // ���� ��� ���� ���θ� ������Ʈ (���� ����)
                    info.has_hp_boost = applied;
                }
            }
        }

        void LoadData()
        {
            // �迭�� ���
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
                ui.base_max_hp = int(pk::load(KEY, li++, 0)); // ?? base_max_hp �ε�
                _cityInfos.insertLast(ui);
            }
        }

        void SaveData(int file_id)
        {
            // ���� �� ���� ���� ��� �������� ������ ������Ʈ (���� �ػ� ���)
            array<cityInfo> temp_infos;
            auto current_cities = pk::list_to_array(pk::get_city_list());

            for (int i = 0; i < int(current_cities.length); i++)
            {
                pk::city@ city = current_cities[i];
                cityInfo info;
                info.city_id = city.get_id();
                
                // ���� _cityInfos���� ���� ã�ƿͼ� ������Ʈ
                for (int j = 0; j < _cityInfos.length; j++)
                {
                    if (_cityInfos[j].city_id == city.get_id())
                    {
                        info.has_hp_boost = _cityInfos[j].has_hp_boost;
                        info.base_max_hp = _cityInfos[j].base_max_hp; // ?? base_max_hp ����
                        break;
                    }
                }
                temp_infos.insertLast(info);
            }
            _cityInfos = temp_infos;

            // ������ ����
            pk::store(KEY, 0, _cityInfos.length);
            int si = 1;
            for (int i = 0; i < _cityInfos.length; i++)
            {
                cityInfo@ ui = _cityInfos[i];
                pk::store(KEY, si++, ui.city_id);
                pk::store(KEY, si++, ui.has_hp_boost ? 1 : 0);
                pk::store(KEY, si++, ui.base_max_hp); // ?? base_max_hp ����
            }
        }
    }
    Main main;
}