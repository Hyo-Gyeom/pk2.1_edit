/*
// 제작자: 
// 설명  : 해당 상황에 맞게 출정만 하는 스크립트(병종은 출진병종부장설정, 교전은 부대교전설정에서 처리)
//        플레이어 세력 우선 공격
// Update: 2025.11.22 / 초기 버전 작성
//         2025.11.25 / 경로 찾기 로직 개선 (BFS 방식으로 최단 경로 계산, 경로 캐싱 추가)
//                     - 플레이어 도시로의 경로상 통로 세력 찾기 및 동맹 신청 로직 추가
//                     - 플레이어 도시 출정 가능 세력과의 친선/동맹 시도 로직 추가
//                     - 동맹 신청 횟수 제한 (최대 2회) 추가
//                     - 우호도 40 이하 시 점령 시도, 초과 시 동맹/친선 시도 로직 추가
//                     - 거점 좌표 및 경로 캐싱으로 성능 최적화
// 기능  : 건물이 8개 이상 지어지기 시작하면 친선 및 동맹 작업 시작
//        플레이어 세력 우선 공격
//        소속지인 도시에 적이 나타날 경우 도시를 소속지로 둔 외부에 있는 부대 퇴각 처리
//        원군 부대 출정
*/

namespace 도시상태별명령
{
    //=======================================================================================
    // 유저 설정
    //---------------------------------------------------------------------------------------

    const bool 스크립트_활성 = true;       // 전체 스크립트 on/off

    // 0: AI 세력 + 플레이어 위임군단, 1: AI 세력만 대상으로 공격 명령 부여
    const int  공격명령_세력조건 = 0;

    const int 공격명령시_최소병력기준 = 40000;
    const int 공격명령시_잔여병력기준 = 15000;
    const int 공격명령시_잔여장수기준 = 3;

    const int 방어명령시_잔여병력기준 = 20000;
    const int 방어명령시_잔여장수기준 = 6;

    const int 원군명령시_잔여병력기준 = 15000;
    const int 원군명령시_잔여장수기준 = 6;

    // 처음에 징병 및 치안부터 시작하도록 해놓았기에 취향껏 설정 
    const bool 징병_치안_시작_여부 = true;  // false면 컴퓨터에게 맡김김
    const bool 징병_치안_행동미완료처리_여부 = true;

    //=======================================================================================

    class Main
    {
        // 거점 좌표 캐시 (초기화 시 한 번만 계산)
        pk::array<pk::point> building_positions;
        bool positions_initialized = false;
        
        // 경로 캐시: [src_id][dst_id] = 경로상의 거점 배열
        pk::array<pk::array<pk::array<pk::building@>>> path_cache;
        bool path_cache_initialized = false;
        
        Main()
        {
            pk::bind(111, pk::trigger111_t(onTurnStart));    // 턴 시작 시점 (우선순위 0 - 먼저 실행, 낮을수록 먼저 실행됨)
            initialize_building_positions();
            initialize_path_cache();
        }
        
        // 거점 좌표 초기화 (한 번만 실행)
        void initialize_building_positions()
        {
            if (positions_initialized) return;
            
            building_positions.length = 건물_거점끝;
            for (int i = 0; i < 건물_거점끝; i++)
            {
                pk::building@ building = pk::get_building(i);
                if (pk::is_alive(building))
                    building_positions[i] = building.pos;
                else
                    building_positions[i] = pk::point(-1, -1);
            }
            positions_initialized = true;
        }
        
        // 경로 캐시 초기화 (플레이어 도시로의 경로만 미리 계산)
        void initialize_path_cache()
        {
            if (path_cache_initialized) return;
            
            path_cache.length = 건물_거점끝;
            for (int i = 0; i < 건물_거점끝; i++)
            {
                path_cache[i].length = 건물_거점끝;
                for (int j = 0; j < 건물_거점끝; j++)
                {
                    path_cache[i][j].length = 0; // 빈 배열로 초기화
                }
            }
            path_cache_initialized = true;
        }
        
        // 경로 캐시에서 가져오기 (없으면 계산해서 저장)
        pk::array<pk::building@> get_cached_path(pk::building@ src, pk::building@ dst, pk::force@ force)
        {
            if (!pk::is_alive(src) or !pk::is_alive(dst) or !pk::is_alive(force)) 
            {
                pk::array<pk::building@> empty;
                return empty;
            }
            
            int src_id = src.get_id();
            int dst_id = dst.get_id();
            
            // 캐시에 경로가 있으면 반환 (거점은 없어지지 않고 세력 소유권만 바뀌므로 경로는 변하지 않음)
            if (path_cache[src_id][dst_id].length > 0)
            {
                return path_cache[src_id][dst_id];
            }
            
            // 캐시에 없으면 계산해서 저장
            pk::array<pk::building@> path = find_all_cities_on_path(src, dst, force);
            path_cache[src_id][dst_id] = path;
            return path;
        }

        // 턴 시작 트리거
        void onTurnStart(pk::force@ force)
        {
            if (!스크립트_활성) return;
            if (!pk::is_normal_force(force)) return;
            
            
            // 시작턴 로그
            string force_name = pk::decode(pk::get_name(force));
            pk::info(pk::format("===== {} 세력 도시 턴 시작=====", force_name));
            
            // 세력의 모든 도시에 대해 행동 처리
            handle_city_actions(force);
            pk::info(pk::format("===== {} 세력 도시 턴 종료=====", force_name));
            
            // 부대교전설정 실행
            부대교전설정::main.handle_force_units(force);
        }

        // 도시조건 함수
        bool check_city_condition(pk::city@ city)
        {
            if (!pk::is_alive(city)) return false;
            
            // 0: AI 세력 + 플레이어 위임군단, 1: AI 세력만
            if (공격명령_세력조건 == 0)
            {
                // 플레이어 직접 조작 도시는 제외
                if (pk::is_player_controlled(city)) return false;
                return true;
            }
            else if (공격명령_세력조건 == 1)
            {
                // AI 세력만 (플레이어 위임군단도 제외)
                if (city.is_player()) return false;
                return true;
            }
            
            return false;
        }
        
