/*
// 제작자:
// 설명: 부대 교전 시 목표 선택, 이동, 공격, 거점 입성 등을 최적화하는 스크립트
// Update: 2025.11.25 / 초기 버전 작성
//         - 적 유닛/거점 공격 로직 추가
//         - 간접 무기 원거리 공격 유지 로직 추가
//         - 공성 무기 처리 로직 추가
//         - 거점 입성 시 병력 용량 체크 및 대체 거점 탐색 로직 추가
//         - 목표 재선정 가중치 시스템 추가
//
*/

namespace 부대교전설정
{
    //=======================================================================================
    // 설정변수
    const int  부대목표변경_세력대상 = 0;     // 0: 컴퓨터AI 와 플레이어_위임군단 모두,  1: 플레이어_위임군단만, 2: 컴퓨터AI만
    
    // 이하 공격 가중치와 방어 가중치에 적용 성장 가능성을 기본으로 성장하지 않는다면 최대 부대 공격력이나 방어력으로 설정 필요
    // 그대로 사용시 다른 가중치의 비해 2배 높은 가중치를 가짐
    int 최대_부대_공방능력치 = 255; 
    // 가중치 추가옵션 설정 (기본 병종/병력(5천이하)만으로 판단)
    const bool 부대목표_병력가중치적용 = true; // 기본 병력 가중치를 2배
    const bool 부대목표_공격가중치적용 = true; // 타겟의 공격력이 높을수록 가중치 높음
    const bool 부대목표_방어가중치적용 = true; // 타겟의 방어력이 낮을수록 가중치 높용    
    const bool 부대목표_거리가중치적용 = true; // 진영 유지를 위해 가까이에 있는 적에 가중치 부여 및 최소한의 움직임으로 공격 가능하도록 설정


    // 교전시  공성병기기 목표 설정 - 기본적으로 거점 이동 및 사거리내에 거점이 있을 시 우선 공격
    const bool 공성병기_전투_참여여부  = true; // 공성병기가 부대간 전투에 참여할지 설정
    const int  공성병기_전투_기준기력  = 75;   // 공성병기가 부대간 전투에 참여할 기력기준 (기력 이상인 경우 참여)
    
    // 간접무기 사거리 설정
    const int  정란_기본사거리 = 3;   // 정란의 기본 사거리
    
    const bool 병력부족부대_복귀허용여부 = true;    // 궤멸직전 AI부대 소속거점으로 복귀 기능 (유저부대 미적용)
    const int  병력부족부대_복귀병력기준 = 1000;     // 소속거점 복귀 기준 병력, 기준 미만 시 거점 복귀함


    //=======================================================================================
    
    
	class Main
	{
        
        Main()
		{
            pk::bind(112, pk::trigger112_t(onTurnStart));
                        
            pk::bind(170, pk::trigger170_t(onUnitCreate));
        }
        //---------------------------------------------------------------------------
        
        //세력 턴 시작 시 부대 목표 재설정
        void onTurnStart(pk::force@ force)
        {            
            pk::info(pk::format("{} 부대 교전 턴 시작",pk::decode(pk::get_name(force))));         

            // 부대 교전 오더 처리
            handle_force_units(force); 
            pk::info(pk::format("{} 부대 교전 턴 종료", pk::decode(pk::get_name(force))));
        }
        
        // 유닛 생성 시 발동
        void onUnitCreate(pk::unit@ unit, int type)
        {
            if (!pk::is_alive(unit)) return;
            if (!check_unit_force_option(unit)) return;
            
            // 수송부대만 처리
            if (unit.type != 부대종류_수송) return;
            
            // 거점 목표가 있는 경우만 처리
            if (unit.target_type != 부대임무대상_거점) return;
            if (unit.target == -1) return;
            pk::info("수송부대");
            pk::building@ target_base = pk::get_building(unit.target);
            if (!pk::is_alive(target_base)) return;
            
            // 거점 목표에 도달 가능하면 바로 도시로 이동
            handle_base_entry(unit, target_base);
        }
                
        //---------------------------------------------------------------------------

        
        // 세력 확인 함수 (unit용)
        bool check_unit_force_option(pk::unit@ unit_t)
        {            
            if (!pk::is_alive(unit_t)) return false;
            if (!pk::is_normal_force(unit_t.get_force_id())) return false;
            
            // 0: 컴퓨터AI 와 플레이어_위임군단 모두
            if (부대목표변경_세력대상 == 0 and !pk::is_player_controlled(unit_t)) return true;
            // 1: 플레이어_위임군단만
            if (부대목표변경_세력대상 == 1 and (unit_t.is_player() and !pk::is_player_controlled(unit_t))) return true;
            // 2: 컴퓨터AI만
            if (부대목표변경_세력대상 == 2 and !unit_t.is_player()) return true;
                        
            return false;
        }


