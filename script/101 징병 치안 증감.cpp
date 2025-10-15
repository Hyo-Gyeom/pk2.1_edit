namespace 징병_치안_증감
{
	class Main
	{
		Main()
		{
			pk::set_func(101, pk::func101_t(callback));
		}

		int callback(pk::city@ city, const pk::detail::arrayptr<pk::person@> &in actors, int troops)
		{
			int n = 100;
			for (int i = 0; i < actors.length; i++)
			{
				pk::person@ actor = actors[i];
				if (pk::is_alive(actor))
					n = n + actor.stat[int(pk::core["징병.능력"])];
			}
			return -troops / n;
		}
	}

	Main main;
}