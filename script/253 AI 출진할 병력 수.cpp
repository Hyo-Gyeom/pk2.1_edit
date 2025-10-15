namespace AI_출진할_병력_수
{
	class Main
	{
		Main()
		{
			pk::set_func(253, pk::func253_t(callback));
		}

		int callback(pk::building@ src, int src_rank, pk::building@ dst, int dst_troops)
		{
			int a = pk::get_person(pk::get_kunshu_id(src)).character;
			int b = (3 - a) * 15;
			int c = src_rank / 2;
			int d = dst_troops + (dst_troops * (b + c) / 100);
			return pk::max(d, 5000);
		}
	}

	Main main;
}