        // 세력 부대 처리
        void handle_force_units(pk::force@ force)
        {
            if (!pk::is_alive(force)) return;
            int force_id = force.get_id();
            
            // 세력의 모든 부대 불러오기
            pk::list<pk::unit@> unit_list = pk::get_unit_list(force);
            
            for (int i = 0; i < unit_list.count; i++)
            {
                pk::unit@ unit = unit_list[i];
                if (!check_unit_force_option(unit)) continue;
                if (!pk::is_alive(unit) or unit.type != 부대종류_전투 or unit.status != 부대상태_통상) continue;
                if (unit.action_done) continue;
               
                // 퇴각해야하는 부대인지 확인 후 퇴각 처리 - 병력이 1000이하 또는 공성병기인데 기력이 10미만이면 퇴각 or 주악이 있으면 기력이 10 미만이여도 퇴각하지 않음 
                if (retreat_weak_unit(unit))
                {
                    pk::building@ retreat_base = get_retreat_base(unit);
                    if (pk::is_alive(retreat_base))
                    {   
                        pk::point retreat_pos = retreat_base.pos;                        
                        if (pk::is_valid_pos(retreat_pos))
                        {
                            int retreat_base_id = pk::get_hex_object_id(retreat_base);
                            unit.order = 부대임무_퇴각;
                            unit.target_pos = retreat_base.pos;
                            unit.target_type = 부대임무대상_거점;
                            unit.target = retreat_base.get_id();
                            
                            // 들어갈 수 있으면 들어가고 아니면 이동 명령 실행
                            if (check_target_base_in_movable_range(unit, retreat_base))
                            {
                                pk::array<pk::point> path;
                                path.insertLast(unit.pos);
                                path.insertLast(unit.target_pos);
                                pk::move(unit, path);
                            }
                            else
                                pk::run_order(unit);                   
                            
                            // 거점이 유효함으로 명령 실행 후 종료
                            unit.action_done = true;
                            continue;
                        } // 거점이 유효판단
                    } // 퇴각 거점 유효 판단
                } // 퇴각 처리 종료

                // 부대 정보 로그
                pk::person@ leader = pk::get_person(unit.leader);
                string unit_name = (pk::is_alive(leader)) ? pk::decode(pk::get_name(leader)) : "??";
                pk::hex_object@ target_obj = pk::get_hex_object(unit.target_pos);
                int target_id = (pk::is_alive(target_obj)) ? pk::get_hex_object_id(target_obj) : -1;

                // 부대 오더와 타겟 저장
                int saved_order = unit.order;
                int saved_target_id = -1;
                pk::hex_object@ saved_target_obj = pk::get_hex_object(unit.target_pos);
                if (pk::is_alive(saved_target_obj))
                    saved_target_id = pk::get_hex_object_id(saved_target_obj);
                
                                
                // 공성병기, 간접병기, 근접병기, 수군 체크
                bool is_siege_weapon = (unit.weapon == 병기_충차 or unit.weapon == 병기_목수 or unit.weapon == 병기_정란 or unit.weapon == 병기_투석);
                bool is_indirect_weapon = (unit.weapon == 병기_노 or unit.weapon == 병기_정란 or unit.weapon == 병기_투석);
                bool is_ship_weapon = (unit.weapon == 병기_주가 or unit.weapon == 병기_누선 or unit.weapon == 병기_투함);
                bool is_melee_unit = (unit.weapon == 병기_군마 or unit.weapon == 병기_극 or unit.weapon == 병기_창);
                // 공격임무가 아닌 것들의 반대 = 공격 유닛 (공격, 수복, 정복, 섬멸, 물림)
                bool is_combat_unit = !(unit.order == 부대임무_대기 or unit.order == 부대임무_이동 or unit.order == 부대임무_설치 or unit.order == 부대임무_퇴각 or unit.order == 부대임무_보급);
                
                // 이하 임무 제외 공격/수복/정복/섬멸 또는 임무가 없는 경우는 포함하여 이하 명령 수행
                if (unit.order == 부대임무_설치 or unit.order == 부대임무_보급 or unit.order == 부대임무_대기) continue;
                
                // 유닛 사거리
                int max_range = get_atk_range_by_tech(unit);
                
                // 유닛의 타겟 정보 뽑기
                pk::building@ target_base = null;
                pk::unit@ target_unit = null;
                bool is_target_enemy = false;
                bool is_target_in_range = false;

                // 목표가 널 인 경우 
                if (unit.target == -1)
                {
                    pk::point unit_pos = unit.pos;
                    int city_id = pk::get_city_id(unit_pos);
                    if (city_id != -1)
                    {
                        pk::building@ current_city = pk::get_building(city_id);
                        if (pk::is_alive(current_city))
                        {
                            int unit_force_id = unit.get_force_id();
                            int city_force_id = current_city.get_force_id();
                            
                            // 부대가 있는 곳이 적대 소속지역이면 해당 소속지를 타겟으로 임무를 부대임무_정복으로 변경 및 타겟 변경
                            if (pk::is_enemy(unit, current_city))
                            {
                                unit.order = 부대임무_정복;
                                unit.target = current_city.get_id();
                                unit.target_type = 부대임무대상_거점;
                                unit.target_pos = current_city.pos;
                            }
                            // 부대가 있는 곳이 같은 세력 소속지역이면 해당 소속지를 타겟으로 변경 및 부대임무_퇴각으로 변경
                            else if (unit_force_id == city_force_id)
                            {
                                unit.order = 부대임무_퇴각;
                                unit.target = current_city.get_id();
                                unit.target_type = 부대임무대상_거점;
                                unit.target_pos = current_city.pos;
                            }
                            // 부대가 있는 곳이 아군 세력 소속지역이면 부대의 소속지를 타겟으로 변경 및 부대임무_퇴각으로 변경
                            else
                            {
                                pk::building@ service_base = pk::get_building(pk::get_service(unit));
                                if (pk::is_alive(service_base))
                                {
                                    unit.order = 부대임무_퇴각;
                                    unit.target = service_base.get_id();
                                    unit.target_type = 부대임무대상_거점;
                                    unit.target_pos = service_base.pos;
                                }
                            }
                        }
                    }
                }

                if (unit.target != -1)
                {
                    if (unit.target_type == 부대임무대상_거점)
                    {
                        @target_base = pk::get_building(unit.target);
                        if (pk::is_alive(target_base) and pk::is_enemy(unit, target_base))
                            is_target_enemy = true;
                        is_target_in_range = check_target_base_in_range(unit, target_base, max_range);
                    }
                    else if (unit.target_type == 부대임무대상_부대)
                    {
                        @target_unit = pk::get_unit(unit.target);
                        if (pk::is_alive(target_unit) and pk::is_enemy(unit, target_unit))
                            is_target_enemy = true;
                        is_target_in_range = check_target_unit_in_range(unit, target_unit, max_range);
                    }
                }

                // 공성병기부터 실행
                if (is_combat_unit and is_siege_weapon and target_base !is null and pk::is_alive(target_base) and is_target_enemy)
                {   
                    pk::info(pk::format("공성부대 {}", unit_name));
                    // 사거리 내 적 거점 탐색 후 공격                   
                    if (is_target_in_range)
                    {   
                        // 목표 거점과 최대 사거리 유지하면서 이동할 좌표 반환                    
                        pk::point move_pos = get_move_pos_in_range(unit, target_base.pos, max_range);
                        if (pk::is_valid_pos(move_pos) and pk::is_alive(target_base))
                        {
                            pk::array<pk::point> path;
                            path.insertLast(unit.pos);
                            path.insertLast(move_pos);
                            pk::move(unit, path);                            
                            pk::attack(unit, target_base);
                            unit.action_done = true;
                            continue;                            
                        }
                    } 
                    // 사거리 내의 타겟이 없으면 주변 거점 탐색하여 공격
                    else if (!is_target_in_range)
                    {
                        // 주변 적 거점 탐색
                        pk::building@ enemy_base = get_enemy_base_in_range(unit, max_range);
                        if (pk::is_alive(enemy_base) and pk::is_enemy(unit, enemy_base))
                        {
                            // 목표 거점과 최대 사거리 유지하면서 이동할 좌표 반환
                            pk::point move_pos = get_move_pos_in_range(unit, enemy_base.pos, max_range);
                            if (pk::is_valid_pos(move_pos) and pk::is_alive(enemy_base))
                            {
                                pk::array<pk::point> path;
                                path.insertLast(unit.pos);
                                path.insertLast(move_pos);
                                pk::move(unit, path);
                                
                                // 이동 후 유효성 및 적 관계 재확인
                                if (pk::is_alive(enemy_base) and pk::is_enemy(unit, enemy_base))
                                {
                                    pk::attack(unit, enemy_base);
                                    unit.action_done = true;
                                    continue;
                                }
                            }
                        }
                    }
                    // 전투 공성병기 전투 참여 불가 또는 참여 가능이여도 기준기력에 미치지 못할 경우 적대 도시와 제일 가까운 곳으로 이동
                    else if (unit.weapon == 병기_충차 or !공성병기_전투_참여여부 or (공성병기_전투_참여여부 and unit.energy > 공성병기_전투_기준기력) and !unit.has_skill(특기_주악))
                    {
                        // 적대 도시(target_base)와 제일 가까운 곳으로 이동                        
                        
                        // 이동 가능한 위치 중 적대 도시에 가장 가까운 위치 찾기
                        pk::array<pk::point> movable_pos = pk::get_movable_pos(unit);
                        pk::point best_pos = pk::point(-1, -1);
                        int best_dist = 999;

                        for (int j = 0; j < int(movable_pos.length); j++)
                        {
                            int dist = pk::get_distance(movable_pos[j], target_base.pos);
                            if (dist < best_dist)
                            {
                                best_dist = dist;
                                best_pos = movable_pos[j];
                            }
                        }                        
                        // 가장 가까운 위치로 이동
                        if (pk::is_valid_pos(best_pos))
                        {
                            pk::array<pk::point> path;
                            path.insertLast(unit.pos);
                            path.insertLast(best_pos);
                            pk::move(unit, path);
                            unit.action_done = true;
                            continue;
                        }
                    }   // 공성병기 천투 참여가 가능하고 기준기력이 넘으면 다음 간접부대에서 처리
                } 
                else 
                {
                    // 타겟이 적이 아니면 임무를 이동으로 바꾸고 입성처리
                    if (is_combat_unit and is_siege_weapon and pk::is_alive(target_base) and !is_target_enemy)
                    {
                        if (unit.target_type == 부대임무대상_거점)
                        {
                            handle_base_entry(unit, target_base);
                            continue;
                        }
                    }
                }  // 공성병기 거점 공격 처리 종료

                // 간접 병기 공격 처리 노, 정란, 투석
                // 주변에 거점이 없으면 간접병기 공격 처리
                if (is_combat_unit and is_target_enemy and (unit.weapon == 병기_노 or (is_siege_weapon and 공성병기_전투_참여여부 and unit.energy >= 공성병기_전투_기준기력)))
                {
                    // 적부대 탐색
                    pk::building@ dummy_base;
                    pk::list<pk::unit@> enemy_units = get_enemy_units_in_range(unit, dummy_base, max_range);
                    if (enemy_units.count > 0)
                    {
                       // 가중치 높은 적 선택(간접병기는 가까운 거리에 적에 가중치 부여 - 진영 유지를 위함)
                        pk::unit@ selected_target_unit = reassign_attack_target_by_weight(unit, enemy_units);
                        if (pk::is_alive(selected_target_unit))
                        {                                                     
                            // 적부대 위치 기준으로 이동할 좌표 찾기 (사거리 내)
                            pk::point move_pos = get_move_pos_in_range(unit, selected_target_unit.pos, max_range);
                            // 이동하여 적 공격
                            if (pk::is_valid_pos(move_pos))
                            {   
                                pk::array<pk::point> path;
                                path.insertLast(unit.pos);
                                path.insertLast(move_pos);
                                pk::move(unit, path);                                
                                pk::attack(unit, selected_target_unit);
                                unit.action_done = true;
                                continue;
                                
                            }
                        }
                    } // 적 부대 탐색 및 공격 종료
                    else if (is_target_enemy and is_target_in_range)
                    {
                        // 적 부대가 없고 적대 거점이 있으면 공격 처리
                        if (target_base !is null)
                        {
                            // 목표 거점과 최대 사거리 유지하면서 이동할 좌표 반환
                            pk::point move_pos = get_move_pos_in_range(unit, target_base.pos, max_range);
                            if (pk::is_valid_pos(move_pos))
                            {
                                pk::array<pk::point> path;
                                path.insertLast(unit.pos);
                                path.insertLast(move_pos);
                                pk::move(unit, path);
                                pk::attack(unit, target_base);
                                unit.action_done = true;
                                continue;
                            }
                        }
                    }
                    // 마지막 퇴각 물림 임우 없음에서 행동종료 예정
                }
                else if (is_target_in_range and !is_target_enemy)
                {
                    // 적도 없고 거점도 적이 아니면 입성 처리
                    if (target_base !is null and unit.target_type == 부대임무대상_거점)
                    {
                        handle_base_entry(unit, target_base);
                        continue;
                    }
                } // 간접병기 공격 처리 종료


                // 근접 부대 처리
                if (is_combat_unit and is_melee_unit)
                {
                    // 적부대 탐색 후 공격
                    pk::building@ dummy_base;
                    pk::list<pk::unit@> enemy_units = get_enemy_units_in_range(unit, dummy_base, max_range);
                    if (enemy_units.count > 0)
                    {
                        // 가중치 높은 적 선택
                        pk::unit@ selected_target_unit2 = reassign_attack_target_by_weight(unit, enemy_units);
                        if (pk::is_alive(selected_target_unit2) and pk::is_enemy(unit, selected_target_unit2))
                        {
                            // 적부대 위치 기준으로 이동할 좌표 찾기 (사거리 내)
                            pk::point move_pos = get_move_pos_in_range(unit, selected_target_unit2.pos, max_range);
                            if (pk::is_valid_pos(move_pos) and pk::is_enemy(unit, selected_target_unit2))
                            {
                                pk::array<pk::point> path;
                                path.insertLast(unit.pos);
                                path.insertLast(move_pos);
                                pk::move(unit, path);
                                pk::attack(unit, selected_target_unit2);
                                unit.action_done = true;
                                continue;
                            }
                        }
                    }
                } // 적 부대 탐색 및 공격 종료
                // 적부대는 없고 적 거점있으면 공격
                else
                {
                    pk::building@ enemy_base = get_enemy_base_in_range(unit, max_range);
                    if (pk::is_alive(enemy_base) and pk::is_enemy(unit, enemy_base))
                    {
                        // 목표 거점과 최대 사거리 유지하면서 이동할 좌표 반환
                        pk::point move_pos = get_move_pos_in_range(unit, enemy_base.pos, max_range);
                        if (pk::is_valid_pos(move_pos))
                        {
                            pk::array<pk::point> path;
                            path.insertLast(unit.pos);
                            path.insertLast(move_pos);
                            pk::move(unit, path);
                            
                            pk::attack(unit, enemy_base);
                            unit.action_done = true;
                            continue;
                        }
                    }
                }
                // 근접부대 처리 종료
               
                // 부대임무가 퇴각 이동(정복하러 또는 공격하는 중) 없음 일 경우 처리
                pk::run_order(unit);
                unit.action_done = true;
                continue;


                // 타겟 정보 가져오기
                // string target_name = "없음";
                // string target_pos_str = "없음";
                // if (target_base !is null and pk::is_alive(target_base))
                // {
                //     target_name = pk::decode(pk::get_name(target_base));
                //     target_pos_str = pk::format("({}, {})", target_base.pos.x, target_base.pos.y);
                // }
                // else if (target_unit !is null and pk::is_alive(target_unit))
                // {
                //     pk::person@ target_leader = pk::get_person(target_unit.leader);
                //     target_name = (pk::is_alive(target_leader)) ? pk::decode(pk::get_name(target_leader)) : "??";
                //     target_pos_str = pk::format("({}, {})", target_unit.pos.x, target_unit.pos.y);
                // }                
                // // 타겟 주변에 있음/없음 확인
                // string target_in_range_str = "없음";
                // if (unit.target != -1)
                // {
                //     target_in_range_str = is_target_in_range ? "있음" : "없음";
                // }
                
                // pk::info(pk::format("{} ({}, {}) 임무:{} 타겟:{} {} 주변:{}", unit_name, unit.pos.x, unit.pos.y, unit.order, target_name, target_pos_str, target_in_range_str));
                
            }
        }

