namespace 거병_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(173, pk::func173_t(callback));
		}

		bool callback()
		{
			if (pk::get_elapsed_years() < 1)
				return false;
			if (pk::is_first_month_of_quarter())
				return false;
			int n = pk::get_elapsed_years() / 3 + 3;
			n = pk::min(n, 8);
			return pk::rand_bool(n);
		}
	}

	Main main;
}