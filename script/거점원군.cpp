/*
// 작성자: 기마책사
// Update: '18.11.10   / 변경내용 : 거점원군 기능 신규 추가
// Update: '19.3.4     / 변경내용 : 거점원군 세팅값 일부 조정, 수송부대/전투부대 선택 알고리즘 적용, 원군부대수 조절기능 추가, 다수거점에서 원군출병 가능
// Update: '19.3.7     / 변경내용 : 교전거점 무장파병 기능 추가, 관문/항구로 수송시 병력/물자 최대치 조정 (증발방지), 출진거점 검색함수 개선
// Update: '19.3.14    / 변경내용 : 거점검색모드 추가, 원군거점 검색오류 조치, 도시거점 원군인 경우 다방면 분산출진 기능 추가, 원군출병 거점의 전방/후방여부에 따라 기준병력 가변
// Update: '19.3.16    / 변경내용 : 교전병력 열세인 거점부터 원군파병
// Update: '19.3.17    / 변경내용 : 동맹세력원군 파병설정 추가
// Update: '19.3.30    / 변경내용 : 동맹세력원군 파병설정 유저세력 제외
// Update: '19.5.26    / 변경내용 : 난이도_특급 설정 시에만 동작하도록 변경
// Update: '20.2.8     / 변경내용 : 동맹세력원군은 무조건 전투부대로 출병하도록 수정
// Update: '20.9.15    / 변경내용 : 전투병과 노병특기 우대함수 오류 수정, 병력있는 빈거점으로 무장소환 추가, 거점원군 실행함수 분리, 거점공략 지원군 기능 추가
// Update: '20.9.20    / 변경내용 : 거점주변 검색 시 부대좌표의 영역도시 확인 추가 (교전 중인 타거점과 거리가 가까운 경우 오인식 안되도록 함)
*/


namespace 거점원군
{
    
    //=======================================================================================
    // 거점방어원군 (by 기마책사)
    const bool 거점원군_설정 = true;       // 인접 도시 원군 수송대 출병 설정
    
    const int  거점원군_적군검색범위 = 5;    // 교전 거점에서부터 적부대를 검색하는 거리 (관문/항구는 최대 6으로 제한됨)
    
    const int  거점원군_거점최소병력 = 20000;  // 적거점 비인접한 후방거점, 기준병력 이상 보유 시 초과 병력만큼 원군 수송
    const int  거점원군_경계병력단위 = 5000;   // 적거점과 인접한 경우, 경계병력 단위로 적병력에 비례하여 거점최소병력 증가
    
    const int  거점원군_최대전투병력 = 8000;   // 전투부대 출진 시 최대병력 기준
    const int  거점원군_최소출진병력 = 3000;   // 출진할 부대 최소병력 기준, 여유 병력이 최소병력 기준 미달 시 출진 안함
    const int  거점원군_도시출진거리 = 2;      // 도시인 경우 거리 만족 거점 있을 시 원군 출병 (1 ~ 3 사이), 관문/항구는 무조건 거리 1 이내만 검색
    const int  거점원군_출진거점무장수 = 2;     // 원군출진할 거점의 잔류무장수, 기준수 이상이고 미행동 있을 경우 출진가능 (적 거점 주변에 있을 경우 잔류무장수 비례증가)
    
    const int  거점원군_거점검색모드 = 1;      // 0: 도시만 검색하여 원군출병,  1: 도시+관문+항구 모두 검색하여 원군출병
    
    // 부대종류모드
    // ㄴ 2번 모드 사용 시 거점간 거리, 거점별 무장수를 기준으로 병종선택 (원거리거점, 교전거점 무장없거나, 교전거점 무장충분, 출진거점 무장부족 시 수송부대 출진)
    const int  거점원군_부대종류모드 = 2;        // 2: 전투/수송부대 모두 허용,  1: 수송부대만 허용,  0: 전투부대만 허용
    const int  거점원군_교전거점무장수 = 10;     // 부대종류모드2 적용 시 : 교전거점의 무장수 기준 이상일 경우 무장충분으로 판단하여 수송부대 출병
    
    const int  거점원군_전투부대수 = 4;      // 한 턴에 출진가능한 최대 부대수 (단, 거점간 거리에 따라 한 거점에서 출진가능 부대수 거리당 1씩 차감, 최소 1부대 가능)
    const int  거점원군_수송부대수 = 2;      // 한 턴에 출진가능한 최대 부대수
    
    const bool 거점원군_무장파병설정 = true;    // 빈 거점에 적군 침략 시 인접 거점에서 무장 파병지원
    
    const bool 거점원군_동맹세력원군 = true;    // 동맹세력의 거점에서도 원군 파병
    
    //------------------------------------------------------------
    // 거점공략원군 (by 기마책사)
    const bool 거점공략_설정 = true;       // 인접한 공략도시에 지원군 출병 설정 (수송대, 공성병기, 전투부대)
    
    const int  거점공략_전투부대수  = 2;     // 공략지원군 전투부대 출진 부대수 (최소1 ~ 최대3)
    
    const bool 거점공략_수송부대허용 = false;   // 공략지원군 수송대 허용
    const int  거점공략_수송부대수 = 1;     // 공략지원군 수송부대 출진 부대수 (최소1 ~ 최대3)
    
    const bool 거점공략_공성병기허용 = true;   // 공략지원군 공성병기 허용
    const int  거점공략_공성부대수 = 3;      // 공략지원군 공성부대 출진 부대수 (최소1 ~ 최대5)
    
    
    //=======================================================================================
    
	class Main
	{
		pk::func261_t@ prev_callback_261;

		Main()
		{
			@prev_callback_261 = cast<pk::func261_t@>(pk::get_func(261));
			pk::reset_func(261);
			pk::set_func(261, pk::func261_t(callback_261));
		}
        
		bool callback_261(pk::force@ force)
		{
			int force_id = force.get_id();

            if (!pk::is_campaign())
			{
                // 플레이어, 이민족 세력은 제외.
                // 난이도 특급 시에만 동작 추가 ('19.5.26)
                if (!force.is_player() and pk::is_normal_force(force_id) and pk::get_scenario().difficulty == 난이도_특급)
                {
                    
                    if (거점원군_설정) func_reinforcement(force);   // 거점 원군 출병 ('18.11.10, 추가)
                    if (거점공략_설정) func_attack_support(force);  // 거점공략지원 실행함수 호출 ('20.9.14, 추가)
                }
			}

			// 이전 핸들러가 있다면 체인.
			if (prev_callback_261 !is null)
				return prev_callback_261(force);
            
			return false;
		}
        
        //---------------------------------------------------------------------------------------
        //          거점 원군 실행 함수
        //---------------------------------------------------------------------------------------
        
        // 수정금지
        pk::list<pk::building@> list_engage_base;        // 원군 필요한 교전거점 리스트
        pk::list<pk::building@> list_reinforce_base;     // 원군 출진거점 리스트
        pk::array<int> arr_reinforce_count(건물_거점끝, 0);   // 원군 후보거점 출진대상 거점수
        pk::array<int> arr_engage_troops(건물_거점끝, 0);     // 교전중인 거점의 병력차
        pk::array<int> unit_count(2, 0);   // [0] 전투부대 출진수, [1] 수송부대 출진수
        
