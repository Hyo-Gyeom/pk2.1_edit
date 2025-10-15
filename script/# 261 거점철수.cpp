namespace 거점철수
{
	class Main
	{
		pk::func261_t@ prev_callback_;

		Main()
		{
			@prev_callback_ = cast<pk::func261_t@>(pk::get_func(261));
			pk::reset_func(261);
			pk::set_func(261, pk::func261_t(callback));
		}

		bool callback(pk::force@ force)
		{
			int force_id = force.get_id();

			// 플레이어, 이민족 세력은 제외.
			if (!force.is_player() and pk::is_normal_force(force_id))
			{
				for (int i = 0; i < 건물_거점끝; i++)
				{
					auto base = pk::get_building(i);
					if (base.get_force_id() == force_id and need_retreat(base))
						retreat(base);
				}
			}

			// 이전 핸들러가 있다면 체인.
			if (prev_callback_ !is null)
				return prev_callback_(force);
			return false;
		}

		/** 철수가 필요한 거점인지 판단(조건문이 매우 허술한 상태입니다. 입맛에 맞게 수정해서 사용해 보세요.) */
		bool need_retreat(pk::building@ base)
		{
			// 근접한 적 부대 수
			int enemy_units1 = 0;
			// 3칸 이내 적 부대 수
			int enemy_units3 = 0;
			// 3칸 이내 적 병력 수
			int enemy_troops3 = 0;

			auto range = pk::range(base.get_pos(), 1, 4 + (base.facility == 시설_도시 ? 1 : 0));
			for (int i = 0; i < range.length; i++)
			{
				auto unit = pk::get_unit(range[i]);
				if (pk::is_alive(unit))
				{
					int distance = pk::get_distance(base.get_pos(), range[i]);
					if (pk::is_enemy(base, unit))
					{
						if (distance <= 1)
						{
							enemy_units1++;
						}
						if (distance <= 3)
						{
							enemy_units3++;
							enemy_troops3 += unit.troops;
						}
					}
					else
					{
					}
				}
			}

			if (enemy_units1 == 6)
				return false;
			int base_troops = pk::get_troops(base);
			int base_hp = base.hp;
			// 잔여 내구가 아슬아슬한 경우
			if (base_troops < enemy_troops3 * 0.8f and base_hp < pk::get_max_hp(base) * 0.5f and base_hp < enemy_troops3 * 0.04f)
				return true;
			// 잔여 병력이 아슬아슬한 경우
			if (base_troops < enemy_troops3 * 0.8f and base_troops < enemy_troops3 * 0.06f)
				return true;
			return false;
		}

		/** 철수 명령 */
		bool retreat(pk::building@ base)
		{
			auto person_list = pk::get_idle_person_list(base);

			// 명령 가능한 무장이 있는지 확인.
			if (person_list.count == 0)
				return false;

			int target = get_nearest_kyoten(base);

			// 철수할 인접 거접이 있는지 확인.
			if (target == -1)
				return false;

			// 통솔이 높은 순으로 정렬.
			person_list.sort(function(a, b)
			{
				return a.stat[무장능력_통솔] > b.stat[무장능력_통솔];
			});

			// 출진 명령 정보 생성.
			pk::com_deploy_cmd_info cmd;
			@cmd.base = @base;
			cmd.type = 부대종류_수송;
			cmd.member[0] = person_list[0].get_id();
			cmd.gold = pk::get_gold(base) * 0.9f;
			cmd.food = pk::get_food(base) * 0.9f;
			cmd.troops = pk::get_troops(base) * 0.9f;
			int i = 0;
			for (int weapon_id = 0; weapon_id < 병기_끝; weapon_id++)
			{
				int weapon_amount = pk::get_weapon_amount(base, weapon_id) * 0.9f;
				if (weapon_amount > 0)
				{
					cmd.weapon_id[i] = weapon_id;
					cmd.weapon_amount[i] = weapon_amount;
					i++;
				}
			}
			cmd.order = 부대임무_이동;
			cmd.target_pos = pk::get_building(target).get_pos();

			// 출진.
			int unit_id = pk::command(cmd);
			if (unit_id != -1)
				pk::set_action_done(pk::get_unit(unit_id));

			return true;
		}

		/** 가장 가까운 아군 거점을 얻음. */
		int get_nearest_kyoten(pk::building@ src)
		{
			int best_dst = -1;
			int best_distance = 0;
			// 도시만 확인.
			for (int i = 0; i < 건물_도시끝; i++)
			{
				auto dst = pk::get_building(i);
				if (src.get_id() == i or src.get_force_id() != dst.get_force_id())
					continue;
				int distance = pk::get_building_distance(src.get_id(), i, src.get_force_id());
				if (distance < best_distance or best_dst == -1)
				{
					best_dst = i;
					best_distance = distance;
				}
			}
			return best_dst;
		}
	};

	Main main;
}