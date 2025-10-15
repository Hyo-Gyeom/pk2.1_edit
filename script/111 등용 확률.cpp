namespace 등용_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(111, pk::func111_t(callback));
		}

		bool callback(pk::person@ target, pk::person@ actor, int type, int seed)
		{
			if (!pk::is_alive(target) or !pk::is_alive(actor)) return false;

			// 실행 무장의 군주가 없다면 항상 실패
			pk::person@ actor_kunshu = pk::get_person(pk::get_kunshu_id(actor));
			if (!pk::is_alive(actor_kunshu)) return false;

			// 특별한 관계가 있는지 확인
			pk::bool_bool tuple = func_4b0040(target, actor, type);
			if (tuple.first) return tuple.second;

			int giri = 10;
			if (type != 0) giri = pk::min(15 - target.giri * 2, 10);
			int n = pk::min(func_5c6030(target, actor, type, seed) * giri / 10, 100);

			// 포로처우인 경우
			if (type != 0) return pk::rand_bool(n);

			return n > pk::rand(100, seed, target.get_id(), actor.get_id(), target.loyalty, actor.stat[무장능력_매력], pk::get_aishou_distance(actor, target.get_id()), 0);
		}

		/**
			목표 무장과 실행 무장 사이에 특별한 관계가 있는지 판단합니다.
			@return 특별한 관계 여부, 등용 성공 여부
		*/
		pk::bool_bool func_4b0040(pk::person@ target, pk::person@ actor, int type)
		{
			int target_force_id = target.get_force_id();
			// 귀환 가능함을 나타냄
			bool is_valid_target_force_id = pk::is_valid_force_id(target_force_id);

			// 목표 무장의 세력이 멸망
			if (type == 2) is_valid_target_force_id = false;

			if (!pk::is_alive(target) or !pk::is_alive(actor)) return pk::bool_bool(true, false);

			int actor_kunshu_id = pk::get_kunshu_id(actor);
			pk::person@ actor_kunshu = pk::get_person(actor_kunshu_id);

			// 실행 무장의 세력에 군주가 없는 경우 항상 실패
			if (!pk::is_alive(actor_kunshu)) return pk::bool_bool(true, false);

			// 목표 무장의 사관 금지 군주가 실행 무장의 군주인 경우 항상 실패
			if (target.banned_kunshu == actor_kunshu_id) return pk::bool_bool(true, false);

			// 목표 무장이 군주인 경우 항상 실패
			if (target.mibun == 신분_군주 and is_valid_target_force_id) return pk::bool_bool(true, false);

			pk::person@ target_gikyoudai = pk::get_person(target.gikyoudai);

			// 목표 무장이 의형제 있음(2인)
			if (pk::is_alive(target_gikyoudai) and @target_gikyoudai != @target)
			{
				// 목표 무장이 의형제와 같은 세력인 경우 항상 실패
				if (is_valid_target_force_id and pk::is_valid_force_id(target_force_id) and target_gikyoudai.get_force_id() == target_force_id) return pk::bool_bool(true, false);
				// 목표 무장이 실행 무장 세력의 군주나 실행 무장과 의형제인 경우 항상 성공
				if (@target_gikyoudai == @actor_kunshu or @target_gikyoudai == @actor) return pk::bool_bool(true, true);
			}

			pk::person@ target_spouse = pk::get_person(target.spouse);

			// 목표 무장이 배우자와 같은 세력인 경우 항상 실패
			if (pk::is_alive(target_spouse))
			{
				if (is_valid_target_force_id and pk::is_valid_force_id(target_force_id) and target_spouse.get_force_id() == target_force_id)
					return pk::bool_bool(true, false);
			}

			pk::list<pk::person@> target_gikyoudai_list = pk::get_gikyoudai_list(target);

			// 목표 무장이 의형제와 같은 세력인 경우 항상 실패
			if (is_valid_target_force_id and pk::is_alive(target_gikyoudai) and pk::is_valid_force_id(target_force_id))
			{
				for (int i = 0; i < target_gikyoudai_list.size; i++)
				{
					if (target_gikyoudai_list[i].get_force_id() == target_force_id)
						return pk::bool_bool(true, false);
				}
			}

			// 목표 무장의 의형제가 다른 세력에 소속되어 있다면 항상 실패
			if (pk::is_alive(target_gikyoudai) and @target_gikyoudai != @target)
			{
				int gikyoudai_force_id = target_gikyoudai.get_force_id();
				if (is_valid_target_force_id or target_force_id != gikyoudai_force_id)
				{
					if (pk::is_valid_force_id(gikyoudai_force_id) and actor_kunshu.get_force_id() != gikyoudai_force_id)
						return pk::bool_bool(true, false);
				}
			}

			// 목표 무장의 배우자가 다른 세력에 소속되어 있다면 항상 실패
			if (pk::is_alive(target_spouse))
			{
				int spouse_force_id = target_spouse.get_force_id();
				if (is_valid_target_force_id or target_force_id != spouse_force_id)
				{
					if (pk::is_valid_force_id(spouse_force_id) and actor_kunshu.get_force_id() != spouse_force_id)
						return pk::bool_bool(true, false);
				}
			}

			// 목표 무장의 배우자가 실행 무장이거나 실행 무장의 군주인 경우 항상 성공
			if (pk::is_alive(target_spouse))
			{
				if (@target_spouse == @actor_kunshu or @target_spouse == @actor)
					return pk::bool_bool(true, true);
			}

			// 목표 무장이 실행 무장의 군주를 혐오하는 경우 항상 실패
			if (pk::is_dislike(target, actor_kunshu_id)) return pk::bool_bool(true, false);

			int actor_id = actor.get_id();

			// 목표 무장이 실행 무장을 혐오하는 경우 항상 실패
			if (pk::is_dislike(target, actor_id)) return pk::bool_bool(true, false);

			int actor_force_id = actor.get_force_id();
			int target_kunshu_id = pk::get_kunshu_id(target);

			if (type == 0)
			{
				// 목표 무장이 실행 무장과 의형제나 부부이거나, 실행 무장의 군주와 의형제나 부부인경우 항상 성공
				if (pk::is_gikyoudai(target, actor_id) or pk::is_gikyoudai(target, actor_kunshu_id) or pk::is_fuufu(target, actor_id) or pk::is_fuufu(target, actor_kunshu_id)) return pk::bool_bool(true, true);

				// 목표 무장의 충성도, 의리 합이 96 이상인 경우 항상 실패
				if (target.loyalty + target.giri >= 96) return pk::bool_bool(true, false);

				// 목표 무장의 의형제가 실행 무장의 세력에 소속되어 있다면 항상 성공
				if (pk::is_alive(target_gikyoudai) and target_gikyoudai.get_force_id() == actor_force_id) return pk::bool_bool(true, true);

				// 목표 무장의 배우자가 실행 무장의 세력에 소속되어 있다면 항상 성공
				if (pk::is_alive(target_spouse) and target_spouse.get_force_id() == actor_force_id) return pk::bool_bool(true, true);

				// 목표 무장이 목표 무장의 군주를 친애한다면 항상 실패
				if (target.mibun != 신분_군주 and pk::is_valid_person_id(target_kunshu_id) and pk::is_like(target, target_kunshu_id)) return pk::bool_bool(true, false);

				// 목표 무장이 실행 무장이나 실행 무장의 군주를 친애한다면 항상 성공
				if (pk::is_like(target, actor_id) or pk::is_like(target, actor_kunshu_id)) return pk::bool_bool(true, true);
			}
			else
			{
				// 목표 무장의 의형제가 실행 무장의 세력에 소속되어 있다면 항상 성공
				for (int i = 0; i < target_gikyoudai_list.size; i++)
				{
					if (target_gikyoudai_list[i].get_force_id() == actor_force_id)
					return pk::bool_bool(true, true);
				}

				// 목표 무장의 배우자가 실행 무장의 세력에 소속되어 있다면 항상 성공
				if (pk::is_alive(target_spouse) and target_spouse.get_force_id() == actor_force_id) return pk::bool_bool(true, true);

				// 목표 무장이 목표 무장의 군주를 친애한다면 항상 실패
				if (is_valid_target_force_id and target.mibun != 신분_군주 and pk::is_valid_person_id(target_kunshu_id) and pk::is_like(target, target_kunshu_id)) return pk::bool_bool(true, false);

				// 목표 무장이 실행 무장의 군주를 친애한다면 항상 성공
				if (pk::is_like(target, actor_kunshu_id)) return pk::bool_bool(true, true);
			}

			return pk::bool_bool(false, false);
		}

		/***/
		int func_5c6030(pk::person@ target, pk::person@ actor, int type, int seed)
		{
			if (!pk::is_alive(target) or !pk::is_alive(actor)) return 0;

			int loyalty = target.loyalty;
			int aishou = 25;
			int giri = target.giri;
			int target_kunshu_id = pk::get_kunshu_id(target);
			int actor_kunshu_id = pk::get_kunshu_id(actor);
			int n = 0;

			if (type == 2)
				loyalty = pk::min(loyalty, 70);

			// 목표 무장이 재야이거나 멸망한 세력의 포로인 경우
			if (target.mibun == 신분_재야 or (target.mibun == 신분_포로 and !pk::is_valid_force_id(target.get_force_id())))
				loyalty = pk::get_scenario().difficulty == 난이도_초급 ? 60 : 70;

			if (pk::is_valid_person_id(target_kunshu_id))
				aishou = pk::get_aishou_distance(target, target_kunshu_id);

			if (target.mibun == 신분_재야)
				giri = 의리_보통;

			n = (aishou - pk::get_aishou_distance(target, actor_kunshu_id)) / 5;
			n -= (giri + 18) * loyalty * 5 / 100;
			n += pk::max(actor.stat[무장능력_매력], 30) * 3 / 5;
			n -= pk::is_like(target, target_kunshu_id) ? 15 : 0;
			n -= pk::is_oyako(target, target_kunshu_id) ? 15 : 0;
			n += pk::is_dislike(target, target_kunshu_id) ? 15 : 0;
			n += pk::rand(의리_끝 - giri, actor.get_id(), target.get_id(), actor.stat[무장능력_매력], seed, actor_kunshu_id, 0, 0);
			n += target.mibun == 신분_포로 ? 15 : 0;
			n += 45;

			return pk::max(n, 0);
		}
	}

	Main main;
}