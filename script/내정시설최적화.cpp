/*
// 제작자: 
// 설명  : 도시의 내정시설을 최적화하여 자동으로 건설하는 스크립트
// Update: 2025.11.25 / 초기 버전 작성
// 기능  : 도시의 빈 개발지를 찾아 우선순위에 따라 내정시설 건설
//        병영, 시장, 농장, 대장간, 마구간 등을 자동으로 건설
*/

namespace 내정시설최적화
{
    //=======================================================================================
    // 설정 변수

    // 적용 대상 세력 (0: AI + 위임군단, 1: AI만)
    const int 적용_대상_세력 = 0;

    // 시설 건설 최소 금액
    const int 최소_건설금액 = 300;

    // 시장 농장을 제외한 시설 기반 우선순위 배열 - 최소 우선순위 시설은 7개 추천(마지막 7번째는 개발지가 20이면 지음) - 시장과 농장은 3개까지는 유지 저절로 지어짐
    array<int> 우선순위_시설_배열 = {
        시설_병영, 시설_대시장, 시설_어시장, 시설_마구간, 시설_군사부, 시설_대장간, 시설_군둔농
    };

    // 농장과 시장 최소 개수 (이 값 건들지 말 것! - 자리 부족 시 이 개수 초과일 때만 자리를 뺏을 수 있음)
    const int 최소_농장개수 = 3;
    const int 최소_시장개수 = 3; 

    // 행동력 최대치 255적용
    const bool행동력_최대치_적용_여부 = true;
    //=======================================================================================

    class Main
    {
        pk::func256_t@ prev_callback_256;
        pk::func256_t@ prev_callback_257;
        int saved_district_ap_;  // 저장된 district 행동력
        bool cmd_handled_;  // 내 스크립트가 명령을 처리했는지 여부
        
        // 0: 조폐, 1~6: 시장 후보, 03: 곡창, 1~6: 농장 후보
        pk::array<pk::array<pk::point>> cached_priority_building_positions;  // 도시 ID별 우선순위 건물 좌표 슬롯
        pk::array<pk::array<pk::point>> cached_mint_market_positions;  // 도시 ID별 조폐/시장 좌표 슬롯
        pk::array<pk::array<pk::point>> cached_granary_farm_positions;  // 도시 ID별 곡창/농장 좌표 슬롯

        Main()
        {   
            pk::bind(102, pk::trigger102_t(on_game_start));
            pk::bind(161, pk::trigger161_t(on_building_destroy));
            
            @prev_callback_256 = cast<pk::func256_t@>(pk::get_func(256));
            pk::reset_func(256);
            pk::set_func(256, pk::func256_t(callback_256));
            
            @prev_callback_257 = cast<pk::func256_t@>(pk::get_func(257));
            pk::reset_func(257);
            pk::set_func(257, pk::func256_t(callback_257));

            cached_priority_building_positions.length = 건물_도시끝;
            cached_mint_market_positions.length = 건물_도시끝;
            cached_granary_farm_positions.length = 건물_도시끝;
        }

        void on_game_start()
        {
            pk::info("게임 시작 - 캐시 갱신");
            initialize_contact_cache();
        }

        // 건물 파괴 시 호출 (169번 트리거)
        void on_building_destroy(pk::building@ building, int type)
        {
            if (!pk::is_alive(building)) return;
            
            // 도시 건물만 처리
            int city_id = pk::get_city_id(building.get_pos());
            if (city_id < 0 or city_id >= 건물_도시끝) return;
            
            pk::city@ city = pk::get_city(city_id);
            if (!pk::is_alive(city)) return;
            
            pk::point destroyed_pos = building.get_pos();
            pk::info(pk::format("건물 파괴 감지 - 도시: {}, 시설: {}, 좌표: ({},{})", 
                pk::decode(pk::get_name(city)), building.facility, destroyed_pos.x, destroyed_pos.y));
            
            // 캐시 초기화
            cached_priority_building_positions[city_id].length = 우선순위_시설_배열.length;
            for (int j = 0; j < int(우선순위_시설_배열.length); j++)
                cached_priority_building_positions[city_id][j] = pk::point(-1, -1);

            cached_mint_market_positions[city_id].length = 7;
            for (int j = 0; j < 7; j++)
                cached_mint_market_positions[city_id][j] = pk::point(-1, -1);

            cached_granary_farm_positions[city_id].length = 7;
            for (int j = 0; j < 7; j++)
                cached_granary_farm_positions[city_id][j] = pk::point(-1, -1);
            
            // 해당 도시의 캐시 재계산
            cache_city_contact_positions(city, destroyed_pos);
        }
                
        // 캐시를 모든 도시에 대해 초기화하여 접촉 정보 준비
        void initialize_contact_cache()
        {   
            pk::info("캐시 저장 시작");
            
            for (int i = 0; i < 건물_도시끝; i++)
            {
                cached_priority_building_positions[i].length = 우선순위_시설_배열.length;
                for (int j = 0; j < int(우선순위_시설_배열.length); j++)
                    cached_priority_building_positions[i][j] = pk::point(-1, -1);

                cached_mint_market_positions[i].length = 7;                
                for (int j = 0; j < 7; j++)
                    cached_mint_market_positions[i][j] = pk::point(-1, -1);

                cached_granary_farm_positions[i].length = 7;
                for (int j = 0; j < 7; j++)
                    cached_granary_farm_positions[i][j] = pk::point(-1, -1);
            
                pk::city@ city = pk::get_city(i);
                cache_city_contact_positions(city);
            }
        }

