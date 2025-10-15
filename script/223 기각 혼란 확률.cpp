namespace 기각_혼란_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(223, pk::func223_t(callback));
		}

		int callback(pk::unit@ attacker, pk::unit@ target)
		{
			if (pk::rand_bool(int(pk::core::skill_constant(attacker, 특기_기각))))
				return 1 + pk::rand(2);
			return 0;
		}
	}

	Main main;
}