namespace 전법_성공_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(202, pk::func202_t(callback));
		}

		int callback(pk::unit@ attacker, const pk::point &in pos, pk::hex_object@ target, int tactics_id)
		{
			// 호위가 보병 칸이동 전법을 당하지 않음
			if (tactics_id == 전법_돌출 or tactics_id == 전법_이단첨 or tactics_id == 전법_갈퀴)
			{
				if (target.has_skill(특기_호위))
					return 0;
			} 

			if (target.is_instance(pk::unit::type_id) and pk::hex_object_to_unit(target).status != 부대상태_통상)
				return 100;

			int n = 0;
			pk::point temp;

			// if (pk::get_tekisei(attacker) == 적성_A) n = 5; else if (pk::get_tekisei(attacker) == 적성_S) n = 10;
			n = int(pk::core["적성"][pk::get_tekisei(attacker)]["전법성공확률"]) + pk::core::tactics_chance(attacker.leader);

			// 정묘 특기 전법 성공률 10% 증가 (특기종합패치)
			if (attacker.has_skill(특기_정묘))
				n = n + 10;

			// 아래 두 특기는 정묘와 중첩 가능

			// 행군 특기 기병 전법 성공률 5% 증가 (특기종합패치)
			if (attacker.has_skill(특기_행군) and tactics_id >= 전법_돌격 and tactics_id <= 전법_돌진)
				n = n + 5;

			// 사수 특기 노병 전법 성공률 5% 증가 (특기종합패치)
			if (attacker.has_skill(특기_사수) and tactics_id >= 전법_화시 and tactics_id <= 전법_난사)
				n = n + 5;
			

			switch (tactics_id)
			{
			case 전법_돌출:
			case 전법_돌격:
				n = n + push_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 70;
				break;

			case 전법_이단첨:
				n = n + push_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 60;
				temp = pk::get_neighbor_pos(target.get_pos(), pk::get_direction(attacker.get_pos(), target.get_pos()));
				n = n + cutoff_diff(get_height_diff(target.get_pos(), temp));
				break;

			case 전법_갈퀴:
				n = n + pull_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 70;
				temp = pk::get_neighbor_pos(attacker.get_pos(), pk::get_direction(target.get_pos(), attacker.get_pos()));
				n = n + cutoff_diff(get_height_diff(attacker.get_pos(), temp));
				break;

			case 전법_나선첨:
			case 전법_횡치:
			case 전법_관시:
				n = n + 70;
				break;

			case 전법_화시:
				n = n + hiya_terrain_bonus(pk::get_hex(target.get_pos()).terrain) + 75;
				break;

			case 전법_돌파:
				n = n + push_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 65;
				break;

			case 전법_돌진:
				n = n + push_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 60;
				temp = pk::get_neighbor_pos(target.get_pos(), pk::get_direction(attacker.get_pos(), target.get_pos()));
				n = n + cutoff_diff(get_height_diff(target.get_pos(), temp));
				break;

			case 전법_선풍:
			case 전법_난사:
			case 전법_함선격돌:
				n = n + 65;
				break;

			default:
				return 100;
			}

			return n;
		}

		// 5af5b0
		int get_height_diff(const pk::point &in src, const pk::point &in dst)
		{
			return pk::get_height_map(pk::hex_pos_to_height_map_pos(src) + 2).height - pk::get_height_map(pk::hex_pos_to_height_map_pos(dst) + 2).height;
		}

		// 5af680
		int cutoff_diff(int n)
		{
			// 상대보다 높음
			if (n >= 10)
				return 10;
			// 상대보다 높음
			else if (n >= 5)
				return 5;
			// 평지
			else if (n >= -5)
				return 0;
			// 상대보다 낮음
			else if (n >= -10)
				return -5;
			// 상대보다 낮음
			else
				return -10;
		}

		// 5af6b0
		int push_bonus(int n)
		{
			return cutoff_diff(n) + 5;
		}

		// 5af6f0
		int pull_bonus(int n)
		{
			return 5 - cutoff_diff(n);
		}

		// 849ae0 지형에 따른 화시 확률 보너스
		int hiya_terrain_bonus(int terrain)
		{
			switch (terrain)
			{
			case 지형_땅: return -5;
			case 지형_모래: return -5;
			case 지형_습지: return -15;
			case 지형_독천: return -15;
			case 지형_숲: return 10;
			case 지형_내: return -10;
			case 지형_강: return -10;
			case 지형_바다: return -10;
			case 지형_여울: return -10;
			case 지형_샛길: return -5;
			}
			return 0;
		}
	}

	Main main;
}