        // 한 도시의 개발지를 순회하며 조폐/곡창 후보 위치를 계산하고 예약
        // 도시 개발지를 순회하여 캐시된 조폐/곡창 후보 위치를 다시 초기화
        void cache_city_contact_positions(pk::city@ city, pk::point destroyed_pos = pk::point(-1, -1))
        {
            int city_id = city.get_id();
            pk::info(pk::format("cache_city_contact_positions 호출 - destroyed_pos: ({},{})", destroyed_pos.x, destroyed_pos.y));
         
            // 이미 지어진 건물 확인 (완성되었거나 건설 중인 것 포함)
            pk::point mint_pos = pk::point(-1, -1);
            pk::point granary_pos = pk::point(-1, -1);
            pk::array<pk::point> empty_devs;
            
            for (int dev_idx = 0; dev_idx < city.max_devs; dev_idx++)
            {
                pk::building@ building = city.dev[dev_idx].building;
                pk::point dev_pos = city.dev[dev_idx].pos;
                
                // 부서진 자리는 아직 건물이 있는 걸로 취급되므로 강제로 빈 자리로 추가
                if (!pk::is_alive(building))
                {
                    // 빈 개발지 좌표 저장
                    empty_devs.insertLast(dev_pos);
                    pk::info(pk::format("빈 개발지: ({},{})", dev_pos.x, dev_pos.y));
                }
                else if (destroyed_pos != pk::point(-1,-1) && dev_pos.x == destroyed_pos.x && dev_pos.y == destroyed_pos.y)
                {
                    // 부서진 건물 좌표 강제 추가
                    empty_devs.insertLast(dev_pos);
                    pk::info(pk::format("부서진 건물 강제 추가: ({},{})", dev_pos.x, dev_pos.y));
                }
                else
                {
                    int facility = building.facility;
                    pk::info(pk::format("건물 있음: ({},{}) - 시설: {}", dev_pos.x, dev_pos.y, facility));
                    
                    // 부서진 건물은 무시
                    if (destroyed_pos != pk::point(-1,-1) && dev_pos.x == destroyed_pos.x && dev_pos.y == destroyed_pos.y)
                    {
                        pk::info(pk::format("부서진 건물 무시: ({},{}) - 시설: {}", dev_pos.x, dev_pos.y, facility));
                    }
                    else
                    {
                        if (facility == 시설_조폐)
                            mint_pos = dev_pos;
                        else if (facility == 시설_곡창)
                            granary_pos = dev_pos;
                    }
                }
            }

            pk::info(pk::format("조폐 위치: ({},{}), 곡창 위치: ({},{})", mint_pos.x, mint_pos.y, granary_pos.x, granary_pos.y));

            // 우상 정렬
            pk::array<pk::point> sorted_dev_indices = build_dev_point_order(empty_devs);
            
            // 조폐 처리: 조폐가 있으면 주변에만 시장 배치, 없으면 최적 위치 찾기
            if (mint_pos != pk::point(-1, -1))
                find_existing_facility_neighbors(city, sorted_dev_indices, mint_pos, 시설_시장, destroyed_pos);
            else
                find_max_development_contact_pos(city, sorted_dev_indices, 시설_시장, destroyed_pos);

            // 도시의 개발지에서 시장 및 조폐가 예약된 자리는 제외
            for (int i = 0; i < 7; i++)
            {
                for (int j = 0; j < int(sorted_dev_indices.length); j++)
                {
                    if (cached_mint_market_positions[city_id][i].x == sorted_dev_indices[j].x and cached_mint_market_positions[city_id][i].y == sorted_dev_indices[j].y)
                    {
                        sorted_dev_indices.removeAt(j);
                        break;                        
                    }
                }
            }            

            // 곡창 처리: 곡창이 있으면 주변에만 농장 배치, 없으면 최적 위치 찾기
            if (granary_pos != pk::point(-1, -1))
                find_existing_facility_neighbors(city, sorted_dev_indices, granary_pos, 시설_농장, destroyed_pos);
            else
                find_max_development_contact_pos(city, sorted_dev_indices, 시설_농장, destroyed_pos);

            // 도시의 개발지에서 농장 및 곡창이 예약된 자리는 제외
            for (int i = 0; i < 7; i++)
            {
                for (int j = 0; j < int(sorted_dev_indices.length); j++)
                {
                    if (cached_granary_farm_positions[city_id][i].x == sorted_dev_indices[j].x and cached_granary_farm_positions[city_id][i].y == sorted_dev_indices[j].y)
                    {
                        sorted_dev_indices.removeAt(j);
                        break;                        
                    }
                }
            }

            // 우선순위 시설 존재 여부 확인 및 시장/농장 개수 파악
            array<bool> has_priority;
            has_priority.length = 우선순위_시설_배열.length;
            for (int j = 0; j < int(우선순위_시설_배열.length); j++)
            {
                has_priority[j] = is_building_in_city(city_id, 우선순위_시설_배열[j], destroyed_pos);
            }
            
            int existing_market_count = count_mar_fam_in_city(city_id, 시설_시장);
            int existing_farm_count = count_mar_fam_in_city(city_id, 시설_농장);
            
            // 우선 시설 배열 캐쉬에 저장
            for (int i = 0; i < int(우선순위_시설_배열.length); i++)
            {
                // 이미 해당 우선순위 시설이 있으면 패스
                if (has_priority[i])
                {
                    cached_priority_building_positions[city_id][i] = pk::point(-1, -1);
                    pk::info(pk::format("우선순위 시설 {} 이미 존재, 패스", i));
                    continue;
                }
                
                // 대도시 아니면 대시장 -1
                bool isLargeCity = city.max_troops == 150000;
                if (우선순위_시설_배열[i] == 시설_대시장 and !isLargeCity)
                {
                    cached_priority_building_positions[city_id][i] = pk::point(-1, -1);
                    continue;
                }                

                // 항구도시 아니면 어시장 -1
                bool is_port_city = false;
                for (int j = 0; j < 5; j++)
                {
                    int gate_id = city.gate[j];
                    if (gate_id >= 건물_항구시작 and gate_id < 건물_항구끝)
                    {
                        is_port_city = true;
                        continue;
                    }
                }                
                if (우선순위_시설_배열[i] == 시설_어시장 and !is_port_city) 
                {
                    cached_priority_building_positions[city_id][i] = pk::point(-1, -1);
                    continue;
                }

                // 남는 빈 자리가 있으면 채우기
                if (sorted_dev_indices.length > 0)
                {
                    int rand_index = pk::rand(sorted_dev_indices.length);
                    cached_priority_building_positions[city_id][i] = sorted_dev_indices[rand_index];
                    sorted_dev_indices.removeAt(rand_index);
                    pk::info(pk::format("우선순위 시설 {} 빈 자리 배치", i));
                    continue;
                }
                
                // 빈 자리가 없으면 농장 3개 초과 시 농장 자리 뺏기
                int fam_count = existing_farm_count + cache_count(city_id, 시설_농장);
                if (fam_count > 3)
                {
                    for (int j = 1; j < int(cached_granary_farm_positions[city_id].length); j++)
                    {
                        if (cached_granary_farm_positions[city_id][j] != pk::point(-1, -1))
                        {
                            cached_priority_building_positions[city_id][i] = cached_granary_farm_positions[city_id][j];
                            cached_granary_farm_positions[city_id][j] = pk::point(-1, -1);
                            pk::info(pk::format("우선순위 시설 {} 농장 자리 뺏음", i));
                            break;
                        }
                    }
                    continue;
                }

                // 농장이 정확히 3개면 곡창 자리 뺏기
                if (fam_count == 3 and cached_granary_farm_positions[city_id][0] != pk::point(-1, -1))
                {
                    cached_priority_building_positions[city_id][i] = cached_granary_farm_positions[city_id][0];
                    cached_granary_farm_positions[city_id][0] = pk::point(-1, -1);
                    pk::info(pk::format("우선순위 시설 {} 곡창 자리 뺏음", i));
                    continue;
                }

                // 시장 3개 초과 시 시장 자리 뺏기 (마지막 우선순위 제외)
                int mar_count = existing_market_count + cache_count(city_id, 시설_시장);
                if (mar_count > 3 and i != 6)
                {
                    for (int j = 1; j < int(cached_mint_market_positions[city_id].length); j++)
                    {
                        if (cached_mint_market_positions[city_id][j] != pk::point(-1, -1))
                        {
                            cached_priority_building_positions[city_id][i] = cached_mint_market_positions[city_id][j];
                            cached_mint_market_positions[city_id][j] = pk::point(-1, -1);
                            pk::info(pk::format("우선순위 시설 {} 시장 자리 뺏음", i));
                            break;
                        }
                    }
                    continue;
                }

                // 시장이 정확히 3개면 조폐 자리 뺏기
                if (mar_count == 3 and cached_mint_market_positions[city_id][0] != pk::point(-1, -1))
                {
                    cached_priority_building_positions[city_id][i] = cached_mint_market_positions[city_id][0];
                    cached_mint_market_positions[city_id][0] = pk::point(-1, -1);
                    pk::info(pk::format("우선순위 시설 {} 조폐 자리 뺏음", i));
                    continue;
                }
                
                // 그 외는 -1
                cached_priority_building_positions[city_id][i] = pk::point(-1, -1);
            }
            
            // 캐시 재계산 완료 - 전체 캐시 출력
            pk::info("========== 캐시된 시장 배열 좌표 ==========");
            for (int i = 0; i < int(cached_mint_market_positions[city_id].length); i++)
            {
                pk::info(pk::format("cached_mint_market_positions[{}]: ({},{})", i, cached_mint_market_positions[city_id][i].x, cached_mint_market_positions[city_id][i].y));
            }
            
            pk::info("========== 캐시된 농장 배열 좌표 ==========");
            for (int i = 0; i < int(cached_granary_farm_positions[city_id].length); i++)
            {
                pk::info(pk::format("cached_granary_farm_positions[{}]: ({},{})", i, cached_granary_farm_positions[city_id][i].x, cached_granary_farm_positions[city_id][i].y));
            }
            
            pk::info("========== 캐시된 우선순위 시설 배열 좌표 ==========");
            for (int i = 0; i < int(cached_priority_building_positions[city_id].length); i++)
            {
                pk::info(pk::format("cached_priority_building_positions[{}]: ({},{})", i, cached_priority_building_positions[city_id][i].x, cached_priority_building_positions[city_id][i].y));
            }
        }

