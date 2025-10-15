namespace 병량_수송_제한
{
	class Main
	{
		Main()
		{
			pk::set_func(107, pk::func107_t(callback));
		}

		uint callback(pk::building@ base)
		{
			return pk::min(pk::get_food(base), 500000);
		}
	}

	Main main;
}