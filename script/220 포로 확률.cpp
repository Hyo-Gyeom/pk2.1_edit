namespace 포로_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(220, pk::func220_t(callback));
		}

		void callback(const pk::destroy_info &in info, pk::list<pk::person@>& captured, pk::list<pk::person@>& escaped, bool tactics_bonus)
		{
			pk::hex_object@ attacker = info.attacker;
			pk::hex_object@ target = info.target;

			int attacker_force_id = attacker.get_force_id();
			int target_force_id = target.get_force_id();
			pk::building@ attacker_building = attacker.get_type_id() == pk::building::type_id ? attacker : null;
			pk::unit@ attacker_unit = attacker.get_type_id() == pk::unit::type_id ? attacker : null;
			pk::building@ target_building = target.get_type_id() == pk::building::type_id ? target : null;
			pk::unit@ target_unit = target.get_type_id() == pk::unit::type_id ? target : null;
			int target_border_radius = pk::is_alive(target_building) and target_building.facility == 시설_도시 ? 2 : 1;
			pk::point attacker_pos = attacker.get_pos();
			pk::point target_pos = target.get_pos();
			bool in_border = pk::is_in_range(target_pos, target_border_radius, attacker_pos);
			bool cant_capture = !pk::is_valid_normal_force_id(attacker_force_id) or !pk::is_valid_normal_force_id(target_force_id);
			bool penalty = pk::get_scenario().difficulty == 난이도_특급 and attacker.is_player() and not target.is_player();
			bool has_hobaku_skill = false;
			int hobaku_chance = 0;
			pk::list<pk::person@> prisoner_list;

			// 공격 오브젝트의 포박 특기 보유 확인
			if (pk::is_alive(attacker_unit))
			{
				has_hobaku_skill = in_border and attacker_unit.has_skill(특기_포박);
				hobaku_chance = int(pk::core::skill_constant(attacker_unit, 특기_포박));
			}
			else if (pk::is_alive(attacker_building))
			{
				int building_id = attacker_building.get_id();
				int hex_object_id = pk::building_id_to_hex_object_id(building_id);
				pk::list<pk::person@> list = pk::get_person_list(hex_object_id, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반));
				pk::filter_list(list, 무장속성_세력, attacker_force_id, COMPARE_EQ);

				if (list.size == 0)
					cant_capture = true;

				if (in_border)
				{
					for (int i = 0; i < list.size; i++)
					{
						if (pk::has_skill(list[i], 특기_포박))
						{
							has_hobaku_skill = true;
							hobaku_chance = pk::max(hobaku_chance, int(pk::core::skill_constant(list[i].get_id(), 특기_포박)));
						}
					}
				}
			}

			// 목표 오브젝트 무장 확인
			if (pk::is_alive(target_unit))
			{
				for (int i = 0; i < 3; i++)
				{
					pk::person@ member = pk::get_person(target_unit.member[i]);
					if (pk::is_alive(member))
						prisoner_list.push_back(member);
				}
			}
			else if (pk::is_alive(target_building))
			{
				int building_id = target_building.get_id();
				int hex_object_id = pk::building_id_to_hex_object_id(building_id);
				prisoner_list = pk::get_person_list(hex_object_id, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반));
				pk::filter_list(prisoner_list, 무장속성_세력, target_force_id, COMPARE_EQ);
			}

			int bordered_unit_count = 0;
			int terrain_id = -1;

			if (pk::is_valid_pos(target_pos))
			{
				array<pk::point>@ range = pk::range(target_pos, 0, target_border_radius);
				for (int i = 0; i < range.length; i++)
				{
					pk::unit@ unit = pk::get_unit(range[i]);
					if (pk::is_alive(unit) and unit.get_force_id() == attacker_force_id)
						bordered_unit_count++;
				}
				terrain_id = pk::get_hex(target_pos).terrain;
			}

			for (int i = 0; i < prisoner_list.size; i++)
			{
				pk::person@ person = prisoner_list[i];

				if (cant_capture)
				{
					escaped.push_back(person);
					continue;
				}

				if (not info.releasable)
				{
					captured.push_back(person);
					continue;
				}

				if (pk::has_item(person, 보물종류_명마) or pk::has_skill(person, 특기_강운))
				{
					escaped.push_back(person);
					continue;
				}

				int n = 0;
				n += int(pk::core["무장.최대능력치"]) + 20 - pk::max(person.stat[무장능력_무력], person.stat[무장능력_지력]);
				n /= 3;
				if (bordered_unit_count > 0)
					n *= 100 * (pk::has_skill(person, 특기_철벽) ? 1 : bordered_unit_count);
				if (terrain_id == 지형_습지 or terrain_id == 지형_독천)
					n *= 1.5f;
				n /= 100;
				n /= penalty ? 2 : 1;
				n += has_hobaku_skill ? hobaku_chance : 0;
				if (tactics_bonus)
					n += 30;

				if (pk::rand_bool(n))
					captured.push_back(person);
				else
					escaped.push_back(person);
			}
		}
	}

	Main main;
}