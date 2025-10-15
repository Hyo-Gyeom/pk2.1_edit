/*
// 만든이: 기마책사
// Update: '19.1.11  / 최초적용: 전투부대 출진 시 소속거점의 미행동 장수와 능력비교하여 부대주장 변경함
// Update: '19.1.21  / 변경내용: 중복 출진 오류 수정
// Update: '19.1.22  / 변경내용: 병력/물자 거점반환 함수수정, 지휘병력 변경에 따른 소실 물자 거점반환 추가
// Update: '19.1.27  / 변경내용: 무장 변경 후 거점 태수 재설정 추가
// Update: '19.2.9   / 변경내용: 중복 출진 오류 수정 (부장편성까지 확인 추가), 무장변경 적성기준 및 가중치 추가, 부대 내 주장/부장 지휘재편 추가
// Update: '19.2.12  / 변경내용: 중복 출진 취소처리 후 거점 태수 재설정 추가
// Update: '19.2.16  / 변경내용: 중복 출진 오류 수정 (출진변경한 모든 무장 체크), 무장변경용 지력/적성 비교수식 변경,
//                            현재 턴세력 부대 아닌 경우 무장변경 동작안하도록 수정, 건설편제 판단조건 추가, 
// Update: '19.2.18  / 변경내용: 중복 출진 확인 함수 수정 (한 부대에 여러명 중복 편성되는 경우 추가)
// Update: '19.3.27  / 변경내용: 수송부대에 대해서 미행동 특기_운반 무장으로 교체기능 추가
// Update: '19.5.13  / 변경내용: 무장 변경 전 변경이 유효한 부대인지 확인 조건 추가
// Update: '20.9.11  / 변경내용: 캠페인에서는 스크립트 동작하지 않도록 수정, 지휘초과병력 반환처리 타이밍 조정, 
//                            중복부대 출진취소 함수 수정, 물자반환 함수에 수송부대 추가, 증발물자 턴종료 재정산 추가
// Update: '20.9.16  / 변경내용: 병종특기 우대 가중치 적용
*/

namespace 출진무장최적화
{
    //---------------------------------------------------------------------------------------
    // 유저설정
    const int  출진무장최적화_세력조건 = 0;     // 0: 컴퓨터AI 와 플레이어_위임군단 모두,  1: 플레이어_위임군단만, 2: 컴퓨터AI만,  3: 모두 미적용
    const bool 출진무장최적화_유저군단적용 = false;   // 세력조건과 별개로 적용여부 설정, 유저의 주장편성 무시되므로 false 권장
    const bool 출진무장최적화_대화표시 = true;

    //---------------------------------------------------------------------------------------
    // 거점에 미출진한 높은 능력의 무장 있는 경우 주장 변경 적용함
    const bool 출진무장최적화_무장변경 = true;    // true=적용,  false=미적용
    
    // 기준값 이하 무장이 주장인 경우 변경
    const int 무장변경_능력기준값 = 40;          
    
    // 출진무장 능력 비교용 가중치 (설정값 범위제약 없음)
    const float 무장변경_통솔가중치 = 0.8f;
    const float 무장변경_무력가중치 = 1.0f;
    const float 무장변경_지력가중치 = 0.5f;
    
    // 무장변경 기준이하 적성 고려 ('19.2.9)
    const bool 무장변경_적성기준적용 = true;      // 기준이하의 적성 시 변경 적용
    const int  무장변경_적성기준값 = 1;           // 0:적성_C,   1:적성_B,   2:적성_A,   3:적성_S 이상
    const float 무장변경_적성가중치 = 3.f;
    
    // 무장변경 특기 보유자 우선 고려 ('20.9.16)
    const bool 무장변경_특기가중치 = true;    //true=적용,  false=미적용
    
    // 건설편제 판단 조건 ('19.2.15)
    const bool 건설편제_제외설정 = true;      // 건설편제 조건 만족 시 AI부대 무장변경 안함 (특기_축성 보유 또는 금/병력 기준 판단)
    const int 건설편제_기준소지금 = 1500;     // 기준 소지금 이상 & 기준 병력 이하일 경우 건설부대로 취급하여 무장변경 안함
    const int 건설편제_기준병력  = 3000;     // 기준 소지금 이상 & 기준 병력 이하일 경우 건설부대로 취급하여 무장변경 안함
    
    //---------------------------------------------------------------------------------------
    // 주장/부장 능력 비교하여 능력 우선으로 재편함
    const bool 출진무장최적화_지휘재편 = false;     // true= 적용,  false= 미적용
    const bool 지휘재편_상시점검 = false;         // true= 부대이동 시 마다 점검,  false= 부대출진 때만 점검
    const bool 지휘재편_관직무시 = true;         // 관직에 따른 지휘가능병력 무시하고, 통/무 능력 우선으로 주장 재편
    
    //---------------------------------------------------------------------------------------
    
    
    
	class Main
	{
        
        Main()
		{
            pk::bind(102, pk::trigger102_t(onGameInit));
            pk::bind(111, pk::trigger111_t(onTurnStart));
            pk::bind(112, pk::trigger112_t(onTurnEnd));
            
            pk::bind(170, 출진무장최적화_트리거_우선순위, pk::trigger170_t(onUnitCreate));
            pk::bind(172, 출진무장최적화_트리거_우선순위, pk::trigger172_t(onUnitPosChange));
            
        }
        
        int 출진무장최적화_트리거_우선순위 = 100; // 값이 높을수록 먼저 실행 (출진부대최적화.cpp 보다 먼저 실행되도록 설정 필요)
        
        pk::unit@ unit;
        pk::building@ base;
        pk::person@ old_leader;
        pk::person@ new_leader;
        int weapon_heishu = -1;
        bool flag_change_leader = false;
        bool flag_best_leader   = false;
        pk::list<pk::person@> idle_person_list;
        pk::list<pk::unit@> unit_list;
        pk::list<pk::person@> leader_list;
        pk::array<int> leader_location(무장_끝, -1);
        
        void onGameInit()
        {
            if (pk::is_campaign()) return;
            
            clear_leader_info();
        }
        
        void onTurnStart(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            
            clear_leader_info();
            clear_exceed_amount_base();
        }
        
        void onTurnEnd(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            if (!pk::is_alive(force) or !pk::is_normal_force(force)) return;  // 소속 세력 확인  
            
            for (int i = 0; i < 건물_거점끝; i++)
            {
                pk::building@ base_t = pk::get_building(i);
                if (pk::is_alive(base_t) and force.get_id() == base_t.get_force_id())
                    restore_exceed_stocks(base_t);
            }
        }
        
        //---------------------------------------------------------------------------------------
        
