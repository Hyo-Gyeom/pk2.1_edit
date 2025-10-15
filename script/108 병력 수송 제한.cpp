namespace 병력_수송_제한
{
	class Main
	{
		Main()
		{
			pk::set_func(108, pk::func108_t(callback));
		}

		uint callback(pk::building@ base)
		{
			return pk::min(pk::get_troops(base), 60000);
		}
	}

	Main main;
}