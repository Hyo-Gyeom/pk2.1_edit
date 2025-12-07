/*
// 작성자:
// 설명: 거점에서 병력 출정 처리
// Update: 
//
*/

namespace 거점병력출정
{
    
    //=======================================================================================
    // 설정변수
    const bool 거점병력출정_설정 = true;       // 거점 병력 출정 기능 활성화
    
    //=======================================================================================
    
	class Main
	{
		Main()
		{
			// 111 이벤트로 변경하여 턴 시작 시 먼저 실행되도록 설정 (priority 0으로 부대교전설정보다 먼저 실행)
			pk::bind(111, 0, pk::trigger111_t(onTurnStart));
		}
        
		// 턴 시작 시 거점병력출정 실행 (부대교전설정보다 먼저 실행)
		void onTurnStart(pk::force@ force)
		{
			int force_id = force.get_id();

            if (!pk::is_campaign())
			{
                // 플레이어, 이민족 세력은 제외
                if (!force.is_player() and pk::is_normal_force(force_id))
                {
                    if (거점병력출정_설정)
                    {
                        func_troop_dispatch(force);
                    }
                }
			}
		}
        
        //---------------------------------------------------------------------------------------
        //          거점 병력 출정 실행 함수
        //---------------------------------------------------------------------------------------
        
