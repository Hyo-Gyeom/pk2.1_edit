namespace 금_수송_제한
{
	class Main
	{
		Main()
		{
			pk::set_func(106, pk::func106_t(callback));
		}

		uint callback(pk::building@ base)
		{
			return pk::min(pk::get_gold(base), 100000);
		}
	}

	Main main;
}