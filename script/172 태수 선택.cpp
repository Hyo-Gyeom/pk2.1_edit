namespace 태수_선택
{
	class Main
	{
		Main()
		{
			pk::set_func(172, pk::func172_t(callback));
		}

		void callback(pk::list<pk::person@>& list)
		{
			list.sort(function(a, b) {
				if ((a.mibun <= 신분_도독 or b.mibun <= 신분_도독) and a.mibun != b.mibun)
					return a.mibun < b.mibun;

				if (pk::get_command(a) != pk::get_command(b))
					return pk::get_command(a) > pk::get_command(b);

				if (a.stat[무장능력_통솔] != b.stat[무장능력_통솔])
					return a.stat[무장능력_통솔] > b.stat[무장능력_통솔];

				if (a.stat[무장능력_무력] != b.stat[무장능력_무력])
					return a.stat[무장능력_무력] > b.stat[무장능력_무력];

				if (a.kouseki != b.kouseki)
					return a.kouseki > b.kouseki;

				return a.get_id() < b.get_id();
			});
		}
	}

	Main main;
}