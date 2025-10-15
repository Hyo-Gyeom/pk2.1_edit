namespace 불_피해
{
	class Main
	{
		Main()
		{
			pk::set_func(216, pk::func216_t(callback));
		}

		int callback(pk::unit@ unit, const pk::point &in pos)
		{
			if (unit.has_skill(특기_화신) or unit.has_skill(특기_화공))
				return 0;
			int n = 50 + pk::rand(50);
			if (unit.has_skill(특기_등갑))
				n = n * 2;
			return n;
		}
	}

	Main main;
}