namespace 포로_탈출_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(157, pk::func157_t(callback));
		}

		int callback(pk::person@ prisoner)
		{
			// 부대에 잡혀있는 포로는 탈출하지 않음
			pk::building@ building = pk::hex_object_to_building(pk::get_hex_object(prisoner.location));
			if (!pk::is_alive(building))
				return 0;

			int n = prisoner.prisoner_timer;

			// 포로가 된지 2턴 미만인 경우 탈출하지 않음
			if (n < 2)
				return 0;

			n = n - 2;
			if (n < 1)
				n = 1;
			n = n * n * pk::max(prisoner.stat[무장능력_지력], prisoner.stat[무장능력_무력], 30) / 166;

			return pk::max(n, 1);
		}
	}

	Main main;
}