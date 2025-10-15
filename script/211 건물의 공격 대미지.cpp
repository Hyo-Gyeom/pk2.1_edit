namespace 건물의_공격_대미지
{
	class Main
	{
		Main()
		{
			pk::set_func(211, pk::func211_t(callback));
		}

		void callback(pk::damage_info& info, pk::building@ attacker, pk::hex_object@ target)
		{
			// 사용하지 않음
			if (target.is_instance(pk::building::type_id))
				pk::assert(false);

			int facility_id = attacker.facility;
			pk::unit@ target_unit = pk::hex_object_to_unit(target);
			int troops_atk = 0, atk = 0, def = 0, troops = 0;

			switch (facility_id)
			{
			case 시설_도시:
			case 시설_관문:
			case 시설_항구:
				func_5af0e0(troops_atk, atk, def, troops, attacker);
				info.troops_damage = func_5aee60(atk, troops, troops_atk, 0, target_unit.attr.stat[부대능력_방어], func_5aee10(target_unit)) * 0.8f;
				if (pk::equipment_id_to_heishu(target_unit.weapon) == 병종_병기)
					info.troops_damage *= 0.8f;
				break;

			case 시설_진:
			case 시설_요새:
			case 시설_성채:
			case 시설_궁노:
			case 시설_연노로:
			case 시설_투석대:
				info.troops_damage = func_5af370(attacker, target_unit);
				if (pk::equipment_id_to_heishu(target_unit.weapon) == 병종_병기 and attacker.facility != 시설_투석대)
					info.troops_damage *= 0.8f;
				break;

			default:
				// 시설.자동공격 대미지
				info.troops_damage = func_5af370(attacker, target_unit);
				if (pk::equipment_id_to_heishu(target_unit.weapon) == 병종_병기)
					info.troops_damage *= 0.8f;
				break;
			}

			if (attacker.is_player())
				info.troops_damage *= float(pk::core["대미지패널티"][pk::get_scenario().difficulty]);

			// 방어강화 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 2배
			if (attacker.has_tech(기교_방어강화) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_성채)
				info.troops_damage *= 2;

			info.src_pos = attacker.get_pos();
			info.dst_pos = target.get_pos();
		}

		/**
			지휘 가능한 병력 수
		*/
		int func_5aee10(pk::unit@ unit)
		{
			pk::person@ leader = pk::get_person(unit.leader);
			if (pk::is_alive(leader))
				return pk::min(pk::get_command(leader), unit.troops);
			return unit.troops;
		}

		/***/
		float func_5aee60(int src_atk, int src_troops, int tactics_atk, int buffed, int dst_def, int dst_troops)
		{
			float n = 0;
			int a = 0;
			int b = 0;
			int c = 0;

			a = pk::max(src_atk, 40);
			a = a * a;

			b = pk::max(dst_def, 40);
			b = b * b;

			n = sqrt(tactics_atk * 64);
			n = n + pk::max((src_troops - dst_troops) / 2000, 0);
			n = n + pk::max((a - b) / 300, 0);
			n = n + 50;
			n = n * (buffed + 10);

			a = (src_troops / 100 + 300) * (src_atk + 50) * (src_atk + 50) / 100;
			b = (dst_troops / 100 + 300) * (dst_def + 50) * (dst_def + 50) / 100;
			c = a + b;
			if (a >= b)
				c = a * 100 / c;
			else
				c = pk::max(100 - (b * 100 / c), 1);
			n = n * c;

			a = pk::max(src_troops / 4, 1);
			n = n * pk::min(sqrt(a), 40.f);

			n = n / 2100;
			n = n + src_troops / 200;

			return n;
		}

		/***/
		void func_5af0e0(int &out src_atk, int &out dst_atk, int &out dst_def, int &out dst_troops, pk::building@ building)
		{
			src_atk = 10;
			pk::person@ taishu = pk::get_person(pk::get_taishu_id(building));
			if (pk::is_alive(taishu))
			{
				dst_atk = pk::max(taishu.stat[무장능력_무력] - 25, 40);
				dst_def = pk::max(taishu.stat[무장능력_통솔] - 25, 40);
				dst_troops = pk::min(pk::get_troops(building), pk::get_command(taishu));
			}
			else
			{
				dst_atk = 30;
				dst_def = 30;
				dst_troops = pk::min(pk::get_troops(building), 7000);
				if (dst_troops != 0)
					dst_troops = pk::max(dst_troops, 500);
			}
		}

		/**
			군사시설의 공격력 계산
		*/
		int func_5af370(pk::building@ attacker, pk::unit@ target)
		{
			int atk = 0;
			int bonus = 0;

			switch (attacker.facility)
			{
			case 시설_진:
				atk = 400;
				bonus = 300;
				break;
			case 시설_요새:
				atk = 600;
				bonus = 300;
				break;
			case 시설_성채:
				atk = 800;
				bonus = 300;
				break;
			case 시설_궁노:
				atk = 300;
				bonus = 300;
				break;
			case 시설_연노로:
				atk = 500;
				bonus = 400;
				break;
			case 시설_투석대:
				atk = 900;
				bonus = 600;
				break;
			default:
				// 시설.자동공격 대미지
				atk = 300;
				bonus = 300;
				break;
			}
			
			float max_hp = pk::max(pk::get_max_hp(attacker), 1);
			float hp = pk::max(attacker.hp, max_hp / 2);

			return hp / max_hp * (152 - int(target.attr.stat[부대능력_방어])) * atk / 132 + pk::rand(bonus);
		}
	}

	Main main;
}