        // 도시 행동 처리 (동맹, 방어, 원군, 공격)
        void handle_city_actions(pk::force@ force)
        {
            pk::info("도시 행동 처리 시작");
            
            // 세력의 모든 도시 순회
            for (int city_id = 0; city_id < 도시_끝; city_id++)
            {
                pk::city@ city = pk::get_city(city_id);
                if (!pk::is_alive(city)) continue;
                if (city.get_force_id() != force.get_id()) continue;
                
                // 도시조건 확인
                if (!check_city_condition(city)) continue;
                
                pk::building@ city_building = pk::city_to_building(city);
                if (!pk::is_alive(city_building)) continue;
                
                // 병영이 있으면 징병 실행
                if (징병_치안_시작_여부 and has_barracks(city))
                {
                    pk::info("징병 실행");
                    execute_recruit(city_building);
                    execute_policing(city_building);
                }
                
                // 건설 중인 건물 수수
                int under_construction_devs = get_under_construction_dev_count(city);
                //  건설 완료된 건물 수
                int completed_devs = get_completed_dev_count(city);
                
                pk::info("건설 완료: " + completed_devs + ", 건설 중: " + under_construction_devs + ", 최대: " + city.max_devs);
                                
                // 도시 소속 지형에 적 시설이나 부대 발견시 방어 - 부대임무_공격
                if (check_enemy_in_territory(city_building))
                {
                    pk::info("방어 부대 출정");
                    deploy_defense_units(city_building);
                    continue;  // 방어가 필요하면 다른 명령은 스킵
                }

                // ----------------------공격 처리----------------------
                pk::building@ player_city = null;
                // 위임 도시(플레이어 세력)가 아닌 경우에만 플레이어 도시를 찾음
                if (!force.is_player())
                    {
                        @player_city = find_nearest_player_city_building(city_building);
                    }
                    pk::info("플레이어 도시 찾기: " + (pk::is_alive(player_city) ? pk::decode(pk::get_name(player_city)) : "없음"));
                    if (pk::is_alive(player_city))
                    {
                        bool can_deploy = can_deploy_to_city(city_building, player_city);
                        pk::info("플레이어 도시 출정 가능 여부: " + (can_deploy ? "가능" : "불가능"));
                    }


                    bool all_path_allied = false;
                    if (can_deploy_to_city(city_building, player_city))
                    {
                        pk::info("플레이어 도시 출정 가능");
                        // 통로 세력 ID 저장
                        array<int> path_force_ids;
                            
                        // 경로상의 모든 도시를 한 번에 찾아서 친선/동맹 시도 (캐시 사용)
                        pk::array<pk::building@> all_path_cities = get_cached_path(city_building, player_city, force);
                        
                        // 경로 정보 로그
                        string city_name = pk::decode(pk::get_name(city_building));
                        string path_log = "[" + city_name + "] 플레이어 거점: " + pk::decode(pk::get_name(player_city));
                        if (all_path_cities.length > 0)
                        {
                            path_log += " / 통로 세력 거점: ";
                            for (int k = 0; k < int(all_path_cities.length); k++)
                            {
                                if (k > 0) path_log += ", ";
                                path_log += pk::decode(pk::get_name(all_path_cities[k]));
                            }
                        }
                        else
                        {
                            path_log += " / 통로 세력 없음";
                        }
                        
                        pk::info(path_log);

                        // 통로 세력 거리순 정렬
                        pk::array<pk::building@> path_forces_sorted_by_distance = get_path_forces_sorted_by_distance(city_building, player_city, force);
                                            
                        // 통로 거점을 순서대로 처리 (플레이어 거점은 동맹 신청 안 함)
                        pk::building@ player_path_target = null;  // 공격할 플레이어 통로 거점
                        all_path_allied = true;
                        
                        if (path_forces_sorted_by_distance.length > 0)
                        {
                            for (int i = 0; i < int(path_forces_sorted_by_distance.length); i++)
                            {
                                pk::building@ path_city = path_forces_sorted_by_distance[i];
                                if (!pk::is_alive(path_city)) continue;
                                
                                pk::force@ path_force = pk::get_force(path_city.get_force_id());
                                if (!pk::is_alive(path_force)) continue;
                                
                                // 플레이어 거점인 경우 동맹 신청 즉시 멈추고 공격으로 넘어가기
                                if (pk::is_player_controlled(path_force))
                                {
                                    // 이전 통로들이 모두 동맹이면 플레이어 거점을 공격 대상으로 설정
                                    if (all_path_allied)
                                    {
                                        @player_path_target = path_city;
                                        pk::info("플레이어 통로 거점 발견 - 동맹 신청 멈추고 공격으로 전환: " + pk::decode(pk::get_name(path_city)));
                                    }
                                    else
                                    {
                                        pk::info("플레이어 통로 거점 발견 - 동맹 신청 멈춤 (이전 통로 동맹 미완료)");
                                    }
                                    break;  // 플레이어 거점을 만나면 즉시 중단
                                }
                                // 일반 거점인 경우 동맹 처리
                                else if (!force.ally[path_city.get_force_id()])
                                {                                   
                                    // 우호도 확인 후 친선/동맹 신청
                                    bool success = try_goodwill_and_alliance_by_threshold(city_building, path_city, force, 70);
                                    if (!success)
                                        all_path_allied = false;
                                }
                            }
                        }
                        
                        // 플레이어 통로 거점이 있고 내정 완료되었으면 공격
                        if (pk::is_alive(player_path_target) and completed_devs + under_construction_devs == city.max_devs)
                        {
                            pk::info("내정 시설 모두 완료 - 플레이어 통로 거점 공격: " + pk::decode(pk::get_name(player_path_target)));
                            deploy_attack_units(city_building, player_path_target);
                            continue;
                        }
                        
                        // 통로 세력이 없거나 모두 동맹이 맺어졌으면 플레이어 거점에 출정할 수 있는 세력들과 친선/동맹 시도
                        if (all_path_allied and !pk::is_alive(player_path_target))
                        {
                            pk::info("통로 세력 모두 동맹 완료");
                            // 플레이어 공격 가능한 도시의 세력 찾기 (가까운 순으로 정렬)
                            pk::array<pk::building@> deployable_cities = find_cities_can_attack_player(city_building, player_city);
                            for (int j = 0; j < int(deployable_cities.length); j++)
                            {
                                pk::building@ deployable_city = deployable_cities[j];
                                if (!pk::is_alive(deployable_city)) continue;
                                
                                if (!force.ally[deployable_city.get_force_id()])
                                {                                   
                                    // 우호도 확인 후 친선/동맹 신청
                                    bool success = try_goodwill_and_alliance_by_threshold(city_building, deployable_city, force, 70);                                
                                }
                                // 2회 제한
                                if (j == 1)
                                    break;
                            }
                            
                            if (completed_devs + under_construction_devs == city.max_devs)
                            {
                                pk::info("내정 시설 모두 완료 - 플레이어 도시 출정");
                                deploy_attack_units(city_building, player_city);
                                continue;
                            }
                        }   
                    }
                    // 위임도시거나, 플레이어 공격이 불가능하거나 통로 동맹이 미완료 일 경우                
                    else if ((city.is_player() and !pk::is_player_controlled(city)) or !all_path_allied or !can_deploy_to_city(city_building, player_city))
                    {
                        pk::info("통로 동맹이 미완료 이거나 플레이어 공격이 불가능");
                        // 제일 가까운 적대 도시 로그 
                        pk::building@ nearest_enemy_city = find_nearest_enemy_city(city_building, player_city, force);
                                        
                        string player_city_str = "없음";
                        string nearest_enemy_str = "없음";

                        if (pk::is_alive(player_city))
                            player_city_str = pk::decode(pk::get_name(player_city));

                        if (pk::is_alive(nearest_enemy_city))
                            nearest_enemy_str = pk::decode(pk::get_name(nearest_enemy_city));

                        // 통로 세력 정보 수집
                        string path_forces_str = get_path_forces_string(city_building, force, player_city);

                        pk::info("플레이어 도시: " + player_city_str + " / 적대 도시: " + nearest_enemy_str + path_forces_str);

                        if (pk::is_alive(nearest_enemy_city) and completed_devs + under_construction_devs == city.max_devs)
                        {
                                pk::info("적대 도시 출정");
                                deploy_attack_units(city_building, nearest_enemy_city);
                                continue;
                        }
                        else
                        {
                            pk::info("적대 도시 없어?");
                        }
                    }
                // 원군공격 출정 처리
                if (has_barracks(city) and under_construction_devs + completed_devs >= 12)
                {                    
                    // 방어할 필요가 없으면 아군 세력 - 동맹이나 같은 세력에 원군 출정 (부대임무_이동)
                    pk::building@ ally_base = find_ally_base_needing_reinforcement(city_building, force);
                    if (pk::is_alive(ally_base))
                    {
                        pk::info("원군 부대 출정");
                        deploy_reinforcement_units(city_building, ally_base);                        
                    }
                }
            }            
        }
        