        //----- 거점원군 실행함수
        void func_reinforcement(pk::force@ force)
        {
            int force_id = force.get_id();
            
            list_engage_base.clear();
            list_reinforce_base.clear();
            clear_reinforce_info();
            
            for (int i = (건물_거점끝 - 1); i >= 0; i--)
            {
                pk::building@ base = pk::get_building(i);
                pk::district@ district = pk::get_district(base.get_district_id());
                
                if (base.get_force_id() == force_id and needReinforce(base))    // 자세력 거점 검색
                {
                    // 빈 거점에 교전 시 무장 지원
                    if (거점원군_무장파병설정 and needPersonSupport(base))
                        PushPersonSupport(base);    // 빈 거점에 무장지원 턴당 1회 수행 (소환 명령)

                    // 병력 있지만 무장이 없는 거점 검색하여 무장을 보내서 수송 지원 유도 ('20.9.13)
                    int send_person = getNearestReinforce(base, force, /*has_person*/false);     
                    if (거점원군_무장파병설정 and send_person != -1)
                        PushPersonSupport(pk::get_building(send_person));  // 무장이 없는 거점으로 무장 지원

                    // 원군출병가능 거점검색
                    int src_target = getNearestReinforce(base, force, /*has_person*/true);
                    if (src_target != -1)
                    {
                        list_engage_base.add(base);     // 원군필요 거점리스트 추가
                        
                        // 원군필요 거점이 도시인 경우: 원군 후보거점 카운트 추가
                        if (base.get_id() < 건물_도시끝)   
                            arr_reinforce_count[src_target] += 1;
                    }
                }
            }
            
            // 병력차를 기준으로 정렬 ('19.3.16)
            list_engage_base.sort(function(a, b)
            {
                if (a.get_id() < 건물_도시끝 and b.get_id() < 건물_도시끝)
                    return (main.arr_engage_troops[a.get_id()] > main.arr_engage_troops[b.get_id()]);
                
                return (a.get_id() > b.get_id());
            });
            
            // 원군필요 거점으로 원군 출병 ('19.3.13)
            pk::array<pk::building@> arr_engage_base = pk::list_to_array(list_engage_base);
            for (int i = 0; i < int(arr_engage_base.length); i++)
            {
                list_reinforce_base.clear();    // 출진완료 거점 리스트 초기화
                pk::building@ base = arr_engage_base[i];
                
                unit_count[0] = 0;  // 출진한 전투부대수 초기화
                unit_count[1] = 0;  // 출진한 수송부대수 초기화
                int push_count = 0;  // push 횟수 확인 변수
                
                int engage_troops = arr_engage_troops[base.get_id()];
                pk::array<int> max_count(2, 0);     // 최대 출진가능 부대수
                max_count[0] = pk::max(1, pk::min(거점원군_전투부대수, 1 + engage_troops/4000));
                max_count[1] = pk::max(1, 거점원군_수송부대수);
                
                // 출진거점 검색을 포함하여 출진 명령 반복 수행 ('19.3.4 수정)
                bool cmd = false;
                while(unit_count[0] < max_count[0] and unit_count[1] < max_count[1] and push_count < (max_count[0] + max_count[1]))
                {
                    push_count += 1;
                    cmd = (PushReinforce(base) or cmd);    // 원군 출병
                }
            }
            
            list_engage_base.clear();
            list_reinforce_base.clear();
            clear_reinforce_info();
        }
        
        
        
        //----------------------------------------------------------------------------------
        //          거점 원군
        //----------------------------------------------------------------------------------
        
        
		/** 원군이 필요한 상황인지 판단 */
		bool needReinforce(pk::building@ base)
		{
			int enemy_units1 = 0;
			int enemy_units2 = 0;
			int enemy_units3 = 0;
			int enemy_troops1 = 0;
			int enemy_troops2 = 0;
			int enemy_troops3 = 0;
			int force_units3 = 0;

            // 적군검색범위
            int ref_city_id = pk::get_city_id(base.pos);   // 거점 좌표가 소속된 도시 확인 ('20.9.20)
            pk::building@ ref_city = pk::get_building(ref_city_id);
			pk::array<pk::point> range = pk::range(base.pos, 1, ((base.facility == 시설_도시) ? (거점원군_적군검색범위 + 1) : pk::min(6, 거점원군_적군검색범위)) );
			for (int i = 0; i < int(range.length); i++)
			{
                pk::point pos = range[i];
				pk::unit@ unit = pk::get_unit(pos);
				if (pk::is_alive(unit))
				{
                    int pos_city_id = pk::get_city_id(unit.pos);   // 부대 좌표가 소속된 도시 확인 ('20.9.20)
                    pk::building@ pos_city = pk::get_building(pos_city_id);
                    if (pk::is_alive(pos_city) and pk::is_alive(ref_city) and pos_city_id == ref_city_id)
                    {
                        int distance = pk::get_distance(base.get_pos(), pos);
                        if (pk::is_enemy(base, unit))
                        {
                            if (distance <= 1)
                            {
                                enemy_units1++;
                            }
                            if (distance <= pk::max(3, (거점원군_적군검색범위/2)))
                            {
                                enemy_units2++;
                                enemy_troops2 += unit.troops;
                            }
                            if (distance <= 거점원군_적군검색범위)
                            {
                                enemy_units3++;
                                enemy_troops3 += unit.troops;
                            }
                        }
                        else if (base.get_force_id() == unit.get_force_id() and unit.type == 부대종류_전투)
                        {
                            if (distance <= 거점원군_적군검색범위)
                            {
                                force_units3++;
                            }
                        }
                    }
				}
			}
            
			int base_troops = pk::get_troops(base);
            
            // 교전 병력차 업데이트
            arr_engage_troops[base.get_id()] = (enemy_troops3 - (base_troops + force_units3));
            
            // 방어부대 수가 공격부대 수보다 적을 경우
            if ((enemy_units1 >= 1 or enemy_units2 >= 2 or enemy_units3 >= 3) and force_units3 <= enemy_units3 and force_units3 > 0)
                return true;
            
			// 원군 출병 발동 될 잔여 내구 조건
			if (base_troops < int(enemy_troops3 * 1.0f) and int(base.hp) > pk::max(500, int(pk::get_max_hp(base) * 0.3f)) and int(base.hp) < int(pk::get_max_hp(base) * 0.9f))
				return true;
            
			// 원군 출병 발동 될 잔여 병력 조건
			if (base_troops < int(enemy_troops2 * 1.0f) and base_troops < int(enemy_troops3 * 0.5f) and int(base.hp) < pk::max(1000, int(pk::get_max_hp(base) * 0.5f)) )
				return true;
            
            // 관문/항구인 경우 병력 없고, 적군 침략 시 즉각 원군출병
            if (base.get_id() >= 건물_도시끝 and base_troops == 0 and enemy_units3 > 0)
                return true;
            
			return false;
		}
        
