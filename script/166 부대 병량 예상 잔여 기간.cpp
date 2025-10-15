namespace 부대_병량_예상_잔여_기간
{
	class Main
	{
		Main()
		{
			pk::set_func(166, pk::func166_t(callback));
		}

		int callback(int food, int troops)
		{
			return food * 100 / troops;
		}
	}

	Main main;
}