        // 부대생성 시
        void onUnitCreate(pk::unit@ unit_t, int type)
        {
            if (pk::is_campaign()) return;
            
            // 정상적인 출진부대인지 확인
            if (!check_valid_unit(unit_t, type)) return ;
            // 출진무장 최적화할 세력 확인
            if (!check_force_option(unit_t)) return ;
            
            if (디버그모드)
            {
                pk::building@ base_t = pk::get_building(pk::get_service(unit_t));
                if (pk::is_alive(base_t)) print_info_base(base_t);
                print_info_unit(unit_t);
            }
            
            // AI건설편제인 경우 ('19.2.15)
            if (건설편제_제외설정 and check_construction_unit(unit_t)) return ;
            
            // 무장 변경 변수 초기화 (유저군단 변경용)
            flag_change_leader = false;
            unit_list.clear();
            
            // 출진부대 주장변경 적용
            if (출진무장최적화_무장변경)
            {
                bool need_leader_change = false;
                
                // 생성된 부대주장이 기존 최적화 편성된 부대의 무장인 경우 출진 취소시킴 ('19.1.21)
                // 중복 출진 확인을 위하여 개체변수들 초기화 안한 상태로 확인함 (중복 출진 버그 개선용!!)
                if (check_duplicated_unit(unit_t)) return;
            
                // 개체변수 업데이트
                @unit = @unit_t;
                @base = pk::get_building(pk::get_service(unit));
                weapon_heishu = pk::equipment_id_to_heishu(unit.weapon);
                
                // 명령 가능한 무장이 있는지 확인
                idle_person_list = pk::get_idle_person_list(base);
                if (idle_person_list.count == 0) return ;    // 미행동 무장 없음 종료
                
                @old_leader = pk::get_person(unit.leader);
                
                // 전투부대 변경
                if (unit.type == 부대종류_전투)
                {
                    // 무장변경 안해도 되는 주장 능력치 확인
                    if (!check_leader_attr(unit)) return;
                    
                    // 무장 능력기준 정렬 : 통솔+무력(+지력),(+적성),(+특기) 
                    idle_person_list.sort(function(a, b)
                    {
                        int stat_a = main.get_stat_sum(a, true) + main.get_stat_tekisei(a) + main.get_stat_skill(a);
                        int stat_b = main.get_stat_sum(b, true) + main.get_stat_tekisei(b) + main.get_stat_skill(b);
                        return (stat_a > stat_b);
                    });
                    @new_leader = pk::get_person(idle_person_list[0].get_id());
                    
                    // 무장 능력 계산식 ('19.2.15 수정)
                    int stat_new_leader = get_stat_sum(new_leader, true)  + get_stat_tekisei(new_leader);     // 개인 지력/적성 stat적용
                    int stat_old_leader = get_stat_sum(old_leader, false) + get_stat_unit_tekisei(unit) 
                                          + int(무장변경_지력가중치 * pk::get_best_member_stat(unit, 무장능력_지력));  // 부대 지력/적성 stat 적용 ('19.2.15)
                    
                    // 무장 변경 실행
                    if (stat_old_leader < stat_new_leader)
                    {
                        need_leader_change = true;  // 변경 필요
                    }
                }
                // 수송부대 변경 ('19.3.27)
                else if (unit.type == 부대종류_수송)    
                {
                    if (unit.has_skill(특기_운반)) return;  // 소속부대 특기_운반 확인
                    if (!pk::is_alive(pk::get_person(base.who_has_skill(특기_운반)))) return;   // 소속거점 특기_운반 무장 확인
                    
                    // 특기_운반 우대 정렬
                    idle_person_list.sort(function(a, b)
                    {
                        bool a_skill = pk::has_skill(a, 특기_운반);
                        bool b_skill = pk::has_skill(b, 특기_운반);
                        if ( a_skill and !b_skill) return true;
                        if (!a_skill and  b_skill) return false;
                        return  (float(a.stat[무장능력_정치]) / float(a.stat[무장능력_무력])) > (float(b.stat[무장능력_정치]) / float(b.stat[무장능력_무력]));
                    });
                    @new_leader = pk::get_person(idle_person_list[0].get_id());
                    
                    // 무장 변경 실행
                    if (pk::has_skill(new_leader, 특기_운반))
                    {
                        need_leader_change = true;  // 변경 필요
                    }
                }
                
                // 출진 주장 변경
                // AI부대 동작조건 수정 ('20.9.5)
                if (need_leader_change)
                {
                    bool chk_self_turn = (unit.get_force_id() == pk::get_current_turn_force_id())? true : false;
                    
                    // 컴퓨터 세력
                    if (!unit.is_player())
                    {
                        if (chk_self_turn) select_change_case(unit, 0);  // AI는 출진취소 안하므로 무장 즉시 변경
                        else               select_change_case(unit, 1);  // 컴퓨터세력 조종 활성화 시 출진취소 가능하므로, 부대가 필드로 이동한 후에 무장 변경하도록 함
                    }
                    // 플레이어 세력 위임군단
                    else if (unit.is_player() and !pk::is_player_controlled(unit))
                        select_change_case(unit, 0);  // AI는 출진취소 안하므로 무장 즉시 변경
                    // 플레이어 세력 유저군단
                    else if (unit.is_player() and pk::is_player_controlled(unit) and 출진무장최적화_유저군단적용)
                        select_change_case(unit, 1);  // 플레이어 군단은 출진취소 가능하므로, 부대가 필드로 이동한 후에 무장 변경하도록 함
                }
            }
            
            // 주장/부장 능력 비교하여 능력 우선 재편
            flag_best_leader = false;
            if (출진무장최적화_지휘재편) flag_best_leader = true;
            
        }
        
        // 부대 좌표 이동 시
        void onUnitPosChange(pk::unit@ unit_t, const pk::point &in pos)
        {
            if (pk::is_campaign()) return;
            
            // 유효부대인지 확인 추가 ('19.5.13)
            if (!pk::is_alive(unit_t) or pk::get_hex(pos).has_building)
            {
                flag_change_leader = false;
                flag_best_leader = false;
                unit_list.clear();
                return;
            }
            // change_case == 0 : 
            if (flag_change_leader and !unit_list.contains(unit_t))
            {
                say_leader_changed();
                flag_change_leader = false;
            }
            // change_case == 1 : 플레이어 군단은 출진취소 가능하므로, 부대가 필드로 이동한 후에 무장 변경하도록 함
            if (flag_change_leader and unit_list.contains(unit_t))
            {
                change_unit_leader();
                say_leader_changed();
                flag_change_leader = false;
                unit_list.clear();
            }
            
            
            // 주장/부장 능력 비교하여 능력 우선으로 재편함
            if (출진무장최적화_지휘재편 and (지휘재편_상시점검 or flag_best_leader))
            {
                set_best_leader(unit_t);
                flag_best_leader = false;
            }
            
        }
        