        // 개발지 주소를 우측 상위 우선순위로 정렬하여 반환
        pk::array<pk::point> build_dev_point_order(pk::array<pk::point> dev_positions)
        {
            pk::array<pk::point> sorted_dev_indices = dev_positions;

            for (int i = 0; i < int(sorted_dev_indices.length) - 1; i++)
            {
                int best_index = i;
                for (int j = i + 1; j < int(sorted_dev_indices.length); j++)
                {
                    if (is_point_more_right_top(sorted_dev_indices[j], sorted_dev_indices[best_index]))
                        best_index = j;
                }
                if (best_index != i)
                {
                    pk::point tmp = sorted_dev_indices[i];
                    sorted_dev_indices[i] = sorted_dev_indices[best_index];
                    sorted_dev_indices[best_index] = tmp;
                }
            }

            return sorted_dev_indices;
        }

        bool is_point_more_right_top(const pk::point& a, const pk::point& b)
        {
            if (a.x > b.x)
                return true;
            if (a.x < b.x)
                return false;
            return a.y < b.y;
        }

        // 접촉수 기준으로 최적의 위치를 찾아 캐시에 저장
        void find_max_development_contact_pos(pk::city@ city, pk::array<pk::point> sorted_dev_indices, int facility_id, pk::point destroyed_pos = pk::point(-1,-1))
        {
            int city_id = city.get_id();
            pk::info(pk::format("find_max_development_contact_pos 호출 - facility: {}, 빈 자리 개수: {}", facility_id, sorted_dev_indices.length));
            
            int max_contact_count = 0;
            pk::point temp_center_pos = pk::point(-1, -1);
            pk::array<pk::point> contact_array;
            contact_array.length = 7;
            for (int i = 0; i < 7; i++)
                contact_array[i] = pk::point(-1, -1);
            pk::array<pk::point> best_contact_array;
            best_contact_array.length = 7;
            for (int i = 0; i < 7; i++)
                best_contact_array[i] = pk::point(-1, -1);
            pk::point best_center_pos = pk::point(-1, -1);
            bool best_assigned = false;

            // 모든 개발지를 순회 (농장/시장과 빈 자리가 많이 접촉하는 곳 찾기)
            for (int dev_idx = 0; dev_idx < city.max_devs; dev_idx++)
            {
                temp_center_pos = city.dev[dev_idx].pos;
                contact_array = count_adjacent_contacts(city, sorted_dev_indices, temp_center_pos, facility_id);
                
                int contact_count = 0;
                for (int idx = 0; idx < int(contact_array.length); idx++)
                {
                    if (contact_array[idx].x != -1 and contact_array[idx].y != -1)
                        contact_count++;
                }

                if (!best_assigned || contact_count > max_contact_count)
                {
                    max_contact_count = contact_count;
                    best_center_pos = temp_center_pos;
                    best_contact_array = contact_array;
                    best_assigned = true;
                }
                if (contact_count == 6)
                    break;
            }

            // 캐쉬 2중 배열에 저장 (빈 자리만)
            if (facility_id == 시설_시장 or facility_id == 시설_시장2단 or facility_id == 시설_시장3단)
            {
                if (best_center_pos.x < 0)
                    return;
                
                pk::info(pk::format("조폐 최적 위치 찾음: ({},{})", best_center_pos.x, best_center_pos.y));
                
                // 조폐 자리가 빈 자리인지 확인
                int center_dev_idx = find_dev_index_by_pos(city, best_center_pos);
                // 부서진 자리거나 빈 자리면 OK
                if (center_dev_idx >= 0 && ((destroyed_pos != pk::point(-1,-1) && best_center_pos.x == destroyed_pos.x && best_center_pos.y == destroyed_pos.y) || !pk::is_alive(city.dev[center_dev_idx].building)))
                {
                    cached_mint_market_positions[city_id][0] = best_center_pos;  // 조폐 자리
                    pk::info(pk::format("조폐 자리[0]: ({},{}) - 빈 자리", best_center_pos.x, best_center_pos.y));
                }
                else
                {
                    cached_mint_market_positions[city_id][0] = pk::point(-1, -1);  // 건물 있으면 -1
                    pk::info("조폐 자리[0]: (-1,-1) - 건물 있음");
                }
                
                // 조폐 자리(best_center_pos) 주변 6칸에서 빈 자리만 찾기
                int idx = 1;
                for (int direction = 0; direction < 방향_끝; direction++)
                {
                    pk::point neighbor_pos = pk::get_neighbor_pos(best_center_pos, direction);
                    if (!pk::is_valid_pos(neighbor_pos))
                        continue;
                    
                    int neighbor_index = find_dev_index_by_pos(city, neighbor_pos);
                    if (neighbor_index < 0)
                        continue;
                    
                    pk::building@ neighbor_building = city.dev[neighbor_index].building;
                    // 빈 자리이거나 부서진 자리면 시장 자리로 저장
                    if (!pk::is_alive(neighbor_building) || (destroyed_pos != pk::point(-1,-1) && neighbor_pos.x == destroyed_pos.x && neighbor_pos.y == destroyed_pos.y))
                    {
                        cached_mint_market_positions[city_id][idx] = neighbor_pos;
                        pk::info(pk::format("시장 자리[{}]: ({},{})", idx, neighbor_pos.x, neighbor_pos.y));
                        idx++;
                        if (idx >= 7) break;
                    }
                }
            }
            else if (facility_id == 시설_농장 or facility_id == 시설_농장2단 or facility_id == 시설_농장3단)
            {
                if (best_center_pos.x < 0)
                    return;
                
                pk::info(pk::format("곡창 최적 위치 찾음: ({},{})", best_center_pos.x, best_center_pos.y));
                
                // 곡창 자리가 빈 자리인지 확인
                int center_dev_idx = find_dev_index_by_pos(city, best_center_pos);
                // 부서진 자리거나 빈 자리면 OK
                if (center_dev_idx >= 0 && ((destroyed_pos != pk::point(-1,-1) && best_center_pos.x == destroyed_pos.x && best_center_pos.y == destroyed_pos.y) || !pk::is_alive(city.dev[center_dev_idx].building)))
                {
                    cached_granary_farm_positions[city_id][0] = best_center_pos;  // 곡창 자리
                    pk::info(pk::format("곡창 자리[0]: ({},{}) - 빈 자리", best_center_pos.x, best_center_pos.y));
                }
                else
                {
                    cached_granary_farm_positions[city_id][0] = pk::point(-1, -1);  // 건물 있으면 -1
                    pk::info("곡창 자리[0]: (-1,-1) - 건물 있음");
                }
                
                // 곡창 자리(best_center_pos) 주변 6칸에서 빈 자리만 찾기
                int idx = 1;
                for (int direction = 0; direction < 방향_끝; direction++)
                {
                    pk::point neighbor_pos = pk::get_neighbor_pos(best_center_pos, direction);
                    if (!pk::is_valid_pos(neighbor_pos))
                        continue;
                    
                    int neighbor_index = find_dev_index_by_pos(city, neighbor_pos);
                    if (neighbor_index < 0)
                        continue;
                    
                    pk::building@ neighbor_building = city.dev[neighbor_index].building;
                    // 빈 자리이거나 부서진 자리면 농장 자리로 저장
                    if (!pk::is_alive(neighbor_building) || (destroyed_pos != pk::point(-1,-1) && neighbor_pos.x == destroyed_pos.x && neighbor_pos.y == destroyed_pos.y))
                    {
                        cached_granary_farm_positions[city_id][idx] = neighbor_pos;
                        pk::info(pk::format("농장 자리[{}]: ({},{})", idx, neighbor_pos.x, neighbor_pos.y));
                        idx++;
                        if (idx >= 7) break;
                    }
                }
            }
        }

