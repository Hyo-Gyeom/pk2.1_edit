namespace 역병_치안_증감
{
	class Main
	{
		Main()
		{
			pk::set_func(160, pk::func160_t(callback));
		}

		int callback(pk::building@ building)
		{
			return -(2 + pk::rand(3));
		}
	}

	Main main;
}