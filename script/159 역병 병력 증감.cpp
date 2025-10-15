namespace 역병_병력_증감
{
	class Main
	{
		Main()
		{
			pk::set_func(159, pk::func159_t(callback));
		}

		int callback(pk::building@ building)
		{
			return -(pk::get_troops(building) * (pk::rand(6) + 5) / 100);
		}
	}

	Main main;
}