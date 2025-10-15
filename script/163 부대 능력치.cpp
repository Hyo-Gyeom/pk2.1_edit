namespace 부대_능력치
{
	class Main
	{
		Main()
		{
			pk::set_func(163, pk::func163_t(callback));
		}

		void callback(pk::unit_attr& attr, const pk::detail::arrayptr<pk::person@> &in member, int weapon_id, uint troops, int type, int status, bool navy, bool shoubyou)
		{
			pk::person@ leader = member[0];

			if (!pk::is_alive(leader))
				return;

			pk::force@ force = pk::get_force(leader.get_force_id());

			if (!pk::is_alive(force))
				return;
			if (!pk::is_valid_equipment_id(weapon_id) or !pk::is_valid_unit_type(type) or !pk::is_valid_unit_status(status))
				return;

			// 부장이 있다면
			if (member[1] !is null or member[2] !is null)
			{
				// 혐오 관계가 있다면 보정하지 않음
				if (func_4960d0(leader, member[1]) or func_4960d0(leader, member[2]) or func_4960d0(member[1], member[2]))
				{
					for (int i = 0; i < 무장능력_끝; i++)
						attr.stat[i] = shoubyou ? leader.stat[i] : leader.max_stat[i];
				}
				// 혐오 관계가 없다면 보정
				else
				{
					for (int i = 0; i < 무장능력_끝; i++)
					{
						int a = 0, b = 0;
						int leader_stat = shoubyou ? leader.stat[i] : leader.max_stat[i];

						// 통솔, 무력은 무장 관계에 따라 보정
						if (i <= 무장능력_무력)
						{
							if (member[1] !is null)
								a = func_495ff0(leader, leader_stat, member[1], shoubyou ? member[1].stat[i] : member[1].max_stat[i]);
							if (member[2] !is null)
								b = func_495ff0(leader, leader_stat, member[2], shoubyou ? member[2].stat[i] : member[2].max_stat[i]);
						}
						// 지력, 정치, 매력은 최대값
						else
						{
							if (member[1] !is null)
								a = pk::max(leader_stat, shoubyou ? member[1].stat[i] : member[1].max_stat[i]);
							if (member[2] !is null)
								b = pk::max(leader_stat, shoubyou ? member[2].stat[i] : member[2].max_stat[i]);
						}

						attr.stat[i] = pk::max(a, b);
					}
				}
			}
			// 부장이 없다면
			else
			{
				// 주장 능력치 그대로 사용
				for (int i = 0; i < 무장능력_끝; i++)
					attr.stat[i] = shoubyou ? leader.stat[i] : leader.max_stat[i];
			}

			// 적성은 최대값
			for (int i = 0; i < 병종_끝; i++)
			{
				int n = 0;
				for (int j = 0; j < member.length; j++)
				{
					if (member[j] !is null)
						n = pk::max(n, member[j].tekisei[i]);
				}
				attr.tekisei[i] = n;
			}

			pk::equipment@ equipment = pk::get_equipment(weapon_id);

			float apt = float(pk::core["적성"][attr.tekisei[pk::equipment_id_to_heishu(weapon_id)]]["부대능력치"]); // (attr.tekisei[pk::equipment_id_to_heishu(weapon_id)] + 7) * 0.1;
			float sts = 1.f;
			float str = 1.f;
			float ldr = 1.f;
			int atk = equipment.stat[병기능력_공격];
			int def = equipment.stat[병기능력_방어];
			int mov = equipment.stat[병기능력_이동];

			if (weapon_id == 병기_검 or (weapon_id == 병기_주가 and type == 부대종류_수송))
				apt = 0.6f;

			if (status == 부대상태_혼란)
				sts = 0.8f;

			if (type == 부대종류_수송)
			{
				str = 0.4f;
				ldr = 1.f / 3.f;
			}

			if (weapon_id <= 병기_군마)
			{
				int tech_id = -1;

				if (weapon_id == 병기_창)
					tech_id = 기교_정예창병;
				else if (weapon_id == 병기_극)
					tech_id = 기교_정예극병;
				else if (weapon_id == 병기_노)
					tech_id = 기교_정예노병;
				else if (weapon_id == 병기_군마)
					tech_id = 기교_정예기병;

				if (pk::has_tech(force, tech_id))
				{
					atk = atk + 10;
					def = def + 10;
				}
			}

			attr.stat[부대능력_공격] = pk::min(255.f, pk::max(1.f, (attr.stat[부대능력_무력] * atk * apt * 0.01f) * str * sts));

			attr.stat[부대능력_방어] = pk::min(255.f, pk::max(1.f, (attr.stat[부대능력_통솔] * def * apt * 0.01f) * ldr * sts));

			attr.stat[부대능력_건설] = pk::min(255.f, pk::max(1.f, (attr.stat[부대능력_정치] * 2.f / 3 + 50) * ldr * sts));

			if (type == 부대종류_전투)
			{
				switch (weapon_id)
				{
				case 병기_창:
					if (pk::has_tech(force, 기교_정예창병))
						mov = mov + 6;
					break;
				case 병기_극:
					if (pk::has_tech(force, 기교_정예극병))
						mov = mov + 6;
					break;
				case 병기_노:
					if (pk::has_tech(force, 기교_정예노병))
						mov = mov + 6;
					break;
				case 병기_군마:
					if (pk::has_tech(force, 기교_정예기병))
						mov = mov + 2;
					if (pk::has_tech(force, 기교_양마산출))
						mov = mov + 4;
					break;
				case 병기_충차:
				case 병기_정란:
				case 병기_투석:
				case 병기_목수:
					if (pk::has_tech(force, 기교_차축강화))
						mov = mov + 4;
					break;
				}

				if (weapon_id <= 병기_노)
				{
					mov = mov + int(pk::core::skill_constant(member, 특기_강행)); // 5
				}
				else if (weapon_id == 병기_군마)
				{
					if (pk::has_skill(member, 특기_강행))
						mov = mov + int(pk::core::skill_constant(member, 특기_강행)); // 5
					else if (pk::has_skill(member, 특기_행군))
						mov = mov + int(pk::core::skill_constant(member, 특기_행군)); // 3
				}
			}
			else
			{
				if (pk::has_tech(force, 기교_목우유마))
					mov = mov + 3;
				mov = mov + 5;
				mov = mov + int(pk::core::skill_constant(member, 특기_운반)); // 5
			}

			if (navy)
				mov = mov + int(pk::core::skill_constant(member, 특기_조타)); // 4

			attr.stat[부대능력_이동] = mov;
		}

		/**
			관계에 따른 능력 보정
		*/
		int func_495ff0(pk::person@ leader, int leader_stat, pk::person@ deputy, int deputy_stat)
		{
			if (leader_stat > deputy_stat)
				return leader_stat;

			int leader_id = leader.get_id();
			int deputy_id = deputy.get_id();

			if (pk::is_gikyoudai(leader, deputy_id) or pk::is_fuufu(leader, deputy_id))
				return deputy_stat;

			if (pk::is_like(leader, deputy_id) or pk::is_like(deputy, leader_id))
				return leader_stat + (deputy_stat - leader_stat) / 2;

			if (pk::is_ketsuen(leader, deputy_id))
				return leader_stat + (deputy_stat - leader_stat) / 3;

			if (pk::is_dislike(leader, deputy_id) or pk::is_dislike(deputy, leader_id))
				return leader_stat;

			return leader_stat + (deputy_stat - leader_stat) / 4;
		}

		/**
			서로 상대방을 혐오하는 관계가 있는지 확인
		*/
		bool func_4960d0(pk::person@ a, pk::person@ b)
		{
			if (a !is null and b !is null)
				return pk::is_dislike(a, b.get_id()) or pk::is_dislike(b, a.get_id());
			return false;
		}
	}

	Main main;
}