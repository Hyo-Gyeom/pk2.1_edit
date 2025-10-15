namespace 월별_치안_감소
{
	class Main
	{
		Main()
		{
			pk::set_func(154, pk::func154_t(callback));
		}

		int callback(pk::city@ city)
		{
			if (!pk::is_first_month_of_quarter(pk::get_month()))
				return 0;
			pk::force@ force = pk::get_force(city.get_force_id());
			if (force is null)
				return 0;
			// 법령정비 연구 시 50% 확률로 감소하지 않음
			if (pk::has_tech(force, 기교_법령정비) and pk::rand_bool(50))
				return 0;
			int n = 90;
			pk::person@ taishu = pk::get_person(city.get_taishu_id());
			if (pk::is_alive(taishu))
				n = n - taishu.stat[무장능력_매력];
			n = pk::max(n, 1) / 10 + pk::rand(3);
			n = pk::min(n, 5);
			return -n;
		}
	}

	Main main;
}