        // 이미 존재하는 시설(조폐/곡창) 주변에만 시장/농장 배치
        void find_existing_facility_neighbors(pk::city@ city, pk::array<pk::point> sorted_dev_indices, pk::point existing_pos, int target_facility, pk::point destroyed_pos = pk::point(-1,-1))
        {
            int city_id = city.get_id();
            
            if (existing_pos.x < 0) return;  // 위치가 없음
            
            pk::info(pk::format("기존 시설 발견: ({},{}), 주변에 배치", existing_pos.x, existing_pos.y));
            
            // 주변 6칸에서 빈 자리만 찾기
            pk::array<pk::point> empty_neighbors;
            for (int direction = 0; direction < 방향_끝; direction++)
            {
                pk::point neighbor_pos = pk::get_neighbor_pos(existing_pos, direction);
                if (!pk::is_valid_pos(neighbor_pos))
                    continue;
                
                int neighbor_index = find_dev_index_by_pos(city, neighbor_pos);
                if (neighbor_index < 0)
                    continue;
                
                pk::building@ neighbor_building = city.dev[neighbor_index].building;
                // 빈 자리이거나 부서진 자리면 추가
                if (!pk::is_alive(neighbor_building) || (destroyed_pos != pk::point(-1,-1) && neighbor_pos.x == destroyed_pos.x && neighbor_pos.y == destroyed_pos.y))
                {
                    empty_neighbors.insertLast(neighbor_pos);
                }
            }
            
            // 캐시에 저장
            pk::info(pk::format("주변 빈 자리 개수: {}", empty_neighbors.length));
            
            if (target_facility == 시설_시장)
            {
                cached_mint_market_positions[city_id][0] = pk::point(-1, -1);  // 조폐는 이미 있으니 -1
                for (int i = 0; i < int(empty_neighbors.length) && i < 6; i++)
                {
                    cached_mint_market_positions[city_id][i + 1] = empty_neighbors[i];
                    pk::info(pk::format("시장 자리[{}]: ({},{})", i+1, empty_neighbors[i].x, empty_neighbors[i].y));
                }
            }
            else if (target_facility == 시설_농장)
            {
                cached_granary_farm_positions[city_id][0] = pk::point(-1, -1);  // 곡창은 이미 있으니 -1
                for (int i = 0; i < int(empty_neighbors.length) && i < 6; i++)
                {
                    cached_granary_farm_positions[city_id][i + 1] = empty_neighbors[i];
                    pk::info(pk::format("농장 자리[{}]: ({},{})", i+1, empty_neighbors[i].x, empty_neighbors[i].y));
                }
            }
        }
        
