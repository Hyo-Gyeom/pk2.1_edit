namespace 관문_병량_수입
{
	class Main
	{
		Main()
		{
			pk::set_func(153, pk::func153_t(callback));
		}

		int callback(pk::building@ building, int city_harvest)
		{
			return city_harvest * 0.2f;
		}
	}

	Main main;
}