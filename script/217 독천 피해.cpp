namespace 독천_피해
{
	class Main
	{
		Main()
		{
			pk::set_func(217, pk::func217_t(callback));
		}

		int callback(pk::unit@ unit, const pk::point &in pos)
		{
			if (unit.has_skill(특기_해독))
				return 0;
			return 200 + pk::rand(200);
		}
	}

	Main main;
}