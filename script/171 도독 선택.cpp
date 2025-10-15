namespace 도독_선택
{
	class Main
	{
		Main()
		{
			pk::set_func(171, pk::func171_t(callback));
		}

		void callback(pk::list<pk::person@>& list)
		{
			list.sort(function(a, b) {
				if ((a.mibun == 신분_군주 and b.mibun != 신분_군주) or (a.mibun != 신분_군주 and b.mibun == 신분_군주))
					return a.mibun == 신분_군주;

				if (pk::get_command(a) != pk::get_command(b))
					return pk::get_command(a) > pk::get_command(b);

				if (a.rank != b.rank)
					return a.rank < b.rank;

				if (a.stat[무장능력_통솔] != b.stat[무장능력_통솔])
					return a.stat[무장능력_통솔] > b.stat[무장능력_통솔];

				return a.p < b.p;
			});
		}
	}

	Main main;
}