/*
@ 만든이: 기마책사 
@ UPDATE: '19.1.27    / 최초적용
@ UPDATE: '19.2.13    / 변경내용: 옥새/동작은 수색가능 리스트 제외, 설전/일기토 이벤트 추가

@ 수정자: HoneyBee ('19.2.15)
@ 수정사항: 태수가 <군주>인 경우 Message 추가 반영, <학자>에게 설전 화술 추가, <무술가>에게 조형 추가 

@ 수정자: 기마책사
@ UPDATE: '19.5.4    / 변경내용: 설전 결과에 따른 경험치, 상병 등을 적용

*/

namespace 도시보물수색
{
	//---------------------------------------------------------------------------------------
    // 유저 설정
	const int ACTION_COST_보물수색 = 0;        // 보물수색에 필요한 행동력
	const int GOLD_COST_보물수색 = 0;         // 보물수색에 필요한 금
    
    const int 보물소재지_발견확률 =100;            // 보물 소재 도시에서 실행 시 보물 발견 확률
    const float 특기안력_발견확률가중치 = 1.0f;     // 특기_안력 무장이 도시에 있을 시 발견 확률 증가
    
    //---------------------------------------------------------------------------------------
    
    
	
	class Main
	{
		
		pk::building@ building;
		pk::force@ force;
		pk::city@ city;
        pk::district@ district;
		pk::person@ kunshu;
		pk::person@ taishu;
        pk::point building_pos;
        uint16 building_gold;
		int building_energy;
        int item_id_found;
        int challange_result;
        array<int> arr_stat = {70, 75, 80, 85, 90};

		Main()
		{
            // 보물수색 메뉴 (유저용)
			pk::menu_item 보물수색;
			보물수색.menu = 102;        // 인재메뉴
			보물수색.init = pk::building_menu_item_init_t(init);
			보물수색.is_visible = pk::menu_item_is_visible_t(isVisible_보물수색);
			보물수색.is_enabled = pk::menu_item_is_enabled_t(isEnabled_보물수색);
			보물수색.get_text = pk::menu_item_get_text_t(getText_보물수색);
			보물수색.get_desc = pk::menu_item_get_desc_t(getDesc_보물수색);
			보물수색.handler = pk::menu_item_handler_t(handler_보물수색);
			pk::add_menu_item(보물수색);
            
		}

        
        //---------------------------------------------------------------------------------------
        
		void init(pk::building@ building_t)
		{
			@building = @building_t;
			@force = pk::get_force(building.get_force_id());
            @city = pk::building_to_city(building);
            @kunshu = pk::get_person(pk::get_kunshu_id(building));
            @taishu = pk::get_person(pk::get_taishu_id(building));
            @district = pk::get_district(building.get_district_id());
            
            building_gold = pk::get_gold(building);
            building_energy = pk::get_energy(building);
            
            building_pos = building.pos;
		}

		string getText_보물수색()
		{
			return pk::encode("보물수색");
		}

		string getDesc_보물수색()
		{
            if (!pk::is_alive(taishu) or taishu.action_done)
                return pk::encode("태수가 없거나 이미 태수의 행동을 완료했습니다");
            else if (int(district.ap) < ACTION_COST_보물수색)
                return pk::encode(pk::format("행동력이 부족합니다 (행동력{} 필요)", ACTION_COST_보물수색));
			else if (building_gold < GOLD_COST_보물수색)
				return pk::encode(pk::format("금이 부족합니다 (금{} 필요)", GOLD_COST_보물수색));
			else
				return pk::encode(pk::format("보물수색를 실행합니다 (금{} 사용)", GOLD_COST_보물수색));
		}

		bool isEnabled_보물수색()
		{ 
            if (!pk::is_alive(taishu) or taishu.action_done) return false;
            else if (int(district.ap) < ACTION_COST_보물수색) return false;
            else if (building_gold < GOLD_COST_보물수색) return false;
			return true;
		}
        
        bool isVisible_보물수색()
        {
            if (pk::is_campaign()) return false;
            if (!pk::is_alive(city)) return false;
            return true;
        }
        
        //---------------------------------------------------------------------------------------

