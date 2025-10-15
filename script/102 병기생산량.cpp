namespace 병기생산량
{
	class Main
	{
		Main()
		{
			pk::set_func(102, pk::func102_t(callback));
		}

		int callback(pk::city@ city, const pk::detail::arrayptr<pk::person@> &in actors, int weapon_id)
		{
			if (!pk::is_valid_equipment_id(weapon_id))
				return 0;
			if (weapon_id >= 병기_충차)
				return 1;

			int n = 0, sum = 0, max = 0, skill_id = -1;
			bool has_skill = false;

			if (weapon_id <= 병기_노)
				skill_id = int(pk::core["병기생산.대장간특기"]);
			else if (weapon_id == 병기_군마)
				skill_id = int(pk::core["병기생산.마구간특기"]);

			for (int i = 0; i < actors.length; i++)
			{
				pk::person@ actor = actors[i];
				if (pk::is_alive(actor))
				{
					int s = actor.stat[int(pk::core["병기생산.능력"])];
					sum = sum + s;
					max = pk::max(max, s);
					if (pk::has_skill(actor, skill_id))
						has_skill = true;
				}
			}

			n = (sum + max + 200) * 5;

			// 관련 특기를 가지고 있다면 2배
			if (has_skill)
				n = n * 2;

			n = n * func_5c7040(city, weapon_id);

			// 컴퓨터일 경우 특급 난이도에서 2배
			if (pk::get_scenario().difficulty == 난이도_특급 and !city.is_player())
				n = n * 2;

			return n;
		}

		float func_5c7040(pk::city@ city, int weapon_id)
		{
			int level1 = 0, level2 = 0;
			if (weapon_id == 병기_창 or weapon_id == 병기_극 or weapon_id == 병기_노)
			{
				for (int i = 0; i < city.max_devs; i++)
				{
					pk::building@ building = city.dev[i].building;
					if (pk::is_alive(building))
					{
						switch (building.facility)
						{
						case 시설_대장간: building.completed ? level1++ : 0; break;
						case 시설_대장간2단: building.completed ? level2++ : level1++; break;
						case 시설_대장간3단: building.completed ? 0 : level2++; break;
						}
					}
				}
				if (city.blacksmith_counter > level1 + level2)
					return 1.5f;
				if (city.blacksmith_counter > level1)
					return 1.2f;
			}
			else if (weapon_id == 병기_군마)
			{
				for (int i = 0; i < city.max_devs; i++)
				{
					pk::building@ building = city.dev[i].building;
					if (pk::is_alive(building))
					{
						switch (building.facility)
						{
						case 시설_마구간: building.completed ? level1++ : 0; break;
						case 시설_마구간2단: building.completed ? level2++ : level1++; break;
						case 시설_마구간3단: building.completed ? 0 : level2++; break;
						}
					}
				}
				if (city.stable_counter > level1 + level2)
					return 1.5f;
				if (city.stable_counter > level1)
					return 1.2f;
			}
			return 1.f;
		}
	}

	Main main;
}