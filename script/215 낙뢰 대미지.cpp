namespace 낙뢰_대미지
{
	class Main
	{
		Main()
		{
			pk::set_func(215, pk::func215_t(callback));
		}

		void callback(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ target, bool critical)
		{
			if (target.is_instance(pk::unit::type_id))
			{
				info.troops_damage = 1500 + pk::rand(1000);
			}
			else
			{
				info.hp_damage = 700 + pk::rand(300);
				// 회복중인 제방일 경우 무효
				pk::building@ building = pk::hex_object_to_building(target);
				if (building.facility == 시설_제방 and !building.completed)
					info.hp_damage = 0;
			}

			info.tactics = -1;
			info.src_pos = attacker.get_pos();
			info.dst_pos = target.get_pos();
		}
	}

	Main main;
}