namespace 기병_전법_사망_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(203, pk::func203_t(callback));
		}

		int callback(pk::person@ attacker, pk::person@ target, int tactics_id, bool critical)
		{
			int n = 0;

			// 돌파 전법은 사용되지 않습니다.
			if (tactics_id == 전법_돌파)
				n = n + 1;
			else if (tactics_id == 전법_돌진)
				n = n + 2;

			if (pk::get_scenario().battle_death == 전사_많음)
				n = n + 2;

			if (critical)
				n = n + 2;

			if (target.character == 성격_소심)
				n = n - 1;
			else if (target.character == 성격_저돌)
				n = n + 1;

			int diff = pk::max(attacker.stat[무장능력_통솔], attacker.stat[무장능력_무력]) - pk::max(target.stat[무장능력_통솔], target.stat[무장능력_무력]);

			// 목표 무장보다 능력치 낮음
			if (diff <= 0)
				n = n - 3;
			// 목표 무장보다 능력치 높음
			else if (diff <= 6)
				n = n - 1;
			else if (diff <= 12)
				n = n + 0;
			else
				n = n + 1;

			return n;
		}
	}

	Main main;
}