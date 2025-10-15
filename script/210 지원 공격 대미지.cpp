namespace 지원_공격_대미지
{
	class Main
	{
		Main()
		{
			pk::set_func(210, pk::func210_t(callback));
		}

		void callback(pk::damage_info& info, pk::unit@ attacker, const pk::point &in target_pos)
		{
			int force_id = attacker.get_force_id();
			pk::force@ force = pk::get_force(force_id);
			int weapon_id = attacker.weapon;
			int command = func_5aee10(attacker);
			pk::unit@ target_unit = pk::get_unit(target_pos);
			pk::building@ target_building = pk::get_building(target_pos);
			int atk = attacker.attr.stat[부대능력_공격];
			int troops_atk = 0;
			int buffed = 0;

			if (pk::is_neighbor_pos(attacker.get_pos(), target_pos))
				troops_atk = 10;

			// 공격력 상승 버프
			if (func_5aed80(attacker.get_pos(), force_id))
			{
				buffed = 1;
				info.buffed = true;
			}

			// 디버프
			if (target_unit !is null)
			{
				pk::force@ target_force = pk::get_force(target_unit.get_force_id());
				int facility_id = 시설_진;
				if (pk::has_tech(target_force, 기교_시설강화))
					facility_id = 시설_요새;
				if (pk::has_tech(target_force, 기교_성벽강화))
					facility_id = 시설_성채;
				if (func_5aedc0(target_unit.get_pos(), 1, pk::get_facility(facility_id).range, target_unit.get_force_id()))
					info.debuffer = facility_id;
			}

			float 정예기교_공격력 = 1.15f;
			float 단련기교_공격력 = 1.10f;

			// 크리티컬
			info.critical = false;

			if (target_unit !is null)
			{
				float troops_damage = func_5aee60(atk, command, troops_atk, buffed, target_unit.attr.stat[부대능력_방어], func_5aee10(target_unit));

				switch (weapon_id)
				{
				case 병기_창:
					if (pk::has_tech(force, 기교_정예창병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_창병단련))
						troops_damage *= 단련기교_공격력;
					break;

				case 병기_극:
					if (pk::has_tech(force, 기교_정예극병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_극병단련))
						troops_damage *= 단련기교_공격력;
					break;

				case 병기_노:
					if (pk::has_tech(force, 기교_정예노병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_노병단련))
						troops_damage *= 단련기교_공격력;
					break;

				case 병기_군마:
					if (pk::has_tech(force, 기교_정예기병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_기병단련))
						troops_damage *= 단련기교_공격력;
					break;
				}

				info.troops_damage = troops_damage;
				if (func_5af230(info, attacker, target_unit, -1))
					troops_damage = 0;

				if (attacker.has_skill(특기_소탕))
					info.energy_damage = pk::max(info.energy_damage, int(pk::core::skill_constant(attacker, 특기_소탕))); // 5

				if (attacker.has_skill(특기_위풍))
					info.energy_damage = pk::max(info.energy_damage, int(pk::core::skill_constant(attacker, 특기_위풍))); // 20

				info.food_damage = func_5aecc0(attacker, target_unit);
				info.food_heal = info.food_damage;

				if (attacker.is_player())
					troops_damage *= float(pk::core["대미지패널티"][pk::get_scenario().difficulty]);

				if (target_unit.has_skill(특기_등갑))
					troops_damage = troops_damage / 2;

				if (info.debuffer == 시설_진)
					troops_damage *= 0.85f;
				else if (info.debuffer == 시설_요새)
					troops_damage *= 0.75f;
				else if (info.debuffer == 시설_성채)
					troops_damage *= 0.65f;

				info.troops_damage = troops_damage;
			}
			else if (target_building !is null and pk::is_general_type(target_building))
			{
				pk::assert(false);
			}

			if (target_building !is null)
			{
				pk::assert(false);
			}

			// 최소 병력 대미지
			if (info.troops_damage != 0)
				info.troops_damage = pk::max(info.troops_damage / 2, pk::rand(100) + 200);

			// 최소 내구 대미지
			if (info.hp_damage != 0)
				info.hp_damage = pk::max(info.hp_damage / 2, pk::rand(10) + 20);

			info.src_pos = attacker.get_pos();
			info.dst_pos = target_pos;
			info.type = 3; // ?
		}

		/**
			획득 병량 계산
		*/
		int func_5aecc0(pk::unit@ attacker, pk::unit@ target)
		{
			if (pk::is_alive(attacker) and pk::is_alive(target) and attacker.weapon == 병기_창 and attacker.has_tech(기교_병량습격))
			{
				int a = pk::rand(attacker.attr.stat[부대능력_공격]) + attacker.attr.stat[부대능력_공격];
				int b = pk::max(attacker.troops / 2, 1);
				int c = target.food;
				int d = pk::get_max_food(attacker) - attacker.food;
				return pk::min(a, b, c, d);
			}
			return 0;
		}

		/**
			공격력 상승 효과를 가진 시설 검색
		*/
		bool func_5aed80(const pk::point &in pos, int force_id)
		{
			/*
			array<pk::point> range = pk::range(pos, 1, 3);
			for (int i = 0; i < range.length; i++)
			{
				pk::building@ building = pk::get_building(range[i]);
				if (pk::is_alive(building) and building.facility == 시설_태고대 and building.completed and building.get_force_id() == force_id)
					return true;
			}
			return false;
			*/

			array<pk::point> range = pk::range(pos, 1, int(pk::core["시설.최대범위"]));
			for (int i = 0; i < range.length; i++)
			{
				pk::building@ building = pk::get_building(range[i]);
				if (pk::is_alive(building) and building.completed and building.get_force_id() == force_id)
				{
					if (bool(pk::core["시설"][building.facility]["공격력상승"]) and pk::get_distance(pos, range[i]) <= pk::get_facility(building.facility).max_range)
						return true;
				}
			}
			return false;
		}

		/**
			진, 요새, 성채 검색
		*/
		bool func_5aedc0(pk::point pos, int min, int max, int force_id)
		{
			auto range = pk::range(pos, min, max);
			for (int i = 0; i < range.length; i++)
			{
				pk::building@ building = pk::get_building(range[i]);
				if (pk::is_alive(building) and (building.facility == 시설_진 or building.facility == 시설_요새 or building.facility == 시설_성채) and building.completed and building.get_force_id() == force_id)
					return true;
			}
			return false;
		}

		/**
			지휘 가능한 병력 수
		*/
		int func_5aee10(pk::unit@ unit)
		{
			pk::person@ leader = pk::get_person(unit.leader);
			if (pk::is_alive(leader))
				return pk::min(pk::get_command(leader), unit.troops);
			return unit.troops;
		}

		/***/
		float func_5aee60(int src_atk, int src_troops, int tactics_atk, int buffed, int dst_def, int dst_troops)
		{
			float n = 0;
			int a = 0;
			int b = 0;
			int c = 0;

			a = pk::max(src_atk, 40);
			a = a * a;

			b = pk::max(dst_def, 40);
			b = b * b;

			n = sqrt(tactics_atk * 64);
			n = n + pk::max((src_troops - dst_troops) / 2000, 0);
			n = n + pk::max((a - b) / 300, 0);
			n = n + 50;
			n = n * (buffed + 10);

			a = (src_troops / 100 + 300) * (src_atk + 50) * (src_atk + 50) / 100;
			b = (dst_troops / 100 + 300) * (dst_def + 50) * (dst_def + 50) / 100;
			c = a + b;
			if (a >= b)
				c = a * 100 / c;
			else
				c = pk::max(100 - (b * 100 / c), 1);
			n = n * c;

			a = pk::max(src_troops / 4, 1);
			n = n * pk::min(sqrt(a), 40.f);

			n = n / 2100;
			n = n + src_troops / 200;

			return n;
		}

		/***/
		bool func_5af230(pk::damage_info& info, pk::unit@ attacker, pk::unit@ target, int tactics_id)
		{
			if (pk::is_valid_tactics_id(tactics_id))
				return false;

			pk::point attacker_pos = attacker.get_pos();
			pk::point target_pos = target.get_pos();

			if (target.weapon == 병기_극)
			{
				if (target.has_tech(기교_화살방패))
				{
					if (!pk::is_neighbor_pos(attacker_pos, target_pos) and pk::rand_bool(30))
					{
						info.def_tech = 기교_화살방패;
						return true;
					}
				}
				if (target.has_tech(기교_큰방패))
				{
					if (pk::is_neighbor_pos(attacker_pos, target_pos) and pk::rand_bool(30))
					{
						info.def_tech = 기교_큰방패;
						return true;
					}
				}
			}

			if (target.has_skill(특기_불굴))
			{
				if (target.troops < int(pk::core::skill_constant(target, 특기_불굴)) and pk::rand_bool(int(pk::core::skill_constant(target, 특기_불굴, 1))))
				{
					info.def_skill = 특기_불굴;
					return true;
				}
			}

			if (target.has_skill(특기_금강))
			{
				if (info.troops_damage < int(pk::core::skill_constant(target, 특기_금강)) and pk::rand_bool(int(pk::core::skill_constant(target, 특기_금강, 1))))
				{
					info.def_skill = 특기_금강;
					return true;
				}
			}

			return false;
		}
	}

	Main main;
}