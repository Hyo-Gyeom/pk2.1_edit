namespace 금_수입
{
	class Main
	{
		Main()
		{
			pk::set_func(150, pk::func150_t(callback));
		}

		int callback(pk::city@ city)
		{

			if (city is null or !pk::is_valid_force_id(city.get_force_id()))
				return 0;

			// 기본 금 수입
			int n = city.revenue;

			for (int i = 0; i < city.max_devs; i++)
			{
				pk::building@ building = city.dev[i].building;
				int facility_id = -1;

				if (pk::is_alive(building))
				{
					facility_id = building.facility;
					switch (facility_id)
					{
					case 시설_시장:
					case 시설_대시장:
					case 시설_어시장:
					case 시설_암시장:
						if (!building.completed)
							continue;
						break;
					case 시설_시장2단:
						if (!building.completed)
							facility_id = 시설_시장;
						break;
					case 시설_시장3단:
						if (!building.completed)
							facility_id = 시설_시장2단;
						break;
					default:
						continue;
					}
				}

				// 내정시설 별 생산력을 더함.
				pk::facility@ facility = pk::get_facility(facility_id);
				if (pk::is_alive(facility))
				{
					int y = facility.yield;
					// 시장은 조폐와 인접한 경우 1.5배.
					if (facility_id == 시설_시장 or facility_id == 시설_시장2단 or facility_id == 시설_시장3단)
					{
						if (func_49ed70(building.get_pos(), 시설_조폐))
							y = y * 1.5f;

						// 항관확장 기교 시장 수입 15% 증가 (특기종합패치)
						if (pk::has_tech(city, 기교_항관확장))
							y = y * 1.15f;
					}
					n = n + y;
				}
			}

			switch (pk::get_scenario().difficulty)
			{
			case 난이도_특급:
				// 특급일 경우 플레이어 0.75배, 컴퓨터 1.5배.
				if (city.is_player())
					n = n * 0.75f;
				else
					n = n * 1.5f;
				break;

			case 난이도_초급:
				// 초급일 경우 플레이어, 컴퓨터 모두 1.25배.
				n = n * 1.25f;
				break;
			}

			// 도시 치안값을 백분율로 곱.
			n = n * pk::max(city.public_order, 50) / 100;

			return n;
		}

		/**
			인접 시설 검색.
		*/
		bool func_49ed70(pk::point pos, int facility_id)
		{
			for (int i = 0; i < 방향_끝; i++)
			{
				pk::point neighbor_pos = pk::get_neighbor_pos(pos, i);
				if (pk::is_valid_pos(neighbor_pos))
				{
					pk::building@ building = pk::get_building(neighbor_pos);
					if (pk::is_alive(building) and building.facility == facility_id and building.completed)
						return true;
				}
			}
			return false;
		}
	}

	Main main;
}