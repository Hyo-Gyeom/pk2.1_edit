namespace 화계_대미지
{
	class Main
	{
		Main()
		{
			pk::set_func(214, pk::func214_t(callback));
		}

		void callback(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ target, bool critical)
		{
			pk::force@ target_force = pk::get_force(target.get_force_id());
			int atk = 0;

			if (target.is_instance(pk::unit::type_id))
			{
				atk = 300;
				info.troops_damage = atk + pk::rand(200);
			}
			else
			{
				atk = 100;
				info.hp_damage = atk + pk::rand(50);
			}

			if (bool(pk::core["폭약연성버그수정"]) ? pk::is_alive(attacker) and attacker.has_tech(기교_폭약연성) : pk::is_alive(target_force) and target.has_tech(기교_폭약연성))
			{
				info.troops_damage += atk;
				info.atk_tech = 기교_폭약연성;
			}

			if (pk::is_alive(attacker) and attacker.has_skill(특기_화신))
			{
				info.troops_damage *= 2;
				info.atk_skill = 특기_화신;
			}
			else if (attacker.has_skill(특기_화공))
			{
				info.troops_damage *= 1.5f;
				info.atk_skill = 특기_화공;
			}
/**
			if (target.weapon == 병기_충차 or target.weapon == 병기_정란 or target.weapon == 병기_투석 or target.weapon == 병기_목수)
			{
				info.troops_damage *= 2.f;
			}
*/
			if (target.has_skill(특기_등갑))
			{
				info.troops_damage *= 2;
				info.def_skill = 특기_등갑;
			}
			else if (target.has_skill(특기_화신))
			{
				info.troops_damage = 0;
				info.def_skill = 특기_화신;
			}


			if (target.is_instance(pk::unit::type_id))
			{
				pk::unit@ target_unit = pk::hex_object_to_unit(target);
				if (target_unit.weapon == 병기_충차 or target_unit.weapon == 병기_정란 
				        or target_unit.weapon == 병기_투석 or target_unit.weapon == 병기_목수)
				{
				        info.troops_damage *= 2.f;
				}
			}

			if (target.is_instance(pk::building::type_id))
			{
				pk::building@ building = pk::hex_object_to_building(target);
				if (building.facility == 시설_제방 and !building.completed)
					info.hp_damage = 0;
			}

			info.tactics = -1;
			if (pk::is_alive(attacker))
				info.src_pos = attacker.get_pos();
			info.dst_pos = target.get_pos();
		}
	}

	Main main;
}