		/** 기준 병력 이상의 가장 가까운 아군 거점을 얻음. */
        pk::building@ dst_t;
		int getNearestReinforce(pk::building@ dst, pk::force@ force, bool has_person=true)
		{
			int best_src = -1;
			int best_distance = 1;
            int dst_id = dst.get_id();
            
            pk::city@ city_of_dst = pk::get_city(pk::get_city_id(dst.pos));
            bool is_enemy_line = pk::is_enemy(city_of_dst, force);
            int max_distance = (dst_id >= 건물_도시끝)? (is_enemy_line? 2 : 1) : pk::max(1, pk::min(3, 거점원군_도시출진거리));
            bool reinforce_mode = pk::is_enemy(force, dst);   // true: 적세력, false: 자세력 또는 동맹군
            if (reinforce_mode) max_distance = pk::min(2, max_distance); // 공략군인 경우 검색거리 2 이하로 제한
            
            @dst_t = @dst;
            pk::list<pk::building@> src_list; 
            
            int search_base = 건물_도시끝;
            if      (has_person and 거점원군_거점검색모드 == 0) search_base = 건물_도시끝;
            else if (has_person and 거점원군_거점검색모드 == 1) search_base = 건물_거점끝;
            else if (!has_person) search_base = 건물_거점끝;     // 검색조건 추가 ('20.9.13)
            
			// 원군 출진 거점 검색
			for (int i = 0; i < search_base; i++)
			{
				pk::building@ src = pk::get_building(i);
                int src_id = src.get_id();
                
                bool check_reinforce_force = false;
                if (!reinforce_mode)   // 자세력 또는 동맹군 거점 검색
                    check_reinforce_force = (거점원군_동맹세력원군)? (!pk::is_enemy(dst, src) and !src.is_player()) : (dst.get_force_id() == src.get_force_id() and !src.is_player());
                else    // 타세력 거점 검색 (함수에서 지정된 세력)
                    check_reinforce_force = (pk::is_enemy(dst, src) and !src.is_player()) and (force.get_id() == src.get_force_id());
                
                // 타 거점, 교전아님, 원군필요거점 아님, 출진완료거점 아님
				if (dst_id != src_id and check_reinforce_force and !pk::enemies_around(src) 
                    and !needReinforce(src) and !list_reinforce_base.contains(src) and !list_attack_base.contains(src))
                {
                    // 도시거리
                    int city_dist = -1;
                    if (dst_id < 건물_도시끝 and src_id < 건물_도시끝)
                        city_dist = pk::get_city_distance(dst_id, src_id);
                    
                    // 거점거리
                    int base_dist = pk::get_building_distance(dst_id, src_id, dst.get_force_id());
                    
                    // 거리 조건 만족 시
                    if ( (0 <= base_dist and base_dist <= max_distance) or city_dist == 1)
                    {
                        int enemy_weight = countNeighborEnemyBase(src);
                        int ref_troops = (enemy_weight * 거점원군_경계병력단위) + 거점원군_거점최소병력;
                        int src_troops = pk::get_troops(src);
                        int src_food = pk::get_food(src);
                        
                        int ref_p_count = pk::max(1, (enemy_weight / 4) + 거점원군_출진거점무장수);
                        int person_count = pk::get_person_list(src, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
                        int idle_p_count = pk::get_idle_person_list(src).count;
                        
                        int weapon_amount = 0;
                        int siege_amount = 0;
            
                        for (int j = 병기_창; j <= 병기_군마; j++)     // 창극노마 병기수량 합하기
                        {
                            weapon_amount += pk::get_weapon_amount(src, j);
                        }
                        for (int j = 병기_충차; j <= 병기_목수; j++)     // 공성병기 수량 합하기
                        {
                            siege_amount += pk::get_weapon_amount(src, j);
                        }
                        
                        // 출진대상거점 조건 (무장수, 병력, 병기, 병량 보유량)
                        if (((has_person and person_count >= ref_p_count and idle_p_count > 0) or (!has_person and person_count == 0))
                            and src_troops > ref_troops and weapon_amount >= ref_troops and src_food >= int(1.5f * ref_troops))
                        {
                            if (!reinforce_mode)    // 자세력 또는 동맹군
                            {
                                best_src = src_id;
                                src_list.add(src);  // 출진가능 거점리스트
                            }
                            else if (reinforce_mode and siege_amount > 2)  // 적세력, 공성병기 보유유무 추가 확인
                            {
                                best_src = src_id;
                                src_list.add(src);  // 출진가능 거점리스트
                            }
                        }
                    }
                }
			}
            
            // 출진가능 거점리스트 정렬 (거점 거리 오름차순, 좌표거리 오름차순)
            if (src_list.count == 0)
                best_src = -1;
            else
            {
                src_list.sort(function(a, b)
                {
                    int build_dist_a = pk::get_building_distance(a.get_id(), main.dst_t.get_id(), a.get_force_id());
                    int build_dist_b = pk::get_building_distance(b.get_id(), main.dst_t.get_id(), b.get_force_id());

                    int pos_dist_a = pk::get_distance(a.pos, main.dst_t.pos);
                    int pos_dist_b = pk::get_distance(b.pos, main.dst_t.pos);
                    
                    return (build_dist_a != build_dist_b)? (build_dist_a < build_dist_b) : (pos_dist_a < pos_dist_b);
                });
                best_src = src_list[0].get_id();
            }
            
			return best_src;
		}

        //----------------------------------------------------------------------------------
        
        // ***** 주변 적거점 수 확인 ***** //
        int countNeighborEnemyBase(pk::building@ src)
        {
            int weight = 0;
            int troops = 0;
            for (int i = 0; i < 건물_거점끝; i++)
			{
                pk::building@ dst = pk::get_building(i);
                int src_id = src.get_id();
                int dst_id = dst.get_id();
                
                if (src_id != dst_id and pk::is_neighbor_base(src_id, dst_id) and pk::is_enemy(src, dst))
                    troops += pk::get_troops(dst);
            }
            
            weight = int(troops / 거점원군_경계병력단위);
            return weight;
        }
        
        
        //----------------------------------------------------------------------------------
        
        
		// ***** 원군 출진 명령 ***** //
        bool PushReinforce(pk::building@ dst_base)
		{
			// 원군 부대가 출진할 인근 거점이 있는지 확인
            pk::force@ force = pk::get_force(dst_base.get_force_id());
			int target = getNearestReinforce(dst_base, force, /*has_person*/true);
			if (target == -1) return false;

            // 원군 출진 거점
            pk::building@ src_base = pk::get_building(target);
            int src_id = src_base.get_id();
            int dst_id = dst_base.get_id();
            
            // 출진 부대 종류 판단 ('19.3.4)
            int reinforce_type = -1;
            if (거점원군_동맹세력원군 and !pk::is_enemy(src_base, dst_base) and src_base.get_force_id() != dst_base.get_force_id())
                reinforce_type = 부대종류_전투; // 동맹세력원군은 무조건 전투부대 ('20.2.8)
            else
            {
                if (거점원군_부대종류모드 == 0) reinforce_type = 부대종류_전투;   // 무조건 전투부대
                if (거점원군_부대종류모드 == 1) reinforce_type = 부대종류_수송;   // 무조건 수송부대
                if (거점원군_부대종류모드 == 2) reinforce_type = get_reinforce_unit_type(src_base, dst_base);
            }
            if (reinforce_type < 0 or reinforce_type > 부대종류_끝) return false;
            
            // 출진거점과 목표거점간 거리
            int distance = pk::get_building_distance(src_id, dst_id, src_base.get_force_id());
            
            // 목표거점 많을 경우 분산 ('19.3.13)
            int divide = pk::max(1, arr_reinforce_count[src_id]);
            
            // 거점 거리 및 종류에 따른 출진가능 부대수 제한 ('19.3.5)
            int engage_troops = arr_engage_troops[dst_base.get_id()];
            pk::array<int> max_unit(2, 0);  // 최대 출진가능 부대수
            max_unit[0] = pk::max(1, pk::min(1 + engage_troops/4000  , int((거점원군_전투부대수 - (distance - 1))/divide) ) );
            max_unit[1] = pk::max(1, int( 거점원군_수송부대수/divide) );
            
            if (dst_id >= 건물_도시끝)   // 관문/거점은 한 거점에서 1부대만
            {
                max_unit[0] = 1;
                max_unit[1] = 1;
            }
            
            int push_count = 0;  // push 횟수 확인 변수
            
            // 출진거점에서 반복적으로 출진 명령 수행 ('19.3.4)
            while(unit_count[0] < max_unit[0] and unit_count[1] < max_unit[1] and  push_count < (max_unit[0] + max_unit[1]))
            {
                push_count += 1;
                
                if (reinforce_type == 부대종류_수송)
                {
                    int unit_id = PushTransportUnit(src_base, dst_base, true);  // 수송부대 출진
                    if (unit_id != -1)
                    {
                        unit_count[1] += 1;     // 수송부대 카운트
                        if (!list_reinforce_base.contains(src_base))
                            list_reinforce_base.add(src_base);
                    }
                }
                else if (reinforce_type == 부대종류_전투)
                {
                    int unit_id = PushCombatUnit(src_base, dst_base, true);     // 전투부대 출진
                    if (unit_id != -1)
                    {
                        unit_count[0] += 1;     // 전투부대 카운트
                        if (!list_reinforce_base.contains(src_base))
                            list_reinforce_base.add(src_base);
                    }
                }
            }
            if ((unit_count[0] + unit_count[1]) > 0) return true;
            
			return false;
		}
        
        //----------------------------------------------------------------------------------
        
        // 출진거점 및 대상거점의 무장수로 출진할 부대종류 결정 ('19.3.4)
        int get_reinforce_unit_type(pk::building@ src_base, pk::building@ dst_base)
        {
            if (!pk::is_alive(src_base) or !pk::is_alive(dst_base)) return -1;

            // 거점별 소속무장 정보
            pk::list<pk::person@> list_src_men = pk::get_person_list(src_base, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반));
            pk::list<pk::person@> list_dst_men = pk::get_person_list(dst_base, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반));
            