        // 부대 퇴각 판단
        bool retreat_weak_unit(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            // 공성병기이고 기력이 10미만만 퇴각 (주악 특기가 있으면 기력이 작아도 후퇴 안함)
            bool is_siege_weapon = (unit.weapon == 병기_목수 or unit.weapon == 병기_목수 or unit.weapon == 병기_정란 or unit.weapon == 병기_투석);
            bool unit_energy_zero = (is_siege_weapon and !unit.has_skill(특기_주악) and 10 > unit.energy);

            if (병력부족부대_복귀병력기준 > int(unit.troops) or (is_siege_weapon and unit_energy_zero))
                return true;
            
            return false;
        }

        // 퇴각 부대 도시/거점 반환
        pk::building@ get_retreat_base(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return null;
            
            int unit_troops = unit.troops;
            pk::point unit_pos = unit.pos;
            
            // 맨 처음: 자기 자리가 소속된 도시 확인
            int city_id = pk::get_city_id(unit_pos);
            if (city_id != -1)
            {
                pk::building@ current_city = pk::get_building(city_id);
                if (pk::is_alive(current_city))
                {
                    // 같은 군단인지 확인
                    if (current_city.get_district_id() == unit.get_district_id())
                    {
                        // 병력 초과 확인
                        int base_troops = pk::get_troops(current_city);
                        int max_troops = pk::get_max_troops(current_city);
                        int exceed = (base_troops + unit_troops) - max_troops;
                        
                        if (exceed < 5000)
                        {
                            return current_city;  // 현재 위치 도시 입성 가능
                        }
                    }
                }
            }
            
            // 부대의 소속 거점에서 군단 가져오기
            pk::building@ service_base = pk::get_building(pk::get_service(unit));
            if (!pk::is_alive(service_base)) return null;
            
            pk::district@ district = pk::get_district(service_base.get_district_id());
            if (!pk::is_alive(district)) 
            {
                pk::person@ leader = pk::get_person(unit.leader);
                string unit_name = (pk::is_alive(leader)) ? pk::decode(pk::get_name(leader)) : "??";
                pk::info(pk::format("[퇴각] {} 부대: 군단 없음, 소속 거점 반환", unit_name));
                return service_base;
            }
            
            // 군단 정보 로그
            pk::person@ unit_leader = pk::get_person(unit.leader);
            string unit_name = (pk::is_alive(unit_leader)) ? pk::decode(pk::get_name(unit_leader)) : "??";
            pk::info(pk::format("[퇴각] {} 부대: 군단 ID={}", unit_name, district.get_id()));
                        
            // 그 이후: 같은 군단의 도시/관문/항구를 거리순으로 가져오기
            pk::array<pk::building@> bases = get_district_bases_sorted_by_distance(unit);
            
            pk::info(pk::format("거점 수={}", bases.length));
            
            // 병력 초과 확인 (5000 이상이면 다음 거점, 5000 미만이면 그 거점 반환)
            for (int i = 0; i < int(bases.length); i++)
            {
                pk::building@ base = bases[i];
                if (!pk::is_alive(base)) continue;
                
                int base_troops = pk::get_troops(base);
                int max_troops = pk::get_max_troops(base);
                int exceed = (base_troops + unit_troops) - max_troops;
                    
                    if (exceed < 5000)
                    {
                    return base;  // 병력 초과 5000 미만이면 그 거점 반환
                    }
                }
            
            // 모든 거점을 순회해도 없으면 가장 가까운 거점 반환
            if (bases.length > 0)
            {
            pk::person@ leader = pk::get_person(unit.leader);
            string leader_name = (pk::is_alive(leader)) ? pk::decode(pk::get_name(leader)) : "??";            
                pk::info(pk::format("{} 거점 수={}", leader_name, bases.length));
                return bases[0];
            }
            
            // 거점이 없으면 소속 거점 반환
            return service_base;
        }

