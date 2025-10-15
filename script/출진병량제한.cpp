namespace 출진병량제한
{
	class Main
	{
		Main()
		{
			pk::bind(170, pk::trigger170_t(food_limit));
		}

		void food_limit(pk::unit@ unit, int type)
		{
			if (!pk::is_alive(unit)) 
				return;

			if (unit.type == 부대종류_전투 and !pk::is_player_controlled(unit))	// AI 또는 위임군단의 전투부대만 적용
			{
				if (unit.food > unit.troops * 2.4f)
				{
					pk::add_food(pk::get_building(pk::get_person(unit.leader).service), unit.food - unit.troops * 2.4f, false);        // 거점에 초과 병량을 더함
					pk::add_food(unit, unit.troops * 2.4f - unit.food, false);        // 부대 병량을 제한함
				}
			}
		}
	}
	Main main;
}