        // 중심 개발지 주변 6칸에서 빈 칸 또는 동일 시설 접촉 좌표를 수집
        pk::array<pk::point> count_adjacent_contacts(pk::city@ city, pk::array<pk::point> sorted_dev_indices, pk::point center_pos, int facility_id)
        {
            pk::array<pk::point> contact_count;
            contact_count.length = 7;
            for (int j = 0; j < 7; j++)
                contact_count[j] = pk::point(-1, -1);

            // 빈 개발지 또는 facility_id에 해당하는 접촉 개발지 수 파악
            for (int direction = 0; direction < 방향_끝; direction++)
            {
                pk::point neighbor_pos = pk::get_neighbor_pos(center_pos, direction);
                if (!pk::is_valid_pos(neighbor_pos))
                    continue;
                
                int neighbor_index = find_dev_index_by_pos(city, neighbor_pos);
                if (neighbor_index < 0)
                    continue;

                pk::building@ neighbor_building = city.dev[neighbor_index].building;
                if (!pk::is_alive(neighbor_building))
                {
                    // 빈 자리면 추가
                    contact_count[direction] = neighbor_pos;
                }
                else
                {
                    int neighbor_facility = neighbor_building.facility;
                    if (facility_id == 시설_시장 and (neighbor_facility == 시설_시장3단 or neighbor_facility == 시설_시장2단 or neighbor_facility == 시설_시장))
                        contact_count[direction] = neighbor_pos;
                    else if (facility_id == 시설_농장 and (neighbor_facility == 시설_농장3단 or neighbor_facility == 시설_농장2단 or neighbor_facility == 시설_농장))
                        contact_count[direction] = neighbor_pos;
                }
            }
            return contact_count;
        }

        // 해당 좌표를 가진 개발지 인덱스를 찾는다
        int find_dev_index_by_pos(pk::city@ city, const pk::point& pos)
        {
            for (int i = 0; i < int(city.max_devs); i++)
            {
                if (city.dev[i].pos.x == pos.x and city.dev[i].pos.y == pos.y)
                    return i;
            }
            return -1;
        }