        // 도시를 부대 위치 기준 가까운 순으로 정렬
        // 같은 군단의 도시/관문/항구를 찾아 거리순으로 정렬하여 반환
        pk::array<pk::building@> get_district_bases_sorted_by_distance(pk::unit@ unit)
        {
            pk::array<pk::building@> bases;
            if (!pk::is_alive(unit)) return bases;
            
            int unit_district_id = unit.get_district_id();
            pk::point unit_pos = unit.pos;
            
            // 모든 건물 목록에서 같은 군단의 도시/관문/항구만 필터링
            pk::list<pk::building@> building_list = pk::get_building_list();
            
            for (int i = 0; i < building_list.count; i++)
            {
                pk::building@ building = building_list[i];
                if (!pk::is_alive(building)) continue;
                if (building.get_district_id() != unit_district_id) continue;
                
                // 도시(시설_도시), 관문(시설_관문), 항구(시설_항구)만 포함
                if (building.facility == 시설_도시 or building.facility == 시설_관문 or building.facility == 시설_항구)
                {
                    bases.insertLast(building);
                }
            }
                      
            // 거리순으로 정렬
            for (int i = 0; i < int(bases.length) - 1; i++)
            {
                for (int j = 0; j < int(bases.length) - 1 - i; j++)
                {
                    pk::building@ base_a = bases[j];
                    pk::building@ base_b = bases[j + 1];
                    if (!pk::is_alive(base_a) or !pk::is_alive(base_b)) continue;
                    
                    int dist_a = pk::get_distance(unit_pos, base_a.pos);
                    int dist_b = pk::get_distance(unit_pos, base_b.pos);
                    
                    if (dist_a > dist_b)
                    {
                        pk::building@ temp = bases[j];
                        @bases[j] = bases[j + 1];
                        @bases[j + 1] = temp;
                    }
                }
            }
            
            return bases;
        }

