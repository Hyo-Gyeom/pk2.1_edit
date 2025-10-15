namespace 벼랑길_피해
{
	class Main
	{
		Main()
		{
			pk::set_func(218, pk::func218_t(callback));
		}

		int callback(pk::unit@ unit, const pk::point &in pos)
		{
			if (unit.has_skill(특기_답파) or unit.has_tech(기교_난소행군))
				return 0;
			return 100 + pk::rand(200);
		}
	}

	Main main;
}