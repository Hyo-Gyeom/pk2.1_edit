namespace 턴별_병력_감소
{
	class Main
	{
		Main()
		{
			pk::set_func(156, pk::func156_t(callback));
		}

		int callback(pk::building@ building)
		{
			int n = pk::max(building.get_troops(), 100);
			n = pk::rand(50) - sqrt(n / 100) * -30;
			return -n;
		}
	}

	Main main;
}