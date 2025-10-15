namespace 나선첨_혼란_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(221, pk::func221_t(callback));
		}

		int callback(pk::unit@ attacker, pk::unit@ target, bool critical)
		{
			if (critical or pk::rand_bool(30))
				return pk::rand(2) + (critical ? 1 : 0);
			return 0;
		}
	}

	Main main;
}