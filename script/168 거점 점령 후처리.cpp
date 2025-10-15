namespace 거점_점령_후처리
{
	class Main
	{
		Main()
		{
			pk::set_func(168, pk::func168_t(callback));
		}

		void callback(pk::building@ base, pk::unit@ attacker)
		{
			pk::person@ leader;

			if (pk::is_alive(attacker))
				@leader = pk::get_person(attacker.leader);

			int n = 5;
			int charisma = 20;

			if (pk::is_alive(leader))
			{
				charisma = leader.stat[무장능력_매력];
				n = pk::max(charisma / 10, 5);
			}

			pk::set_gold(base, pk::get_gold(base) * n / 100);
			pk::set_food(base, pk::get_food(base) * n / 100);
			pk::set_troops(base, pk::get_troops(base) * n / 100);
			for (int i = 0; i < 병기_끝; i++)
				pk::set_weapon_amount(base, i, pk::get_weapon_amount(base, i) * n / 100);

			pk::city@ city = pk::building_to_city(base);

			// 도시가 아닌경우 여기서 종료
			if (!pk::is_alive(city))
				return;

			int city_id = city.get_id();
			int city_force_id = city.get_force_id();
			pk::list<pk::building@> city_devs;
			pk::list<pk::building@> buildings = pk::get_building_list();

			for (int i = 0; i < buildings.size; i++)
			{
				pk::building@ building = buildings[i];
				if (pk::is_alive(building) and pk::get_city_id(building.get_pos()) == city_id)
				{
					switch (pk::get_facility_type(building))
					{
					case 시설종류_내정시설:
						if (building.completed)
						{
							// 건설 완료된 내정시설은 동작대가 아니라면 무작위로 파괴.
							if (building.facility != 시설_동작대)
								city_devs.push_back(building);
						}
						else
						{
							// 건설중인 동작대가 파괴된 경우 동작 보물도 사라짐.
							if (building.facility == 시설_동작대)
							{
								pk::item@ item = pk::get_item(보물_동작);
								if (pk::is_alive(item))
									pk::kill(item);
							}
							// 건설중인 내정시설은 모두 파괴.
							else
							{
								pk::kill(building, false);
							}
						}
						break;

					case 시설종류_군사시설:
						// 구역 내 도시를 점령하고 있던 세력과 같은 세력의 군사시설은 모두 파괴
						if (pk::is_valid_force_id(city_force_id) and city_force_id == building.get_force_id())
							pk::kill(building, false);
						break;
					}
				}
			}

			n = city_devs.size;
			if (pk::is_valid_normal_force_id(attacker.get_force_id()))
				n = n - pk::min(charisma / 20, n);
			city_devs.shuffle();
			for (int i = 0; i < n; i++)
				pk::kill(city_devs[i], false);
		}
	}

	Main main;
}