        // 도시에 병영이 있는지 확인 (완성된 병영만)
        bool has_barracks(pk::city@ city)
        {
            if (!pk::is_alive(city)) return false;
            
            for (int i = 0; i < city.max_devs; i++)
            {
                pk::building@ building = city.dev[i].building;
                if (pk::is_alive(building) and building.completed)
                {
                    if (building.facility == 시설_병영 or 
                        building.facility == 시설_병영2단 or 
                        building.facility == 시설_병영3단)
                    {
                        return true;
                    }
                }
            }
            
            return false;
        }
        
        // 징병 실행
        void execute_recruit(pk::building@ city_building)
        {
            if (!pk::is_alive(city_building)) return;
            
            // 실행할 무장 찾기
            pk::list<pk::person@> person_list = pk::get_idle_person_list(city_building);
            if (person_list.count == 0) return;
            
            // 매력이 높은 무장 선택 (최대 3명)
            person_list.sort(function(a, b)
            {
                return a.stat[무장능력_매력] > b.stat[무장능력_매력];
            });
            
            // 징병 명령 정보 생성 (recruit_cmd_info = 징병)
            pk::recruit_cmd_info cmd;
            @cmd.base = city_building;
            
            int actor_count = int(person_list.count);
            
            if (actor_count == 0) return;
            
            bool is_has_fame = false;
            if (actor_count > 3)
            {   
                // 처음 3명에 명성이 없고, 전체 리스트에 명성이 있는 사람이 있으면 마지막 자리 교체            
                for (int i = 0; i < person_list.count; i++)
                {                    
                    // 3명 안에 명성이 있으면 브레이크
                    if (2 >= i)
                    {
                        @cmd.actors[i] = person_list[i];
                        if (!is_has_fame and pk::has_skill(person_list[i], 특기_명성))
                            is_has_fame = true;

                        if (i == 2 and is_has_fame)
                            break;
                    }
                    // 3명 넘어가면 마지막 자리를 명성 있는 사람으로 바꾸기
                    else if (!is_has_fame)
                    {   
                        if (pk::has_skill(person_list[i], 특기_명성))
                        {
                            @cmd.actors[2] = person_list[i];
                            is_has_fame = true;
                            break;
                        }
                    }                    
                }
            }            
            else
            {
                // 3명 이하면 모두 할당
                for (int i = 0; i < actor_count; i++)
                {
                    @cmd.actors[i] = person_list[i];
                }
            }
        
            // 징병 명령 실행
            bool success = pk::command(cmd);
            
            // 징병시 행동 미완료 처리
            if (징병_치안_행동미완료처리_여부 and success)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (cmd.actors[i] !is null)
                    {
                        cmd.actors[i].action_done = false;
                    }
                }
            }
        }

        // 치안 실행
        void execute_policing(pk::building@ city_building)
        {
            if (!pk::is_alive(city_building)) return;
            
            // 실행할 무장 찾기
            pk::list<pk::person@> person_list = pk::get_idle_person_list(city_building);
            if (person_list.count == 0) return;

            // 통솔이 높은 무장 선택
            person_list.sort(function(a, b)
            {
                return a.stat[무장능력_통솔] > b.stat[무장능력_통솔];
            });
            
            // 치안 명령 정보 생성
            pk::inspections_cmd_info cmd;
            @cmd.base = city_building;
            int actor_count = int(person_list.count);
            
            if (actor_count == 0) return;
            
            bool is_has_fame = false;               
            // 처음 3명에 명성이 없고, 전체 리스트에 명성이 있는 사람이 있으면 마지막 자리 교체            
            for (int i = 0; i < person_list.count; i++)
            {                    
                // 3명 안에 명성이 있으면 브레이크                
                @cmd.actors[i] = person_list[i];
                if (i == 2) break;              
            }
            
            bool success = pk::command(cmd);
            
            // 치안시 행동 미완료 처리
            if (징병_치안_행동미완료처리_여부 and success)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (cmd.actors[i] !is null)
                    {
                        cmd.actors[i].action_done = false;
                    }
                }
            }
        }

        // 플레이어 도시 찾기 (제일 가까운 거리의 플레이어 도시, 위임군단 포함, 관문/항구 포함)
        pk::building@ find_nearest_player_city_building(pk::building@ src)
        {
            if (!pk::is_alive(src)) return null;
            
            pk::building@ best_player_city = null;
            int best_distance = -1;
            
            // 모든 거점을 대상으로 검색 (도시, 관문, 항구)
            for (int i = 0; i < 건물_거점끝; i++)
            {
                pk::building@ dst = pk::get_building(i);
                if (!pk::is_alive(dst)) continue;
                if (dst.get_id() == src.get_id()) continue;
                
                // 도시, 관문, 항구만 대상으로
                if (dst.facility != 시설_도시 and dst.facility != 시설_관문 and dst.facility != 시설_항구) continue;
                
                pk::force@ dst_force = pk::get_force(dst.get_force_id());
                if (!pk::is_alive(dst_force)) continue;
                // 플레이어 직접 컨트롤 또는 위임군단 포함
                if (!dst.is_player()) continue;
                
                int distance = pk::get_building_distance(src.get_id(), dst.get_id(), src.get_force_id());
                if (distance >= 0 and (best_distance == -1 or distance < best_distance))
                {
                    best_distance = distance;
                    @best_player_city = dst;
                }
            }
            
            return best_player_city;
        }

        // 플레이어와의 통로 도시의 세력을 가까운 순으로 정렬해서 반환
        pk::array<pk::building@> get_path_forces_sorted_by_distance(pk::building@ city_building, pk::building@ player_city, pk::force@ force)
        {
            pk::array<pk::building@> result;
            if (!pk::is_alive(city_building) or !pk::is_alive(player_city) or !pk::is_alive(force)) return result;
            
            // 경로상의 도시들 가져오기
            pk::array<pk::building@> path = get_cached_path(city_building, player_city, force);
            if (path.length == 0) return result;
            
            // 세력별로 가장 가까운 도시만 저장 (중복 제거)
            array<bool> force_added(세력_끝, false);
            
            // 거리 순으로 정렬하기 위해 거리와 도시를 함께 저장
            array<int> distances;
            pk::array<pk::building@> cities;
            
            for (int i = 0; i < int(path.length); i++)
            {
                pk::building@ path_city = path[i];
                if (!pk::is_alive(path_city)) continue;
                
                int path_force_id = path_city.get_force_id();
                if (path_force_id < 0 or path_force_id >= 세력_끝) continue;
                
                // 이미 추가된 세력이면 스킵
                if (force_added[path_force_id]) continue;
                
                // 거리 계산
                int distance = pk::get_building_distance(city_building.get_id(), path_city.get_id(), city_building.get_force_id());
                
                // 거리 순으로 삽입
                int insert_pos = 0;
                for (int j = 0; j < int(distances.length); j++)
                {
                    if (distance < distances[j])
                    {
                        insert_pos = j;
                        break;
                    }
                    insert_pos = j + 1;
                }
                
                distances.insertAt(insert_pos, distance);
                cities.insertAt(insert_pos, path_city);
                force_added[path_force_id] = true;
            }
            
            return cities;
        }

        // 우호도와 두 세력을 매개변수로 받아서 해당 우호도 미만이면 친선신청 후 동맹, 아니면 동맹만 신청
        bool try_goodwill_and_alliance_by_threshold(pk::building@ src, pk::building@ dst, pk::force@ force, int goodwill_threshold)
        {
            if (!pk::is_alive(src) or !pk::is_alive(dst) or !pk::is_alive(force)) return false;
            if (force.ally[dst.get_force_id()]) return false; // 이미 동맹이면 스킵
            
            // 자기 자신과 동맹 시도 방지
            if (src.get_force_id() == dst.get_force_id()) return false;
            
            pk::force@ dst_force = pk::get_force(dst.get_force_id());
            if (!pk::is_alive(dst_force)) return false;
            
            // 우호도 확인
            int relations = force.relations[dst_force.get_id()];
            
            // 우호도가 threshold 미만이면 친선 먼저 실행
            if (relations < goodwill_threshold)
            {
                try_goodwill_with_city(force, dst_force);
            }
            
            // 동맹 신청
            return try_alliance_negotiation_with_city(src, force, dst_force);
        }
        
        // 친선 실행 (두 세력의 포스 값을 받아서 친선 실행)
        void try_goodwill_with_city(pk::force@ src_force, pk::force@ dst_force)
        {
            if (!pk::is_alive(src_force) or !pk::is_alive(dst_force)) return;
            
            // 소속 건물 찾기 (무장 선택을 위해 필요)
            pk::list<pk::city@> src_cities = pk::get_city_list(src_force);
            if (src_cities.count == 0) return;
            pk::building@ src = pk::city_to_building(src_cities[0]);
            
            // 금 5천 이상 확인
            if (pk::get_gold(src) < 5000) return;
            
            // 실행할 무장 찾기
            pk::list<pk::person@> person_list = pk::get_idle_person_list(src);
            if (person_list.count == 0) return;
            
            // 정치력이 높은 무장 선택
            person_list.sort(function(a, b)
            {
                return a.stat[무장능력_정치] > b.stat[무장능력_정치];
            });
            
            pk::goodwill_cmd_info cmd;
            @cmd.target = dst_force;
            @cmd.actor = person_list[0];
            cmd.gold = 1000;
            
            pk::command(cmd);
        }
        
        // 동맹 협상 대상 도시 반환
        // 건설 중인 내정시설 개수 확인
        int get_under_construction_dev_count(pk::city@ city)
        {
            if (!pk::is_alive(city)) return 0;
            
            int count = 0;
            for (int i = 0; i < city.max_devs; i++)
            {
                pk::building@ building = city.dev[i].building;
                if (pk::is_alive(building) and !building.completed)
                {
                    count++;
                }
            }
            
            return count;
        }
        
        // 완성된 내정시설 개수 확인
        int get_completed_dev_count(pk::city@ city)
        {
            if (!pk::is_alive(city)) return 0;
            
            int count = 0;
            for (int i = 0; i < city.max_devs; i++)
            {
                pk::building@ building = city.dev[i].building;
                if (pk::is_alive(building) and building.completed)
                {
                    count++;
                }
            }
            
            return count;
        }
        
        // 특정 거리의 플레이어가 아닌 가장 가까운 도시 찾기 (동맹/정전 제외)
        pk::building@ find_nearest_non_player_city_at_distance(pk::building@ src, int target_distance)
        {
            if (!pk::is_alive(src)) return null;
            
            pk::force@ src_force = pk::get_force(src.get_force_id());
            if (!pk::is_alive(src_force)) return null;
            
            pk::building@ best_city = null;
            int best_distance = 999;
            
            for (int i = 0; i < 건물_도시끝; i++)
            {
                pk::building@ dst = pk::get_building(i);
                if (!pk::is_alive(dst)) continue;
                if (dst.get_id() == src.get_id()) continue;
                
                pk::force@ dst_force = pk::get_force(dst.get_force_id());
                if (!pk::is_alive(dst_force)) continue;
                
                // 플레이어 도시 제외
                if (pk::is_player_controlled(dst_force)) continue;
                
                // 동맹이면 제외
                if (src_force.ally[dst.get_force_id()]) continue;
                
                // 정전 상태면 제외
                if (src_force.ceasefire_timer[dst.get_force_id()] > 0) continue;
                
                int distance = pk::get_building_distance(src.get_id(), dst.get_id(), src.get_force_id());
                if (distance == target_distance and distance < best_distance)
                {
                    best_distance = distance;
                    @best_city = dst;
                }
            }
            
            return best_city;
        }
        
        // 플레이어 도시로 가는 중간 도시 찾기 (거리 1에 있는 도시)
        pk::building@ find_intermediate_city_to_player(pk::building@ src)
        {
            if (!pk::is_alive(src)) return null;
            
            // 플레이어 도시 찾기
            pk::building@ player_city = null;
            int player_distance = 999;
            
            for (int i = 0; i < 건물_도시끝; i++)
            {
                pk::building@ dst = pk::get_building(i);
                if (!pk::is_alive(dst)) continue;
                if (dst.get_id() == src.get_id()) continue;
                
                pk::force@ dst_force = pk::get_force(dst.get_force_id());
                if (!pk::is_alive(dst_force)) continue;
                if (!pk::is_player_controlled(dst_force)) continue;
                
                int distance = pk::get_building_distance(src.get_id(), dst.get_id(), src.get_force_id());
                if (distance == 2 and distance < player_distance)
                {
                    player_distance = distance;
                    @player_city = dst;
                }
            }
            
            if (!pk::is_alive(player_city)) return null;
            
            // 중간 도시 찾기 (src와 player_city 사이 거리 1에 있는 도시)
            for (int i = 0; i < 건물_도시끝; i++)
            {
                pk::building@ intermediate = pk::get_building(i);
                if (!pk::is_alive(intermediate)) continue;
                if (intermediate.get_id() == src.get_id() or intermediate.get_id() == player_city.get_id()) continue;
                
                int dist_src = pk::get_building_distance(src.get_id(), intermediate.get_id(), src.get_force_id());
                int dist_player = pk::get_building_distance(intermediate.get_id(), player_city.get_id(), intermediate.get_force_id());
                
                if (dist_src == 1 and dist_player == 1)
                {
                    return intermediate;
                }
            }
            
            return null;
        }
        
        // 경로상의 도시들 찾기 (src에서 player_city까지의 경로에서 특정 거리에 있는 모든 도시)
        // 실제 최단 경로상의 거점만 찾기 (경로의 연속성을 검증)
        // BFS 방식으로 최단 거리까지의 모든 경로 찾기
        pk::array<pk::building@> find_all_cities_on_path(pk::building@ src, pk::building@ player_city, pk::force@ force)
        {
            pk::array<pk::building@> result;
            if (!pk::is_alive(src) or !pk::is_alive(player_city) or !pk::is_alive(force)) return result;
            
            // 전체 거리 계산 (도달 불가능하면 -1 반환)
            int total_distance = pk::get_building_distance(src.get_id(), player_city.get_id(), src.get_force_id());
            if (total_distance <= 1 or total_distance == -1) return result; // 거리가 1 이하거나 도달 불가능하면 통로 거점 없음
            
            // BFS로 최단 거리까지의 모든 경로 찾기
            pk::array<pk::building@> queue;
            pk::array<int> distances;
            pk::array<pk::array<pk::building@>> paths;
            pk::array<pk::array<pk::building@>> found_paths; // 찾은 경로 저장 (첫 번째 경로만)
            
            queue.insertLast(src);
            distances.insertLast(0);
            pk::array<pk::building@> initial_path;
            initial_path.insertLast(src);
            paths.insertLast(initial_path);
            
            while (queue.length > 0)
            {
                pk::building@ current = queue[0];
                int current_distance = distances[0];
                pk::array<pk::building@> current_path = paths[0];
                
                queue.removeAt(0);
                distances.removeAt(0);
                paths.removeAt(0);
                
                // 플레이어 도시에 도달했으면 첫 번째 경로만 저장하고 종료
                if (current.get_id() == player_city.get_id())
                {
                    // 첫 번째로 찾은 최단 경로만 저장하고 종료
                    found_paths.insertLast(current_path);
                    break; // 첫 번째 경로만 찾고 종료
                }
                
                
                // 현재 거점에서 실제 이웃인 거점만 확인
                for (int i = 0; i < 건물_거점끝; i++)
                {
                    pk::building@ next = pk::get_building(i);
                    if (!pk::is_alive(next)) continue;
                    
                    // 경로상에 이미 포함된 거점은 제외 (순환 방지)
                    bool already_in_path = false;
                    for (int j = 0; j < int(current_path.length); j++)
                    {
                        if (current_path[j].get_id() == next.get_id())
                        {
                            already_in_path = true;
                            break;
                        }
                    }
                    if (already_in_path) continue;
                    
                    // 실제 이웃인지 확인
                    if (!pk::is_reachable_neighbor(current.get_id(), next.get_id(), current.get_force_id()))
                        continue;
                    
                    pk::force@ next_force = pk::get_force(next.get_force_id());
                    if (!pk::is_alive(next_force)) continue;
                    
                    // 플레이어 도시는 통과만 가능 (결과에 추가하지 않음)
                    if (pk::is_player_controlled(next_force))
                    {
                        if (next.get_id() == player_city.get_id())
                        {
                            // 플레이어 도시에 도달
                            pk::array<pk::building@> new_path = current_path;
                            new_path.insertLast(next);
                            queue.insertLast(next);
                            distances.insertLast(current_distance + 1);
                            paths.insertLast(new_path);
                        }
                        continue;
                    }
                    
                    // next에서 player_city까지의 거리 확인 (도달 불가능하면 제외)
                    int dist_from_next_to_player = pk::get_building_distance(next.get_id(), player_city.get_id(), next.get_force_id());
                    if (dist_from_next_to_player == -1) continue; // 도달 불가능
                    if (dist_from_next_to_player > total_distance - current_distance - 1) continue; // 거리가 너무 먼 경우 제외
                    
                    // 다음 단계로 진행
                    pk::array<pk::building@> new_path = current_path;
                    new_path.insertLast(next);
                    queue.insertLast(next);
                    distances.insertLast(current_distance + 1);
                    paths.insertLast(new_path);
                }
            }
            
            // 찾은 모든 경로에서 거점 수집 (중복 제거)
            pk::array<bool> added;
            added.length = 건물_거점끝;
            for (int i = 0; i < 건물_거점끝; i++)
                added[i] = false;
            
            for (int i = 0; i < int(found_paths.length); i++)
            {
                pk::array<pk::building@> path = found_paths[i];
                // 경로상의 거점들만 추출 (src와 player_city 제외)
                for (int j = 1; j < int(path.length) - 1; j++)
                {
                    pk::building@ path_city = path[j];
                    if (added[path_city.get_id()]) continue; // 이미 추가된 거점은 제외
                    
                    pk::force@ path_force = pk::get_force(path_city.get_force_id());
                    if (pk::is_alive(path_force))
                    {
                        // 플레이어 통로도 포함 (목표 도시는 이미 제외됨)
                        // 해당 세력이 플레이어 도시에 도달할 수 있는지 확인 (희진세력 등 제외)
                        if (can_deploy_to_city(path_city, player_city))
                        {
                            result.insertLast(path_city);
                            added[path_city.get_id()] = true;
                        }
                    }
                }
            }
            
            return result;
        }
        
        pk::array<pk::building@> find_cities_on_path(pk::building@ src, pk::building@ player_city, int path_distance, pk::force@ force)
        {
            pk::array<pk::building@> result;
            if (!pk::is_alive(src) or !pk::is_alive(player_city) or !pk::is_alive(force)) return result;
            
            // src에서 path_distance 거리에 있고, player_city까지의 거리가 (전체 거리 - path_distance)인 거점 찾기
            int total_distance = pk::get_building_distance(src.get_id(), player_city.get_id(), src.get_force_id());
            if (total_distance <= path_distance) return result;
            
            for (int i = 0; i < 건물_거점끝; i++)
            {
                pk::building@ path_city = pk::get_building(i);
                if (!pk::is_alive(path_city)) continue;
                if (path_city.get_id() == src.get_id() or path_city.get_id() == player_city.get_id()) continue;
                
                pk::force@ path_force = pk::get_force(path_city.get_force_id());
                if (!pk::is_alive(path_force)) continue;
                
                // 플레이어 도시 제외
                if (pk::is_player_controlled(path_force)) continue;
                
                // 동맹이면 제외
                if (force.ally[path_city.get_force_id()]) continue;
                
                // 정전이면 제외
                if (force.ceasefire_timer[path_city.get_force_id()] > 0) continue;
                
                int dist_from_src = pk::get_building_distance(src.get_id(), path_city.get_id(), src.get_force_id());
                int dist_to_player = pk::get_building_distance(path_city.get_id(), player_city.get_id(), path_city.get_force_id());
                
                // src에서 path_distance 거리이고, player_city까지의 거리가 (total_distance - path_distance)인 도시
                if (dist_from_src == path_distance and dist_to_player == (total_distance - path_distance))
                {
                    result.insertLast(path_city);
                }
            }
            
            return result;
        }        

        // 두 세력의 포스 값을 받아서 동맹 협상 진행 (성공 여부 반환)
        bool try_alliance_negotiation_with_city(pk::building@ src, pk::force@ src_force, pk::force@ dst_force)
        {
            if (!pk::is_alive(src) or !pk::is_alive(src_force) or !pk::is_alive(dst_force)) return false;
            
            // 자기 자신과 동맹 시도 방지
            if (src_force.get_id() == dst_force.get_id()) return false;
            
            // 이미 동맹이면 스킵
            if (src_force.ally[dst_force.get_id()]) return false;           
            // 돈 1만 이상 확인
            if (pk::get_gold(src) < 10000) return false;
            
            // 실행할 무장 찾기
            pk::list<pk::person@> person_list = pk::get_idle_person_list(src);
            if (person_list.count == 0) return false;
            
            // 정치력이 높은 무장 선택
            person_list.sort(function(a, b)
            {
                return a.stat[무장능력_정치] > b.stat[무장능력_정치];
            });
            
            pk::alliance_cmd_info cmd;
            @cmd.target = dst_force;
            @cmd.actor = person_list[0];
            cmd.gold = 3000;
            
            bool success = pk::command(cmd);
            
            // 로그 출력
            string src_force_name = pk::decode(pk::get_name(src_force));
            string dst_force_name = pk::decode(pk::get_name(dst_force));
            if (success)
                pk::info(src_force_name + "이 " + dst_force_name + "에게 동맹 신청 성공");
            else
                pk::info(src_force_name + "이 " + dst_force_name + "에게 동맹 신청 실패");
            
            // 동맹 성공 여부 확인
            if (success and src_force.ally[dst_force.get_id()])
            {
                return true;
            }
            
            return false;
        }
        
        // 도시 소속 지형에 적 시설이나 부대 발견 여부 확인
        bool check_enemy_in_territory(pk::building@ city_building)
        {
            if (!pk::is_alive(city_building)) return false;
            
            pk::force@ force = pk::get_force(city_building.get_force_id());
            if (!pk::is_alive(force)) return false;
            
            int city_id = city_building.get_id();
            
            // 도시 주변 범위 확인 (도시는 10칸, 관문/항구는 7칸)
            int range = (city_building.facility == 시설_도시) ? 10 : 7;
            auto territory_range = pk::range(city_building.get_pos(), 0, range);
            
            for (int i = 0; i < territory_range.length; i++)
            {
                pk::point pos = territory_range[i];
                
                // 적 부대 확인
                pk::unit@ unit = pk::get_unit(pos);
                if (pk::is_alive(unit) and pk::is_enemy(city_building, unit))
                {
                    return true;
                }
                
                // 적 시설 확인
                pk::building@ building = pk::get_building(pos);
                if (pk::is_alive(building) and pk::is_enemy(city_building, building))
                {
                    return true;
                }
            }
            
            return false;
        }
        
        // 방어 부대 출정
        void deploy_defense_units(pk::building@ city_building)
        {
            if (!pk::is_alive(city_building)) return;
            
            int city_troops = pk::get_troops(city_building);
            int city_persons = pk::get_person_list(city_building, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            
            // 적 부대 찾기
            pk::unit@ enemy_unit = find_nearest_enemy_in_territory(city_building);
            if (!pk::is_alive(enemy_unit)) return;
            
            // 부대 출정
            bool success = true;
            while(city_troops > 방어명령시_잔여병력기준 and city_persons > 방어명령시_잔여장수기준 and success)
            {
                success = deploy_unit_with_order(city_building, enemy_unit.pos, 부대임무_공격);
                city_troops = pk::get_troops(city_building);
                city_persons = pk::get_person_list(city_building, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            }

            // 소속지인 도시에 적이 나타날 경우 도시를 소속지로 둔 외부에 있는 부대 퇴각 처리
            pk::force@ force = pk::get_force(city_building.get_force_id());
            if (pk::is_alive(force))
            {
                pk::list<pk::unit@> unit_list = pk::get_unit_list(force);
                int city_id = city_building.get_id();
                int city_range = (city_building.facility == 시설_도시) ? 7 : 5;
                
                for (int i = 0; i < unit_list.count; i++)
                {
                    pk::unit@ unit = unit_list[i];
                    if (!pk::is_alive(unit)) continue;
                    if (unit.action_done) continue;
                    
                    // 도시를 소속지로 둔 부대인지 확인
                    int service_base_id = pk::get_service(unit);
                    if (service_base_id != city_id) continue;
                    
                    // 도시 외부에 있는 부대인지 확인 (도시 영토 범위 밖)
                    int dist = pk::get_distance(city_building.get_pos(), unit.pos);
                    if (dist > city_range)
                    {
                        // 퇴각 처리
                        pk::building@ retreat_base = city_building;
                        unit.order = 부대임무_퇴각;
                        unit.target_pos = retreat_base.pos;
                        unit.target_type = 부대임무대상_거점;
                        unit.target = retreat_base.get_id();
                    }
                }
            }
        }
        
        // 영토 내 가장 가까운 적 부대 찾기
        pk::unit@ find_nearest_enemy_in_territory(pk::building@ city_building)
        {
            if (!pk::is_alive(city_building)) return null;
            
            pk::unit@ nearest = null;
            int best_dist = 999;
            
            int range = (city_building.facility == 시설_도시) ? 7 : 5;
            auto territory_range = pk::range(city_building.get_pos(), 0, range);
            
            for (int i = 0; i < territory_range.length; i++)
            {
                pk::unit@ unit = pk::get_unit(territory_range[i]);
                if (pk::is_alive(unit) and pk::is_enemy(city_building, unit))
                {
                    int dist = pk::get_distance(city_building.get_pos(), unit.pos);
                    if (dist < best_dist)
                    {
                        best_dist = dist;
                        @nearest = unit;
                    }
                }
            }
            
            return nearest;
        }
        
        // 원군이 필요한 아군/동맹 거점 찾기
        pk::building@ find_ally_base_needing_reinforcement(pk::building@ city_building, pk::force@ force)
        {
            if (!pk::is_alive(city_building) or !pk::is_alive(force)) return null;
            
            pk::building@ best_base = null;
            int best_distance = 999;
            
            for (int i = 0; i < 건물_끝; i++)
            {
                pk::building@ base = pk::get_building(i);
                if (!pk::is_alive(base)) continue;
                if (base.get_id() == city_building.get_id()) continue;
                
                pk::force@ base_force = pk::get_force(base.get_force_id());
                if (!pk::is_alive(base_force)) continue;
                
                // 같은 세력이거나 동맹인 경우만
                if (base.get_force_id() != force.get_id() and !force.ally[base.get_force_id()]) continue;
                
                // 적이 근처에 있는지 확인
                if (check_enemy_near_base(base))
                {
                    int distance = pk::get_building_distance(city_building.get_id(), base.get_id(), city_building.get_force_id());
                    if (distance >= 0 and distance < best_distance)
                    {
                        best_distance = distance;
                        @best_base = base;
                    }
                }
            }
            
            return best_base;
        }
        
        // 제일 가까운 적대 도시 찾기 (동맹/정전 제외)
        pk::building@ find_nearest_enemy_city(pk::building@ city_building, pk::building@ player_city, pk::force@ force)
        {
            if (!pk::is_alive(city_building) or !pk::is_alive(force)) return null;
            
            if (pk::is_alive(player_city))
            {
                // 통로 도시 목록 가져오기
                pk::array<pk::building@> path_forces = get_path_forces_sorted_by_distance(city_building, player_city, force);
                array<bool> is_path_force(세력_끝, false);
                for (int i = 0; i < int(path_forces.length); i++)
                {
                    pk::building@ path_city = path_forces[i];
                    if (!pk::is_alive(path_city)) continue;
                    is_path_force[path_city.get_force_id()] = true;
                }
                
                // 1단계: 거리 1인 도시 중 통로 도시가 맞고 우호도 40 이하면 반환
                for (int i = 0; i < 건물_도시끝; i++)
                {
                    pk::building@ dst = pk::get_building(i);
                    if (!pk::is_alive(dst)) continue;
                    if (dst.get_id() == city_building.get_id()) continue;
                    
                    pk::force@ dst_force = pk::get_force(dst.get_force_id());
                    if (!pk::is_alive(dst_force)) continue;
                    
                    int dst_force_id = dst.get_force_id();
                    
                    // 거리 1 확인
                    int distance = pk::get_building_distance(city_building.get_id(), dst.get_id(), city_building.get_force_id());
                    if (distance != 1) continue;
                    
                    // 통로 도시이고 우호도 40 이하면 반환
                    if (is_path_force[dst_force_id])
                    {
                        int relations = force.relations[dst_force_id];
                        if (relations < 40)
                        {
                            return dst;
                        }
                    }
                }
            }
            
            // 2단계: 동맹 및 정전 제외하고 남은 도시 중에서 제일 가까운 도시 반환
            pk::building@ nearest_enemy_city = null;
            int nearest_enemy_distance = 999;
            
            for (int i = 0; i < 건물_도시끝; i++)
            {
                pk::building@ dst = pk::get_building(i);
                if (!pk::is_alive(dst)) continue;
                if (dst.get_id() == city_building.get_id()) continue;
                
                pk::force@ dst_force = pk::get_force(dst.get_force_id());
                if (!pk::is_alive(dst_force)) continue;
                
                int dst_force_id = dst.get_force_id();
                
                // 자기 세력이면 제외
                if (force.get_id() == dst_force_id) continue;
                
                // 동맹이면 제외
                if (force.ally[dst_force_id]) continue;
                
                // 정전이면 제외
                if (force.ceasefire_timer[dst_force_id] > 0) continue;
                
                int distance = pk::get_building_distance(city_building.get_id(), dst.get_id(), city_building.get_force_id());
                if (distance >= 0 and distance < nearest_enemy_distance)
                {
                    nearest_enemy_distance = distance;
                    @nearest_enemy_city = dst;
                }
            }
            
            return nearest_enemy_city;
        }
        
        // 플레이어 도시로 출정 가능 여부 확인 (거리 3 이하만 가능)
        bool can_deploy_to_city(pk::building@ src, pk::building@ dst)
        {
            if (!pk::is_alive(src) or !pk::is_alive(dst)) return false;
            
            // 거리 확인 (-1이면 경로 없음, 3 초과면 출정 불가)
            int distance = pk::get_building_distance(src.get_id(), dst.get_id(), src.get_force_id());
            return (distance != -1 and distance <= 3);
        }
        
        // 플레이어 공격 가능한 세력 찾기 (can_deploy_to_city 사용, 가까운 순으로 정렬)
        pk::array<pk::building@> find_cities_can_attack_player(pk::building@ city_building, pk::building@ player_city)
        {
            pk::array<pk::building@> result;
            if (!pk::is_alive(city_building) or !pk::is_alive(player_city)) return result;
            
            // 거리 순으로 정렬하기 위해 거리와 도시를 함께 저장
            array<int> distances;
            pk::array<pk::building@> cities;
            
            for (int i = 0; i < 건물_도시끝; i++)
            {
                pk::building@ dst = pk::get_building(i);
                if (!pk::is_alive(dst)) continue;
                if (dst.get_id() == player_city.get_id()) continue;
                
                pk::force@ dst_force = pk::get_force(dst.get_force_id());
                if (!pk::is_alive(dst_force)) continue;
                
                // 플레이어 도시 제외
                if (dst.is_player()) continue;
                
                // 플레이어 공격 가능한지 확인
                if (can_deploy_to_city(dst, player_city))
                {
                    // 거리 계산
                    int distance = pk::get_building_distance(city_building.get_id(), dst.get_id(), city_building.get_force_id());
                    if (distance == -1) continue; // 도달 불가능한 경우 스킵
                    
                    // 거리 순으로 삽입
                    int insert_pos = 0;
                    for (int j = 0; j < int(distances.length); j++)
                    {
                        if (distance < distances[j])
                        {
                            insert_pos = j;
                            break;
                        }
                        insert_pos = j + 1;
                    }
                    
                    distances.insertAt(insert_pos, distance);
                    cities.insertAt(insert_pos, dst);
                }
            }
            
            return cities;
        }
        
        
        
        // 통로/동맹/정전 제외한 제일 가까운 도시 찾기
        // 플레이어 거점에 출정할 수 있는 도시 찾기 (바로 옆이거나 통로 세력과 동맹인 세력)
        // 통로 세력 정보를 문자열로 반환
        string get_path_forces_string(pk::building@ city_building, pk::force@ force, pk::building@ player_city)
        {
            if (!pk::is_alive(player_city)) return "";
            
            int player_city_distance = pk::get_building_distance(city_building.get_id(), player_city.get_id(), city_building.get_force_id());
            if (player_city_distance <= 1 or player_city_distance > 4) return "";
            
            array<string> path_force_names;
            for (int path_distance = 1; path_distance < player_city_distance; path_distance++)
            {
                pk::array<pk::building@> path_cities = find_cities_on_path(city_building, player_city, path_distance, force);
                for (int j = 0; j < int(path_cities.length); j++)
                {
                    pk::building@ path_city = path_cities[j];
                    if (pk::is_alive(path_city))
                    {
                        pk::force@ path_force = pk::get_force(path_city.get_force_id());
                        if (pk::is_alive(path_force))
                        {
                            string path_force_name = pk::decode(pk::get_name(path_force));
                            if (path_force_names.find(path_force_name) < 0)
                                path_force_names.insertLast(path_force_name);
                        }
                    }
                }
            }
            
            if (path_force_names.length > 0)
            {
                string result = " / 통로 세력: ";
                for (int i = 0; i < int(path_force_names.length); i++)
                {
                    if (i > 0) result += ", ";
                    result += path_force_names[i];
                }
                return result;
            }
            
            return "";
        }
        
        // 거점 근처에 적이 있는지 확인
        bool check_enemy_near_base(pk::building@ base)
        {
            if (!pk::is_alive(base)) return false;
            
            int range = (base.facility == 시설_도시) ? 7 : 5;
            auto nearby_range = pk::range(base.get_pos(), 0, range);
            
            for (int i = 0; i < nearby_range.length; i++)
            {
                pk::unit@ unit = pk::get_unit(nearby_range[i]);
                if (pk::is_alive(unit) and pk::is_enemy(base, unit))
                {
                    return true;
                }
            }
            
            return false;
        }
        
        // 원군 부대 출정
        void deploy_reinforcement_units(pk::building@ city_building, pk::building@ target_base)
        {
            if (!pk::is_alive(city_building) or !pk::is_alive(target_base)) return;
            
            int city_troops = pk::get_troops(city_building);
            int city_persons = pk::get_person_list(city_building, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            int reserve_persons = pk::min(원군명령시_잔여장수기준, int(city_persons * 0.2f));
           
            // 부대 출정
            bool success = true;
            while(city_troops > 원군명령시_잔여병력기준 and city_persons > reserve_persons and success)
            {
                success = deploy_unit_with_order(city_building, target_base.pos, 부대임무_이동);
                city_troops = pk::get_troops(city_building);
                city_persons = pk::get_person_list(city_building, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            }
        }
        
        // 공격 대상 도시 찾기 (1순위 플레이어)
        pk::building@ find_attack_target(pk::building@ city_building, pk::force@ force)
        {
            if (!pk::is_alive(city_building) or !pk::is_alive(force)) return null;
            
            pk::building@ player_city = null;
            pk::building@ enemy_city = null;
            int player_distance = 999;
            int enemy_distance = 999;
            
            for (int i = 0; i < 건물_도시끝; i++)
            {
                pk::building@ dst = pk::get_building(i);
                if (!pk::is_alive(dst)) continue;
                if (dst.get_id() == city_building.get_id()) continue;
                
                pk::force@ dst_force = pk::get_force(dst.get_force_id());
                if (!pk::is_alive(dst_force)) continue;
                
                // 동맹이면 제외
                if (force.ally[dst.get_force_id()]) continue;
                
                // 정전이면 제외
                if (force.ceasefire_timer[dst.get_force_id()] > 0) continue;
                
                int distance = pk::get_building_distance(city_building.get_id(), dst.get_id(), city_building.get_force_id());
                
                // 플레이어 도시 우선
                if (dst_force.is_player())
                {
                    if (distance >= 0 and distance < player_distance)
                    {
                        player_distance = distance;
                        @player_city = dst;
                    }
                }
                else
                {
                    if (distance >= 0 and distance < enemy_distance)
                    {
                        enemy_distance = distance;
                        @enemy_city = dst;
                    }
                }
            }
            
            // 플레이어 도시가 있으면 우선 반환
            if (pk::is_alive(player_city)) return player_city;
            return enemy_city;
        }
        
        // 정복 임무를 맡은 부대의 타겟 찾기
        pk::building@ get_conquer_target_from_existing_units(pk::building@ city_building)
        {
            if (!pk::is_alive(city_building)) return null;
            
            pk::force@ force = pk::get_force(city_building.get_force_id());
            if (!pk::is_alive(force)) return null;
            
            pk::list<pk::unit@> unit_list = pk::get_unit_list(force);
            
            for (int i = 0; i < unit_list.count; i++)
            {
                pk::unit@ unit = unit_list[i];
                if (!pk::is_alive(unit)) continue;
                
                // 정복 임무를 맡은 부대가 있으면 그 타겟 반환
                if (unit.order == 부대임무_정복 and unit.target != -1)
                {
                    pk::building@ target = pk::get_building(unit.target);
                    if (pk::is_alive(target))
                    {
                        return target;
                    }
                }
            }
            
            return null;
        }
        
        // 공격 부대 출정
        void deploy_attack_units(pk::building@ city_building, pk::building@ target_city)
        {
            pk::info("공격 부대 출정 처리 시작");
            if (!pk::is_alive(city_building)) return;
            
            // 목표가 플레이어 도시가 아니면 기존 정복 부대의 타겟으로 출정
            bool is_player_target = pk::is_alive(target_city) and target_city.is_player();
            if (!is_player_target)
            {
                // 이미 정복 임무를 맡은 부대가 있으면 그 부대의 타겟으로 출정
                pk::building@ existing_conquer_target = get_conquer_target_from_existing_units(city_building);
                if (pk::is_alive(existing_conquer_target))
                {
                    pk::info("정복 임무를 맡은 부대가 있음 - 그 타겟으로 출정: " + pk::decode(pk::get_name(existing_conquer_target)));
                    @target_city = existing_conquer_target;
                }
            }
            
            if (!pk::is_alive(target_city)) return;
            
            int city_troops = pk::get_troops(city_building);
            
            if (공격명령시_최소병력기준 > city_troops) return;
            
            int city_persons = pk::get_person_list(city_building, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            
            // 건물 지어지는 것, 완성된 것, 최대 개발지 개수 변수
            pk::city@ city = pk::get_city(city_building.get_id());
            int under_construction_devs = get_under_construction_dev_count(city);
            int completed_devs = get_completed_dev_count(city);
            int max_devs = city.max_devs;
            int remaining_officers = pk::get_person_list(city_building, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            int reserve_persons = pk::min(공격명령시_잔여장수기준, int(remaining_officers * 0.2f));

            if (completed_devs + under_construction_devs == max_devs)
            {
                bool success = true;
                // 부대 출정
                while(city_troops > 공격명령시_잔여병력기준 and city_persons > reserve_persons and success)
                {
                    success = deploy_unit_with_order(city_building, target_city.pos, 부대임무_정복);
                    city_troops = pk::get_troops(city_building);                
                    city_persons = pk::get_person_list(city_building, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
                }
            }
        }
        
        // 부대 출정 (명령 포함)
        bool deploy_unit_with_order(pk::building@ src, pk::point target_pos, int order)
        {
            if (!pk::is_alive(src)) return false;
            
            // 실행할 무장 찾기
            pk::info("실행할 무장 찾기");
            pk::list<pk::person@> person_list = pk::get_idle_person_list(src);
            if (person_list.count < 3) return false;
            
            // 무력, 통솔, 지력이 높은 무장 선택 (중복 제외)
            pk::person@ leader = null;
            pk::person@ member1 = null;
            pk::person@ member2 = null;
            int max_무력 = -1;
            int max_통솔 = -1;
            int max_지력 = -1;
            
            // 먼저 군주가 있으면 무조건 군주를 주장으로 선택
            for (int i = 0; i < person_list.count; i++)
            {
                pk::person@ person = person_list[i];
                if (person.mibun == 신분_군주)
                {
                    @leader = person;
                    pk::info("군주를 주장으로 선택: " + pk::decode(pk::get_name(person)));
                    break;
                }
            }
            
            // 군주가 없으면 무력 최고 선택
            if (leader is null)
            {
                pk::info("무력이 높은 무장 선택");
                for (int i = 0; i < person_list.count; i++)
                {
                    pk::person@ person = person_list[i];
                    if (person.stat[무장능력_무력] > max_무력)
                    {
                        max_무력 = person.stat[무장능력_무력];
                        @leader = person;
                    }
                }
            }
            
            // 통솔 최고 선택 (leader 제외)
            for (int i = 0; i < person_list.count; i++)
            {
                pk::person@ person = person_list[i];
                if (person is leader) continue;
                if (person.stat[무장능력_통솔] > max_통솔)
                {
                    max_통솔 = person.stat[무장능력_통솔];
                    @member1 = person;
                }
            }
            
            // 지력 최고 선택 (leader, member1 제외)
            for (int i = 0; i < person_list.count; i++)
            {
                pk::person@ person = person_list[i];
                if (person is leader or person is member1) continue;
                if (person.stat[무장능력_지력] > max_지력)
                {
                    max_지력 = person.stat[무장능력_지력];
                    @member2 = person;
                }
            }
            
            int deploy_troops = pk::get_command(leader);
            
            int water_weapon_id = 병기_주가;

            if (pk::get_weapon_amount(src, 병기_누선) > 0) water_weapon_id = 병기_누선;
            else if (pk::get_weapon_amount(src, 병기_투함) > 0) water_weapon_id = 병기_투함;
            else water_weapon_id = 병기_주가;      
            
            int unit_troops = (pk::max(3000, deploy_troops));
            
            // 출진 명령 정보 생성
            pk::info("출진 부대 " + pk::decode(pk::get_name(leader)) + " " + pk::decode(pk::get_name(member1)) + " " + pk::decode(pk::get_name(member2)));
            pk::com_deploy_cmd_info cmd;
            @cmd.base = src;
            cmd.type = 부대종류_전투;
            cmd.member[0] = leader.get_id();
            cmd.member[1] = member1.get_id();
            cmd.member[2] = member2.get_id();
            cmd.gold = (pk::get_gold(src) >= 10000) ? 1000 : 0;
            cmd.food = int(unit_troops * 1.2f);   //240일
            cmd.troops = unit_troops;
            
            // 병기 설정 - 제일 많이 가지고 있는 병기로 설정
            pk::info("병기 설정");
            int best_weapon_id = -1;
            int max_weapon_amount = 0;
            
            // 제일 많이 가지고 있는 병기 - 병종은 출진병종무장설정에서 변경됨
            for (int weapon_id = 병기_창; weapon_id <= 병기_군마; weapon_id++)
            {
                int weapon_amount = pk::get_weapon_amount(src, weapon_id);
                if (weapon_amount > max_weapon_amount)
                {
                    max_weapon_amount = weapon_amount;
                    best_weapon_id = weapon_id;
                }
            }

            
            if (best_weapon_id != -1 and max_weapon_amount > 0)
            {
                cmd.weapon_id[0] = best_weapon_id;
                cmd.weapon_amount[0] = pk::min(deploy_troops, max_weapon_amount);
            }
            else
            {
                pk::info("병기 부족");
                return false;
            }
            
            cmd.weapon_id[1] = water_weapon_id;
            cmd.weapon_amount[1] = (water_weapon_id == 병기_주가)? 0 : 1;
            
            cmd.order = order;
            cmd.target_pos = target_pos;
            
            // 출진
            pk::info("출진 실행");
            int unit_id = pk::command(cmd);
            if (unit_id != -1)
            {
                pk::unit@ unit = pk::get_unit(unit_id);
                if (pk::is_alive(unit))
                {
                    unit.action_done = true;
                    pk::info("출진 실행 완료");
                    return true;
                }
            }
            return false;
          
        }
    }

    Main main;
}
