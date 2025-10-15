namespace 역병_상병_변화
{
	class Main
	{
		Main()
		{
			pk::set_func(161, pk::func161_t(callback));
		}

		int callback(pk::building@ building, pk::person@ person)
		{
			if (pk::rand_bool(8))
				return 상병_경증;
			else if (pk::rand_bool(2))
				return 상병_중증;
			else
				return -1;
		}
	}

	Main main;
}