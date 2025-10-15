namespace 처형
{
	class Main
	{
		int TP_COST = 666;
		pk::building@ building_;
		pk::force@ force_;

		Main()
		{
			pk::menu_item item;
			item.menu = 105;
			item.pos = 6;
			item.shortcut = "6";
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(is_visible);
			item.is_enabled = pk::menu_item_is_enabled_t(is_enabled);
			item.get_text = pk::menu_item_get_text_t(get_text);
			item.get_desc = pk::menu_item_get_desc_t(get_desc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
		}

		bool is_visible()
		{
			return !pk::is_campaign();
		}

		bool is_enabled()
		{
			return get_target_list().count > 0 and force_.tp > TP_COST;
		}

		string get_text()
		{
			return pk::u8encode("처형");
		}

		string get_desc()
		{
			if (get_target_list().count == 0)
				return pk::u8encode("처형 가능한 무장이 없습니다.");
			else if (force_.tp < TP_COST)
				return pk::u8encode(pk::format("기교P가 부족합니다.(기교P {}/인)", TP_COST));
			else
				return pk::u8encode(pk::format("무장을 처형합니다.(기교P {}/인)", TP_COST));
		}

		bool handler()
		{
			pk::list<pk::person@> target_list = get_target_list();
			pk::list<pk::person@> temp_list;

			target_list.sort(function(a, b) {
				if (a.mibun != b.mibun)
					return a.mibun > b.mibun;
				if (a.loyalty != b.loyalty)
					return a.loyalty < b.loyalty;
				return a.p < b.p;
			});

			// 처형할 무장 선택.
			while (true)
			{
				temp_list = pk::person_selector(pk::u8encode("대상무장선택"), pk::u8encode(pk::format("처형할 무장을 선택해 주십시오.({}명까지)", force_.tp / TP_COST)), target_list, 1, pk::min(target_list.count, force_.tp / TP_COST), temp_list);
				if (temp_list.count == 0)
					return false;
				if (pk::yes_no(pk::u8encode("정말로 처형하시겠습니까?")))
					break;
				// 취소를 눌렀을 때 한 명일 경우 선택 복구하지 않음.
				if (temp_list.count == 1)
					temp_list.clear();
			}

			target_list = temp_list;

			pk::person@ kunshu = pk::get_person(force_.kunshu);
			int force_id = force_.get_force_id();
			array<int> death_count(세력_끝, 0);

			// 기교 감소.
			pk::add_tp(force_, target_list.count * -TP_COST, force_.get_pos());

			// 처형.
			for (int i = 0; i < target_list.count; i++)
			{
				pk::person@ target = target_list[i];
				pk::play_se(5);
				pk::message_box(pk::get_msg(pk::msg_param(5929, target, kunshu)), target);
				int target_force_id = target.get_force_id();
				if (pk::is_valid_force_id(target_force_id))
					death_count[target_force_id]++;
				pk::kill(target, kunshu, pk::get_hex_object(target.location), null, 2);
			}

			// 자세력 무장 충성도 감소.
			int turn_counter = pk::get_scenario().turn_counter;
			// 155번 월별 충성도 감소 함수를 위해 1월로 변경함.
			pk::get_scenario().turn_counter = turn_counter + (13 - pk::get_month()) * 3;
			for (int i = 0; i < 건물_거점끝; i++)
			{
				pk::building@ building = pk::get_building(i);
				if (pk::is_alive(building) and building.get_force_id() == force_id)
				{
					temp_list = pk::get_person_list(building, pk::mibun_flags(신분_도독, 신분_태수, 신분_일반));
					for (int j = 0; j < temp_list.count; j++)
						pk::add_loyalty(temp_list[j], cast<pk::func155_t@>(pk::get_func(155, true))(building, temp_list[j]));
				}
			}
			pk::get_scenario().turn_counter = turn_counter;

			// 처형시킨 세력과의 우호도 악화.
			for (int i = 0; i < 세력_끝; i++)
			{
				if (i != force_id)
					pk::add_relations(force_, i, death_count[i] > 0 ? -100 : 0);
			}

			return true;
		}

		pk::list<pk::person@> get_target_list()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if (!pk::is_absent(person) and !pk::is_unitize(person) and person.mibun != 신분_군주 and person.mibun != 신분_포로)
							list.add(person);
					}
					else if (person.mibun == 신분_포로)
					{
						pk::hex_object@ hexobj = pk::get_hex_object(person.location);
						if (pk::is_alive(hexobj) and hexobj.get_force_id() == force_id)
							list.add(person);
					}
				}
			}
			return list;
		}
	}

	Main main;
}