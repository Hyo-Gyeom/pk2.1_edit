namespace 포로_처우
{
	class Main
	{
		Main()
		{
			pk::set_func(258, pk::func258_t(callback));
		}

		int callback(pk::person@ prisoner, const pk::prisoner_info &in info)
		{
			pk::person@ judge = pk::get_person(pk::get_kunshu_id(info.attacker));

			if (func_4ae3a0(prisoner, judge))
				return 포로처우_처단;

			if (cast<pk::func111_t>(pk::get_func(111))(prisoner, judge, info.releasable ? 1 : 2, 0))
				return 포로처우_등용;

			if (func_4af5b0(prisoner, judge))
				return 포로처우_처단;

			if (prisoner.mibun == 신분_군주)
				return 포로처우_해방;

			if (!func_4b0740(info))
				return 포로처우_해방;

			return 포로처우_포로;
		}

		/***/
		bool func_4ae3a0(pk::person@ prisoner, pk::person@ judge)
		{
			if (!pk::is_alive(prisoner) or !pk::is_alive(judge))
				return false;

			// 포로를 혐오하는 경우 처단
			if (pk::is_dislike(judge, prisoner.get_id()))
				return true;

			if (prisoner.mibun == 신분_군주)
			{
				pk::force@ judge_force = pk::get_force(judge.get_force_id());
				pk::force@ prisoner_force = pk::get_force(prisoner.get_force_id());

				if (!pk::is_alive(judge_force) or !pk::is_alive(prisoner_force))
					return false;

				// 포로가 황제이고 내가 후한 황제를 보호중일 경우 처단
				if (prisoner_force.title == 작위_황제 and pk::is_protecting_the_emperor(judge_force))
					return true;

				// 내가 황제이고 포로가 후한 황제를 보호중일 경우 처단
				if (pk::is_protecting_the_emperor(prisoner_force) and judge_force.title == 작위_황제)
					return true;
			}

			return false;
		}

		/***/
		bool func_4af5b0(pk::person@ prisoner, pk::person@ judge)
		{
			int prisoner_id = prisoner.get_id();

			// 의형제인 경우 처단하지 않음(pk::is_gikyoudai 함수를 사용하는 것이 맞다고 생각함)
			if (prisoner.gikyoudai == judge.gikyoudai) return false;

			// 부부인 경우 처단하지 않음
			if (pk::is_fuufu(judge, prisoner_id)) return false;

			// 포로를 친애하는 경우 처단하지 않음
			if (pk::is_like(judge, prisoner_id)) return false;

			int max_stat = 0;
			int avg_stat = 0;
			int death = 10;

			for (int i = 0; i < 무장능력_끝; i++)
			{
				int s = prisoner.stat[i];
				max_stat = pk::max(max_stat, s);
				avg_stat = avg_stat + s;
			}
			avg_stat = avg_stat / 무장능력_끝;

			switch (pk::get_scenario().battle_death)
			{
			case 전사_없음: death = 20; break;
			case 전사_표준: death = 10; break;
			case 전사_많음: death = 0; break;
			}

			int n = 0;

			if (prisoner.mibun == 신분_군주)
			{
				n += (judge.ambition + prisoner.ambition) * 2;
				n -= (pk::max(max_stat, 50) + pk::max(avg_stat, 50)) / 2;
				n += (16 - judge.giri) * 5;
				n -= death;
				n -= pk::rand(20);
				n = pk::max(n, 0);
			}
			else
			{
				n += 100 - (pk::max(max_stat, 50) + pk::max(avg_stat, 50)) / 2;
				n -= pk::max(prisoner.kouseki, 20000) / 2000 * (judge.promotion == 기용_실적 ? 1.5f : 1);
				n -= prisoner.ambition * (judge.promotion == 기용_의리 ? 5 : 1);
				n -= pk::rand(25) * (judge.promotion == 기용_임의 ? 1.5f : 1);
				n -= death;
				n = pk::max(n, 0);
			}

			return pk::rand_bool(n);
		}

		/***/
		bool func_4b0740(const pk::prisoner_info &in info)
		{
			// 멸망시킨경우 모두 해방
			if (!info.releasable) return false;

			// 거점을 공격해 포로를 잡은 경우
			if (info.target.is_instance(pk::building::type_id))
			{
				// 부대가 잡은게 아니라면 해방
				if (!info.attacker.is_instance(pk::unit::type_id)) return false;

				pk::building@ building = pk::hex_object_to_building(info.target);
				pk::unit@ unit = pk::hex_object_to_unit(info.attacker);
				int pay = 0;

				// 부대 무장들의 봉록 합
				for (int i = 0; i < 3; i++)
				{
					pk::person@ member = pk::get_person(unit.member[i]);
					if (pk::is_alive(member) and member.mibun >= 신분_도독 and member.mibun <= 신분_일반)
					{
						pk::rank@ rank = pk::get_rank(member.rank);
						if (pk::is_alive(rank))
							pay += rank.pay;
					}
				}

				// 부대가 잡고 있는 포로 비용 합
				pay += (pk::get_person_list(pk::get_hex_object_id(unit), pk::mibun_flags(신분_포로)).size + 2) * 50;

				pay *= 2;

				// 금이 부족하다면 해방
				if (unit.gold + pk::get_gold(building) < pay) return false;
			}
			// 부대를 공격해 포로를 잡은 경우
			else
			{
				pk::person@ attacker;
				pk::building@ service;
				int pay = 0;

				if (info.attacker.is_instance(pk::unit::type_id))
					@attacker = pk::get_person(pk::hex_object_to_unit(info.attacker).leader);
				else if (info.attacker.is_instance(pk::building::type_id))
					@attacker = pk::get_person(pk::get_taishu_id(pk::hex_object_to_building(info.attacker)));
				else
					@attacker = pk::get_person(pk::get_kunshu_id(info.attacker));

				if (pk::is_alive(attacker))
					@service = pk::get_building(attacker.service);

				// 포로를 잡은 무장의 소속이 없다면 해방
				if (!pk::is_alive(service)) return false;

				// 소속 거점 무장들의 봉록 합
				pk::list<pk::person@> list = pk::get_person_list(service, pk::mibun_flags(신분_도독, 신분_태수, 신분_일반));
				for (int i = 0; i < list.size; i++)
				{
					pk::rank@ rank = pk::get_rank(list[i].rank);
					if (pk::is_alive(rank))
						pay += rank.pay;
				}

				// 소속 거점 포로 비용 합
				pay += (pk::get_person_list(pk::get_hex_object_id(service), pk::mibun_flags(신분_포로)).size + 2) * 50;

				pay *= 2;

				// 소속 거점으로 귀환했을 때 포로를 유지할 수 있는 비용이 부족하다면 해방
				if (pk::get_gold(service) < pay) return false;
			}

			return true;
		}
	}

	Main main;
}