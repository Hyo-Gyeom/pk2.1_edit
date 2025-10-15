namespace 부대_혼란_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(164, pk::func164_t(callback));
		}

		int callback(pk::unit@ unit)
		{
			int troops = unit.troops;

			// 병력 수가 1000 이상일 경우 회피
			if (troops >= 1000)
				return 0;

			// 부대가 통상 상태가 아닐 경우 회피
			if (unit.status != 부대상태_통상)
				return 0;

			// 병력 수 8% + 10 확률로 회피
			if (pk::rand_bool(pk::min(90, pk::max(10, troops * 8 / 100 + 10))))
				return 0;

			// 통솔 20% + 20 확률로 회피
			if (pk::rand_bool(unit.attr.stat[부대능력_통솔] / 5 + 20))
				return 0;

			pk::int_int_int tuple = func_59b130(unit);
			int sum_own_troops = tuple.first;
			int sum_enemy_troops = tuple.second;
			int max_enemy_troops = tuple.third;

			// 적 부대 중 최대 병력 수가 500 미만일 경우 회피
			if (max_enemy_troops < 500)
				return 0;

			// 적 부대 병력 합이 자세력 부대 병력 수의 20% 미만일 경우 60% 확률로 회피
			if (sum_enemy_troops < sum_own_troops / 5)
			{
				if (pk::rand_bool(60))
					return 0;
			}
			// 적 부대 병력 합이 자세력 부대 병력 수의 50% 미만일 경우 40% 확률로 회피
			else if (sum_enemy_troops < sum_own_troops / 2)
			{
				if (pk::rand_bool(40))
					return 0;
			}

			// 병력 수를 20배한 값이 적 부대 중 최대 병력 수보다 높고, 병력 수를 40배한 값이 적 부대 병력 합보다 높다면 회피
			if (troops * 20 >= max_enemy_troops and troops * 40 >= sum_enemy_troops)
				return 0;

			return 100;
		}

		pk::int_int_int func_59b130(pk::unit@ src)
		{
			// 인접 자세력 부대 병력 합
			int sum_own_troops = 0;
			// 3칸 이하 적 부대 병력 합
			int sum_enemy_troops = 0;
			// 3칸 이하 적 부대 중 최대 병력
			int max_enemy_troops = 0;

			array<pk::point> arr = pk::range(src.get_pos(), 1, 3);
			for (int i = 0; i < arr.length; i++)
			{
				pk::unit@ dst = pk::get_unit(arr[i]);
				if (dst is null or dst.status != 부대상태_통상)
					continue;

				if (pk::is_enemy(src, dst))
				{
					int troops = dst.troops;
					sum_enemy_troops = sum_enemy_troops + troops;
					max_enemy_troops = pk::max(max_enemy_troops, troops);
				}
				else if (pk::is_own(src, dst))
				{
					if (pk::is_neighbor_pos(src.get_pos(), dst.get_pos()))
						sum_own_troops = sum_own_troops + dst.troops;
				}
			}

			return pk::int_int_int(sum_own_troops, sum_enemy_troops, max_enemy_troops);
		}
	}

	Main main;
}