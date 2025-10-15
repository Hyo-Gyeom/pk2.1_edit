namespace 관문_금_수입
{
	class Main
	{
		Main()
		{
			pk::set_func(152, pk::func152_t(callback));
		}

		int callback(pk::building@ building, int city_revenue)
		{
			return city_revenue * 0.2f;
		}
	}

	Main main;
}