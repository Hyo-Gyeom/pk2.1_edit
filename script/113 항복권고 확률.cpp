namespace 항복권고_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(113, pk::func113_t(callback));
		}

		pk::int_bool callback(const pk::solicit_cmd_info &in info)
		{
			bool accurate = true;

			// 1대1 상황일 경우 항상 실패
			if (pk::get_force_list().size <= 2) return pk::int_bool(2, accurate);

			pk::force@ actor_force = pk::get_force(info.actor.get_force_id());

			if (!pk::is_alive(actor_force)) return pk::int_bool(2, accurate);

			// 시나리오 개시 1년 미만인 경우 항상 실패
			if (pk::get_elapsed_years() < 1) return pk::int_bool(2, accurate);

			// 실행 무장 세력의 도시수가 목표 세력 도시수의 4배 미만일 경우 항상 실패
			if (pk::get_city_count(actor_force) < pk::get_city_count(info.target) * 4) return pk::int_bool(2, accurate);

			// 실행 무장 세력의 병력수가 목표 세력 병력수의 3배 미만일 경우 항상 실패
			if (pk::get_troops(actor_force) < pk::get_troops(info.target) * 3) return pk::int_bool(2, accurate);

			// 목표 세력의 도시가 5개 이상인 경우 항상 실패
			if (pk::get_city_count(info.target) >= 5) return pk::int_bool(2, accurate);

			pk::person@ target_kunshu = pk::get_person(info.target.kunshu);

			if (!pk::is_alive(target_kunshu)) return pk::int_bool(2, accurate);

			// 인접한 도시가 없다면 항상 실패
			if (!pk::is_neighbor_force(actor_force, info.target)) return pk::int_bool(2, accurate);

			// 목표 세력 군주가 실행 무장의 군주를 혐오한다면 항상 실패
			if (pk::is_dislike(target_kunshu, pk::get_kunshu_id(info.actor))) return pk::int_bool(2, accurate);

			// 목표 세력 군주가 실행 무장을 혐오한다면 항상 실패
			if (pk::is_dislike(target_kunshu, info.actor.get_id())) return pk::int_bool(2, accurate);

			int k = info.target.kunshu;

			// 절대 항복하지 않는 군주
			if (k == 무장_여포 or k == 무장_동탁 or k == 무장_장각 or k == 무장_유비 or k == 무장_관우 or k == 무장_장비 or k == 무장_손견 or k == 무장_손책 or k == 무장_조조) return pk::int_bool(2, true);

			if (pk::get_city_count(info.target) <= 0) return pk::int_bool(2, accurate);

			int n = 0;
			int g = 0;
			int difficulty = pk::get_scenario().difficulty;

			n += (pk::get_city_count(actor_force) / pk::get_city_count(info.target)) / 2;
			n += (pk::get_city_count(actor_force) - pk::get_city_count(info.target)) / 5;
			n -= pk::get_aishou_distance(target_kunshu, info.actor.get_id()) / 7;
			n += pk::is_protecting_the_emperor(actor_force) ? 10 : 0;
			n += func_5b3f40(info.actor, target_kunshu, 0, 10, 20);
			n += info.actor.stat[무장능력_매력];
			if (difficulty == 난이도_상급)
				n = n * 0.8f;
			else if (difficulty == 난이도_특급)
				n = n * 0.7f;
			n /= 2;
			n += pk::rand(n, info.gold, info.target.get_id(), info.actor.get_id(), info.actor.service, 0, 0, 0);

			g = target_kunshu.character * 7 - info.target.relations[actor_force.get_id()] / 4;

			if (n * 2 >= g + 100) accurate = false;

			if (n >= g + 100) return pk::int_bool(0, accurate);

			if (n >= g + 95) return pk::int_bool(1, accurate);

			return pk::int_bool(2, accurate);
		}

		/**
			실행무장과 목표세력 군주의 관계에 따른 보너스
		*/
		int func_5b3f40(pk::person@ actor, pk::person@ target_kunshu, int bad, int good, int best)
		{
			int actor_id = actor.get_id();
			if (pk::is_gikyoudai(target_kunshu, actor_id) or pk::is_fuufu(target_kunshu, actor_id)) return best;
			if (pk::is_like(target_kunshu, actor_id)) return good;
			if (pk::is_dislike(target_kunshu, actor_id)) return bad;
			return 0;
		}
	}

	Main main;
}