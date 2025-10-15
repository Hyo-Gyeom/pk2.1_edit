namespace AI_출진_가능한_병력_수
{
	class Main
	{
		Main()
		{
			pk::set_func(254, pk::func254_t(callback));
		}

		int callback(pk::building@ building, int troops)
		{
			pk::person@ kunshu = pk::get_person(pk::get_kunshu_id(building));
			int character = building.is_player() ? 성격_냉정 : kunshu.character;
			return troops * (character + 3);
		}
	}

	Main main;
}