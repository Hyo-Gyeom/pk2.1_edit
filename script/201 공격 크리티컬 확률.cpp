namespace 공격_크리티컬_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(201, pk::func201_t(callback));
		}

		bool callback(pk::unit@ attacker, pk::hex_object@ target, int tactics_id, bool ranged)
		{
			pk::unit@ target_unit = pk::hex_object_to_unit(target);
			int target_strength = int(pk::core["무장.최대능력치"]) + 1;

			if (target_unit !is null)
				target_strength = pk::get_best_member_stat(target_unit, 무장능력_무력);

			// 전법
			if (pk::is_valid_tactics_id(tactics_id))
			{
				if (attacker.has_skill(특기_패왕)) return true;
				if (pk::get_best_member_stat(attacker, 특기_용장, 무장능력_무력) > target_strength) return true;

				switch(pk::get_weapon_id(attacker))
				{
				case 병기_창:
					if (attacker.has_skill(특기_투신)) return true;
					if (attacker.has_skill(특기_창신)) return true;
					if (pk::get_best_member_stat(attacker, 특기_창장, 무장능력_무력) > target_strength) return true;
					if (pk::get_best_member_stat(attacker, 특기_신장, 무장능력_무력) > target_strength) return true;
					if (pk::get_best_member_stat(attacker, 특기_비장, 무장능력_무력) > target_strength) return true;
					break;

				case 병기_극:
					if (attacker.has_skill(특기_투신)) return true;
					if (attacker.has_skill(특기_극신)) return true;
					if (pk::get_best_member_stat(attacker, 특기_극장, 무장능력_무력) > target_strength) return true;
					if (pk::get_best_member_stat(attacker, 특기_신장, 무장능력_무력) > target_strength) return true;
					if (pk::get_best_member_stat(attacker, 특기_비장, 무장능력_무력) > target_strength) return true;
					break;

				case 병기_노:
					if (attacker.has_skill(특기_궁신)) return true;
					if (pk::get_best_member_stat(attacker, 특기_궁장, 무장능력_무력) > target_strength) return true;
					if (pk::get_best_member_stat(attacker, 특기_신장, 무장능력_무력) > target_strength) return true;
					if (pk::get_best_member_stat(attacker, 특기_비장, 무장능력_무력) > target_strength) return true;
					break;

				case 병기_군마:
					if (attacker.has_skill(특기_기신)) return true;
					if (pk::get_best_member_stat(attacker, 특기_기장, 무장능력_무력) > target_strength) return true;
					if (pk::get_best_member_stat(attacker, 특기_신장, 무장능력_무력) > target_strength) return true;
					if (pk::get_best_member_stat(attacker, 특기_비장, 무장능력_무력) > target_strength) return true;
					break;

				case 병기_충차:
				case 병기_정란:
				case 병기_투석:
				case 병기_목수:
					if (attacker.has_skill(특기_공신)) return true;
					break;

				case 병기_주가:
				case 병기_누선:
				case 병기_투함:
					if (attacker.has_skill(특기_수신)) return true;
					if (pk::get_best_member_stat(attacker, 특기_수장, 무장능력_무력) > target_strength) return true;
					break;
				}
			}
			// 공격, 일제
			else
			{
				if (pk::get_best_member_stat(attacker, 특기_신장, 무장능력_무력) > target_strength) return true;
				if (pk::get_best_member_stat(attacker, 특기_구축, 무장능력_무력) > target_strength) return true;

				if (pk::get_weapon_id(attacker) == 병기_군마)
				{
					if (ranged and attacker.has_skill(특기_백마)) return true;
				}
			}

			// 공성 특기 보유, 건물 목표
			if (attacker.has_skill(특기_공성) and target.is_instance(pk::building::type_id)) return true;

			// 부대 목표, 숲에서 공격, 난전 특기 보유
			if (target_unit !is null and pk::get_hex(attacker.get_pos()).terrain == 지형_숲 and attacker.has_skill(특기_난전)) return true;

			int n = 0;
			pk::person@ leader = pk::get_person(attacker.leader);

			if (attacker.attr.stat[부대능력_무력] >= 80)
				n = 2;
			else if (attacker.attr.stat[부대능력_무력] >= 60)
				n = 1;

			n = n + pk::get_tekisei(attacker);

			for (int i = 0; i < 2; i++)
			{
				int deputy = attacker.deputy[i];
				if (pk::is_valid_person_id(deputy))
				{
					if (pk::is_gikyoudai(leader, deputy) or pk::is_fuufu(leader, deputy))
						n = n + 4;
					else if (pk::is_like(leader, deputy))
						n = n + 2;
					else if (pk::is_dislike(leader, deputy))
						n = n - 5;
				}
			}

			return pk::rand_bool(pk::max(n, 0));
		}
	}

	Main main;
}