namespace ai_script
{
	/**
	이 샘플은 컴퓨터 인공지능이 어떤 흐름으로 실행되는지 설명합니다.
	개발, 흡수합병 등 개별 행동의 일부를 수정하는건 구현이 복잡하고 제약사항이 많기 때문에 인공지능 전체를 새로 작성해야합니다.
	반드시 샘플이 나타내는 흐름대로 작성하실 필요는 없으며 자유롭게 작성하시면 됩니다.
	*/
	class Main
	{
		Main()
		{
			pk::set_func(261, pk::func261_t(callback));
		}

		// 콜백이 호출되는 시점에 군단 행동력은 이미 초기화된 상태입니다.
		bool callback(pk::force@ force)
		{
			if (force.is_player())
				player_ai(force);
			else if (pk::is_normal_force(force.get_id()))
				com_ai(force);
			else
				com_iminzoku_ai(force);
			// 전체를 재작성 하므로 기존 인공지능은 무시함.
			return true;
		}

		void player_ai(pk::force@ force)
		{
			// 1군단의 행동완료되지 않은 부대
			int first_district_id = pk::get_district_id(force, 1);
			for (int i = 0; i < 부대_끝; i++)
			{
				auto unit = pk::get_unit(i);
				if (pk::is_alive(unit) and unit.get_district_id() == first_district_id and !unit.action_done)
				{
					if (pk::is_valid_unit_order(unit.order) or pk::is_valid_force_id(unit.requested_from))
					{
						unit_ai(unit);
						pk::run_order(unit);
					}
				}
			}

			// 나머지 군단
			for (int i = 2; i < 9; i++)
			{
				auto district = pk::get_district(pk::get_district_id(force, i));
				if (pk::is_alive(district))
				{
					unit_ai(district);
					building_ai(district);
				}
			}
		}

		void com_ai(pk::force@ force)
		{
			force_ai(force);
			for (int i = 1; i < 9; i++)
			{
				auto district = pk::get_district(pk::get_district_id(force, i));
				if (pk::is_alive(district))
				{
					// 컴퓨터에 대해서는 최소 행동력을 고정하여 패널티를 주는 것 같습니다.
					if (!district.is_player())
						district.ap = pk::max(40 * pk::get_city_list(district).count, district.ap);
					unit_ai(district);
					building_ai(district);
				}
			}
		}

		void com_iminzoku_ai(pk::force@ force)
		{
			int force_id = force.get_id();
			for (int i = 0; i < 부대_끝; i++)
			{
				pk::unit@ unit = pk::get_unit(i);
				if (pk::is_alive(unit) and unit.get_district_id() == force_id and !unit.action_done)
				{
					unit_ai(unit);
					pk::run_order(unit);
				}
			}
		}

		void force_ai(pk::force@ force)
		{
			// 추방, 관직, 군사, 군단, 수여, 외교, 기교연구, 능력연구
		}

		void building_ai(pk::district@ district)
		{
			int district_id = district.get_id();
			pk::list<pk::building@> list;

			for (int i = 0; i < 건물_거점끝; i++)
			{
				auto building = pk::get_building(i);
				if (pk::is_alive(building) and building.get_district_id() == district_id)
					list.add(building);
			}

			// list.sort(function(a, b) {});

			for (int i = 0; i < list.count; i++)
				building_ai(list[i]);
		}

		void building_ai(pk::building@ building)
		{
			// 개발, 징병, 생산, 순찰, 거래, 훈련, 방어, 공격, 설치, 수송, 이동, 소환, 탐색, 등용, 포상, 이호경식, 구호탄랑, 유언비어, 철거, 흡수합병
		}

		void unit_ai(pk::district@ district)
		{
			int district_id = district.get_id();
			pk::list<pk::unit@> list;

			// 부대 상태에 따라 임무를 수정합니다.
			for (int i = 0; i < 부대_최대; i++)
			{
				auto unit = pk::get_unit(i);
				if (pk::is_alive(unit) and unit.get_district_id() == district_id and !unit.action_done)
				{
					unit_ai(unit);
					list.add(unit);
				}
			}

			// 행동 순서를 결정합니다.
			list.sort(function(a, b)
			{
				if (pk::is_valid_district_id(a.requested_from))
					return true;
				if (pk::is_valid_district_id(b.requested_from))
					return false;
				if (pk::is_valid_unit_order(a.order) and pk::is_valid_unit_order(b.order))
				{
					int a_priority = UNIT_ORDER_PRIORITY[a.order];
					int b_priority = UNIT_ORDER_PRIORITY[b.order];
					if (a_priority != b_priority)
						return a_priority > b_priority;
				}
				if (a.gold != b.gold)
					return a.gold > b.gold;
				return a.p < b.p;
			});

			// 행동합니다.
			for (int i = 0; i < list.count; i++)
				pk::run_order(list[i]);
		}

		void unit_ai(pk::unit@ unit)
		{
			// 부대 상태에 따라 임무를 귀환이나 정복으로 수정합니다.
			// 정복의 경우 귀환이 불가능할 경우입니다.
		}
	}

	Main main;
	array<int> UNIT_ORDER_PRIORITY = { -1, 5, 2, 3, 5, 4, 1, -1, 2, 3, 5, -1 };
}