        // 건물을 지으려고 할 때
        // 256번 콜백: 거점 개발 명령 시 예약되지 않은 개발지에 무장 선택 후 병영 건설
        void callback_256(pk::ai_context@ context, pk::building@ building, int cmd)
        {

            // 적용 대상 세력 확인
            bool is_ai = !building.is_player();  // AI 세력
            bool is_delegated = building.is_player() and !pk::is_player_controlled(building);  // 위임군단
            
            if (적용_대상_세력 == 1 and !is_ai)  // AI만 적용
                return;
            if (적용_대상_세력 == 0 and !is_ai and !is_delegated)  // AI + 위임군단만 적용
                return;            
            
            // 행동력 최대치로 설정
            if (행동력_최대치_적용_여부)
            {
                context.ap = 255;	
                pk::get_district(building.get_district_id()).ap = 255;
            }

            if (cmd == 거점AI_개발)
            {   
                // 내 스크립트가 먼저 실행되도록 기본 AI 차단
                pk::district@ district = pk::get_district(building.get_district_id());
                saved_district_ap_ = district.ap;
                context.ap = 0;  // 기본 AI가 실행되지 않도록 행동력 0으로 설정
                district.ap = 0;  // district 행동력도 0으로 설정
                cmd_handled_ = false;  // 명령 처리 여부 초기화
                
                // 명령을 가로채서 내 스크립트 실행
                pk::info("===========================================");
                pk::info("내정 간섭");
                int city_id = building.get_id();

                pk::city@ city = pk::get_city(city_id);
                if (pk::is_alive(city))
                {   
                    pk::info(pk::format("도시: {}", pk::decode(pk::get_name(city))));               

                    for (int i = 0; i < int(cached_priority_building_positions[city_id].length); i++)
                    {                        
                        pk::info(pk::format("cached_priority_building_positions[{}]: ({},{})", get_facility_name(우선순위_시설_배열[i]), cached_priority_building_positions[city_id][i].x, cached_priority_building_positions[city_id][i].y));
                    }

                    pk::list<pk::person@> idle_persons = pk::get_idle_person_list(building);
                    // 행동이 미완료된 사람만 추출
                    pk::list<pk::person@> available_persons;
                    for (int i = 0; i < idle_persons.count; i++)
                    {
                        if (pk::is_alive(idle_persons[i]) and !idle_persons[i].action_done)
                        {
                            available_persons.add(idle_persons[i]);
                        }
                    }
                    idle_persons = available_persons;
                    
                    pk::facility@ target_facility;
                    int facility_id = -1;
                    pk::point target_pos = pk::point(-1, -1);
                    pk::array<int> actor_indices;
                    pk::develop_cmd_info build_cmd;
                    
                    // 캐시된 시장 배열 좌표 출력
                    pk::info("========== 캐시된 시장 배열 좌표 ==========");
                    for (int i = 0; i < int(cached_mint_market_positions[city_id].length); i++)
                    {
                        pk::info(pk::format("cached_mint_market_positions[{}]: ({},{})", i, cached_mint_market_positions[city_id][i].x, cached_mint_market_positions[city_id][i].y));
                    }
                    
                    // 캐시된 농장 배열 좌표 출력
                    pk::info("========== 캐시된 농장 배열 좌표 ==========");
                    for (int i = 0; i < int(cached_granary_farm_positions[city_id].length); i++)
                    {
                        pk::info(pk::format("cached_granary_farm_positions[{}]: ({},{})", i, cached_granary_farm_positions[city_id][i].x, cached_granary_farm_positions[city_id][i].y));
                    }
                    
                    // 캐시된 우선순쉬 시설 배열 좌표 출력
                    pk::info("========== 캐시된 우선순쉬 시설 배열 좌표 ==========");
                    for (int i = 0; i < int(cached_priority_building_positions[city_id].length); i++)
                    {
                        pk::info(pk::format("cached_priority_building_positions[{}]: ({},{})", i, cached_priority_building_positions[city_id][i].x, cached_priority_building_positions[city_id][i].y));
                    }

                    for (int i = 0; i < int(우선순위_시설_배열.length); i++)
                    {                        
                        pk::info(pk::format("우선순위_시설_배열[{}]: {} 있어?{}", i, get_facility_name(우선순위_시설_배열[i]), is_building_in_city(city_id, 우선순위_시설_배열[i])));
                        
                        if (is_building_in_city(city_id, 우선순위_시설_배열[i]) or cached_priority_building_positions[city_id][i] == pk::point(-1, -1)) continue;                        
                        pk::info(pk::format("{} 없으면 지어", get_facility_name(우선순위_시설_배열[i])));
                        facility_id = 우선순위_시설_배열[i];
                        target_pos = cached_priority_building_positions[city_id][i];
                        @target_facility = pk::get_facility(facility_id);
                        actor_indices = select_for_facility(idle_persons, target_facility.hp);
                        if (actor_indices.length == 0) return;
                        cached_priority_building_positions[city_id][i] = pk::point(-1, -1);                        
                        break;
                    }
                        
                    pk::info(pk::format("cache_count(city_id, -1): {}", cache_count(city_id, -1)));

                    if (facility_id == -1 and target_pos == pk::point(-1, -1) and cache_count(city_id, -1) == 0)                    
                    {
                        
                        
                        int mar_count = cache_count(city_id, 시설_시장);
                        int fam_count = cache_count(city_id, 시설_농장);
                        pk::info(pk::format("시장: {}, 농장: {}", mar_count, fam_count));
                        if (mar_count > 0)
                        {
                            for (int j = 1; j < int(cached_mint_market_positions[city_id].length); j++)
                            {
                                pk::info("시장");
                                if (cached_mint_market_positions[city_id][j] == pk::point(-1, -1)) continue;
                                target_pos = cached_mint_market_positions[city_id][j];
                                facility_id = 시설_시장;
                                @target_facility = pk::get_facility(facility_id);
                                actor_indices = select_for_facility(idle_persons, target_facility.hp);
                                if (actor_indices.length == 0) return;;
                                cached_mint_market_positions[city_id][j] = pk::point(-1, -1);
                                break;
                            }                        
                        }
                        else if (mar_count == 0 and cached_mint_market_positions[city_id][0] != pk::point(-1, -1))
                        {   
                            pk::info("조폐");
                            target_pos = cached_mint_market_positions[city_id][0];
                            facility_id = 시설_조폐;
                            @target_facility = pk::get_facility(시설_조폐);
                            actor_indices = select_for_facility(idle_persons, target_facility.hp);
                            if (actor_indices.length == 0) return;;
                            cached_mint_market_positions[city_id][0] = pk::point(-1, -1);
                            
                        }                        
                        else if (fam_count > 0)
                        {
                            for (int j = 1; j < int(cached_granary_farm_positions[city_id].length); j++)
                            {
                                pk::info("농장");
                                if (cached_granary_farm_positions[city_id][j] == pk::point(-1, -1)) continue;
                                target_pos = cached_granary_farm_positions[city_id][j];
                                facility_id = 시설_농장;
                                @target_facility = pk::get_facility(facility_id);
                                actor_indices = select_for_facility(idle_persons, target_facility.hp);
                                if (actor_indices.length == 0) return;;
                                cached_granary_farm_positions[city_id][j] = pk::point(-1, -1);
                                break;                                
                            }
                        }
                        else if (fam_count == 0 and cached_granary_farm_positions[city_id][0] != pk::point(-1, -1))
                        {
                            
                            pk::info("곡창");
                            target_pos = cached_granary_farm_positions[city_id][0];
                            facility_id = 시설_곡창;
                            @target_facility = pk::get_facility(시설_곡창);
                            actor_indices = select_for_facility(idle_persons, target_facility.hp);
                            if (actor_indices.length == 0) return;
                            cached_granary_farm_positions[city_id][0] = pk::point(-1, -1);
                            pk::info(pk::format("자리 찾음 target_pos: ({},{})", target_pos.x, target_pos.y));
                            
                        }
                        else
                        {
                            // Todo 컴퓨터 활용 자리
                        }
                    }
                    pk::info(pk::format("facility_id: {}", get_facility_name(facility_id)));
                    pk::info(pk::format("target_pos: ({},{})", target_pos.x, target_pos.y));
                    // target_pos와 facility_id가 설정되었으면 내 스크립트가 처리할 수 있는 상태
                    // 이 경우 기본 AI는 완전히 차단해야 함
                    if (target_pos != pk::point(-1, -1) and facility_id >= 0 and target_facility !is null)
                    {
                        
                        pk::info(pk::format("target_pos: ({},{})", target_pos.x, target_pos.y));
                        pk::info(pk::format("actor_indices: {}", actor_indices.length));
                        pk::info(pk::format("facility_id: {}, hp: {}", get_facility_name(facility_id), target_facility.hp));
                        if (int(actor_indices.length) > 0)
                        {   
                            // pk::info(pk::format("actor: {}", pk::decode(pk::get_name(idle_persons[actor_indices[0]]))));     
                            bool building_exists = false;
                            
                            @build_cmd.base = building;
                            build_cmd.facility = facility_id;
                            build_cmd.pos = target_pos;
                            for (int i = 0; i < int(actor_indices.length); i++)
                            {   
                                if (actor_indices[i] == -1) continue;
                                int actor_index = actor_indices[i];
                                @build_cmd.actors[i] = idle_persons[actor_index];
                                pk::person@ actor = idle_persons[actor_index];
                                pk::info(pk::format("actor{}: {}:{} (action_done: {}, id {}, location: {})", 
                                    i, actor_index, pk::decode(pk::get_name(actor)), 
                                    actor.action_done, actor.get_id(), actor.location));
                            }
                                             
                            // 행동력 복원하여 내 명령 실행
                            pk::district@ district = pk::get_district(building.get_district_id());
                            context.ap = 255;  // 명령 실행 전 행동력 풀 복원
                            district.ap = 255;  // district 행동력도 풀 복원
                            pk::info(pk::format("명령 실행 직전 행동력: context.ap={}, district.ap={}", context.ap, district.ap));
                            bool result = pk::command(build_cmd);
                            pk::info(pk::format("명령 실행 결과: {}", result));
                            pk::info(pk::format("명령 실행 후 행동력: {}", context.ap));
                            
                            // 실제 건물이 지어졌는지 확인 (도시의 개발지 배열에서 확인)
                            
                            for (int i = 0; i < city.max_devs; i++)
                            {
                                if (city.dev[i].pos.x == target_pos.x and city.dev[i].pos.y == target_pos.y)
                                {
                                    pk::building@ dev_building = city.dev[i].building;
                                    if (pk::is_alive(dev_building) and dev_building.facility == facility_id)
                                    {
                                        building_exists = true;
                                        break;
                                    }
                                }
                            }
                            pk::info(pk::format("실제 건물 존재 여부: {}", building_exists));
                            
                            if (result and building_exists)
                            {
                                pk::info("건설 성공");
                                cmd_handled_ = true;  // 명령 처리 완료
                                context.ap = 0;
                                district.ap = 0;
                                return;  // 기본 AI 호출하지 않음
                            }
                            else if (result and !building_exists)
                            {
                                pk::info("명령은 성공했지만 건물이 실제로 지어지지 않음 - 행동력 부족 가능성");
                            }                            
                        }
                        // 내 스크립트가 처리 시도했으면 기본 AI 완전 차단하고 종료
                        return;  // 기본 AI 호출하지 않음
                    }
                }
                
                // 내 스크립트가 명령을 처리했으면 prev_callback_256 호출하지 않음
                if (cmd_handled_)
                {
                    return;  // 기본 AI 호출하지 않음
                }
                else
                {
                    // 내 스크립트가 명령을 처리하지 못했으면 행동력 복원 후 기본 AI 호출
                    pk::district@ district = pk::get_district(building.get_district_id());
                    district.ap = saved_district_ap_;
                    context.ap = saved_district_ap_;  // context.ap도 복원
                }
            }
            
            // 내 스크립트가 처리하지 않은 경우에만 기본 AI 호출
            if (prev_callback_256 !is null)
                prev_callback_256(context, building, cmd);
        }

