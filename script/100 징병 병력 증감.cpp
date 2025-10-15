namespace 징병_병력_증감
{
	class Main
	{
		Main()
		{
			pk::set_func(100, pk::func100_t(callback));
		}

		int callback(pk::building@ building, const pk::detail::arrayptr<pk::person@> &in actors)
		{
			if (pk::is_alive(building) and actors[0] !is null)
			{
				pk::city@ city = pk::building_to_city(building);

				if (pk::is_alive(city))
				{
					int n = 0;
					int sum = 0;
					int mul = 100;
					for (int i = 0; i < actors.length; i++)
					{
						pk::person@ actor = actors[i];
						if (pk::is_alive(actor))
						{
							sum += actor.stat[int(pk::core["징병.능력"])];
							if (pk::has_skill(actor, int(pk::core["징병.특기"])))
								mul = 150;
						}
					}

					// 도시에 위압 특기가 있으면 치안이 낮아도 최대 징병량 유지
					if (building.has_skill(특기_위압))
						n = (1000 + 120 * sum * 5 / 100) * mul / 100 * func_5c4600(city);
					else
						n = (1000 + (city.public_order + 20) * sum * 5 / 100) * mul / 100 * func_5c4600(city);

					// 특급 난이도 컴퓨터일 경우 2배
					if (pk::get_scenario().difficulty == 난이도_특급 and !city.is_player())
						n *= 2;

					// 기교 인심장악이 있으면 15% 증가 (특기종합패치)
					if (pk::has_tech(city, 기교_인심장악))
						n *= 1.2f;

					// 주변에 적 부대가 존재할 경우 반감
					if (pk::enemies_around(building))
						n /= 2;

					return n;
				}
			}
			return 0;
		}

		float func_5c4600(pk::city@ city)
		{
			int level1 = 0, level2 = 0;
			for (int i = 0; i < city.max_devs; i++)
			{
				pk::building@ building = city.dev[i].building;
				if (pk::is_alive(building))
				{
					switch (building.facility)
					{
					case 시설_병영: building.completed ? level1++ : 0; break;
					case 시설_병영2단: building.completed ? level2++ : level1++; break;
					case 시설_병영3단: building.completed ? 0 : level2++; break;
					}
				}
			}
			if (city.barracks_counter > level1 + level2)
				return 1.5f;
			if (city.barracks_counter > level1)
				return 1.2f;
			return 1.f;
		}
	}

	Main main;
}