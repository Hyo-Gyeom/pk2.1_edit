namespace 지형_이동_가능_설정
{
	class Main
	{
		Main()
		{
			pk::set_func(169, pk::func169_t(callback));
		}

		bool callback(pk::unit@ unit, int terrain_id)
		{
			if (bool(pk::core["지형"][terrain_id]["이동가능"]) == false)
				return false;
			int weapon_id = pk::is_water_terrain(terrain_id) ? pk::get_sea_weapon_id(unit) : pk::get_ground_weapon_id(unit);
			pk::equipment@ equipment = pk::get_equipment(weapon_id);
			if (equipment !is null and equipment.movement_cost[terrain_id] == 255)
				return false;
			if (bool(pk::core["지형"][terrain_id]["난소행군필요"]) and !unit.has_tech(기교_난소행군))
				return false;
			return true;
		}
	}

	Main main;
}