        void callback_257(pk::ai_context@ context, pk::building@ building, int cmd)
        {
            if (cmd == 거점AI_개발)
            {
                if (cmd_handled_ == true)
                {
                    // 내 스크립트가 명령을 처리했으면 district.ap 복원
                    pk::district@ district = pk::get_district(building.get_district_id());
                    district.ap = saved_district_ap_;
                }
                // cmd_handled_ == false인 경우는 이미 callback_256에서 복원했으므로 여기서는 할 필요 없음
            }
            
            //  prev_callback_257 호출
            if (prev_callback_257 !is null)
                prev_callback_257(context, building, cmd);
        }

        // 도시에서 건물 없는 개발지 중 캐시에 예약되지 않은 좌표만 반환
        pk::array<pk::point> get_available_development_positions(pk::city@ city)
        {
            pk::array<pk::point> positions;
            if (!pk::is_alive(city))
                return positions;
            int city_id = city.get_id();
            for (int i = 0; i < int(city.max_devs); i++)
            {
                pk::building@ dev_building = city.dev[i].building;
                if (pk::is_alive(dev_building))
                    continue;

                pk::point dev_pos = city.dev[i].pos;
                bool is_continue = false;

                // 시장과 겹치는지 확인
                for (int j = 0; j < int(cached_mint_market_positions[city_id].length); j++)
                {
                    if (cached_mint_market_positions[city_id][j].x == dev_pos.x and cached_mint_market_positions[city_id][j].y == dev_pos.y)
                    {
                        is_continue = true;
                        break;
                    }                     
                }
                if (is_continue) 
                    continue;
                for (int j = 0; j < int(cached_granary_farm_positions[city_id].length); j++)
                {
                    if (cached_granary_farm_positions[city_id][j].x == dev_pos.x and cached_granary_farm_positions[city_id][j].y == dev_pos.y)
                    {
                        is_continue = true;
                        break;
                    }
                }
                if (is_continue) 
                    continue;

                positions.insertLast(dev_pos);
            }
            return positions;
        }