        // 거점 병력 출정 처리
        void func_troop_dispatch(pk::force@ force)
        {
            if (!pk::is_alive(force)) return;
            
            // 병영을 포함한 내정건물 8개 이상 있고 병력 3만이 넘으면 병력 출정 준비
            int force_id = force.get_id();
            
            // 세력의 모든 도시 순회
            for (int city_id = 0; city_id < 도시_끝; city_id++)
            {
                pk::city@ city = pk::get_city(city_id);
                if (!pk::is_alive(city)) continue;
                if (city.get_force_id() != force_id) continue;
                
                pk::building@ city_building = pk::city_to_building(city);
                if (!pk::is_alive(city_building)) continue;

                // 플레이어 도시 간의 거리 확인
                int player_city_distance = find_nearest_player_city(city_building);
                
                // 거리가 2면 지나가는 도시와 최우선 동맹 협상, 그 다음에 다른 거리 1의 가까운 도시와 협상
                if (player_city_distance == 2)
                {
                    // 지나가는 도시 찾기 (거리 1에 있는 중간 도시)
                    pk::building@ intermediate_city = find_intermediate_city_to_player(city_building);
                    if (pk::is_alive(intermediate_city) and !force.ally[intermediate_city.get_force_id()])
                    {
                        try_alliance_negotiation_with_city(city_building, intermediate_city);                        
                    }                    
                    // 이미 중간도시와 동맹이 아니면 다른 거리 1의 가까운 도시와 협상                    
                    else
                    {
                        pk::building@ dist1_city = find_nearest_non_player_city_at_distance(city_building, 1);
                        if (pk::is_alive(dist1_city) and !force.ally[dist1_city.get_force_id()])
                        {
                            try_alliance_negotiation_with_city(city_building, dist1_city);                        
                        }
                    }
                }
                // 거리가 1이면 가까운 도시부터 동맹 협상 진행
                else if (player_city_distance == 1)
                {
                    pk::building@ dist1_city = find_nearest_non_player_city_at_distance(city_building, 1);
                    if (pk::is_alive(dist1_city))
                    {
                        try_alliance_negotiation_with_city(city_building, dist1_city);
                    }
                }
                // 병영을 포함한 내정건물 8개 이상 있고 병력 3만이 넘으면 병력 출정 준비
                int completed_devs = get_completed_dev_count(city);
                int city_troops = pk::get_troops(city_building);
                
                if (completed_devs >= 8 and has_barracks(city) and city_troops >= 30000)
                {
                    // 플레이어의 도시간의 거리가 2일때 지나치는 도시가 동맹이 아니면 이 도시를 제외한 다른 제일 가까운 도시에 출정
                    // 최소남길 병력 15000명 또는 남은 장수 3명 이하일 때 까지 부대 부대임무_정복으로로 출정
                    if (player_city_distance == 2)
                    {
                        // 지나치는 도시 찾기
                        pk::building@ intermediate_city = find_intermediate_city_to_player(city_building);
                        
                        // 지나치는 도시가 동맹이 아니면 이 도시를 제외한 다른 제일 가까운 도시에 출정
                        if (pk::is_alive(intermediate_city) and !force.ally[intermediate_city.get_force_id()])
                        {
                            // 중간 도시를 제외한 다른 제일 가까운 도시 찾기
                            pk::building@ target_city = find_nearest_city_excluding(city_building, intermediate_city);
                            
                            if (pk::is_alive(target_city))
                            {
                                int remaining_troops = city_troops;
                                int remaining_persons = pk::get_person_list(city_building, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
                                
                                // 최소남길 병력 15000명 또는 남은 장수 3명 이하일 때 까지 부대 출정
                                while (remaining_troops > 15000 and remaining_persons > 3)
                                {
                                    int deploy_troops = pk::min(50000, remaining_troops - 15000);
                                    if (deploy_conquer_unit(city_building, target_city, deploy_troops))
                                    {
                                        remaining_troops -= deploy_troops;
                                        remaining_persons = pk::get_person_list(city_building, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
                                    }
                                    else
                                    {
                                        break; // 출정 실패 시 중단
                                    }
                                }
                            }
                        }
                    }
                    




                }
        
            
            }

            // 도시 간 거리 2이하에서 플레이어 도시가 없으면 제일 가까운 적 정복임무부여하여 공격
            
        }
        
        //---------------------------------------------------------------------------------------
        //          내정시설 병영 확인 및 건설 함수
        //---------------------------------------------------------------------------------------
       
        // 도시에 완성된 내정시설이 몇 개 있는지 확인
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
        
        // 플레이어 도시 간의 거리 반환 (없으면 -1)
        int find_nearest_player_city(pk::building@ src)
        {
            if (!pk::is_alive(src)) return -1;
            
            int best_distance = -1;
            
            for (int i = 0; i < 건물_도시끝; i++)
            {
                pk::building@ dst = pk::get_building(i);
                if (!pk::is_alive(dst)) continue;
                if (dst.get_id() == src.get_id()) continue;
                
                pk::force@ dst_force = pk::get_force(dst.get_force_id());
                if (!pk::is_alive(dst_force)) continue;
                if (!pk::is_player_controlled(dst_force)) continue;
                
                int distance = pk::get_building_distance(src.get_id(), dst.get_id(), src.get_force_id());
                if (distance >= 0 and (best_distance == -1 or distance < best_distance))
                {
                    best_distance = distance;
                }
            }
            
            return best_distance;
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
        
        // 특정 도시와 동맹 협상 진행
        void try_alliance_negotiation_with_city(pk::building@ src, pk::building@ dst)
        {
            if (!pk::is_alive(src) or !pk::is_alive(dst)) return;
            
            pk::force@ src_force = pk::get_force(src.get_force_id());
            pk::force@ dst_force = pk::get_force(dst.get_force_id());
            if (!pk::is_alive(src_force) or !pk::is_alive(dst_force)) return;
            
            // 이미 동맹이면 스킵
            if (src_force.ally[dst.get_force_id()]) return;
            
            // 실행할 무장 찾기
            pk::list<pk::person@> person_list = pk::get_idle_person_list(src);
            if (person_list.count == 0) return;
            
            // 정치력이 높은 무장 선택
            person_list.sort(function(a, b)
            {
                return a.stat[무장능력_정치] > b.stat[무장능력_정치];
            });
            
            pk::alliance_cmd_info cmd;
            @cmd.target = dst_force;
            @cmd.actor = person_list[0];
            cmd.gold = pk::min(5000, pk::get_gold(src));
            
            bool success = pk::command(cmd);
            // success가 true면 동맹 협상 성공, false면 실패
        }
        
        // 동맹이나 정전 세력이 아닌 제일 가까운 도시의 세력과 동맹 협상 진행 (플레이어 도시 제외)
        void try_alliance_negotiation(pk::building@ src)
        {
            if (!pk::is_alive(src)) return;
            
            pk::force@ src_force = pk::get_force(src.get_force_id());
            if (!pk::is_alive(src_force)) return;
            
            pk::building@ best_city = null;
            int best_distance = 999;
            
            // 모든 도시를 순회하여 동맹도 정전도 아닌 가장 가까운 도시 찾기
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
                
                // 거리 계산
                int distance = pk::get_building_distance(src.get_id(), dst.get_id(), src.get_force_id());
                if (distance >= 0 and distance < best_distance)
                {
                    best_distance = distance;
                    @best_city = dst;
                }
            }
            
            // 가장 가까운 도시를 찾았으면 동맹 협상 진행
            if (pk::is_alive(best_city))
            {
                try_alliance_negotiation_with_city(src, best_city);
            }
        }
        
        // 특정 도시를 제외한 다른 제일 가까운 도시 찾기
        pk::building@ find_nearest_city_excluding(pk::building@ src, pk::building@ exclude_city)
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
                if (pk::is_alive(exclude_city) and dst.get_id() == exclude_city.get_id()) continue; // 제외할 도시 제외
                
                pk::force@ dst_force = pk::get_force(dst.get_force_id());
                if (!pk::is_alive(dst_force)) continue;
                
                // 플레이어 도시 제외
                if (pk::is_player_controlled(dst_force)) continue;
                
                // 동맹이면 제외
                if (src_force.ally[dst.get_force_id()]) continue;
                
                // 정전 상태면 제외
                if (src_force.ceasefire_timer[dst.get_force_id()] > 0) continue;
                
                int distance = pk::get_building_distance(src.get_id(), dst.get_id(), src.get_force_id());
                if (distance >= 0 and distance < best_distance)
                {
                    best_distance = distance;
                    @best_city = dst;
                }
            }
            
            return best_city;
        }
        
