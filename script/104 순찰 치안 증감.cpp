namespace 순찰_치안_증감
{
	class Main
	{
		Main()
		{
			pk::set_func(104, pk::func104_t(callback));
		}

		int callback(pk::building@ building, const pk::detail::arrayptr<pk::person@> &in actors)
		{
			if (pk::is_alive(building) and actors[0] !is null)
			{
				pk::city@ city = pk::building_to_city(building);
				if (pk::is_alive(city))
				{
					int n = 0;
					for (int i = 0; i < actors.length; i++)
					{
						pk::person@ actor = actors[i];
						if (pk::is_alive(actor))
							n = n + actor.stat[int(pk::core["순찰.능력"])];
					}
					n = n / 28 + 2;
					if (pk::enemies_around(building))
						n = n / 2;
					return n;
				}
			}
			return 0;
		}
	}

	Main main;
}