        //---------------------------------------------------------------------------------------
        // 주장 변경 시퀀스 선택
        void select_change_case(pk::unit@ unit_t, int change_case)
        {
            if (change_case == 0)   // AI auto
            {   // AI는 출진취소 안하므로 무장 즉시 변경
                // 병과변경 패치보다 먼저 동작하도록 트리거 우선순위 설정 필요
                change_unit_leader();   
                flag_change_leader = true;
                unit_list.clear();
                
                if (디버그모드) pk::info("출진무장최적화: 유닛생성 직후 무장변경");
            }
            else if (change_case == 1)  // user control
            {   // 유닛이동 트리거와 연계, 유닛이동 후 무장 변경 및 병력/병기 반환 실행
                flag_change_leader = true;
                unit_list.add(unit_t);
                
                if (디버그모드) pk::info("출진무장최적화: 출진이동 후 무장변경");
            }
        }
            
        // 주장 변경 함수
        void change_unit_leader()
        {
            if (old_leader is null or new_leader is null) return;
            if (base is null or unit is null) return;
            
            string old_leader_name = pk::decode(pk::get_name(old_leader));
            string new_leader_name = pk::decode(pk::get_name(new_leader));
            string base_name       = pk::decode(pk::get_name(base));

            if (디버그모드)
                pk::info(pk::format("출진무장최적화: {}에서 {} 대신 {} 출진", base_name, old_leader_name, new_leader_name));
            
            // 초과 지휘병력 조정 함수 호출
            return_over_troops(base, unit, new_leader);
            
            // 소속 변경 실행 : 무장 location 변경설정을 위하여 hex_obj ID 반환
            old_leader.location = pk::get_hex_object_id(base);
            old_leader.update();
            unit.leader = new_leader.get_id();
            new_leader.location = pk::get_hex_object_id(unit);
            new_leader.update();
            unit.update();  // 부대병과 및 부대능력 업데이트
            
            // 무장 미행동 설정
            old_leader.action_done = false;
            new_leader.action_done = true;
            
            // 태수 재설정 ('19.1.27)
            pk::reset_taishu(base);
            
            // 새주장 무장리스트 및 위치정보 추가 ('19.2.16)
            leader_list.add(new_leader);
            leader_location[new_leader.get_id()] = new_leader.location;
            
        }
        
