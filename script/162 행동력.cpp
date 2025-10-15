namespace 행동력
{
	class Main
	{
		Main()
		{
			pk::set_func(162, pk::func162_t(callback));
		}

		int callback(pk::district@ district)
		{
			int n = 0;

			pk::person@ totoku = pk::get_person(district.totoku);
			n = n + pk::max(totoku.stat[무장능력_통솔], totoku.stat[무장능력_매력]) / 5;
			if (n < 6)
				n = 6;

			int city_ap = pk::min(district.query_value(군단속성_도시수), 6) * 10;
			n = n + city_ap + 10 + pk::min(city_ap, pk::get_person_list(district, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반)).size);

			pk::person@ gunshi = pk::get_person(pk::get_force(district.get_force_id()).gunshi);
			if (pk::is_alive(gunshi))
				n = (int(gunshi.stat[무장능력_지력] - 60) / 2 + 100) * n / 100;

			n = n + district.query_value(군단속성_건설완료시설수 + 시설_부절태 - 시설_시장) * 5;

			return n;
		}
	}

	Main main;
}