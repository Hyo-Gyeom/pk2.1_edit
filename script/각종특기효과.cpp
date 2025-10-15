namespace 각종특기효과
{
	class Main
	{
		Main()
		{
			pk::bind(110, pk::trigger110_t(NewYear));
			pk::bind(173, pk::trigger173_t(get_exp));
		}

		void NewYear()
		{
			for (int i = 0; i < 도시_끝; i++)
			{
				pk::city@ city = pk::get_city(i);

				if (city is null)
					continue;

				pk::building@ building = pk::city_to_building(city);

				// 인정, 풍수, 기원이 한 도시에서 새해를 맞으면 100% 풍작
				if (pk::is_valid_person_id(building.who_has_skill(특기_인정)) and pk::is_valid_person_id(building.who_has_skill(특기_풍수)) and pk::is_valid_person_id(building.who_has_skill(특기_기원)))
					city.next_housaku = true;
			}
		}

		void get_exp(pk::unit@ unit, int type)
		{
			if (!pk::is_alive(unit))
				return;

			// 전법 성공시 앙양 부대 기력 5 회복
			if ((type == 6) or (type == 7) or (type == 9) or (type == 10) or (type == 12) or (type == 13) or (type == 15) or (type == 16) or (type == 18) or (type == 19) or (type == 21) or (type == 22) or (type == 24) or (type == 25) or (type == 27) or (type == 28) or (type == 30) or (type == 31) or (type == 33) or (type == 34) or (type == 36) or (type == 37) or (type == 39) or (type == 40) or (type == 42) or (type == 43) or (type == 45) or (type == 46) or (type == 48) or (type == 49) or (type == 51) or (type == 52))
			{
				if (unit.has_skill(특기_앙양))
					pk::add_energy(unit, 5, true);
			}

			// 전법 실패시 노발 부대 기력 10 회복
			if ((type == 8) or (type == 11) or (type == 14) or (type == 17) or (type == 20) or (type == 23) or (type == 26) or (type == 29) or (type == 32) or (type == 35) or (type == 38) or (type == 41) or (type == 44) or (type == 47) or (type == 50) or (type == 53))
			{
				if (unit.has_skill(특기_노발))
					pk::add_energy(unit, 10, true);
			}
		}
	}

	Main main;
}