		bool handler_보물수색()
		{
			if (force.is_player())
            {
                if (pk::choose({ pk::encode("예"), pk::encode("아니오") }, pk::encode(pk::format("미발견된 보물을 찾아보겠습니까? \n (금{} 사용)", GOLD_COST_보물수색)), taishu ) == 1)
                    return false;
            }
           
            taishu.action_done = true;
            
			pk::add_tp(force, 30, force.get_pos());
			pk::add_gold(city, -GOLD_COST_보물수색, true);
            
            if (force.is_player())
            {
                district.ap -= ACTION_COST_보물수색;
			}
            
            
            // 보물수색
            int item_id = -1; 
            item_id = get_hidden_item_id(true); // 세력내 미발견 보물검색
            string kunshu_name = pk::decode(pk::get_name(kunshu));
            if (pk::is_valid_item_id(item_id))  // 세력내 미발견 보물 유효
            {
                pk::item@ item = pk::get_item(item_id);
                pk::building@ location = pk::get_building(item.location);
                
                string item_name = pk::decode(pk::get_name(item));
                string item_location = pk::decode(pk::get_name(location));
                
                // 미발견 보물이 있는 도시에서 일정 확률로 발견
                if (building.get_id() == item.location and pk::rand_bool(get_chance_item_found(building)))
                {
                    // 아이템 설전/일기토 대전 ('19.2.13)
                    item_id_found = item_id;
                    pk::scene(pk::scene_t(scene_challange));
                    
                    // 보물 획득 성공
                    if (challange_result == 0)
                    {
                        // 보물 발견 처리
                        set_item_found(item);
                        pk::play_se(6);

                        // 보물 발견 메시지
                        if (kunshu.get_id() == taishu.get_id())
                            pk::message_box(pk::encode(pk::format("훗. \x1b[2x{}\x1b[0x에서 \x1b[1x{}\x1b[0x을(를) 얻었군", item_location, item_name)), taishu);
                        else
                            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님, \x1b[2x{}\x1b[0x에서 \x1b[1x{}\x1b[0x을(를) 얻었습니다", kunshu_name, item_location, item_name)), taishu);
                    }
                    // 보물 획득 실패
                    else if (challange_result == 1)
                    {
                        pk::play_se(7);
						
						if (kunshu.get_id() == taishu.get_id())
                            pk::message_box(pk::encode(pk::format("\x1b[2x{}\x1b[0x에서 \x1b[1x{}\x1b[0x을(를) 갖고 있는 자를 찾았으나 얻는데 실패하였군", item_location, item_name)), taishu);
                        else
							pk::message_box(pk::encode(pk::format("\x1b[2x{}\x1b[0x에서 \x1b[1x{}\x1b[0x을(를) 갖고 있는 자를 찾았으나 얻는데 실패했습니다", item_location, item_name)), taishu);
                    }
                }
                else    // 미발견 보물이 없는 도시
                {
                    pk::play_se(6);
                    
                    if (pk::rand_bool(10))       // 일정 확률로 소재+보물명 표시
						if (kunshu.get_id() == taishu.get_id())
							pk::message_box(pk::encode(pk::format("\x1b[2x{}\x1b[0x에서 \x1b[1x{}\x1b[0x을(를) 본 것 같다는 소문이로군.", item_location, item_name)), taishu);
						else
							pk::message_box(pk::encode(pk::format("\x1b[2x{}\x1b[0x에서 \x1b[1x{}\x1b[0x을(를) 본 것 같다는 소문이군요.", item_location, item_name)), taishu);
                    else 
                    {
                        if (pk::rand_bool(50))  // 일정 확률로 소재만 표시
						{
                            if (kunshu.get_id() == taishu.get_id())
								pk::message_box(pk::encode(pk::format("자국 내 어딘가에 \x1b[1x{}\x1b[0x이(가) 있는 듯 하군.", item_name)), taishu);
							else
								pk::message_box(pk::encode(pk::format("자국 내 어딘가에 \x1b[1x{}\x1b[0x이(가) 있는 듯 합니다.", item_name)), taishu);
                        }
						else                         // 영토 내 보물 존재여부만 표시
                        {	
							if (kunshu.get_id() == taishu.get_id())
								pk::message_box(pk::encode("영지에 괜찮은 보물이 있는 것 같군."), taishu);
							else
								pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님 영지에 괜찮은 보물이 있는 것 같습니다.", kunshu_name)), taishu);
						}
					}
                }
            }
            else
            {
                pk::play_se(7);
                
                item_id = get_hidden_item_id(false); // 타세력 미발견 보물검색
                if (pk::is_valid_item_id(item_id) and pk::rand_bool(50))
				{
					if (kunshu.get_id() == taishu.get_id())
						pk::message_box(pk::encode("이곳에는 보물이 없나 보군. 그렇다면 다른 곳을 찾아보는 것이 좋겠지."), taishu);
					else
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님 영지에는 보물이 없나 봅니다. 다른 곳을 찾아보시는 게 어떻겠습니까?.", kunshu_name)), taishu);
                }
				else
				{
					if (kunshu.get_id() == taishu.get_id())
						pk::message_box(pk::encode("아무래도 미발견된 보물은 없는 거 같군."), taishu);
					else
						pk::message_box(pk::encode("아무래도 미발견된 보물은 없는 것 같습니다."), taishu);
				}
            }
            
			return true;
			
		}
        
        //---------------------------------------------------------------------------------------
        
        // 미발견 보물 ID 반환 함수
        // in_force =true  : 자세력 검색
        // in_force =false : 타세력 검색
        int get_hidden_item_id(bool in_force)
        {
            int item_id = -1;
            
            for (int i = 0; i < 보물_끝; i++)
            {
                pk::item@ item = pk::get_item(i);
                if (pk::is_alive(item) and item.status == 보물상태_미등장 and (i != 보물_옥새 and i != 보물_동작))
                {
                    pk::building@ location = pk::get_building(item.location);
                    
                    if (in_force and location.get_force_id() == force.get_id())
                    {
                        item_id = i;
                        return item_id;
                    }
                    else if (!in_force and location.get_force_id() != force.get_id())
                    {
                        item_id = i;
                        return item_id;
                    }
                }
            }
            
            return item_id;
        }
        
        // 보물 발견 확률
        int get_chance_item_found(pk::building@ building_t)
        {
            int chance = 보물소재지_발견확률;
            
            if (!pk::is_alive(building_t)) return 0;
            if (pk::is_valid_person_id(building_t.who_has_skill(특기_안력)))
                chance = pk::min(100, int(특기안력_발견확률가중치 * chance));
            
            return chance;
        }
        
        //---------------------------------------------------------------------------------------
        
        // 설전/일기토
        void scene_challange()
        {
            int mode;
            pk::item@ item = pk::get_item(item_id_found);
            if (item.type == 5) mode = 0;   // 서책: 설전 모드
            else                mode = 1;   // 무기: 일기토 모드
            
            pk::fade(0);
            pk::background(4);  //배경: 시장
			pk::fade(255);
            
			// 설전
            if (mode == 0)
            {
                pk::person@ person = pk::get_person(무장_학자);
                person.base_stat[무장능력_지력] = arr_stat[pk::rand(5)];
				
				// 화술 추가
				person.wajutsu_daikatsu = true;
				person.wajutsu_kiben = true;
				person.wajutsu_mushi = true;
				person.wajutsu_chinsei = true;
				person.wajutsu_gyakujou = true;
			
                person.update();
                
                scene_message(person, mode);
                pk::background(-1);
				challange_result = pk::debate(taishu, person, pk::is_player_controlled(taishu), false, true, true).first;
            }
            // 일기토
			else if (mode == 1)
            {
                pk::unit@ dummy_unit;
                pk::person@ person = pk::get_person(무장_무술가);
                person.base_stat[무장능력_무력] = arr_stat[pk::rand(5)];
				
				// 무장 조형
				person.body[0] = 74;
				person.body[1] = 74;
				person.body[2] = 74;
				person.body[3] = 74;
				person.body[4] = 74;
				person.body[5] = 74;
				
                person.update();
                
                scene_message(person, mode);
                pk::background(-1);
				challange_result = pk::duel(dummy_unit, dummy_unit, taishu, null, null, person, null, null, pk::is_player_controlled(taishu), false, 0, true).first;
            }
        }
        
        void scene_message(pk::person@ person, int mode)
        {
            pk::item@ item = pk::get_item(item_id_found);
            string item_name = pk::decode(pk::get_name(item));
            string mode_name = (mode == 0)? "설전" : "일기토";
            
            pk::message_box(pk::encode("이보게. 자네가 괜찮은 보물을 갖고 있다던데 나에게 건내줄 수 있겠는가. 내가 사례를 하겠네."), taishu);
            pk::message_box(pk::encode(pk::format("이 \x1b[2x{}\x1b[0x 말입니까. 저와 \x1b[1x{}\x1b[0x을(를) 겨뤄서 이기면 드리겠습니다", item_name, mode_name)), person);
        }
        
        // 보물 발견 처리
        void set_item_found(pk::item@ item)
        {
            if (!pk::is_valid_item_id(item.get_id())) return;
            
            item.status   = 보물상태_등장;
            item.location = kunshu.location;
            item.owner    = kunshu.get_id();
            item.update();
        }
        
        
        //---------------------------------------------------------------------------------------
	}

	Main main;
}