        pk::array<int> select_for_facility(pk::list<pk::person@>& person_list, int facility_max_hp)
        {
            pk::array<int> selection;
            if (person_list.count == 0) return selection;

            int base_value = facility_max_hp - facility_max_hp / 4;

            // 시설 3000 이상이면 최소 정치력 1명 선택
            if (facility_max_hp >= 3000) {
                int min_politics = 0x7fffffff;
                int min_idx = -1;
                for (int i = 0; i < int(person_list.count); i++) {
                    int politics = person_list[i].stat[무장능력_정치];
                    if (politics < min_politics) {
                        min_politics = politics;
                        min_idx = i;  // 안전하게 i 사용
                    }
                }
                if (min_idx >= 0) selection.insertLast(min_idx);
                return selection;
            }

            int best_turns = 0x3fffffff;
            pk::array<int> best_combo;

            // 1~3명 조합 반복문
            for (int i = 0; i < int(person_list.count); i++) {
                int sum1 = person_list[i].stat[무장능력_정치];
                int turns1 = (base_value + sum1 - 1) / sum1;

                if (turns1 > 10) {
                    // 10턴 초과 시 최소 정치력 1명
                    int min_politics = 0x7fffffff;
                    int min_idx = -1;
                    for (int k = 0; k < int(person_list.count); k++) {
                        int politics = person_list[k].stat[무장능력_정치];
                        if (politics < min_politics) {
                            min_politics = politics;
                            min_idx = k;
                        }
                    }
                    turns1 = 10;
                    if (turns1 < best_turns && min_idx >= 0) {
                        best_turns = turns1;
                        best_combo.resize(0);
                        best_combo.insertLast(min_idx);
                    }
                } else if (turns1 < best_turns) {
                    best_turns = turns1;
                    best_combo.resize(0);
                    best_combo.insertLast(i);
                }

                for (int j = i + 1; j < int(person_list.count); j++) {
                    int sum2 = sum1 + person_list[j].stat[무장능력_정치];
                    int turns2 = (base_value + sum2 - 1) / sum2;
                    if (turns2 > 10) continue;
                    if (turns2 < best_turns) {
                        best_turns = turns2;
                        best_combo.resize(0);
                        best_combo.insertLast(i);
                        best_combo.insertLast(j);
                    }

                    for (int k = j + 1; k < int(person_list.count); k++) {
                        int sum3 = sum2 + person_list[k].stat[무장능력_정치];
                        int turns3 = (base_value + sum3 - 1) / sum3;
                        if (turns3 > 10) continue;
                        if (turns3 < best_turns) {
                            best_turns = turns3;
                            best_combo.resize(0);
                            best_combo.insertLast(i);
                            best_combo.insertLast(j);
                            best_combo.insertLast(k);
                        }
                    }
                }
            }

            for (int i = 0; i < int(best_combo.length); i++)
                selection.insertLast(best_combo[i]);

            return selection;
        }

        // 지어지고 있는 건물이나 완성된 건물 중에 특정 건물이 있는지 확인
        bool is_building_in_city(int city_id, int facility_id, pk::point destroyed_pos = pk::point(-1,-1))
        {
            pk::city@ city = pk::get_city(city_id);
            if (!pk::is_alive(city))
                return false;

            for (int i = 0; i < int(city.max_devs); i++)
            {
                pk::building@ building = city.dev[i].building;
                pk::point building_pos = city.dev[i].pos;
                
                // 부서진 건물은 무시
                if (destroyed_pos != pk::point(-1,-1) && building_pos.x == destroyed_pos.x && building_pos.y == destroyed_pos.y)
                    continue;
                
                if (pk::is_alive(building))                    
                {   
                    if (building.facility == facility_id)
                        return true;
                    else if (facility_id == 시설_병영 and (building.facility == 시설_병영 or building.facility == 시설_병영2단 or building.facility == 시설_병영3단))
                        return true;
                    else if (facility_id == 시설_마구간 and (building.facility == 시설_마구간 or building.facility == 시설_마구간2단 or building.facility == 시설_마구간3단))
                        return true;
                    else if (facility_id == 시설_대장간 and (building.facility == 시설_대장간 or building.facility == 시설_대장간2단 or building.facility == 시설_대장간3단))
                        return true;
                    else if (facility_id == 시설_공방 and (building.facility == 시설_공방 or building.facility == 시설_공방2단 or building.facility == 시설_공방3단))
                        return true;
                    else if (facility_id == 시설_조선 and (building.facility == 시설_조선 or building.facility == 시설_조선2단 or building.facility == 시설_조선3단))
                        return true;
                }
                    
            }
            return false;
        }

        // 지어지고 있는 건물이나 완성된 건물 중에 특정 건물이 있는지 확인
        int count_mar_fam_in_city(int city_id, int facility_id)
        {   
            int count = 0;
            pk::city@ city = pk::get_city(city_id);
            if (!pk::is_alive(city))
                return count;

            for (int i = 0; i < int(city.max_devs); i++)
            {
                pk::building@ building = city.dev[i].building;
                if (pk::is_alive(building) && building.facility == facility_id)
                    count ++;
            }
            return count;
        }


        int cache_count(int city_id, int facility_id)
        {   
            // 0은 조폐 또는 곡창
            int count = 0;
            if (facility_id == 시설_농장)
            {
                
                for (int i = 1; i < int(cached_granary_farm_positions[city_id].length); i++)
                {
                    if (cached_granary_farm_positions[city_id][i] != pk::point(-1, -1))                
                        count++;
                
                }
            }
            else if (facility_id == 시설_시장)
            {
                for (int i = 1; i < int(cached_mint_market_positions[city_id].length); i++)
                {
                    if (cached_mint_market_positions[city_id][i] != pk::point(-1, -1))
                        count++;
                }
            }
            else
            {
                for (int i = 0; i < int(cached_priority_building_positions[city_id].length); i++)
                {
                    if (cached_priority_building_positions[city_id][i] != pk::point(-1, -1))
                        count++;
                }
            }

            return count;
        }

        // 시설 이름 가져오기
        string get_facility_name(int facility_id)
        {
            if (facility_id == 시설_병영) return "병영";
            if (facility_id == 시설_시장) return "시장";
            if (facility_id == 시설_농장) return "농장";
            if (facility_id == 시설_대장간) return "단야";
            if (facility_id == 시설_마구간) return "마구간";
            if (facility_id == 시설_공방) return "공방";
            if (facility_id == 시설_조선) return "조선";
            if (facility_id == 시설_대시장) return "대시장";
            if (facility_id == 시설_어시장) return "어시장";
            if (facility_id == 시설_동작대) return "동작대";
            if (facility_id == 시설_조폐) return "조폐";
            if (facility_id == 시설_곡창) return "곡창";
            if (facility_id == 시설_부절태) return "부절태";
            if (facility_id == 시설_군사부) return "군사부";
            if (facility_id == 시설_인재부) return "인재부";
            if (facility_id == 시설_외교부) return "외교부";
            if (facility_id == 시설_계략부) return "계략부";
            if (facility_id == 시설_연병소) return "연병소";
            if (facility_id == 시설_군둔농) return "군둔농";
            return "내정시설";
        }

        
    }

    Main main;
}