            // 무장수
            int src_count = list_src_men.count;    // 출진거점 미행동 무장수
            int dst_count = list_dst_men.count;    // 목표거점 전체 무장수
            
            // 거점거리
            int distance = pk::get_building_distance(src_base.get_id(), dst_base.get_id(), src_base.get_force_id());
            
            // 출진 부대종류 결정
            int reinforce_type = -1;
            if (pk::get_troops(dst_base) < 3000 and unit_count[1] < 1)
                reinforce_type = 부대종류_수송;    // 병력부족 거점에 수송부대 안보낸 경우 수송부대 출진
            
            else if (distance <= 1)
                reinforce_type = 부대종류_전투;    // 가까운 거리인 경우 : 전투부대
            
            else if (dst_count == 0)
                reinforce_type = 부대종류_전투;    // 목표거점 무장없을 시 : 전투부대
            
            else if (dst_count >= pk::max(1, 거점원군_교전거점무장수))
                reinforce_type = 부대종류_수송;    // 목표거점 n명 이상 : 수송부대
            
            else if (src_count <= pk::max(1, 거점원군_출진거점무장수)) 
                reinforce_type = 부대종류_수송;    // 출진무장 적을 경우 : 수송부대
            
            else                     
                reinforce_type = 부대종류_전투;    // 전투부대
            
            return reinforce_type;
        }
        
        // 건물ID에 따라 거점 종류값 반환
        int get_base_type(int building_id)
        {
            int type = -1;
            if (건물_도시시작 <= building_id and building_id < 건물_도시끝) type = 0;
            if (건물_관문시작 <= building_id and building_id < 건물_관문끝) type = 1;
            if (건물_항구시작 <= building_id and building_id < 건물_항구끝) type = 2;
            return type;
        }
        
        //----------------------------------------------------------------------------------
        
