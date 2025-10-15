namespace 충돌_대미지
{
	class Main
	{
		Main()
		{
			pk::set_func(212, pk::func212_t(callback));
		}

		void callback(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ sub_target)
		{
			int n = 0;

			// ?
			n = pk::rand(attacker.attr.stat[부대능력_공격]) + attacker.attr.stat[부대능력_공격] / 2;
			info._20 = -pk::max(n, 1);

			if (sub_target.is_instance(pk::unit::type_id))
			{
				n = attacker.attr.stat[부대능력_공격] * 2 - pk::hex_object_to_unit(sub_target).attr.stat[부대능력_방어];
				if (n < 20) n = 20;
				info.troops_damage = pk::max(n + pk::rand(n), 1);
			}
			else
			{
				pk::building@ building = pk::hex_object_to_building(sub_target);
				if (!pk::is_trap_type(building))
				{
					n = attacker.attr.stat[부대능력_공격] + pk::rand(attacker.attr.stat[부대능력_공격] / 2);
					n = n / 2;
					info.hp_damage = pk::max(n, 1);
				}
				else if (building.facility == 시설_제방 and !building.completed)
				{
					info.hp_damage = 0;
				}
			}

			info.tactics = -1;
			info.src_pos = attacker.get_pos();
			info.dst_pos = sub_target.get_pos();
		}
	}

	Main main;
}