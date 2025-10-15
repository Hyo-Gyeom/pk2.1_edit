namespace 부대_병량_소비
{
	class Main
	{
		Main()
		{
			pk::set_func(165, pk::func165_t(callback));
		}

		int callback(pk::unit@ unit)
		{
			if (!pk::is_alive(unit))
				return 0;

			int n = 0;
			float m = 1.f;

			if (unit.type == 부대종류_전투)
			{
				int facility_id = func_49d8e0(unit);
				if (facility_id == 시설_성채)
					m = 1.f;
				else if (facility_id == 시설_요새)
					m = 4.f / 3; // 1.333...
				else if (facility_id == 시설_진)
					m = 5.f / 3; // 1.666...
				else
					m = 2.f;
			}

			if (unit.is_on_fire())
				n = n + (6.f - unit.attr.stat[부대능력_정치] / 20.f) * unit.food / 100;

			n = n + unit.troops * m / 20;

			if (n <= 0 and unit.troops > 0)
				return 1;
			return n;
		}

		/***/
		int func_49d8e0(pk::unit@ unit)
		{
			int force_id = unit.get_force_id();
			pk::force@ force = pk::get_force(force_id);

			if (!pk::is_alive(force))
				return -1;

			int facility_id = 시설_진;

			if (pk::has_tech(force, 기교_성벽강화))
				facility_id = 시설_성채;
			else if (pk::has_tech(force, 기교_시설강화))
				facility_id = 시설_요새;

			array<pk::point> arr = pk::range(unit.get_pos(), 1, pk::get_facility(facility_id).max_range);

			for (int i = 0; i < arr.length; i++)
			{
				pk::building@ building = pk::get_building(arr[i]);
				if (pk::is_alive(building) and building.facility == facility_id and building.completed and building.get_force_id() == force_id)
					return facility_id;
			}

			return -1;
		}
	}

	Main main;
}