        // 타겟 거점이 사거리 내에 있는지 확인
        bool check_target_base_in_range(pk::unit@ unit, pk::building@ target_base, int attack_range)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(target_base)) return false;
            
            pk::point target_pos = target_base.pos;
            if (!pk::is_valid_pos(target_pos)) return false;
            
            // 이동 가능한 범위 + 사거리 내에 타겟이 있는지 확인
            pk::array<pk::point> search_pos = get_movable_range_pos(unit, attack_range);
            for (int i = 0; i < int(search_pos.length); i++)
            {
                if (search_pos[i] == target_pos)
                    return true;
            }
            
            return false;
        }
        
        // 타겟 유닛이 사거리 내에 있는지 확인
        bool check_target_unit_in_range(pk::unit@ unit, pk::unit@ target_unit, int attack_range)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(target_unit)) return false;
            
            pk::point target_pos = target_unit.pos;
            if (!pk::is_valid_pos(target_pos)) return false;
            
            // 이동 가능한 범위 + 사거리 내에 타겟이 있는지 확인
            pk::array<pk::point> search_pos = get_movable_range_pos(unit, attack_range);
            for (int i = 0; i < int(search_pos.length); i++)
            {
                if (search_pos[i] == target_pos)
                    return true;
            }
            
            return false;
        }
        
        // 이동 거리안에 목표 거점이 있는지 확인
        bool check_target_base_in_movable_range(pk::unit@ unit, pk::building@ target_base)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(target_base)) return false;
            
            pk::point target_pos = target_base.pos;
            pk::array<pk::point> movable_pos = pk::get_movable_pos(unit);
            
            for (int i = 0; i < int(movable_pos.length); i++)
            {
                if (movable_pos[i] == target_pos)
                    return true;
            }
            
            return false;
        }
        
        // 거점 입성 처리 (기존 로직 함수화)
        bool handle_base_entry(pk::unit@ unit, pk::building@ preferred_base)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(preferred_base)) return false;
            
            // 병력 초과 체크 후 입성 가능한 거점 반환 (병력 초과 시 다른 거점 순회, 없으면 원래 거점 반환) 거점 입성 처리            
            int unit_troops = unit.troops;
            int base_troops = pk::get_troops(preferred_base);
            int max_troops = pk::get_max_troops(preferred_base);
            
            // 병력 초과 체크 (5000 이상 초과 시 다른 거점 찾기)
            if (base_troops + unit_troops > max_troops + 5000)
            {
                // 병력 초과 시 다른 거점 찾기
                pk::building@ available_base = get_available_city_for_entry(unit, preferred_base);
                if (pk::is_alive(available_base))
                {
                    @preferred_base = available_base;
                }
            }

            unit.target_pos = preferred_base.pos;
            unit.target_type = 부대임무대상_거점;
            unit.target = preferred_base.get_id();
            unit.order = 부대임무_이동;
            
            // 들어갈 수 있으면 들어가고 아니면 이동 명령 실행
            if (check_target_base_in_movable_range(unit, preferred_base))
            {                                
                pk::array<pk::point> path;
                path.insertLast(unit.pos);
                path.insertLast(unit.target_pos);
                pk::move(unit, path);
                unit.action_done = true;
                return true;
            }
            pk::run_order(unit);   
            unit.action_done = true;
            return false;
        }
                
        // 이동가능한 거리 + max_range 거리 좌표 반환
        pk::array<pk::point> get_movable_range_pos(pk::unit@ unit, int max_range)
        {
            pk::array<pk::point> search_pos;
            if (!pk::is_alive(unit)) return search_pos;
            
            pk::point unit_pos = unit.pos;
            pk::array<pk::point> movable_pos = pk::get_movable_pos(unit);
            // 자기 자신의 좌표 추가            
            movable_pos.insertLast(unit_pos);
            
            pk::array<pk::point> search_list;
            dictionary x_to_y_min;
            dictionary x_to_y_max;
            
            // 이동 가능한 위치 추가 및 x별 y 최소/최대값 계산
            for (int i = 0; i < int(movable_pos.length); i++)
            {
                pk::point pos = movable_pos[i];
                if (!pk::is_valid_pos(pos)) continue;
                
                // 중복 체크
                bool exists = false;
                for (int k = 0; k < int(search_list.length); k++)
                {
                    if (search_list[k] == pos)
                    {
                        exists = true;
                        break;
                    }
                }
                
                if (!exists)
                    search_list.insertLast(pos);
                
                int x = pos.x;
                int y = pos.y;
                string x_key = x + "";
                
                if (!x_to_y_min.exists(x_key))
                {
                    x_to_y_min[x_key] = y;
                    x_to_y_max[x_key] = y;
                }
                else
                {
                    if (y < int(x_to_y_min[x_key]))
                        x_to_y_min[x_key] = y;
                    if (y > int(x_to_y_max[x_key]))
                        x_to_y_max[x_key] = y;
                }
            }
            
            // 각 x 좌표의 테두리(y 최소/최대)에서만 사거리 범위 좌표 추가
            array<string> keys = x_to_y_min.getKeys();
            for (int i = 0; i < int(keys.length); i++)
            {
                int x = parseInt(keys[i]);
                int y_min = int(x_to_y_min[keys[i]]);
                int y_max = int(x_to_y_max[keys[i]]);
                
                // y 최소값 위치에서 사거리 범위
                pk::point border_pos_min = pk::point(x, y_min);
                if (pk::is_valid_pos(border_pos_min))
                {
                    pk::array<pk::point> range_pos = pk::range(border_pos_min, 1, max_range);
                    for (int j = 0; j < int(range_pos.length); j++)
                    {
                        pk::point pos = range_pos[j];
                        if (!pk::is_valid_pos(pos)) continue;
                        
                        // 중복 체크
                        bool exists = false;
                        for (int k = 0; k < int(search_list.length); k++)
                        {
                            if (search_list[k] == pos)
                            {
                                exists = true;
                                break;
                            }
                        }
                        
                        if (!exists)
                            search_list.insertLast(pos);
                    }
                }
                
                // y 최대값 위치에서 사거리 범위 (y_min == y_max인 경우 중복 방지)
                if (y_max != y_min)
                {
                    pk::point border_pos_max = pk::point(x, y_max);
                    if (pk::is_valid_pos(border_pos_max))
                    {
                        pk::array<pk::point> range_pos = pk::range(border_pos_max, 1, max_range);
                        for (int j = 0; j < int(range_pos.length); j++)
                        {
                            pk::point pos = range_pos[j];
                            if (!pk::is_valid_pos(pos)) continue;
                            
                            // 중복 체크
                            bool exists = false;
                            for (int k = 0; k < int(search_list.length); k++)
                            {
                                if (search_list[k] == pos)
                                {
                                    exists = true;
                                    break;
                                }
                            }
                            
                            if (!exists)
                                search_list.insertLast(pos);
                        }
                    }
                }
            }
            
            return search_list;
        }

        // 부대 사거리를 포함한 주변 적부대와 거점 반환
        pk::list<pk::unit@> get_enemy_units_in_range(pk::unit@ unit, pk::building@ &out enemy_base, int attack_range)
        {
            pk::list<pk::unit@> enemy_units;
            @enemy_base = null;
            
            if (!pk::is_alive(unit)) return enemy_units;
            
            // 부대 위치에서 움직일 수 있는 범위 + 사거리를 포함한 내의 적 부대/거점 탐색
            pk::array<pk::point> search_pos = get_movable_range_pos(unit, attack_range);
            
            pk::list<pk::building@> enemy_bases;
            
            // 수집한 모든 좌표에서 적 부대와 거점 탐색
            for (int i = 0; i < int(search_pos.length); i++)
            {
                pk::point pos = search_pos[i];
                if (!pk::is_valid_pos(pos)) continue;
                
                // 적 부대 탐색
                pk::unit@ target_unit = pk::get_unit(pos);
                if (pk::is_alive(target_unit) and pk::is_enemy(unit, target_unit))
                {
                    if (!enemy_units.contains(target_unit))
                        enemy_units.add(target_unit);
                }
                
                // 적 거점 탐색 (0~15 범위의 시설만)
                pk::building@ target_base = pk::get_building(pos);
                if (pk::is_alive(target_base) and pk::is_enemy(unit, target_base))
                {
                    if (target_base.facility >= 0 and target_base.facility <= 15)
                    {
                        if (!enemy_bases.contains(target_base))
                            enemy_bases.add(target_base);
                    }
                }
            }
            
            // 거점 번호 기준으로 정렬하여 가장 작은 번호의 거점 반환 (0~15 = 0:도시 ~ 15:본거지)
            if (enemy_bases.count > 0)
            {
                pk::array<pk::building@> bases_array = pk::list_to_array(enemy_bases);
                bases_array.sort(function(a, b)
                {
                    int a_facility = a.facility;
                    int b_facility = b.facility;
                    
                    // 0~15 범위 내의 시설만 비교
                    if (a_facility >= 0 and a_facility <= 15 and b_facility >= 0 and b_facility <= 15)
                    {
                        if (a_facility != b_facility) return a_facility < b_facility;
                    }
                    
                    return a.get_id() < b.get_id();
                });
                
                if (bases_array.length > 0)
                    @enemy_base = bases_array[0];
            }
            
            return enemy_units;
        }
        
        // 적 건축물 탐색 0~15 = 0:도시 ~ 15:본거지
        pk::building@ get_enemy_base_in_range(pk::unit@ unit, int attack_range)
        {
            pk::building@ enemy_base;
            get_enemy_units_in_range(unit, enemy_base, attack_range);
            return enemy_base;
        }


        // 목표 좌표와 최대 사거리 유지하면서 이동할 좌표를 가까운순으로 반환 (단일 좌표만 반환)
        pk::point get_move_pos_in_range(pk::unit@ unit, pk::point target_pos, int max_range)
        {
            pk::point invalid_pos = pk::point(-1, -1);
            if (!pk::is_alive(unit) or !pk::is_valid_pos(target_pos)) return invalid_pos;
            pk::point unit_pos = unit.pos;
            pk::array<pk::point> movable_pos = pk::get_movable_pos(unit);
            pk::array<pk::point> result_pos;
            
            // 최소 사거리 계산 (간접 병기는 최소 사거리 2)
            int min_range = 1;
            bool is_indirect_weapon = (unit.weapon == 병기_노 or unit.weapon == 병기_정란 or unit.weapon == 병기_투석);
            if (is_indirect_weapon)
                min_range = 2;
            
            // 간접 무기는 최대 사거리부터 최소 사거리(2)까지, 일반 무기는 최대 사거리부터 최소 사거리까지 점진적으로 시도
            int start_range = max_range;
            int end_range = min_range;
            
            for (int try_range = start_range; try_range >= end_range; try_range--)
            {
                result_pos.removeRange(0, result_pos.length);
                
                // 이동 가능한 좌표 중에서 목표 거점과 사거리 범위 내에 있는 좌표만 필터링
            for (int i = 0; i < int(movable_pos.length); i++)
            {
                pk::point pos = movable_pos[i];
                if (!pk::is_valid_pos(pos)) continue;
                
                    int dist_to_target = pk::get_distance(pos, target_pos);
                    if (dist_to_target >= min_range and dist_to_target <= try_range)
                    {
                        result_pos.insertLast(pos);
                    }
                }
                
                // 공격 가능한 위치를 찾으면 종료
                if (result_pos.length > 0) break;
            }
            
            if (result_pos.length == 0) return invalid_pos;
            
            // 부대 위치에서 가까운 순으로 정렬 후 첫 번째 좌표 반환
            if (result_pos.length > 1)
            {
                pk::point unit_pos_sort = unit.pos;
                // 거리 기준으로 정렬 (버블 정렬)
                for (int i = 0; i < int(result_pos.length) - 1; i++)
                {
                    for (int j = 0; j < int(result_pos.length) - 1 - i; j++)
                    {
                        int dist_a = pk::get_distance(result_pos[j], unit_pos_sort);
                        int dist_b = pk::get_distance(result_pos[j + 1], unit_pos_sort);
                        if (dist_a > dist_b)
                        {
                            pk::point temp = result_pos[j];
                            result_pos[j] = result_pos[j + 1];
                            result_pos[j + 1] = temp;
                        }
                    }
                }
            }
            
            return result_pos[0];
        }


        // 가중치에의한 공격 대상 재설정
        // 가중치 구성:
        // 1. 상성 가중치: 최대 +1.0, 최소 -1.0
        // 2. 병력 가중치: 최대 2.0 (부대목표변경_병력가중치적용 true시), 최대 1.0 (false시), 최소 0.0
        // 3. 공격력 가중치: 최대 1.0 (최대_부대_공방능력치 기준), 최소 0.0
        // 4. 방어력 가중치: 최대 255.0 (방어력 1일 때), 최소 1.0 (방어력 255일 때)
        // 5. 거리 가중치 (간접 무기 또는 부대목표_거리가중치적용 true시): 최대 3.0 (거리 0일 때), 최소 0.0 (거리 10 이상일 때)
        pk::unit@ reassign_attack_target_by_weight(pk::unit@ unit, pk::list<pk::unit@> enemy_units)
        {
            if (!pk::is_alive(unit) or enemy_units.count == 0) return null;
            
            pk::unit@ best_target = null;
            float max_weight = -1.0f;
            
            int unit_weapon = unit.weapon;
            bool is_indirect_weapon = (unit_weapon == 병기_노 or unit_weapon == 병기_정란 or unit_weapon == 병기_투석);
            pk::point unit_pos = unit.pos;
            
            for (int i = 0; i < enemy_units.count; i++)
            {
                pk::unit@ enemy = enemy_units[i];
                if (!pk::is_alive(enemy)) continue;
                
                float enemy_weight = get_target_unit_weight(unit, enemy);
                
                // 간접 무기는 거리 가중치 추가 (가까울수록 높은 가중치)
                // 거리 가중치: 최대 3.0 (거리 0일 때), 최소 0.0 (거리 10 이상일 때)
                if (is_indirect_weapon or 부대목표_거리가중치적용)
                {
                    int dist = pk::get_distance(unit_pos, enemy.pos);
                    // 거리가 가까울수록 높은 가중치 (최대 거리 10 기준으로 역가중치, 최대값 3.0)
                    float distance_weight = (10.0f - float(dist)) / 10.0f * 3.0f;
                    // 부대목표_거리가중치적용이 true일 때 거리 가중치 2배 적용
                    if (부대목표_거리가중치적용)
                        distance_weight *= 2.0f;
                    enemy_weight += distance_weight;
                }
                
                // 가장 높은 가중치의 적부대 선택
                if (enemy_weight > max_weight)
                {
                    max_weight = enemy_weight;
                    @best_target = enemy;
                }
            }            
            return best_target;
        }

        
        // 해당 부대의 대한 목표부대 가중치 설정
        float get_target_unit_weight(pk::unit@ unit, pk::unit@ target_unit)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(target_unit)) return 0.0f;

            float weight = 0.0f;
          
            int weapon_id  = unit.weapon;
            // 상성 가중치 적용 최대 +1.0, 최소 -1.0
            weight += get_affinity_weight(weapon_id, target_unit.weapon);
            // 병력 가중치 적용 최대 1(추가 병력 가중치 적용시 2배) 최소 0
            if (5000 >= target_unit.troops and 부대목표_병력가중치적용)
                weight += (float(target_unit.troops) / float(5000.0f)) * 2.0f;
            else if (5000 >= target_unit.troops)
                weight += (float(target_unit.troops) / float(5000.0f));

            // 공격력 가중치 적용 (최대 성장치 기준)
            if (부대목표_공격가중치적용) 
                weight += float(target_unit.attr.stat[부대능력_공격]) / float(최대_부대_공방능력치);
            
            // 방어력 가중치 적용 (방어력이 낮을수록 우선순위 상승)
            if (부대목표_방어가중치적용)
                weight += float(최대_부대_공방능력치) / float(target_unit.attr.stat[부대능력_방어]);
                        
            return weight;
        }

        // 병종 간 상성 가중치 반환 
        float get_affinity_weight(int weapon_id, int target_weapon_id)
        {      
            // 창>기병, 극>창, 기병>극: +1.0 / 반대: -1.0
            if ((weapon_id == 병기_창 and target_weapon_id == 병기_군마) or
                (weapon_id == 병기_극 and target_weapon_id == 병기_창) or
                (weapon_id == 병기_군마 and target_weapon_id == 병기_극))
                return 1.0f;
            if ((weapon_id == 병기_군마 and target_weapon_id == 병기_창) or
                (weapon_id == 병기_창 and target_weapon_id == 병기_극) or
                (weapon_id == 병기_극 and target_weapon_id == 병기_군마))
                return -1.0f;
            return 0.0f;
        }

        // 세력의 기교와 유닛의 특기로로 사거리 반환
        int get_atk_range_by_tech(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return 1;
            
            int atk_range = 1;
            // 기병은 따로 거리주지 않음 (단 기력이 10 이상일때만 거리증가 - 전법 불가)
            if ((unit.weapon == 병기_군마 and unit.has_tech(기교_기사)) or unit.has_skill(특기_백마) and 10 > unit.energy) atk_range = 2;

            if (unit.weapon == 병기_노) 
            {
                unit.has_tech(기교_강노) ? atk_range = 2 : atk_range = 1;
            }

            if (unit.weapon == 병기_정란 or unit.weapon == 병기_투석) 
            {
                atk_range = 정란_기본사거리;
                if (unit.has_skill(특기_사정)) atk_range += 1;
            }                
            return atk_range;
        }
        
        // 기동 범위 내 같은 세력 도시 확인 및 반환
        pk::building@ get_nearest_force_city(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return null;
            
            int force_id = unit.get_force_id();
            
            // 부대의 이동 가능한 좌표들
            pk::array<pk::point> movable_pos = pk::get_movable_pos(unit);
            
            // 이동 가능한 좌표 중에 도시가 있는지 확인
            for (int i = 0; i < int(movable_pos.length); i++)
            {
                pk::building@ city = pk::get_building(movable_pos[i]);
                if (!pk::is_alive(city)) continue;
                if (city.get_force_id() != force_id) continue;
                if (건물_도시시작 <= city.get_id() and city.get_id() < 건물_도시끝)
                {
                    return city;  // 기동 범위 내 도시 발견, 반환
                }
            }
            
            return null;  // 기동 범위 내 도시 없음
        }
       
        // 병력 초과 체크 후 입성 가능한 도시 반환 (병력 초과 시 다른 도시 순회, 없으면 원래 도시 반환)
        // 수송부대는 병력 초과 체크 제외
        pk::building@ get_available_city_for_entry(pk::unit@ unit, pk::building@ preferred_city)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(preferred_city)) return preferred_city;
            
            // 수송부대는 병력 초과 체크 없이 바로 반환
            if (unit.type == 부대종류_수송)
            {
                return preferred_city;
            }
            
            int unit_troops = unit.troops;
            int force_id = unit.get_force_id();
            pk::point unit_pos = unit.pos;
            
            // 선호 도시의 병력 체크
            int city_troops = pk::get_troops(preferred_city);
            int city_max_troops = pk::get_max_troops(preferred_city);
            
            // 입성 가능 여부: (현재 병력 + 부대 병력) <= (최대 병력 + 5000)
            if (city_troops + unit_troops <= city_max_troops + 5000)
            {
                return preferred_city;  // 입성 가능
            }

            // 병력 초과 시 같은 군단의 다른 도시/관문/항구 찾기
            pk::array<pk::building@> bases = get_district_bases_sorted_by_distance(unit);
            
            // 가까운 순서대로 입성 가능한 도시/관문/항구 찾기
            for (int i = 0; i < int(bases.length); i++)
            {
                pk::building@ base = bases[i];
                if (!pk::is_alive(base)) continue;
                if (base.get_id() == preferred_city.get_id()) continue;  // 이미 체크한 거점 제외
                
                int check_troops = pk::get_troops(base);
                int check_max_troops = pk::get_max_troops(base);
                
                // 입성 가능 여부 체크
                if (check_troops + unit_troops <= check_max_troops + 5000)
                {
                    return base;  // 입성 가능한 거점 발견
                }
            }
            
            // 모든 도시를 순회해도 없으면 원래 도시 반환 (한계 반환)
            return preferred_city;
        }
       
        
        // 턴 종료 시 공격, 수복, 정복, 섬멸 임무를 가진 부대만 원래 임무로 복귀 및 해당 임무나 목표가 없을 시에 제일 가까운 부대의 임무와 동일하게 임무 부여
        // 제일 가까운 부대 중 유효한 임무를 가진 부대 찾기
        pk::unit@ get_nearest_unit_with_valid_order(pk::unit@ unit, pk::force@ force)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(force)) return null;
            
            pk::list<pk::unit@> unit_list = pk::get_unit_list(force);
            pk::unit@ nearest_unit = null;
            int min_distance = 9999;
            pk::point unit_pos = unit.pos;
            
            for (int i = 0; i < unit_list.count; i++)
            {
                pk::unit@ other_unit = unit_list[i];
                if (!pk::is_alive(other_unit)) continue;
                if (other_unit.get_id() == unit.get_id()) continue; // 자기 자신 제외
                if (other_unit.type != 부대종류_전투) continue;
                
                // 공격, 수복, 정복, 섬멸 임무만 고려
                if (other_unit.order != 부대임무_공격 and other_unit.order != 부대임무_수복 and 
                    other_unit.order != 부대임무_정복 and other_unit.order != 부대임무_섬멸)
                    continue;
                
                // 목표가 유효한지 확인
                if (other_unit.target == -1) continue;
                pk::hex_object@ target_obj = pk::get_hex_object(other_unit.target);
                if (!pk::is_alive(target_obj)) continue;
                
                // 거리 계산
                int distance = pk::get_distance(unit_pos, other_unit.pos);
                if (distance < min_distance)
                {
                    min_distance = distance;
                    @nearest_unit = other_unit;
                }
            }
            
            return nearest_unit;
        }



        
        // 무기 ID를 한글 이름으로 반환
        string get_weapon_name(int weapon_id)
        {
            string weapon_name;
            switch(weapon_id)
            {
                case 병기_창  : weapon_name = "창"; break;
                case 병기_극  : weapon_name = "극"; break;
                case 병기_노  : weapon_name = "노"; break;
                case 병기_군마 : weapon_name = "군마"; break;
                case 병기_충차 : weapon_name = "충차"; break;
                case 병기_정란 : weapon_name = "정란"; break;
                case 병기_목수 : weapon_name = "목수"; break;
                case 병기_투석 : weapon_name = "투석"; break;
                case 병기_주가 : weapon_name = "주가"; break;
                case 병기_누선 : weapon_name = "누선"; break;
                case 병기_투함 : weapon_name = "투함"; break;
                
                default : weapon_name = "??"; break;
            }
            return weapon_name;
        }
        
        // 부대 임무(오더) 숫자를 한글로 반환
        string get_order_name(int order)
        {
            string name;
            switch(order)
            {
                case 0  : name = "부대임무_대기"; break;
                case 1  : name = "부대임무_이동"; break;
                case 2  : name = "부대임무_설치"; break;
                case 3  : name = "부대임무_공격"; break;
                case 4  : name = "부대임무_퇴각"; break;
                case 5  : name = "부대임무_물림"; break;
                case 6  : name = "부대임무_호위"; break;
                case 7  : name = "부대임무_공성"; break;
                case 8  : name = "부대임무_수복"; break;
                case 9  : name = "부대임무_정복"; break;
                case 10 : name = "부대임무_보급"; break;
                case 11 : name = "부대임무_섬멸"; break;
                case 12 : name = "부대임무_추종"; break;
                case 13 : name = "부대임무_끝" ; break;
                default : name = "알수없음"; break;
            }
            
            return name;
        }        

	}
    
	Main main;
}