        void say_leader_changed()
        {
            if (!출진무장최적화_대화표시) return;
            
            if (old_leader is null or new_leader is null) return;
            if (base is null or unit is null) return;
            
            string old_leader_name = pk::decode(pk::get_name(old_leader));
            string new_leader_name = pk::decode(pk::get_name(new_leader));
            string base_name       = pk::decode(pk::get_name(base));
            
            // 출진무장 최적화 안내 대화창
            pk::building@ service = pk::get_building(new_leader.service);
            if (pk::is_alive(service) and (new_leader.location != pk::get_hex_object_id(service)))
                pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x님 대신 이 \x1b[1x{}\x1b[0x이(가)\n 출진하겠습니다", old_leader_name, new_leader_name)), new_leader);
            
        }
        
        // 지휘가능병력 감소 시 소속거점으로 병력/병기 반환 (수송부대는 지휘가능병력 무관함)
        void return_over_troops(pk::building@ base_t, pk::unit@ unit_t, pk::person@ leader_t)
        {
            if (leader_t is null) return;
            if (base_t is null or unit_t is null) return;
            if (unit_t.type == 부대종류_수송) return;    // 수송부대 제외
            
            int troops  = int(unit_t.troops);
            int command = int(pk::get_command(leader_t));


            if (troops > command)
            {
                int return_amount = troops - command;
                int weapon_id = pk::get_ground_weapon_id(unit_t);
                
                // 메시지
                string weapon_name = get_weapon_name(weapon_id);
                string base_name = pk::decode(pk::get_name(base_t));
                string unit_name = pk::decode(pk::get_name(new_leader));
                
                // 병력 반환 시 초과여부 확인 ('20.9.11)
                int exceed_troops = pk::max(0, (return_amount + pk::get_troops(base_t) - pk::get_max_troops(base_t)));
                
                // 병력 반환 실행
                pk::set_troops(unit_t, command);                  // 부대 병력 조정 ('20.9.7)
                pk::add_troops(base_t, +return_amount, false);    // 거점 병력 반환
                unit_t.update();    // 부대정보 및 능력치 업데이트
                
                // 병기 반환 (병력 비례하는 창,극,노,군마만 해당)
                bool flag_exceed = false;
                int exceed_weapon = 0;
                if (병기_창 <= weapon_id and weapon_id <= 병기_군마)
                {
                    // 병기 반환 시 초과여부 확인 ('20.9.11)
                    exceed_weapon = pk::max(0, (return_amount + pk::get_weapon_amount(base_t, weapon_id) - pk::get_max_weapon_amount(base_t, weapon_id)));
                    // 거점 병기 반환
                    pk::add_weapon_amount(base_t, weapon_id, return_amount, false);   
                }
                
                // 반환초과 병력/병기 수량 저장 ('20.9.12, 턴종료 시 일괄 재반환)
                string exceed_info = pk::format("{}대 {} 반환초과 ", unit_name, base_name);
                if (exceed_troops > 0 or exceed_weapon > 0)
                {
                    flag_exceed = true;
                    int base_id = base_t.get_id();
                    is_exceed_base[base_id] = true;
                    exceed_amount_base[base_id][0]           += exceed_troops;   // 병력
                    exceed_amount_base[base_id][3+weapon_id] += exceed_weapon;   // 병기
                    
                    if (exceed_troops > 0) exceed_info += pk::format(",{}({})", arr_amount_name[0]          , exceed_troops);
                    if (exceed_weapon > 0) exceed_info += pk::format(",{}({})", arr_amount_name[3+weapon_id], exceed_weapon);
                }
                
                // 처리 결과 메시지
                if (출진무장최적화_대화표시)
                    pk::say(pk::encode(pk::format("지휘변경에 따라 \x1b[17x{} \x1b[1x{}명\x1b[0x을 \n\x1b[2x{}\x1b[0x(으)로 반환하겠습니다", weapon_name, return_amount, base_name)), new_leader);

                if (디버그모드)
                {
                    pk::info(pk::format("{}대 지휘변경: {} {}명({}→{}) {}(으)로 반환", unit_name, weapon_name, return_amount, troops, command, base_name));
                    
                    if (flag_exceed)
                        pk::info(pk::format("{}", exceed_info));
                    else
                        pk::info(pk::format("{}:없음", exceed_info));
                }
            }
        }
        
        
        //---------------------------------------------------------------------------------------
        // 디버그모드에서 거점/부대 정보 콘솔창에 출력
        void print_info_base(pk::building@ base_t)
        {        
            if (!pk::is_alive(base_t)) return;
            
            string base_name = pk::decode(pk::get_name(base_t));
            string wpn_info  = "";
            for (int i = 0; i < 병기_끝; i++)
            {
                int wpn_amt = pk::get_weapon_amount(base_t, i);
                if (wpn_amt > 0)
                    wpn_info += pk::format(",{}({})", pk::decode(pk::get_name(pk::get_equipment(i))), wpn_amt);
            }
                
            pk::info(pk::format("{} 재고: 병력({}),금({}),병량({}){}", base_name, pk::get_troops(base_t), pk::get_gold(base_t), pk::get_food(base_t), wpn_info));
        }
        void print_info_unit(pk::unit@ unit_t)
        {
            if (!pk::is_alive(unit_t)) return;
            
            string unit_name = pk::decode(pk::get_name(pk::get_person(unit_t.leader)));
            int gnd_wpn_id   = pk::get_ground_weapon_id(unit_t);
            int sea_wpn_id   = pk::get_sea_weapon_id(unit_t);
            string gnd_wpn_name  = pk::decode(pk::get_name(pk::get_equipment(gnd_wpn_id)));
            string sea_wpn_name  = pk::decode(pk::get_name(pk::get_equipment(sea_wpn_id)));
            
            pk::info(pk::format("{}대 출진({},{},{})", unit_name, unit_t.troops, gnd_wpn_name, sea_wpn_name));
        }
        
        //---------------------------------------------------------------------------------------
        //---------------------------------------------------------------------------------------
        
        // 중복 출진부대인지 확인하고 출진 취소시킴
        bool check_duplicated_unit(pk::unit@ unit_t)
        {
            if (!pk::is_alive(unit_t)) return false;
            
            // 부대 편성된 무장 정보
            pk::array<pk::person@> members(3, null);
            for (int i = 0; i < 3; i++)
            {
                pk::person@ member_t = pk::get_person(unit_t.member[i]);
                if (pk::is_alive(member_t))
                    @members[i] = @member_t;
            }
            
            // 중복 편성된 무장 확인 함수 ('19.2.18)
            bool dupl_check = false;
            pk::list<pk::person@> dupl_leaders;
            dupl_leaders.clear();
            for (int i = 0; i < 3; i++)
            {
                if (leader_list.contains(members[i]))
                {
                    dupl_check = true;  // 중복 유효
                    dupl_leaders.add(members[i]);   // 중복리스트 추가
                }
            }
            
            // 중복출진일 경우
            if (pk::is_alive(base) and dupl_check)
            {
                
                // 출진취소 함수 --- 물자반환 방식 수정 ('20.9.9)
                int unit_id   = unit_t.get_id();
                int leader_id = unit_t.leader;
                update_unit_cancel_info(unit_t, base);   // 출진취소한 부대의 물자정보 저장
                update_exceed_amount(unit_t, base);      // 물자반환 시 초과되어 증발되는 물자량 계산 (턴종료 시 다시 반환)
                restore_unit_stocks(unit_id, leader_id); // 부대 물자반환 실행
                cancel_unit_march(unit_t);               // 출진취소 실행 (부대제거), 거점 물자 차감
                
                if (디버그모드) print_info_base(base);
                
                // 나머지 무장 행동: 출진취소하였으므로 미행동으로 변경 ('19.2.18)
                for (int j = 0; j < 3; j++) // 현재부대무장 (최대3명까지)
                {
                    pk::person@ member_t = members[j];
                    if (pk::is_alive(member_t) and !dupl_leaders.contains(member_t))
                    {
                        member_t.location = pk::get_hex_object_id(base);
                        member_t.action_done = false;
                        member_t.update();
                    }
                }
                
                // 중복출진된 무장의 소재 재지정 함수 ('19.2.18)
                pk::array<pk::person@> arr_dupl = pk::list_to_array(dupl_leaders);
                for (int j = 0; j < int(arr_dupl.length); j++)
                {
                    pk::person@ dupl_leader = arr_dupl[j];
                    int location_id = leader_location[dupl_leader.get_id()];
                    pk::unit@ dupl_unit = pk::get_unit(pk::hex_object_id_to_unit_id(location_id));
                    if (pk::is_alive(dupl_unit))
                    {
                        dupl_leader.location = location_id;
                        dupl_unit.update();
                    }
                    else
                    {
                        dupl_leader.location = pk::get_hex_object_id(base);
                        base.update();
                    }
                    dupl_leader.action_done = true;
                }
                
                // 태수 업데이트 ('19.2.12)
                pk::reset_taishu(base);
                
                return true;
            }
            return false;
        }
        
        
        // 출진 취소 함수
        void cancel_unit_march(pk::unit@ unit_t)
        {
            string unit_name = pk::decode(pk::get_name(pk::get_person(unit_t.leader)));
            
            // 부대해산
            pk::kill(unit_t);
            
            // 행동력 반환
            pk::district@ district = pk::get_district(base.get_district_id());
            district.ap += get_march_ap(base);
            
            if (디버그모드)
                pk::info(pk::format("{}대 중복출진취소", unit_name));
        }
        
        
        //---------------------------------------------------------------------------------------

        // 물자 반환 정보 초기화
        array<int> unit_cancel_info(18, -1);
        void clear_unit_cancel_info()
        {
            for (int i = 0; i < 18; i++)
            {
                if (i < 3) unit_cancel_info[i] = -1;
                else unit_cancel_info[i] = 0;
            }
        }
        
        // 물자 반환 정보 유닛제거 전 저장 (유닛ID, 소속거점ID, 타입, 병력, 금, 병량, 병기_0~11)
        void update_unit_cancel_info(pk::unit@ unit_t, pk::building@ base_t)
        {
            if(!pk::is_alive(unit_t)) return;
            if(!pk::is_alive(base_t)) return;
            
            // 물자 반환 정보 초기화
            clear_unit_cancel_info();
            
            // 부대 정보 업데이트
            unit_cancel_info[0] = unit_t.get_id();
            unit_cancel_info[1] = base_t.get_id();
            unit_cancel_info[2] = unit_t.type;
            unit_cancel_info[3] = unit_t.troops;
            unit_cancel_info[4] = unit_t.gold;
            unit_cancel_info[5] = unit_t.food;
            
            if (unit_t.type == 부대종류_전투)
            {
                // 육상병기
                int gnd_wpn_id = pk::get_ground_weapon_id(unit_t);
                int gnd_wpn_amount = 0;
                if     (병기_충차 <= gnd_wpn_id and gnd_wpn_id <= 병기_목수) gnd_wpn_amount = 1;
                else if (병기_창 <= gnd_wpn_id and gnd_wpn_id <= 병기_군마) gnd_wpn_amount = unit_t.troops;
                // 함선병기
                int sea_wpn_id = pk::get_sea_weapon_id(unit_t);
                int sea_wpn_amount = (병기_주가 < sea_wpn_id)? 1 : 0;
                
                unit_cancel_info[6] = gnd_wpn_id;
                unit_cancel_info[7] = gnd_wpn_amount;
                unit_cancel_info[8] = sea_wpn_id;
                unit_cancel_info[9] = sea_wpn_amount;
            }
            else if (unit_t.type == 부대종류_수송)
            {
                for (int i = 0; i < 12; i++)
                    unit_cancel_info[6 + i] = unit_t.stock[i].amount;
            }            
        }
        
        // 부대제거 후 거점으로 물자반환 실행함수 (유닛ID 를 Key값으로 이용)
        void restore_unit_stocks(int unit_id, int leader_id)
        {
            if (unit_cancel_info[0] != unit_id) return;
            
            // 부대 정보 업데이트
            pk::building@ base_t = pk::get_building(unit_cancel_info[1]);   // 거점
            int type   = unit_cancel_info[2];   // 부대종류 (전투, 수송)
            int troops = unit_cancel_info[3];   // 병력
            int gold   = unit_cancel_info[4];   // 금
            int food   = unit_cancel_info[5];   // 병량
            
            string unit_name = pk::decode(pk::get_name(pk::get_person(leader_id)));
            string base_name = pk::decode(pk::get_name(base_t));
            
            // 공통: 소속도시로 병력/금/병량 물자 되돌림
            pk::add_gold(base_t, gold, false);
            pk::add_food(base_t, food, false);
            pk::add_troops(base_t, troops, false);
            
            string restore_info = pk::format("병력({}),금({}),병량({})", troops, gold, food);
            
            if (type == 부대종류_전투)
            {
                int gnd_wpn_id     = unit_cancel_info[6];
                int gnd_wpn_amount = unit_cancel_info[7];
                int sea_wpn_id     = unit_cancel_info[8];
                int sea_wpn_amount = unit_cancel_info[9];
                
                if (gnd_wpn_id > 0 and gnd_wpn_amount > 0)
                    pk::add_weapon_amount(base_t, gnd_wpn_id, gnd_wpn_amount, false);
                
                if (sea_wpn_id > 병기_주가 and sea_wpn_amount > 0)
                    pk::add_weapon_amount(base_t, sea_wpn_id, sea_wpn_amount, false);
                
                string gnd_wpn_name = pk::decode(pk::get_name(pk::get_equipment(gnd_wpn_id)));
                string sea_wpn_name = pk::decode(pk::get_name(pk::get_equipment(sea_wpn_id)));
                restore_info += pk::format(",{}({}),{}({})", gnd_wpn_name, gnd_wpn_amount, sea_wpn_name, sea_wpn_amount);
            }
            else if (type == 부대종류_수송)
            {
                for (int i = 0; i < 병기_끝; i++)
                {
                    int wpn_amount = unit_cancel_info[6 + i];
                    if (wpn_amount > 0)
                    {
                        pk::add_weapon_amount(base, i, wpn_amount, false);
                    
                        string wpn_name = pk::decode(pk::get_name(pk::get_equipment(i)));
                        restore_info += pk::format(",{}({})", wpn_name, wpn_amount);
                    }
                }
            }     
            
            if (디버그모드)
                pk::info(pk::format("{}대 중복출진취소, {}(으)로 {} 반환", unit_name, base_name, restore_info));
            
            // 물자 반환 정보 초기화
            clear_unit_cancel_info();
        }
        
        
        // 물자 반환 함수 ----- 수송부대 추가 ('20.9.6) ... 사용하지 않음
        void return_unit_stocks(pk::unit@ unit_t)
        {
            string unit_name = pk::decode(pk::get_name(pk::get_person(unit_t.leader)));
            string base_name = pk::decode(pk::get_name(base));
            
            // 공통: 소속도시로 병력/금/병량 물자 되돌림
            pk::add_gold(base, unit_t.gold, false);
            pk::add_food(base, unit_t.food, false);
            pk::add_troops(base, unit_t.troops, false);
            
            string restore_info = pk::format("병력({}),금({}),병량({})", unit_t.troops, unit_t.gold, unit_t.food);
            
            // 전투부대: 소속도시로 병장기 물자 되돌림
            if (unit_t.type == 부대종류_전투)
            {   
                // 육상병기
                int gnd_wpn_id = pk::get_ground_weapon_id(unit_t);
                int gnd_wpn_amount = 0;
                if     (병기_충차 <= gnd_wpn_id and gnd_wpn_id <= 병기_목수) gnd_wpn_amount = 1;
                else if (병기_창 <= gnd_wpn_id and gnd_wpn_id <= 병기_군마) gnd_wpn_amount = unit_t.troops;
                if (gnd_wpn_id > 0 and gnd_wpn_amount > 0)
                    pk::add_weapon_amount(base, gnd_wpn_id, gnd_wpn_amount, false);
                // 함선병기
                int sea_wpn_id = pk::get_sea_weapon_id(unit_t);
                int sea_wpn_amount = (병기_주가 < sea_wpn_id)? 1 : 0;
                if (sea_wpn_id > 병기_주가 and sea_wpn_amount > 0)
                    pk::add_weapon_amount(base, sea_wpn_id, sea_wpn_amount, false);
                
                string gnd_wpn_name = pk::decode(pk::get_name(pk::get_equipment(gnd_wpn_id)));
                string sea_wpn_name = pk::decode(pk::get_name(pk::get_equipment(sea_wpn_id)));
                restore_info += pk::format(",{}({}),{}({})", gnd_wpn_name, gnd_wpn_amount, sea_wpn_name, sea_wpn_amount);
            }
            // 수송부대: 소속도시로 병장기 물자 되돌림
            else if (unit_t.type == 부대종류_수송)
            {   
                for (int i = 0; i < 병기_끝; i++)
                {
                    int wpn_amount = unit_t.stock[i].amount;
                    if (wpn_amount > 0)
                    {
                        pk::add_weapon_amount(base, i, wpn_amount, false);
                    
                        string wpn_name = pk::decode(pk::get_name(pk::get_equipment(i)));
                        restore_info += pk::format(",{}({})", wpn_name, wpn_amount);
                    }
                }
            }
            
            if (디버그모드)
                pk::info(pk::format("{}대 중복출진취소, {}(으)로 {} 반환", unit_name, base_name, restore_info));
        }
        
        //---------------------------------------------------------------------------------------
        //---------------------------------------------------------------------------------------
        
        // 물자 반환 시 초과분 저장했다가 턴종료 시 일괄 반영
        array<bool> is_exceed_base(건물_거점끝, false);
        array<array<int>> max_amount_base(건물_거점끝, array<int>(15, 0));
        array<array<int>> exceed_amount_base(건물_거점끝, array<int>(15, 0));
        array<string> arr_amount_name = {"병력", "금", "병량", "검", "창", "극", "노", "군마", "충차", "정란", "투석", "목수", "주가", "누선", "투함"};
        
        // 거점 초과분 저장 : 병력, 금, 병량, 병기0~11
        void update_exceed_amount(pk::unit@ unit_t, pk::building@ base_t)
        {
            if (!pk::is_alive(base_t)) return;
            if (!pk::is_alive(unit_t)) return;
            
            update_max_amount_base(base_t);
            
            // --- 거점 현재 재고 확인
            array<int> arr_amount_base(15, 0);
            arr_amount_base[0] = pk::get_troops(base_t);  // 병력
            arr_amount_base[1] = pk::get_gold(base_t);    // 금
            arr_amount_base[2] = pk::get_food(base_t);    // 병량
            for (int j = 0; j < 병기_끝; j++)
            {
                arr_amount_base[3 + j] = pk::get_weapon_amount(base_t, j);
            }
            
            // 재고 초과 반환량 계산
            string unit_name = pk::decode(pk::get_name(pk::get_person(unit_t.leader)));
            string base_name = pk::decode(pk::get_name(base_t));
            string exceed_info_t = pk::format("{}대 {} 반환초과 ", unit_name, base_name);
            string exceed_info   = exceed_info_t;
            
            int base_id = base_t.get_id();
            if (unit_t.type == 부대종류_수송)
            {
                for (int i = 0; i < 15; i++)    // 병력, 금, 병량, 무기0~11 초과량
                {
                    int exceed_amount = pk::max(0, (arr_amount_base[i] + unit_cancel_info[3+i]) - max_amount_base[base_id][i]);
                    exceed_amount_base[base_id][i] += exceed_amount;
                    
                    if (exceed_amount > 0) exceed_info += pk::format(",{}({})", arr_amount_name[i], exceed_amount);
                }
            }
            else if (unit_t.type == 부대종류_전투)
            {
                for (int i = 0; i < 3; i++) // 병력, 금, 병량 초과량
                {
                    int exceed_amount = pk::max(0, (arr_amount_base[i] + unit_cancel_info[3+i]) - max_amount_base[base_id][i]);
                    exceed_amount_base[base_id][i] += exceed_amount;
                    
                    if (exceed_amount > 0) exceed_info += pk::format(",{}({})", arr_amount_name[i], exceed_amount);
                }
                // 전투부대 병기 초과량
                int gnd_wpn_id     = unit_cancel_info[6];
                int gnd_wpn_amount = unit_cancel_info[7];
                int sea_wpn_id     = unit_cancel_info[8];
                int sea_wpn_amount = unit_cancel_info[9];
                
                int exceed_gnd_wpn = pk::max(0, (arr_amount_base[3+gnd_wpn_id] + gnd_wpn_amount) - max_amount_base[base_id][3+gnd_wpn_id]);
                int exceed_sea_wpn = pk::max(0, (arr_amount_base[3+sea_wpn_id] + sea_wpn_amount) - max_amount_base[base_id][3+sea_wpn_id]);
                
                exceed_amount_base[base_id][3+gnd_wpn_id] += exceed_gnd_wpn;
                exceed_amount_base[base_id][3+sea_wpn_id] += exceed_sea_wpn;
                
                if (exceed_gnd_wpn > 0) exceed_info += pk::format(",{}({})", arr_amount_name[3+gnd_wpn_id], exceed_gnd_wpn);
                if (exceed_sea_wpn > 0) exceed_info += pk::format(",{}({})", arr_amount_name[3+sea_wpn_id], exceed_sea_wpn);
            }

            // 초과물자 발생 거점 flag
            bool flag_exceed = false;
            if (exceed_info_t != exceed_info)
            {
                flag_exceed = true;
                is_exceed_base[base_id] = true;
            }
            
            if (디버그모드)
            {
                if (flag_exceed)
                    pk::info(pk::format("{}", exceed_info));
                else
                    pk::info(pk::format("{}:없음", exceed_info_t));
            }
        }
        
        // 초과 물자 턴종료 반환 : 병력, 금, 병량, 병기0~11
        void restore_exceed_stocks(pk::building@ base_t)
        {
            if (!pk::is_alive(base_t)) return;
            
            int base_id = base_t.get_id();
            if (!is_exceed_base[base_id]) return;   // 초과물자 미발생 거점 제외
            
            pk::add_troops(base_t, exceed_amount_base[base_id][0], false); // 병력
            pk::add_gold(base_t,   exceed_amount_base[base_id][1], false); // 금
            pk::add_food(base_t,   exceed_amount_base[base_id][2], false); // 병량
            
            string base_name = pk::decode(pk::get_name(base_t));
            string restore_info = pk::format("병력({}),금({}),병량({})", exceed_amount_base[base_id][0], exceed_amount_base[base_id][1], exceed_amount_base[base_id][2]);
            
            for (int j = 0; j < 병기_끝; j++)
            {
                int exceed_amount = exceed_amount_base[base_id][3 + j];
                pk::add_weapon_amount(base_t, j, exceed_amount);    // 병기
                
                string wpn_name = pk::decode(pk::get_name(pk::get_equipment(j)));
                if (exceed_amount > 0)
                    restore_info += pk::format(",{}({})", wpn_name, exceed_amount);
            }
            
            if (디버그모드)
            {
                pk::info(pk::format("{} 초과물자 반환처리: {} 반환", base_name, restore_info));
                print_info_base(base_t);
            }
        }
        
        // 거점 최대값 업데이트
        void update_max_amount_base(pk::building@ base_t)
        {
            if (!pk::is_alive(base_t)) return;
            
            int base_id = base_t.get_id();
            max_amount_base[base_id][0] = pk::get_max_troops(base_t);  // 병력
            max_amount_base[base_id][1] = pk::get_max_gold(base_t);    // 금
            max_amount_base[base_id][2] = pk::get_max_food(base_t);    // 병량
            
            for (int j = 0; j < 병기_끝; j++)
            {
                max_amount_base[base_id][3 + j] = pk::get_max_weapon_amount(base_t, j);
            }
            
        }
        
        // 거점별 초과 반환값 초기화
        void clear_exceed_amount_base()
        {
            for (int i = 0; i < 건물_거점끝; i++)
            {
                is_exceed_base[i] = false;
                
                for (int j = 0; j < 15; j++)
                {
                    exceed_amount_base[i][j] = 0;
                }
            }
        }
        
        
        //---------------------------------------------------------------------------------------
        //---------------------------------------------------------------------------------------
        
        void set_best_leader(pk::unit@ unit_t)
        {
            if (!pk::is_alive(unit_t)) return ;
            
            // 출진무장 최적화할 세력 확인
            if (!check_force_option(unit_t)) return ;
            
            // 무장 편성정보
            pk::person@ leader0 = pk::get_person(unit_t.leader);    // 기존 주장
            pk::list<pk::person@> member_list;
            for (int i = 0; i < 3; i++)
            {
                pk::person@ person_t = pk::get_person(unit_t.member[i]);
                if (pk::is_alive(person_t))
                    member_list.add(person_t);
            }
            // 단독 편성 부대인 경우 취소종료
            if (member_list.count < 2) return;  
            
            // 통솔+무력 기준 무장 정렬 (지력, 적성 제외)
            member_list.sort(function(a, b)
            {
                int stat_a = main.get_stat_sum(a, false);
                int stat_b = main.get_stat_sum(b, false);
                int cmd_a = pk::get_command(a);
                int cmd_b = pk::get_command(b);

                if (지휘재편_관직무시)  return (stat_a > stat_b);
                else   return ((cmd_a == cmd_b)? (stat_a > stat_b) : (cmd_a > cmd_b));
            });
            
            // 초과 지휘병력 조정 함수 호출
            return_over_troops(base, unit_t, member_list[0]);
            
            // 주장/부장 재지정
            for (int j = 0; j < int(member_list.count); j++)
            {
                unit_t.member[j] = member_list[j].get_id();
                unit_t.update();
            }
            pk::person@ leader1 = pk::get_person(unit_t.leader);    // 새 주장
            
            // 부대지휘 최적화 안내 대화창
            if (출진무장최적화_대화표시 and leader0.get_id() != leader1.get_id() )
            {
                string old_leader_name = pk::decode(pk::get_name(leader0));
                string new_leader_name = pk::decode(pk::get_name(leader1));
                pk::say(pk::encode(pk::format("이제부터 \x1b[1x{}\x1b[0x님 대신\n\x1b[1x{}\x1b[0x이(가) 부대를 지휘하겠습니다", old_leader_name, new_leader_name)), leader1);
            }
        }
        
        //---------------------------------------------------------------------------------------
        
        // 정상적인 출진부대인지 확인
        bool check_valid_unit(pk::unit@ unit_t, int type)
        {
            if (!pk::is_alive(unit_t))      return false;
            if (type != 0)                  return false;	// 출진 부대가 아닌 경우 제외 (임의 생성 부대)
			
            if (unit_t.attr.stat[부대능력_이동] < 2)	return false;	// 2칸 이상 이동불가 시 부대생성 자동 취소 (이동력 2~4 = 평지 1칸)
            if (pk::get_service(unit_t) >= 건물_거점끝) return false;      // 소속 거점 확인
            
            int force_id = unit_t.get_force_id();
            pk::force@ force = pk::get_force(force_id);
			if (!pk::is_alive(force) or !pk::is_normal_force(force)) return false;  // 소속 세력 확인  
            //if (force_id != pk::get_current_turn_force_id()) return false;  // 현재 턴세력 아닌 경우 ('19.2.15)        
            
            return true;
        }
        
        // 출진무장 최적화할 세력 확인
        bool check_force_option(pk::unit@ unit_t)
        {
            // 플레이어 군단 미적용 (유저의 부대편성 무시하면서 주장편성하므로 미적용 권장)
            if (pk::is_player_controlled(unit_t)) 
            {
                if (출진무장최적화_유저군단적용) return true;
                else return false;   
            }
            
            // 0: 컴퓨터AI 와 플레이어_위임군단 모두,  1: 플레이어_위임군단만, 2: 컴퓨터AI만,  3: 모두 미적용
            if      (출진무장최적화_세력조건 == 0 and !pk::is_player_controlled(unit_t)) return true;
            else if (출진무장최적화_세력조건 == 1 and unit_t.is_player() and !pk::is_player_controlled(unit_t)) return true;
            else if (출진무장최적화_세력조건 == 2 and !unit_t.is_player()) return true;
            else if (출진무장최적화_세력조건 == 3) return false;
            
            return false;
        }
        
        // 건설편제 여부 판단 ('19.2.15)
        bool check_construction_unit(pk::unit@ unit_t)
        {
            if (pk::is_player_controlled(unit_t)) return false; // 유저군단은 무조건 제외
            
            if (unit_t.has_skill(특기_축성) and unit_t.gold >= 건설편제_기준소지금) return true;  // 축성 특기
            
            if (unit_t.type == 부대종류_전투 and pk::get_member_count(unit_t) <= 1)   // 1인대
            {
                if (unit_t.gold >= 건설편제_기준소지금 and unit_t.troops <= 건설편제_기준병력) 
                    return true; 
            }
            return false;
        }
        
        // 무장변경 안해도 되는 부대능력치 조건 확인
        bool check_leader_attr(pk::unit@ unit_t)
        {
            int stat_ref = int(무장변경_능력기준값 * (무장변경_통솔가중치 + 무장변경_무력가중치 + 무장변경_지력가중치));
            
            pk::person@ leader_t = pk::get_person(unit_t.leader);
            
            // 비교에 사용할 지력값 변경 : 개인지력 → 부대지력 ('19.2.15)
            //int stat_chk = get_stat_sum(leader_t, true);
            int stat_chk = get_stat_sum(leader_t, false) + int(무장변경_지력가중치 * pk::get_best_member_stat(unit_t, 무장능력_지력));
            
            bool chk_tekisei = true;
            if (무장변경_적성기준적용)
            {
                //chk_tekisei = (leader_t.tekisei[weapon_heishu] > pk::max(0, pk::min(3, 무장변경_적성기준값))); //개인적성
                chk_tekisei = (pk::get_tekisei(unit_t) > pk::max(0, pk::min(3, 무장변경_적성기준값)));           //부대적성
            }
            if (stat_chk > stat_ref and chk_tekisei) return false;
            
            return true;
        }
        
        //---------------------------------------------------------------------------------------
        
        // 부대출진 행동력 계산
        int get_march_ap(pk::building@ building)
        {
            pk::city@ city = pk::building_to_city(building);
            if (pk::is_alive(city))
            {
                if (pk::has_facility(city, 시설_군사부))
                    return 5;
            }
            return 10;
        }
        
        // 무장 능력치 계산식
        int get_stat_sum(pk::person@ person_t, bool include_wisdom)
        {
            int stat = int(무장변경_통솔가중치 * person_t.stat[무장능력_통솔] 
                         + 무장변경_무력가중치 * person_t.stat[무장능력_무력]);
                         
            if (include_wisdom)
                stat += int(무장변경_지력가중치 * person_t.stat[무장능력_지력]);
            
            return stat;
        }
        
        // 무장 적성 능력가중치 환산
        int get_stat_tekisei(pk::person@ person_t)
        {
            int stat = 0;
            int tekisei = person_t.tekisei[weapon_heishu];
            if (무장변경_적성기준적용)
            {
                int stat_base = (tekisei > pk::max(0, pk::min(3, 무장변경_적성기준값)))? 30 : 0;
                stat = int(무장변경_적성가중치 * (stat_base + 15 * pk::min(3, tekisei)));
            }
            return stat;
        }
        
        // 부대 적성 능력가중치 환산
        int get_stat_unit_tekisei(pk::unit@ unit_t)
        {
            int stat = 0;
            int tekisei = pk::get_tekisei(unit_t, unit_t.weapon);
            if (무장변경_적성기준적용)
            {
                int stat_base = (tekisei > pk::max(0, pk::min(3, 무장변경_적성기준값)))? 30 : 0;
                stat = int(무장변경_적성가중치 * (stat_base + 15 * pk::min(3, tekisei)));
            }
            return stat;
        }
        
        // 무장 특기 가중치 환산
        int get_stat_skill(pk::person@ person_t)
        {
            int stat = 0;
            if(무장변경_특기가중치)
            {
                if (pk::has_skill(person_t, 특기_비장)) stat = pk::max(5, stat);
                if (pk::has_skill(person_t, 특기_패왕)) stat = pk::max(5, stat);
                if (pk::has_skill(person_t, 특기_용장)) stat = pk::max(3, stat);
                    
                if (weapon_heishu == 병종_창병)
                {
                    if (pk::has_skill(person_t, 특기_신장)) stat = pk::max(4, stat);
                    if (pk::has_skill(person_t, 특기_투신)) stat = pk::max(4, stat);
                    if (pk::has_skill(person_t, 특기_창신)) stat = pk::max(5, stat);
                    if (pk::has_skill(person_t, 특기_창장)) stat = pk::max(5, stat);
                    if (pk::has_skill(person_t, 특기_맹장)) stat = pk::max(3, stat);
                }
                else if (weapon_heishu == 병종_극병)
                {
                    if (pk::has_skill(person_t, 특기_신장)) stat = pk::max(4, stat);
                    if (pk::has_skill(person_t, 특기_투신)) stat = pk::max(4, stat);
                    if (pk::has_skill(person_t, 특기_극신)) stat = pk::max(5, stat);
                    if (pk::has_skill(person_t, 특기_극장)) stat = pk::max(5, stat);
                    if (pk::has_skill(person_t, 특기_맹장)) stat = pk::max(3, stat);
                    if (pk::has_skill(person_t, 특기_위풍)) stat = pk::max(3, stat);
                }
                else if (weapon_heishu == 병종_노병)
                {
                    if (pk::has_skill(person_t, 특기_신장)) stat = pk::max(3, stat);
                    if (pk::has_skill(person_t, 특기_궁신)) stat = pk::max(5, stat);
                    if (pk::has_skill(person_t, 특기_궁장)) stat = pk::max(5, stat);
                    if (pk::has_skill(person_t, 특기_사수)) stat = pk::max(3, stat);
                    if (pk::has_skill(person_t, 특기_위풍)) stat = pk::max(3, stat);
		    if (pk::has_skill(person_t, 특기_공성)) stat = pk::max(3, stat);
                }
                else if (weapon_heishu == 병종_기병)
                {
                    if (pk::has_skill(person_t, 특기_신장)) stat = pk::max(3, stat);
                    if (pk::has_skill(person_t, 특기_기신)) stat = pk::max(5, stat);
                    if (pk::has_skill(person_t, 특기_기장)) stat = pk::max(5, stat);
                    if (pk::has_skill(person_t, 특기_백마)) stat = pk::max(4, stat);
                    if (pk::has_skill(person_t, 특기_질주)) stat = pk::max(5, stat);
                    if (pk::has_skill(person_t, 특기_맹장)) stat = pk::max(3, stat);
                }
                else if (weapon_heishu == 병종_병기)
                {
                    if (pk::has_skill(person_t, 특기_공신)) stat = pk::max(5, stat);
                    if (pk::has_skill(person_t, 특기_공성)) stat = pk::max(3, stat);
                    if (pk::has_skill(person_t, 특기_사정)) stat = pk::max(4, stat);
                    if (pk::has_skill(person_t, 특기_주악)) stat = pk::max(2, stat);
                }
            }
            return stat;
        }
        
        
        //---------------------------------------------------------------------------------------
        
        // 출진변경 정보 초기화
        void clear_leader_info()
        {
            leader_list.clear();
            for (int i = 0; i < 무장_끝; i++)
            {
                leader_location[i] = -1;
            }
        }
        
        // 변수 초기화 함수 (미사용)
        void clear_variant()
        {
            @old_leader = null;
            @new_leader = null;
            @unit = null;
            @base = null;  
        }
        
        string get_weapon_name(int weapon_id)
        {
            string weapon_name;
            switch(weapon_id)
            {
                case 병기_검  : weapon_name = "검병"; break;
                case 병기_창  : weapon_name = "창병"; break;
                case 병기_극  : weapon_name = "극병"; break;
                case 병기_노  : weapon_name = "노병"; break;
                case 병기_군마 : weapon_name = "기병"; break;
                case 병기_충차 : weapon_name = "충차"; break;
                case 병기_정란 : weapon_name = "정란"; break;
                case 병기_투석 : weapon_name = "투석"; break;
                case 병기_목수 : weapon_name = "목수"; break;
                case 병기_주가 : weapon_name = "수군"; break;
                case 병기_누선 : weapon_name = "수군"; break;
                case 병기_투함 : weapon_name = "수군"; break;
                default : weapon_name = "??"; break;
            }
            return weapon_name;
        }
        
        //---------------------------------------------------------------------------------------
        
        bool 디버그모드 = false;
        
    }
    
	Main main;
}

