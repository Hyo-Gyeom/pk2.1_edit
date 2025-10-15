namespace 철거금지
{
	const int KEY = pk::hash("철거금지");

	class Main
	{
		Main()
		{
			pk::bind(102, pk::trigger102_t(game_init));
			pk::bind(105, pk::trigger105_t(serialize));
			pk::bind(111, pk::trigger111_t(turn_start));
			pk::bind(112, pk::trigger112_t(turn_end));

			@func256_ = cast<pk::func256_t@>(pk::get_func(256));
			pk::reset_func(256);
			pk::set_func(256, pk::func256_t(func256));

			@func257_ = cast<pk::func257_t@>(pk::get_func(257));
			pk::reset_func(257);
			pk::set_func(257, pk::func256_t(func257));

			pk::menu_item item;
			item.menu = 0;
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(is_visible);
			item.get_text = pk::menu_item_get_text_t(get_text);
			item.get_desc = pk::menu_item_get_desc_t(get_desc);
			item.get_image_id = pk::menu_item_get_image_id_t(get_image_id);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		bool is_enabled(int force_id, int city_id)
		{
			return (enabled_[force_id] & (uint64(1) << city_id)) != 0;
		}

		void set(int force_id, int city_id, bool value)
		{
			if (value)
				enabled_[force_id] = enabled_[force_id] | uint64(1) << city_id;
			else
				enabled_[force_id] = enabled_[force_id] & ~(uint64(1) << city_id);
		}

		void toggle(int force_id, int city_id)
		{
			if (is_enabled(force_id, city_id))
			{
				set(force_id, city_id, false);
				if (force_id == pk::get_current_turn_force_id())
				{
					for (int facility_id = 0; facility_id < 시설_끝; facility_id++)
					{
						min_[city_id][facility_id] = int(pk::core["인공지능.개발.최소시설수"][city_id][facility_id]);
						pk::core["인공지능.개발.최소시설수"][city_id][facility_id] = 0;
					}
				}
			}
			else
			{
				set(force_id, city_id, true);
				if (force_id == pk::get_current_turn_force_id())
				{
					for (int facility_id = 0; facility_id < 시설_끝; facility_id++)
					{
						pk::core["인공지능.개발.최소시설수"][city_id][facility_id] = min_[city_id][facility_id];
					}
				}
			}
		}

		void game_init()
		{
			for (int i = 0; i < 세력_끝; i++)
				enabled_[i] = uint64(pk::load(KEY, i, uint64(-1)));
			// 불러오기를 한 경우 턴 시작 트리거를 건너뛰므로 한 번 실행해줌.
			if (pk::get_scenario().loaded)
				turn_start(pk::get_force(pk::get_current_turn_force_id()));
		}

		void serialize(int file_id)
		{
			for (int i = 0; i < 세력_끝; i++)
				pk::store(KEY, i, enabled_[i]);
		}

		void turn_start(pk::force@ force)
		{
			// 최소시설수는 철거뿐만 아니라 개발, 공격 명령에도 관련되어 있음.
			if (force.is_player())
			{
				int force_id = force.get_id();
				for (int city_id = 0; city_id < 도시_끝; city_id++)
				{
					if (pk::get_city(city_id).get_force_id() == force_id)
					{
						if (!is_enabled(force_id, city_id))
						{
							for (int facility_id = 0; facility_id < 시설_끝; facility_id++)
							{
								min_[city_id][facility_id] = int(pk::core["인공지능.개발.최소시설수"][city_id][facility_id]);
								pk::core["인공지능.개발.최소시설수"][city_id][facility_id] = 0;
							}
						}
					}
					else
					{
						// 도시를 빼앗긴 경우 리셋
						set(force_id, city_id, true);
					}
				}
			}
		}

		void turn_end(pk::force@ force)
		{
			if (force.is_player())
			{
				int force_id = force.get_id();
				for (int city_id = 0; city_id < 도시_끝; city_id++)
				{
					if (pk::get_city(city_id).get_force_id() == force_id)
					{
						if ((enabled_[force_id] & 1 << city_id) == 0)
						{
							for (int facility_id = 0; facility_id < 시설_끝; facility_id++)
							{
								pk::core["인공지능.개발.최소시설수"][city_id][facility_id] = min_[city_id][facility_id];
							}
						}
					}
				}
			}
		}

		void func256(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 거점AI_철거)
			{
				pk::city@ city = pk::building_to_city(building);
				if (city !is null and !is_enabled(city.get_force_id(), city.get_id()))
				{
					prev_ap_ = context.ap;
					context.ap = 0;
				}
			}

			if (func256_ !is null)
				func256_(context, building, cmd);
		}

		void func257(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 거점AI_철거)
			{
				pk::city@ city = pk::building_to_city(building);
				if (city !is null and !is_enabled(city.get_force_id(), city.get_id()))
				{
					context.ap = prev_ap_;
				}
			}

			if (func257_ !is null)
				func257_(context, building, cmd);
		}

		void init(pk::building@ building)
		{
			menu_city_id_ = -1;
			pk::city@ city = pk::building_to_city(building);
			if (city !is null)
				menu_city_id_ = city.get_id();
			menu_force_id_ = building.get_force_id();
		}

		bool is_visible()
		{
			return menu_city_id_ != -1 and menu_force_id_ == pk::get_current_turn_force_id();
		}

		string get_text()
		{
			return is_enabled(menu_force_id_, menu_city_id_) ? pk::u8encode("철거허용") : pk::u8encode("철거금지");
		}

		string get_desc()
		{
			return is_enabled(menu_force_id_, menu_city_id_) ? pk::u8encode("자세력 군단의 철거가 허용된 도시입니다.") : pk::u8encode("자세력 군단의 철거가 금지된 도시입니다.");
		}

		int get_image_id()
		{
			return is_enabled(menu_force_id_, menu_city_id_) ? 889 : 1249;
		}

		bool handler()
		{
			toggle(menu_force_id_, menu_city_id_);
			return true;
		}

		// 철거 가능 여부
		array<uint64> enabled_(세력_끝, -1);
		// 인공지능 최소시설수
		array<array<int>> min_(도시_끝, array<int>(시설_끝, -2));
		//
		pk::func256_t@ func256_;
		//
		pk::func257_t@ func257_;
		//
		int prev_ap_;
		// 건물 메뉴 도시
		int menu_city_id_;
		// 건물 메뉴 세력
		int menu_force_id_;
	}

	Main main;
}