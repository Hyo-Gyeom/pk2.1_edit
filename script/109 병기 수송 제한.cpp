namespace 병기_수송_제한
{
	class Main
	{
		Main()
		{
			pk::set_func(109, pk::func109_t(callback));
		}

		uint callback(pk::building@ base, int weapon_id)
		{
			if (weapon_id < 병기_충차)
				return pk::min(pk::get_weapon_amount(base, weapon_id), 100000);
			return pk::min(pk::get_weapon_amount(base, weapon_id), 100);
		}
	}

	Main main;
}