namespace 수상부대휩쓸림
{
	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			auto list = pk::list_to_array(pk::get_unit_list());
			for (int i = 0; i < list.length; i++)
			{
				pk::unit@ unit = list[i];
				if (!pk::is_in_water(unit) or pk::get_tekisei(unit) >= 적성_S)
					continue;
				int n = pk::rand(적성_S - pk::get_tekisei(unit));
				if (n == 0)
					continue;
				array<pk::point> steps;
				pk::point pos = unit.get_pos();
				for (int j = 0; j < n; j++)
				{
					pk::hex@ hex = pk::get_hex(pos);
					int direction = hex.direction;
					if (!pk::is_valid_hex_direction(direction))
						break;
					pos = pk::get_neighbor_pos(pos, direction);
					@hex = pk::get_hex(pos);
					if (!pk::is_valid_pos(pos) || hex.has_unit || hex.has_building || !pk::is_water_terrain(hex.terrain) || !cast<pk::func169_t@>(pk::get_func(169))(unit, hex.terrain))
						break;
					steps.insertLast(pos);
				}
				if (steps.length == 0)
					continue;
				switch (pk::rand(2))
				{
				case 0: pk::say(pk::u8encode("당황하지마라!"), pk::get_person(unit.leader)); break;
				case 1: pk::say(pk::u8encode("키가 생각대로 움직이지 않는다!"), pk::get_person(unit.leader)); break;
				}
				pk::move(unit, steps);
			}
		}
	}

	Main main;
}