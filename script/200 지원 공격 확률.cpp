namespace 지원_공격_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(200, pk::func200_t(callback));
		}

		int callback(pk::unit@ assister, pk::unit@ attacker, pk::unit@ target)
		{
			pk::person@ assister_leader = pk::get_person(assister.leader);
			int attacker_leader = attacker.leader;
			// 무장.xml 설정값
			int bonus = pk::core::assist_chance(assister.leader);

			// 배우자나 의형제일 경우 50%
			if (pk::is_fuufu(assister_leader, attacker_leader) or pk::is_gikyoudai(assister_leader, attacker_leader))
				return 50 + bonus;

			// 보좌 특기일 경우 혐오무장만 아니면 30%
			if (pk::has_skill(assister_leader, 특기_보좌) and !pk::is_dislike(assister_leader, attacker_leader))
				return 30 + bonus;

			// 친애무장일 경우 30%
			if (pk::is_like(assister_leader, attacker_leader))
				return 30 + bonus;

			// 혈연이고 혐오무장이 아닐경우 20%
			if (pk::is_ketsuen(assister_leader, attacker_leader) and !pk::is_dislike(assister_leader, attacker_leader))
				return 20 + bonus;

			return 0 + bonus;
		}
	}

	Main main;
}