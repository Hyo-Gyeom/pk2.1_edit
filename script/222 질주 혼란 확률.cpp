namespace 질주_혼란_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(222, pk::func222_t(callback));
		}

		int callback(pk::unit@ attacker, pk::unit@ target)
		{
			if (attacker.attr.stat[부대능력_공격] > target.attr.stat[부대능력_공격] and pk::rand_bool(int(pk::core::skill_constant(attacker, 특기_질주))))
				return 1 + pk::rand(2);
			return 0;
		}
	}

	Main main;
}