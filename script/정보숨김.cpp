namespace 정보숨김
{
	class Main
	{
		uint force_flags_0_;
		uint force_flags_1_;
		uint district_flags_0_;
		uint district_flags_1_;
		uint person_flags_0_;
		uint person_flags_1_;
		uint building_flags_0_;
		uint building_flags_1_;
		uint unit_flags_0_;
		uint unit_flags_1_;
		dictionary visible_pos_;

		Main()
		{
			// 이 파일의 시나리오 초기화 이벤트는 세력 재배치 이후에 발생해야함.
			pk::bind(102, -1, pk::trigger102_t(game_init));
			pk::bind(111, pk::trigger111_t(turn_start));
			pk::bind(132, pk::trigger132_t(force_leave_control));
			pk::bind(140, pk::trigger140_t(district_create));
			pk::bind(152, pk::trigger152_t(person_force_change));
			pk::bind(160, pk::trigger160_t(building_create));
			pk::bind(161, pk::trigger161_t(building_destroy));
			pk::bind(170, pk::trigger170_t(unit_create));
			pk::bind(171, pk::trigger171_t(unit_destroy));
			pk::bind(172, pk::trigger172_t(unit_pos_change));
		}

		void game_init()
		{
			force_flags_0_ = -1;
			force_flags_1_ = -1;
			district_flags_0_ = -1;
			district_flags_1_ = -1;
			person_flags_0_ = -1;
			person_flags_1_ = -1;
			building_flags_0_ = -1;
			building_flags_1_ = -1;
			unit_flags_0_ = -1;
			unit_flags_1_ = -1;

			switch (pk::get_scenario().difficulty)
			{
			case 난이도_상급:
				force_flags_0_ = 6;
				district_flags_0_ = 6;
				building_flags_0_ = 0x3e;
				unit_flags_0_ = 0x1e;
				break;

			case 난이도_특급:
				force_flags_0_ = 0;
				district_flags_0_ = 0;
				person_flags_0_ = 0;
				person_flags_1_ = 0;
				building_flags_0_ = 0;
				unit_flags_0_ = 0;
				break;
			}

			if (pk::get_scenario().loaded)
				turn_start(pk::get_force(pk::get_current_turn_force_id()));
		}

		void turn_start(pk::force@ force)
		{
			if (!force.is_player())
				return;

			// 시야 초기화
			visible_pos_.deleteAll();

			// 모든 정보를 숨김 상태로 초기화
			pk::core::info(pk::force::type_id, 0);
			pk::core::info(pk::district::type_id, 0);
			pk::core::info(pk::person::type_id, 0);
			pk::core::info(pk::building::type_id, 0);
			pk::core::info(pk::unit::type_id, 0);

			int force_id = force.get_id();

			for (int i = 0; i < 세력_끝; i++)
			{
				auto force = pk::get_force(i);
				if (force.is_player())
					pk::core::info(force, -1, force_flags_1_);
				else
					pk::core::info(force, -1, force_flags_0_);
			}

			for (int i = 0; i < 군단_끝; i++)
			{
				auto district = pk::get_district(i);
				if (district.is_player())
					pk::core::info(district, -1, district_flags_1_);
				else
					pk::core::info(district, -1, district_flags_0_);
			}

			for (int i = 0; i < 무장_끝; i++)
			{
				auto person = pk::get_person(i);
				if (person.is_player())
					pk::core::info(person, -1, person_flags_1_);
				else
					pk::core::info(person, -1, person_flags_0_);
			}

			for (int i = 0; i < 건물_끝; i++)
			{
				auto building = pk::get_building(i);
				if (building.is_player())
				{
					pk::core::info(building, -1, building_flags_1_);
					auto range = get_sight_range(building);
					update_hex_object(force_id, building.get_pos(), range.first, range.second);
				}
				else
				{
					pk::core::info(building, -1, building_flags_0_ | uint(pk::core::info(building, -1)));
				}
			}

			for (int i = 0; i < 부대_끝; i++)
			{
				auto unit = pk::get_unit(i);
				if (unit.is_player())
				{
					pk::core::info(unit, -1, unit_flags_1_);
					auto range = get_sight_range(unit);
					update_hex_object(force_id, unit.get_pos(), range.first, range.second);
				}
				else
				{
					pk::core::info(unit, -1, unit_flags_0_ | uint(pk::core::info(unit, -1)));
				}
			}
		}

		void force_leave_control(pk::force@ force)
		{
			// 모든 세력을 위임한 경우
			if (pk::get_player_count() == 1)
			{
				// 모든 정보를 보이는 상태로 초기화
				pk::core::info(pk::force::type_id, -1);
				pk::core::info(pk::district::type_id, -1);
				pk::core::info(pk::person::type_id, -1);
				pk::core::info(pk::building::type_id, -1);
				pk::core::info(pk::unit::type_id, -1);
			}
		}

		void district_create(pk::district@ district, int type)
		{
			if (district.is_player())
				pk::core::info(district, -1, district_flags_1_);
		}

		void person_force_change(pk::person@ person, pk::force@ force)
		{
			if (force.is_player())
				pk::core::info(person, -1, person_flags_1_);
		}

		void building_create(pk::building@ building, int type)
		{
			if (building.is_player())
			{
				auto range = get_sight_range(building);
				update_hex_object(building.get_force_id(), building.get_pos(), range.first, range.second);
			}
			else if (visible_pos_.exists(make_key(building.get_pos())))
			{
				pk::core::info(building, -1, building_flags_1_);
			}
		}

		void building_destroy(pk::building@ building, int type)
		{
			pk::core::info(building, -1, building_flags_0_);
		}

		void unit_create(pk::unit@ unit, int type)
		{
			if (unit.is_player())
				pk::core::info(unit, -1, unit_flags_1_);
		}

		void unit_destroy(pk::unit@ unit, int type)
		{
			pk::core::info(unit, -1, unit_flags_0_);
		}

		void unit_pos_change(pk::unit@ unit, const pk::point &in pos)
		{
			if (unit.is_player())
			{
				auto range = get_sight_range(unit);
				update_hex_object(unit.get_force_id(), pos, range.first, range.second);
			}
			else if (visible_pos_.exists(make_key(pos)))
			{
				pk::core::info(unit, -1, unit_flags_1_);
			}
		}

		bool is_player_turn()
		{
			pk::force@ force = pk::get_force(pk::get_current_turn_force_id());
			return force !is null and force.is_player();
		}

		string make_key(const pk::point& in pos)
		{
			return pk::format("{}_{}", pos.x, pos.y);
		}

		void update_hex_object(int force_id, const pk::point& in pos, int min, int max)
		{
			visible_pos_[make_key(pos)] = 0;
			auto range = pk::range(pos, min, max);
			for (int i = 0; i < range.length; i++)
			{
				visible_pos_[make_key(range[i])] = 0;
				auto hex_obj = pk::get_hex_object(range[i]);
				if (hex_obj !is null and hex_obj.get_force_id() != force_id)
				{
					if (hex_obj.is_instance(pk::unit::type_id))
						pk::core::info(hex_obj, -1, unit_flags_1_ | uint(pk::core::info(hex_obj, -1)));
					else
						pk::core::info(hex_obj, -1, building_flags_1_ | uint(pk::core::info(hex_obj, -1)));

					auto person_list = pk::get_person_list(pk::get_hex_object_id(hex_obj), -1);
					for (int j = 0; j < person_list.count; j++)
						pk::core::info(person_list[j], -1, person_flags_1_);
				}
			}
		}

		pk::int_int get_sight_range(pk::object@ obj)
		{
			if (obj.is_instance(pk::unit::type_id))
			{
				return pk::int_int(1, 2);
			}
			else if (obj.is_instance(pk::building::type_id))
			{
				// pk::hex_object 타입으로 들어온 경우가 있을 수 있으므로 한번 변환
				auto building = pk::hex_object_to_building(obj);
				auto facility = pk::get_facility(building.facility);
				if (facility !is null)
					return pk::int_int(facility.min_range, facility.max_range + (building.facility == 시설_도시 ? 1 : 0));
			}
			return pk::int_int(0, 0);
		}
	}

	Main main;
}