        // 수송부대 출진 처리
        int PushTransportUnit(pk::building@ src_base, pk::building@ dst_base, bool do_cmd = true)
        {
            if (!pk::is_alive(src_base) or !pk::is_alive(dst_base)) return -1;
            
            int enemy_weight = countNeighborEnemyBase(src_base);    // 적병력 가중치
            int ref_troops = (enemy_weight * 거점원군_경계병력단위) + 거점원군_거점최소병력;
            if (pk::get_troops(src_base) <= (ref_troops + 거점원군_최소출진병력)) return -1;    // 병력 부족
              
            int person_count = pk::get_person_list(src_base, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            if (person_count <= pk::max(1, (enemy_weight/4) + 거점원군_출진거점무장수)) return -1;   // 무장 부족
            
            auto person_list = pk::get_idle_person_list(src_base);
            int base_type = get_base_type(dst_base.get_id());
            if (base_type == 0 and person_list.count <= 1) return -1;   // 무장 부족 (도시)
            if (base_type > 0  and person_list.count < 1)  return -1;   // 무장 부족 (관문,항구)
            
            // 정치력 높은 순으로 정렬.
			person_list.sort(function(a, b)
			{
				return (float(a.stat[무장능력_정치]) / float(a.stat[무장능력_무력])) > (float(b.stat[무장능력_정치]) / float(b.stat[무장능력_무력]));
			});
            pk::person@ leader = pk::get_person(person_list[0].get_id());
            
            // 원군 병력 산정 : 기준 병력 초과분
            int reinforce_troops = pk::min( pk::max(1, (45000 - ref_troops)), pk::max(1, pk::get_troops(src_base) - ref_troops));
            
            // 대상거점이 관문/항구인 경우 원군병력 제한
            if (base_type > 0)
            {
                if (pk::has_tech(dst_base, 기교_항관확장))
                    reinforce_troops = pk::min(20000, reinforce_troops);
                else
                    reinforce_troops = pk::min(10000, reinforce_troops);
            }
            
            // 병량 계산
            int unit_food = int(pk::min(2.0f * reinforce_troops, pk::max( 0.5f * pk::get_food(src_base), 1.2f * reinforce_troops)));
            if (unit_food < int(0.5f * reinforce_troops)) return -1;   // 병량 부족
            
            // 출진 명령
            if (do_cmd)
            {
                // 출진 명령 정보 생성.
                pk::com_deploy_cmd_info cmd;
                @cmd.base = @src_base;
                cmd.type = 부대종류_수송;
                cmd.member[0] = leader.get_id();
                cmd.gold = (pk::get_gold(src_base) >= 1000) ? int(pk::min(2000.f, pk::get_gold(src_base) * 0.1f)) : 0;
                cmd.food = pk::min(unit_food, 500000);
                cmd.troops = pk::min(60000, pk::max(1, reinforce_troops));
                
                // 창극노마 병기수량 합하기
                int weapon_sum = 0;
                for (int j = 병기_창; j <= 병기_군마; j++)     
                {
                    weapon_sum += pk::get_weapon_amount(src_base, j);
                }
                
                // 병기 전체 수량은 병력만큼, 병기별 비율대로 배분
                int i = 0;
                for (int weapon_id = 병기_창; weapon_id <= 병기_군마; weapon_id++)
                {
                    int weapon_amount = int(pk::get_weapon_amount(src_base, weapon_id) * 0.9f);
                    if (weapon_id <= 병기_군마 and weapon_amount > 0)
                    {
                        cmd.weapon_id[i] = weapon_id;
                        cmd.weapon_amount[i] = pk::min(100000, weapon_amount, reinforce_troops * weapon_amount / weapon_sum);
                        i++;
                    }
                    else if (is_siege_weapon(weapon_id) and weapon_amount > 0)
                    {
                        cmd.weapon_id[i] = weapon_id;
                        cmd.weapon_amount[i] = pk::min(100, (1 + weapon_amount / 4));
                        i++;
                    }
                }
                
                cmd.order = 부대임무_이동;
                cmd.target_pos = dst_base.get_pos();  // 목표는 전투중인 거점

                // 출진.
                int unit_id = pk::command(cmd);
                pk::unit@ unit_cmd = pk::get_unit(unit_id);
                if (pk::is_alive(unit_cmd))
                {
                    unit_cmd.action_done = true;
                    
                    if (디버그모드) 
                    {
                        string src_name  = pk::decode(pk::get_name(src_base));
                        string dst_name  = pk::decode(pk::get_name(dst_base));
                        string unit_name = pk::decode(pk::get_name(leader));
                        string order_str = get_order_info(unit_cmd.order);
                        string cmd_info  = (pk::is_enemy(src_base, dst_base))? "거점공략" : "거점원군";
                        pk::info(pk::format("{}: {}대 {}: {}→{}", cmd_info, unit_name, order_str, src_name, dst_name));
                    }
                    return unit_id;
                }
            }
            
            return -1;
        }
        
        //----------------------------------------------------------------------------------
        
        // 전투부대 출진 처리
        int PushCombatUnit(pk::building@ src_base, pk::building@ dst_base, bool do_cmd = true)
        {
            if (!pk::is_alive(src_base) or !pk::is_alive(dst_base)) return -1;
            
            int enemy_weight = countNeighborEnemyBase(src_base);    // 적병력 가중치
            int ref_troops = (enemy_weight * 거점원군_경계병력단위) + 거점원군_거점최소병력;
            if (pk::get_troops(src_base) <= (ref_troops + 거점원군_최소출진병력)) return -1;    // 병력 부족
            
            int person_count = pk::get_person_list(src_base, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            if (person_count <= pk::max(1, (enemy_weight/4) + 거점원군_출진거점무장수)) return -1;   // 무장 부족
            
            auto person_list = pk::get_idle_person_list(src_base);
            if (person_list.count == 0) return -1;   // 무장 부족
            
            // 대상거점이 관문/항구인 경우 노병 출진 우대
            int base_type = get_base_type(dst_base.get_id());
            cmd_archer = (base_type > 0);
            
            // 통솔+무력 높은 순으로 정렬.
			person_list.sort(function(a, b)
			{
                if (main.cmd_archer)    // 노병 우대 출진
                {
                    // 궁병 특기 반영 ('20.9.13)
                    bool a_archer = (pk::has_skill(a, 특기_궁신) or pk::has_skill(a, 특기_궁장) or pk::has_skill(a, 특기_사수));
                    bool b_archer = (pk::has_skill(b, 특기_궁신) or pk::has_skill(b, 특기_궁장) or pk::has_skill(b, 특기_사수));
                    if ( a_archer and !b_archer) return true;   
                    if (!a_archer and  b_archer) return false;  
                    // 궁병 적성
                    if (a.tekisei[병종_노병] != b.tekisei[병종_노병]) return (a.tekisei[병종_노병] > b.tekisei[병종_노병]);
                    // 무장 능력
                    return (a.stat[무장능력_무력] + a.stat[무장능력_통솔]) > (b.stat[무장능력_무력] + b.stat[무장능력_통솔]);
                }
                
				return (a.stat[무장능력_무력] + a.stat[무장능력_통솔]) > (b.stat[무장능력_무력] + b.stat[무장능력_통솔]);
			});
            pk::person@ leader = pk::get_person(person_list[0].get_id());
            
            // 원군 병력 산정 : 기준 병력 초과분, 지휘가능병력 확인
            int reinforce_troops = pk::min(거점원군_최대전투병력, pk::get_command(leader), pk::max(1, pk::get_troops(src_base) - ref_troops));

            // 최적 무기 선택
            int ground_weapon_id = 병기_검;
            int water_weapon_id = 병기_주가;
            int unit_troops = reinforce_troops;
            
            // 육상 무기 선택
            get_ground_weapon(src_base, leader, reinforce_troops, ground_weapon_id, unit_troops);
            if (ground_weapon_id == 0) return -1;    // 병기 부족
            
            // 수상 무기 선택
            if (check_building_has_port(src_base) and check_building_has_port(dst_base))
            {
                if (leader.tekisei[병종_수군] == 적성_C)
                    water_weapon_id = 병기_주가;
                else
                {
                    if      (pk::get_weapon_amount(src_base, 병기_누선) > 0) water_weapon_id = 병기_누선;
                    else if (pk::get_weapon_amount(src_base, 병기_투함) > 0) water_weapon_id = 병기_투함;
                    else water_weapon_id = 병기_주가;
                }
            }
            else
                water_weapon_id = 병기_주가;
            
            // 병량 계산
            int unit_food = int(pk::min(2.0f * unit_troops, pk::max( 0.5f * pk::get_food(src_base), 1.2f * unit_troops)));
            if (unit_food < int(0.5f * unit_troops)) return -1;   // 병량 부족
            
            // 출진 명령
            if (do_cmd)
            {
                // 출진 명령 정보 생성.
                pk::com_deploy_cmd_info cmd;
                @cmd.base = @src_base;
                cmd.type = 부대종류_전투;
                cmd.member[0] = leader.get_id();
                cmd.gold = (pk::get_gold(src_base) >= 1000) ? int(pk::min(1000.f, pk::get_gold(src_base) * 0.1f)) : 0;
                cmd.troops = pk::max(1, unit_troops);
                cmd.weapon_id[0] = ground_weapon_id;
                cmd.weapon_id[1] = water_weapon_id;
                cmd.weapon_amount[0] = (is_siege_weapon(ground_weapon_id))? 1 : pk::max(1, unit_troops);
                cmd.weapon_amount[1] = (water_weapon_id == 병기_주가)? 0 : 1;
                cmd.food = pk::min(50000, unit_food);

                cmd.order = (src_base.get_force_id() == dst_base.get_force_id())? 부대임무_공격 : 
                                             ((pk::is_enemy(src_base, dst_base))? 부대임무_공격 : 부대임무_이동);    // 자세력, 적거점, 동맹군

                cmd.target_pos = dst_base.get_pos();  // 목표는 전투중인 거점

                // 출진.
                int unit_id = pk::command(cmd);
                pk::unit@ unit_cmd = pk::get_unit(unit_id);
                if (pk::is_alive(unit_cmd))
                {
                    unit_cmd.action_done = true;
                    
                    if (디버그모드) 
                    {
                        string src_name  = pk::decode(pk::get_name(src_base));
                        string dst_name  = pk::decode(pk::get_name(dst_base));
                        string unit_name = pk::decode(pk::get_name(leader));
                        string order_str = get_order_info(unit_cmd.order);
                        string cmd_info  = (pk::is_enemy(src_base, dst_base))? "거점공략" : "거점원군";
                        pk::info(pk::format("{}: {}대 {}: {}→{}", cmd_info, unit_name, order_str, src_name, dst_name));
                    }
                    return unit_id;
                }
            }
            
            return -1;
        }
        
        // 노병우대 출진여부
        bool cmd_archer = false;    
        
        
        // 공성병기 부대 출진 처리
        int PushSiegeUnit(pk::building@ src_base, pk::building@ dst_base, bool do_cmd = true)
        {
            if (!pk::is_alive(src_base) or !pk::is_alive(dst_base)) return -1;
            
            int enemy_weight = countNeighborEnemyBase(src_base);    // 적병력 가중치
            int ref_troops = (enemy_weight * 거점원군_경계병력단위) + 거점원군_거점최소병력;
            if (pk::get_troops(src_base) <= (ref_troops + 거점원군_최소출진병력)) return -1;    // 병력 부족
            
            int person_count = pk::get_person_list(src_base, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            if (person_count <= pk::max(1, (enemy_weight/4) + 거점원군_출진거점무장수)) return -1;   // 무장 부족
            
            auto person_list = pk::get_idle_person_list(src_base);
            if (person_list.count == 0) return -1;   // 무장 부족

            // 거점 공성병기 재고 확인
            int siege_dir_id = (pk::has_tech(src_base, 기교_목수개발))? 병기_목수 : 병기_충차;
            int siege_rng_id = (pk::has_tech(src_base, 기교_투석개발))? 병기_투석 : 병기_정란;
            int amt_siege_dir = pk::get_weapon_amount(src_base, siege_dir_id);  // 직접 공성병기 수량
            int amt_siege_rng = pk::get_weapon_amount(src_base, siege_rng_id);  // 간접 공성병기 수량
            if ((amt_siege_rng + amt_siege_dir) == 0) return -1;     // 공성병기 없음
            
            // 공성특기 우대, 기력회복 특기 우대, 통솔+무력 높은 순으로 정렬.
			person_list.sort(function(a, b)
			{
                // 공성 특기 우대
                bool a_skill = (pk::has_skill(a, 특기_공신) or pk::has_skill(a, 특기_공성) or pk::has_skill(a, 특기_사정));
                bool b_skill = (pk::has_skill(b, 특기_공신) or pk::has_skill(b, 특기_공성) or pk::has_skill(b, 특기_사정));
                if ( a_skill and !b_skill) return true;   
                if (!a_skill and  b_skill) return false;  
                // 기력회복 특기 우대
                bool a_energy = (pk::has_skill(a, 특기_주악) or pk::has_skill(a, 특기_시상) or pk::has_skill(a, 특기_노발));
                bool b_energy = (pk::has_skill(b, 특기_주악) or pk::has_skill(b, 특기_시상) or pk::has_skill(b, 특기_노발));
                if ( a_energy and !b_energy) return true;   
                if (!a_energy and  b_energy) return false;  
                // 병기 적성
                if (a.tekisei[병종_병기] != b.tekisei[병종_병기]) return (a.tekisei[병종_병기] > b.tekisei[병종_병기]);
                // 무장 능력
                return (a.stat[무장능력_무력] + a.stat[무장능력_통솔]) > (b.stat[무장능력_무력] + b.stat[무장능력_통솔]);
			});
            pk::person@ leader = pk::get_person(person_list[0].get_id());
            
            // 원군 병력 산정 : 기준 병력 초과분, 지휘가능병력 확인
            int reinforce_troops = pk::min(거점원군_최대전투병력, pk::get_command(leader), pk::max(1, pk::get_troops(src_base) - ref_troops));

            // 최적 무기 선택
            int ground_weapon_id = 병기_검;
            int water_weapon_id = 병기_주가;
            int unit_troops = reinforce_troops;
            
            // 간접 공성병기부터 우선 선택 (사정특기는 간접, 공성특기는 직접 병기 우선 배정)
            if      (amt_siege_rng > 0 and pk::has_skill(leader, 특기_사정)) ground_weapon_id = siege_rng_id;
            else if (amt_siege_dir > 0 and pk::has_skill(leader, 특기_공성)) ground_weapon_id = siege_dir_id;
            else if (amt_siege_rng > 0)  ground_weapon_id = siege_rng_id;
            else if (amt_siege_dir > 0)  ground_weapon_id = siege_dir_id;
            
            
            // 병량 계산
            int unit_food = int(pk::min(2.0f * unit_troops, pk::max( 0.5f * pk::get_food(src_base), 1.2f * unit_troops)));
            if (unit_food < int(0.5f * unit_troops)) return -1;   // 병량 부족
            
            // 출진 명령
            if (do_cmd)
            {
                // 출진 명령 정보 생성.
                pk::com_deploy_cmd_info cmd;
                @cmd.base = @src_base;
                cmd.type = 부대종류_전투;
                cmd.member[0] = leader.get_id();
                cmd.gold = 0;   // 공성병기 부대는 건설불가로 금소지 안함
                cmd.troops = pk::max(1, unit_troops);
                cmd.weapon_id[0] = ground_weapon_id;
                cmd.weapon_id[1] = water_weapon_id;
                cmd.weapon_amount[0] = (is_siege_weapon(ground_weapon_id))? 1 : pk::max(1, unit_troops);
                cmd.weapon_amount[1] = (water_weapon_id == 병기_주가)? 0 : 1;
                cmd.food = pk::min(50000, unit_food);

                cmd.order = (src_base.get_force_id() == dst_base.get_force_id())? 부대임무_공격 : 
                                             ((pk::is_enemy(src_base, dst_base))? 부대임무_공격 : 부대임무_이동);    // 자세력, 적거점, 동맹군

                cmd.target_pos = dst_base.get_pos();  // 목표는 전투중인 거점
                
                // 출진.
                int unit_id = pk::command(cmd);
                pk::unit@ unit_cmd = pk::get_unit(unit_id);
                if (pk::is_alive(unit_cmd))
                {
                    unit_cmd.action_done = true;
                    
                    if (디버그모드) 
                    {
                        string src_name  = pk::decode(pk::get_name(src_base));
                        string dst_name  = pk::decode(pk::get_name(dst_base));
                        string unit_name = pk::decode(pk::get_name(leader));
                        string order_str = get_order_info(unit_cmd.order);
                        string cmd_info  = (pk::is_enemy(src_base, dst_base))? "거점공략" : "거점원군";
                        pk::info(pk::format("{}: {}대 {}: {}→{}", cmd_info, unit_name, order_str, src_name, dst_name));
                    }
                    return unit_id;
                }
            }
            
            return -1;
        }
        
        
        //----------------------------------------------------------------------------------
        
        // 무기 선택 함수
        void get_ground_weapon(pk::building@ base, pk::person@ leader, int troops_max, int &out weapon_sel, int &out troops_sel)
        {
            int troops_min = 거점원군_최소출진병력;
            int weapon_max = 0;
            int best_tekisei = 적성_C;
            
            weapon_sel = 병기_검;
            troops_sel = 0;
            
            // 노병 우대 출진
            if (cmd_archer)
            {
                int tekisei = leader.tekisei[pk::equipment_id_to_heishu(병종_노병)];
                int weapon = pk::get_weapon_amount(base, 병기_노);
                if (troops_min <= weapon and 적성_B <= tekisei)
                {
                    weapon_sel = 병기_노;
                    troops_sel = pk::min(weapon, troops_max);
                }
            }
            // 일반 출진
            else    
            {
                // 출병병력 이상 보유한 무기 중 최고 적성 확인
                for (int id = 병기_창; id <= 병기_군마; id++)
                {
                    int tekisei = leader.tekisei[pk::equipment_id_to_heishu(id)];
                    int weapon_t = pk::get_weapon_amount(base, id);
                    if (troops_min <= weapon_t and best_tekisei <= tekisei)
                        best_tekisei = tekisei;
                }
                
                // 최고 적성 이상의 병과 중 무기 가장 많은 병과 선택
                for (int id = 병기_창; id <= 병기_군마; id++)
                {
                    int tekisei = leader.tekisei[pk::equipment_id_to_heishu(id)];
                    int weapon_t = pk::get_weapon_amount(base, id);
                    if (troops_min <= weapon_t and weapon_max <= weapon_t and best_tekisei <= tekisei)
                    {
                        best_tekisei = tekisei;
                        weapon_max = weapon_t;
                        weapon_sel = id;
                        troops_sel = pk::min(weapon_max, troops_max);
                    }
                }
            }
            
            if (weapon_sel == 0)
            {
                troops_sel = troops_min;
            }
        }
        
        bool is_siege_weapon(int weapon_id)
        {
            if (병기_충차 <= weapon_id and weapon_id <= 병기_목수) return true;
            return false;
        }
        
        bool check_building_has_port(pk::building@ base)
        {
            bool has_port = false;
            int base_id = base.get_id();
            if (건물_항구시작 <= base_id and base_id < 건물_항구끝) return true;
            
            if (건물_도시시작 <= base_id and base_id < 건물_도시끝)
            {
                pk::city@ city = pk::building_to_city(base);
                for (int i = 0; i < 5; i++)
                {
                    int sub_id = city.gate[i];
                    if (sub_id != -1)
                    {
                        pk::building@ sub_t = pk::get_building(sub_id);
                        if (pk::is_alive(sub_t) and (건물_항구시작 <= sub_id and sub_id < 건물_항구끝)) return true;
                    }
                }
            }
            return false;
        }
        
        
        //----------------------------------------------------------------------------------
        /** 출진 대상 거점 카운트 초기화 **/
        void clear_reinforce_info()
        {
            for (int i = 0; i < 건물_거점끝; i++)
            {
                arr_reinforce_count[i] = 0;
            }
        }
        
        
        
        //----------------------------------------------------------------------------------
        //  빈 거점 무장 지원
        //----------------------------------------------------------------------------------
        
        /** 무장파견이 필요한지 판단 **/
        bool needPersonSupport(pk::building@ base)
		{
            if (!pk::is_alive(base)) return false;
            
            int enemy_troops = 0;   // 주위의 적군 수 확인 추가
            pk::array<pk::point> range = pk::range(base.pos, 1, ((base.facility == 시설_도시) ? 6 : 5));
			for (int i = 0; i < int(range.length); i++)
			{
				pk::unit@ unit = pk::get_unit(range[i]);
				if (pk::is_alive(unit) and pk::is_enemy(base, unit))
                    enemy_troops += unit.troops;
			}
            
            int count = pk::get_person_list(base, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            if (count == 0 and pk::get_troops(base) >= 1000 and (pk::enemies_around(base) or enemy_troops >= 4000)) 
                return true;
            
            return false;
        }
        
        // ***** 무장 지원 명령 ***** //
        bool PushPersonSupport(pk::building@ dst_base)
        {
            // 무장이 있는 인근 거점이 있는지 확인
			int target = getNearestPerson(dst_base);
			if (target == -1) return false;
            
            // 지원가능 무장 소속거점
            pk::building@ src_base = pk::get_building(target);
            
            int person_count = pk::get_person_list(src_base, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).count;
            if (person_count <= pk::max(1, 거점원군_출진거점무장수)) return false;   // 무장 부족
            
            // 미행동 무장리스트
            auto person_list = pk::get_idle_person_list(src_base);
            if (person_list.count == 0) return false;   // 무장 부족
            
            // 통솔+무력 높은 순으로 정렬.
			person_list.sort(function(a, b)
			{
				return (a.stat[무장능력_무력] + a.stat[무장능력_통솔]) > (b.stat[무장능력_무력] + b.stat[무장능력_통솔]);
			});
            
            // 소환할 무장수
            int max = pk::max(1, pk::min(5, (person_list.count / 4)));
            
            // 소환 대상 무장
            pk::list<pk::person@> actors;
            for (int i = 0; i < max; i++)
            {
                actors.add(person_list[i]);
            }
            
            // 소환 명령
            pk::summon_cmd_info cmd_summon;
            @cmd_summon.base   = dst_base;
            cmd_summon.actors  = actors;
            pk::command(cmd_summon);
            
            return true;
        }
        
        /** 무장이 있는 가까운 아군 거점을 얻음. */
        int getNearestPerson(pk::building@ dst)
		{
			int best_src = -1;
			int best_distance = 0;
            
			// 거점까지 확인.
			for (int i = 0; i < 건물_거점끝; i++)
			{
				pk::building@ src = pk::get_building(i);
                
				if (dst.get_id() != i and dst.get_force_id() == src.get_force_id() and (!pk::enemies_around(src) and !needReinforce(src)) )
                {
                    int distance = pk::get_building_distance(dst.get_id(), i, dst.get_force_id());
                    
                    auto person_list = pk::get_idle_person_list(src);
                    
                    int max_distance = pk::max(1, pk::min(3, 거점원군_도시출진거리));
                    if ((0 <= distance and distance < best_distance and distance <= max_distance) or best_src == -1)
                    {
                        if (person_list.count > 거점원군_출진거점무장수)
                        {
                            best_src = i;
                            best_distance = distance;
                        }
                    }
                }
			}
			return best_src;
		}
        
        
        
        //---------------------------------------------------------------------------------------
        //  거점공략지원 실행함수
        //---------------------------------------------------------------------------------------
        
        pk::list<pk::building@> list_target_base;     // 공략군 필요한 교전거점 리스트
        pk::list<pk::building@> list_attack_base;     // 공략군 출진거점 리스트
        
        
        void func_attack_support(pk::force@ force)
        {
            int force_id = force.get_id();
            
            // 턴 세력 도시 구역별 : 교전하는 병력합, 부대수 업데이트
            clear_engage_info();
            update_engage_info();
            list_target_base.clear();
            list_attack_base.clear();
            
            for (int i = (건물_도시끝 - 1); i >= 0; i--)
            {
                pk::building@ dst_base = pk::get_building(i);
                int atk_force_id = get_force_invaded(dst_base);
                if (pk::is_enemy(force, dst_base) and is_invaded_base(dst_base) and atk_force_id == force_id) // 타세력 교전 중인 거점
                {
                    int src_target = getNearestReinforce(dst_base, force, /*has_person*/true);    // 공략지원 가능한 거점 반환
                    pk::building@ src_base = pk::get_building(src_target);
                    if (pk::is_alive(src_base) and !list_target_base.contains(dst_base))
                        list_target_base.add(dst_base);
                }
            }
            
            // 교전 병력차를 기준으로 정렬 (수비측 우세 지역 우선 파병)
            list_target_base.sort(function(a, b)
            {
                if (a.get_id() < 건물_도시끝 and b.get_id() < 건물_도시끝)
                    return (main.get_engage_status(a.get_id()) < main.get_engage_status(b.get_id()));
                
                return (a.get_id() > b.get_id());
            });
            
            // 공략 거점으로 순차적으로 지원군 추가 파병
            pk::array<pk::building@> arr_target_base = pk::list_to_array(list_target_base);
            for (int i = 0; i < int(arr_target_base.length); i++)
            {
                list_attack_base.clear();
                pk::building@ dst_base = arr_target_base[i];
            
                bool cmd = PushAttackSupport(dst_base, force);
            }
            
            // 턴 세력 도시 구역별 : 교전하는 병력합, 부대수 업데이트
            clear_engage_info();
            update_engage_info();
            list_target_base.clear();
            list_attack_base.clear();
        }
        
        
        
        //---------------------------------------------------------------------------------------
        
        // 공략중인 거점으로 보낼 부대종류 지정
        bool PushAttackSupport(pk::building@ dst_base, pk::force@ force)
        {
            // 원군 부대가 출진할 인근 거점이 있는지 확인
            int src_target = getNearestReinforce(dst_base, force, /*has_person*/true);
			if (src_target == -1) return false;
            
            // 공략군 출진 거점
            pk::building@ src_base = pk::get_building(src_target);
            
            int city_id = pk::get_city_id(dst_base.pos);    // 거점의 구역 도시
            int status  = get_engage_status(city_id);   // 공-방 병력차 : 공격측이 많으면 양수, 수비측이 많으면 음수
            string status_info = "";
            
            bool cmd = false;
            if ((2*ref_status) <= status and status < (4*ref_status))
            {   // 공격측 우세
                status_info = "공격측 병력 우세";
                int num_atk_support_unit = count_atk_unit_type(src_base, dst_base, 부대종류_수송, /*siege_weapon*/false);
                int unit_type = (!거점공략_수송부대허용 or pk::enemies_around(src_base) or num_atk_support_unit > max_unit_supply)? 부대종류_전투 : 부대종류_수송;
                cmd = (PushAttackUnit(dst_base, src_base, unit_type, /*siege_weapon*/false, /*max_unit*/1) or cmd); // 전투 or 수송부대
            }
            else if (0 <= status and status < (2*ref_status))
            {   // 공방 치열
                status_info = "공방 치열";
                int num_atk_support_unit = count_atk_unit_type(src_base, dst_base, 부대종류_전투, /*siege_weapon*/true);
                bool siege = (!거점공략_공성병기허용 or pk::enemies_around(src_base) or num_atk_support_unit > max_unit_siege)? false : true;
                cmd = (PushAttackUnit(dst_base, src_base, 부대종류_전투, /*siege_weapon*/siege, /*max_unit*/1) or cmd); // 전투 or 공성부대
            }
            else if ((-1*ref_status) <= status and status < 0)
            {   // 수비측 우세
                status_info = "수비측 병력 우세";
                int max_unit = (pk::enemies_around(src_base))? 1 : max_unit_combat;
                cmd = (PushAttackUnit(dst_base, src_base, 부대종류_전투, /*siege_weapon*/false, /*max_unit*/max_unit) or cmd); // 전투부대
            }
        
            if (디버그모드)
            {
                pk::force@ def_force  = pk::get_force(dst_base.get_force_id());
                pk::force@ atk_force  = force;
                string dst_name       = pk::decode(pk::get_name(dst_base));
                string src_name       = pk::decode(pk::get_name(src_base));
                string def_force_name = pk::decode(pk::get_name(pk::get_person(def_force.kunshu)));
                string atk_force_name = pk::decode(pk::get_name(pk::get_person(atk_force.kunshu)));
                
                if (cmd)
                    pk::info(pk::format("{}군, {}군 {} 공략 중, {} ({}에서 파병)", atk_force_name, def_force_name, dst_name, status_info, src_name));
            }
            return true;
        }
        
        int ref_status = 10000;    // 공방우위_기준병력
        int max_unit_combat = pk::max(1, pk::min(3, 거점공략_전투부대수));
        int max_unit_supply = pk::max(1, pk::min(3, 거점공략_수송부대수));
        int max_unit_siege  = pk::max(1, pk::min(5, 거점공략_공성부대수));
        
        
        bool PushAttackUnit(pk::building@ dst_base, pk::building@ src_base, int attack_type, bool siege_weapon=false, int max_unit=1)
        {
            // 원군 출진 거점
            int src_id = src_base.get_id();
            int dst_id = dst_base.get_id();

            // 출진 명령 조건
            int unit_count = 0;
            int push_count = 0;  // push 횟수 확인 변수
            
            if (dst_id >= 건물_도시끝)   // 관문/거점은 한 거점에서 1부대만
                max_unit = 1;
                
            // 출진거점에서 반복적으로 출진 명령 수행
            while(unit_count < max_unit and push_count < max_unit)
            {
                push_count += 1;
                
                if (attack_type == 부대종류_수송)
                {
                    int unit_id = PushTransportUnit(src_base, dst_base, true);  // 수송부대 출진
                    if (unit_id != -1)
                    {
                        unit_count += 1;     // 수송부대 카운트
                        if (!list_attack_base.contains(src_base))
                            list_attack_base.add(src_base);
                    }
                }
                else if (attack_type == 부대종류_전투 and !siege_weapon)
                {
                   int unit_id = PushCombatUnit(src_base, dst_base, true);     // 전투부대 출진
                    if (unit_id != -1)
                    {
                        unit_count += 1;     // 전투부대 카운트
                        if (!list_attack_base.contains(src_base))
                            list_attack_base.add(src_base);
                    }
                }
                else if (attack_type == 부대종류_전투 and siege_weapon)
                {
                    int unit_id = PushSiegeUnit(src_base, dst_base, true);     // 공성부대 출진
                    if (unit_id != -1)
                    {
                        unit_count += 1;     // 전투부대 카운트
                        if (!list_attack_base.contains(src_base))
                            list_attack_base.add(src_base);
                    }
                }
            }
            if (unit_count > 0) return true;
            
			return false;
        }
        
        // 목표 적 거점으로 진군 중인 부대수
        int count_atk_unit_type(pk::building@ src_base, pk::building@ dst_base, int check_type, bool siege_weapon=false)
        {
            int count = 0;
            if (!pk::is_alive(src_base) or !pk::is_alive(dst_base)) return -1;
            int src_id = src_base.get_id();
            int dst_id = dst_base.get_id();
            if (src_id == dst_id) return -1;
            
            pk::district@ district = pk::get_district(src_base.get_district_id());
            pk::array<pk::unit@> arr_unit = pk::list_to_array(pk::get_unit_list(district));
            for (int i = 0; i < int(arr_unit.length); i++)
            {
                pk::unit@ unit = arr_unit[i];
                int service_id = pk::get_service(unit);
                
                bool is_valid_type = false;
                if (check_type == 부대종류_수송)
                    is_valid_type = (unit.type == check_type);
                else if (check_type == 부대종류_전투)
                {
                    if (siege_weapon and is_siege_weapon(pk::get_ground_weapon_id(unit)))
                        is_valid_type = (unit.type == check_type);
                    else if (!siege_weapon and !is_siege_weapon(pk::get_ground_weapon_id(unit)))
                        is_valid_type = (unit.type == check_type);
                }
                
                if (is_valid_type and service_id == src_id and unit.target_type == 부대임무대상_거점)
                {
                    pk::building@ building_t = pk::get_building(unit.target);
                    if (pk::is_alive(building_t) and building_t.get_id() == dst_id and pk::is_enemy(unit, building_t))
                        count++;
                }
            }
            return count;
        }
        
        
        
        //---------------------------------------------------------------------------------------
        // 거점별 병력 현황 업데이트
        //---------------------------------------------------------------------------------------
        
        // 수정금지
        array<int> def_troopsbase(건물_도시끝, 0);   // 구역 내 거점벙력
        array<int> def_troops_sum(건물_도시끝, 0);   // 구역 내 수비군 벙력합
        array<int> def_unit_count(건물_도시끝, 0);   // 구역 내 수비군 부대수
        array<int> atk_troops_sum(건물_도시끝, 0);   // 구역 내 공격군 벙력합
        array<int> atk_unit_count(건물_도시끝, 0);   // 구역 내 공격군 부대수
        
        // 턴 세력 거점 구역별 : 병력정보 초기화
        void clear_engage_info()
        {
            for (int city_id = 0; city_id < 건물_도시끝; city_id++)
            {
                def_troopsbase[city_id] = 0;
                def_troops_sum[city_id] = 0;
                def_unit_count[city_id] = 0;
                atk_troops_sum[city_id] = 0;
                atk_unit_count[city_id] = 0;
            }
        }
        
        // 턴 세력 거점 구역별 : 교전하는 병력합, 부대수 업데이트
        void update_engage_info()
        {
            pk::array<pk::unit@> arr_unit = pk::list_to_array(pk::get_unit_list());
            for (int i = 0; i < int(arr_unit.length); i++)
            {
                pk::unit@ unit = arr_unit[i];
                int unit_id = unit.get_id();
                int city_id = pk::get_city_id(unit.pos);    // 부대가 위치한 좌표 구역의 거점
                if (city_id >= 0 and city_id <= 건물_도시끝)
                {
                    pk::city@ city = pk::get_city(city_id);
                    if (unit.get_force_id() == city.get_force_id()) // 부대와 거점이 동일 세력
                    {
                        def_troops_sum[city_id] += unit.troops; // 구역 수비병력합
                        def_unit_count[city_id] += 1;           // 구역 수비부대수
                    }
                    else if (pk::is_enemy(unit, city))          // 부대와 거점이 적대세력
                    {
                        atk_troops_sum[city_id] += unit.troops; // 구역 공격병력합
                        atk_unit_count[city_id] += 1;           // 구역 공격부대수
                    }
                }
            }
            for (int city_id = 0; city_id < 건물_도시끝; city_id++)
            {
                pk::city@ city = pk::get_city(city_id);
                def_troopsbase[city_id] = pk::get_troops(city);
            }
        }
        
        // 침략한 적부대 병력수준 확인
        bool is_invaded_base(pk::building@ base)
        {
            if (!pk::is_alive(base)) return false;
            
            int city_id = pk::get_city_id(base.pos);
            if (atk_unit_count[city_id] == 0) return false;
            
            if (atk_unit_count[city_id] <= 2 and get_engage_status(city_id) < 0) return false;
            
            return true;
        }
        
        // 침략한 세력 확인
        int get_force_invaded(pk::building@ base)
        {
            // 범위 내 적 부대들의 소속 세력 카운트
            pk::array<int> arr_force_unit_count(세력_끝, 0);
            pk::array<pk::point> range = pk::range(base.pos, 1, 6);
            for (int i = 0; i < int(range.length); i++)
            {
                pk::unit@ unit = pk::get_unit(range[i]);
                if (pk::is_alive(unit) and pk::is_enemy(unit, base))
                    arr_force_unit_count[unit.get_force_id()]++;
            }
            // 가장 많은 적부대의 소속세력 반환
            int max_force_id    = -1;
            int max_force_count = -1;
            for (int j = 0; j < 세력_끝; j++)
            {
                int count = arr_force_unit_count[j];
                if (max_force_count < count)
                {
                    max_force_count = count;
                    max_force_id = j;
                }
            }
            
            if (max_force_count <= 0) return -1;
            if (!pk::is_normal_force(max_force_id)) return -1;
            
            return max_force_id;
        }
        
        // 교전 병력 비교 결과
        int get_engage_status(int city_id)
        {
            if (city_id < 0 and city_id > 건물_도시끝) return 0;
            
            int def_base_troops = def_troopsbase[city_id];
            int def_unit_troops = def_troops_sum[city_id];
            int atk_unit_troops = atk_troops_sum[city_id];
            
            int status = atk_unit_troops - (def_base_troops + def_unit_troops);
            return status;      // 공격측이 많으면 양수, 수비측이 많으면 음수
        }
        
        
        
        //---------------------------------------------------------------------------------------
        
        string get_order_info(int order)
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
                default : name = "부대임무_없음"; break;
            }
            
            return name;
        }
        
        //---------------------------------------------------------------------------------------
        bool 디버그모드 = false;
        
        
        
        
	};

	Main main;
}