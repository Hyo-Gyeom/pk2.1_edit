/*
// 제작자: 
// 설명: 재야 및 미발견 무장을 랜덤하게 도시에 재배치
// 세력당 도시 1개인 경우에만 쓰길 권장
// 적용시나리오 번호 필수 확인
// Update: 2025.11.06 / 초기 버전 작성
*/

namespace 재야무장재배치
{
    //---------------------------------------------------------------------------------------
    // 설정변수
    const bool 재배치_활성화 = true;           // true: 기능 활성화, false: 기능 비활성화
    const bool 재야무장_재배치 = true;         // true: 재야 무장 재배치, false: 제외
    const bool 미발견무장_재배치 = true;       // true: 미발견 무장 재배치, false: 제외
    const bool 미발견을_재야로_변경 = true;    // true: 미발견 무장을 재야로 변경, false: 미발견 그대로 유지
    const int  적용시나리오 = 15;              // -1: 모든 시나리오, 0~N: 특정 시나리오 번호만
    const bool 선택확인 = true;                // true: 선택창 표시, false: 자동 실행
    const bool 최소_도시_무장_수_고려 = true;         // true: 세력별 기존 무장 수 고려하여 적은 곳에 우선 배치, false: 균등 배치
    const int  최소_도시_무장_수 = 8;          // 각 세력이 최소한 가져야 할 무장 수 (군주 + 일반)    
    const int  제외_군주_무장번호 = 794;        // 이 무장이 군주인 세력은 배치 제외 (-1: 제외 안함, 예) 군주 무장번호)

    //---------------------------------------------------------------------------------------
    
    class Main
    {
        Main()
        {
            pk::bind(102, 10, pk::trigger102_t(callback));  // 우선순위 10 (낮음) - 세력재배치 후에 실행
        }
        
        void callback()
        {
            if (pk::get_scenario().loaded) return;  // 세이브 파일 불러올 때는 실행 안함
            if (!재배치_활성화) return;            
            
            // 특정 시나리오만 적용 (적용시나리오가 -1이 아닌 경우)
            if (적용시나리오 != -1 && pk::get_scenario().no != 적용시나리오) return;
            
            // 선택 확인
            if (선택확인)
            {
                if (pk::choose(pk::u8encode("재야/미발견 무장을 재배치 하시겠습니까?"), 
                    { pk::u8encode("예"), pk::u8encode("아니오") }) == 1) 
                    return;
            }
            
            // 재야 및 미발견 무장 목록 가져오기
            pk::list<pk::person@> person_list;
            
            if (재야무장_재배치)
            {
                pk::list<pk::person@> zaiya_list = pk::get_person_list(pk::mibun_flags(신분_재야));
                for (int i = 0; i < zaiya_list.count; i++)
                    person_list.add(zaiya_list[i]);
            }
            
            if (미발견무장_재배치)
            {
                pk::list<pk::person@> mihakken_list = pk::get_person_list(pk::mibun_flags(신분_미발견));
                for (int i = 0; i < mihakken_list.count; i++)
                    person_list.add(mihakken_list[i]);
            }
            
            if (person_list.count == 0)
            {
                pk::message_box(pk::u8encode("재배치할 무장이 없습니다."));
                return;
            }
            
            // 무장 목록 섞기
            person_list.shuffle();
            
            int deployed_count = 0;  // 배치된 무장 수
            int person_index = 0;     // 현재 배치할 무장 인덱스
            
            if (최소_도시_무장_수_고려)
            {
                // 1단계: 도시를 돌면서 최소 무장 수 미만인 도시에 배치
                for (int i = 0; i < 도시_끝; i++)
                {
                    pk::city@ city = pk::get_city(i);
                    if (!pk::is_alive(city)) continue;
                    
                    pk::building@ building = pk::city_to_building(city);
                    int force_id = building.get_force_id();
                    if (force_id == -1) continue;
                    
                    pk::force@ force = pk::get_force(force_id);
                    if (!pk::is_alive(force) || !pk::is_normal_force(force)) continue;
                    
                    // 제외_군주_무장번호가 설정되어 있으면 해당 무장이 군주인 세력 제외
                    if (제외_군주_무장번호 != -1)
                    {
                        pk::person@ kunshu = pk::get_person(force.kunshu);
                        if (kunshu.get_id() == 제외_군주_무장번호)
                            continue;
                    }
                    
                    // 현재 도시의 무장 수 계산 (군주 + 일반만)
                    pk::list<pk::person@> existing_persons = pk::get_person_list(building, pk::mibun_flags(신분_군주, 신분_일반));
                    int current_count = existing_persons.count;
                    
                    // 최소 무장 수 미만이면 무장 배치
                    while (current_count < 최소_도시_무장_수 && person_index < person_list.count)
                    {
                        pk::person@ person = person_list[person_index];
                        person_index++;
                        
                        if (!pk::is_alive(person)) continue;
                        
                        int building_id = building.get_id();
                        person.location = building_id;
                        person.service = building_id;
                        
                        // 신분 설정 (미발견 -> 재야로 변경 옵션)
                        if (person.mibun == 신분_미발견 && 미발견을_재야로_변경)
                            person.mibun = 신분_재야;
                        
                        current_count++;
                        deployed_count++;
                    }
                }
            }
            
            // 2단계: 남은 무장을 순환 배치 (제외 군주 세력 패스)
            for (int i = person_index; i < person_list.count; i++)
            {
                pk::person@ person = person_list[i];
                if (!pk::is_alive(person)) continue;
                
                // 도시 순환하면서 배치 (제외 세력 건너뛰기)
                for (int j = 0; j < 도시_끝; j++)
                {
                    int city_id = (i + j) % 도시_끝;
                    pk::city@ city = pk::get_city(city_id);
                    if (!pk::is_alive(city)) continue;
                    
                    pk::building@ building = pk::city_to_building(city);
                    int force_id = building.get_force_id();
                    if (force_id == -1) continue;
                    
                    pk::force@ force = pk::get_force(force_id);
                    if (!pk::is_alive(force) || !pk::is_normal_force(force)) continue;
                    
                    // 제외_군주_무장번호 세력 제외
                    if (제외_군주_무장번호 != -1)
                    {
                        pk::person@ kunshu = pk::get_person(force.kunshu);
                        if (kunshu.get_id() == 제외_군주_무장번호)
                            continue;
                    }
                    
                    // 배치
                    int building_id = building.get_id();
                    person.location = building_id;
                    person.service = building_id;
                    
                    // 신분 설정 (미발견 -> 재야로 변경 옵션)
                    if (person.mibun == 신분_미발견 && 미발견을_재야로_변경)
                        person.mibun = 신분_재야;
                    
                    deployed_count++;
                    break;  // 배치 완료
                }
            }
            
            pk::message_box(pk::u8encode(pk::format("{}명의 무장을 재배치했습니다.", deployed_count)));
        }
    }
    
    Main main;
}

