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
			bool has_myungseong = false;
			for (int i = 0; i < actors.length; i++)
			{
				pk::person@ actor = actors[i];
				if (pk::is_alive(actor))
					n = n + actor.stat[int(pk::core["징병.능력"])];
				
				if (pk::has_skill(actor, 특기_명성))		
					has_myungseong = true;
			}

			// 법령정비 기교가 치안 저하를 줄임
			if (pk::has_tech(city, 기교_법령정비))
				n *= 1.5f;

			// 명성이 치안 저하를 줄임
			if (has_myungseong)
				n *= 1.5f;

			return -troops / n;
		}
	}

	Main main;
}
