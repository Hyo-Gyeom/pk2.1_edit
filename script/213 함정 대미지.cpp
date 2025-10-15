namespace 함정_대미지
{
	class Main
	{
		Main()
		{
			pk::set_func(213, pk::func213_t(callback));
		}

		void callback(pk::damage_info& info, int trap, pk::unit@ attacker, pk::hex_object@ target, bool critical)
		{
			int n = 0;
			int bakuyaku = 0; // 폭약연성 기교 추가 피해량

			if (target.is_instance(pk::unit::type_id))
			{
				bakuyaku = 300;
				switch (trap)
				{
				case 시설_불씨:
				case 시설_화구:
					n = 300 + pk::rand(100) + 50;
					break;
				case 시설_화염종:
					n = 400 + pk::rand(200) + 50;
					break;
				case 시설_화염구:
				case 시설_화선:
					n = 400 + pk::rand(100) + 50;
					break;
				case 시설_업화구:
				case 시설_업화종:
					n = 500 + pk::rand(200) + 50;
					break;
				case 시설_낙석:
					n = 1500 + pk::rand(500) + 50;
					break;
				default:
					n = 0 + pk::rand(0) + 50;
					break;
				}
			}
			else
			{
				bakuyaku = 100;
				switch (trap)
				{
				case 시설_불씨:
					n = 100 + pk::rand(100) + 50;
					break;
				case 시설_화구:
				case 시설_화염종:
				case 시설_화선:
					n = 200 + pk::rand(100) + 50;
					break;
				case 시설_화염구:
				case 시설_업화종:
					n = 300 + pk::rand(100) + 50;
					break;
				case 시설_업화구:
					n = 400 + pk::rand(100) + 50;
					break;
				case 시설_낙석:
					n = 800 + pk::rand(1000) + 50;
					break;
				case 시설_제방:
					n = pk::hex_object_to_building(target).hp;
					n = pk::max(n - pk::max(n - pk::rand(40) - 980, 1), 0);
					break;
				default:
					n = 0 + pk::rand(0) + 50;
					break;
				}
			}

			if (trap == 시설_낙석 or trap == 시설_제방)
			{
				if (target.has_skill(특기_등갑))
				{
					n = n / 2;
					info.def_skill = 특기_등갑;
				}
			}
			else
			{
				if (pk::is_alive(attacker) and attacker.has_tech(기교_폭약연성))
					n = n + bakuyaku;
				if (pk::is_alive(attacker) and attacker.has_skill(특기_화신))
					n = n * 2;

				if (target.has_skill(특기_등갑))
				{
					n = n * 2;
					info.def_skill = 특기_등갑;
				}
				else if (target.has_skill(특기_화신))
				{
					n = 0;
				}
			}

			if (target.has_skill(특기_답파))
			{
				n = n * 0.1;
				info.def_skill = 특기_답파;
			}

			if (target.is_instance(pk::unit::type_id))
				info.troops_damage = n;
			else
				info.hp_damage = n;

			if (target.is_instance(pk::building::type_id))
			{
				pk::building@ building = pk::hex_object_to_building(target);
				if (building.facility == 시설_제방 and not building.completed)
					info.hp_damage = 0;
			}
		}
	}

	Main main;
}