        // 정복 임무로 부대 출정
        bool deploy_conquer_unit(pk::building@ src, pk::building@ dst, int available_troops)
        {
            if (!pk::is_alive(src) or !pk::is_alive(dst)) return false;
            
            // 실행할 무장 찾기
            pk::list<pk::person@> person_list = pk::get_idle_person_list(src);
            if (person_list.count < 3) return false; // 최소 3명 필요
            
            // 통솔이 높은 무장 선택
            person_list.sort(function(a, b)
            {
                return a.stat[무장능력_통솔] > b.stat[무장능력_통솔];
            });
            
            // 출진 병력 계산 (최소 1만, 최대 available_troops)
            int deploy_troops = pk::max(10000, pk::min(available_troops, 50000));
            
            // 출진 명령 정보 생성
            pk::com_deploy_cmd_info cmd;
            @cmd.base = src;
            cmd.type = 부대종류_전투;
            cmd.member[0] = person_list[0].get_id();
            cmd.member[1] = person_list[1].get_id();
            cmd.member[2] = person_list[2].get_id();
            cmd.gold = pk::min(0, pk::get_gold(src));
            cmd.food = pk::min(deploy_troops * 100, pk::get_food(src));
            cmd.troops = deploy_troops;
            
            // 병기 설정 - 제일 많이 가지고 있는 병기로 설정
            int best_weapon_id = -1;
            int max_weapon_amount = 0;
            
            for (int weapon_id = 병기_창; weapon_id <= 병기_군마; weapon_id++)
            {
                int weapon_amount = pk::get_weapon_amount(src, weapon_id);
                if (weapon_amount > max_weapon_amount)
                {
                    max_weapon_amount = weapon_amount;
                    best_weapon_id = weapon_id;
                }
            }
            
            // 가장 많이 가지고 있는 병기가 있으면 설정
            if (best_weapon_id != -1 and max_weapon_amount > 0)
            {
                cmd.weapon_id[0] = best_weapon_id;
                cmd.weapon_amount[0] = pk::min(max_weapon_amount, deploy_troops);
            }
            
            cmd.order = 부대임무_정복;
            cmd.target_pos = dst.pos;
            
            // 출진
            int unit_id = pk::command(cmd);
            if (unit_id != -1)
            {
                pk::unit@ unit = pk::get_unit(unit_id);
                if (pk::is_alive(unit))
                {
                    unit.action_done = true;
                    return true;
                }
            }
            
            return false;
        }
        
    }
	Main main;
}

