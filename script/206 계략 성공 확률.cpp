namespace 계략_성공_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(206, pk::func206_t(callback));
		}

		pk::int_bool callback(pk::unit@ src, const pk::point &in src_pos, const pk::point &in dst_pos, int strategy_id)
		{
			if (!pk::is_alive(src))
				return pk::int_bool(0, false);

			int src_int = src.attr.stat[부대능력_지력];
			int dst_int = 0;
			pk::unit@ dst = pk::get_unit(dst_pos);
			int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, n = 0;

			// 목표 부대 존재
			if (dst !is null)
			{
				// 낙뢰가 아닌 경우 목표 부대가 통찰 특기를 가지고 있다면 항상 실패
				if (strategy_id != 계략_낙뢰 and pk::is_enemy(src, dst) and dst.has_skill(특기_통찰)) return pk::int_bool(0, true);

				dst_int = dst.attr.stat[부대능력_지력];
				if (dst.status == 부대상태_혼란)
					a = 10;
			}

			switch (strategy_id)
			{
			case 계략_화계:
				b = pk::get_building(dst_pos) !is null ? func_5af510(pk::get_building(dst_pos).facility) : data_849a60(pk::get_hex(dst_pos).terrain);
				if (b == 100) return pk::int_bool(100, false);

				c = 100;
				c -= dst_int * 9 / 10;
				c /= 2;
				c *= src_int * src_int * 100;
				c /= dst_int * dst_int + src_int * src_int;
				c /= 55;
				c -= (100 - src_int) / 10;
				c += a;
				c += b / 2;
				c -= 5;
				if (src_int < dst_int) c -= (dst_int - src_int) / 3;
				if (c < 1) c = 1;

				d = a + src_int * 3 / 10 - dst_int / 5 + 55;
				if (dst is null) d += 10;

				n = pk::max(1, pk::min(99, c, d));

				if (dst !is null)
				{
					if (dst.has_skill(특기_화신)) return pk::int_bool(0, true);
					if (pk::get_best_member_stat(dst, 특기_간파, 무장능력_지력) > src_int) return pk::int_bool(0, true);
					if (pk::get_best_member_stat(dst, 특기_신산, 무장능력_지력) > src_int) return pk::int_bool(0, true);

					if (pk::get_best_member_stat(src, 특기_화공, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
					if (pk::get_best_member_stat(src, 특기_허실, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
					if (pk::get_best_member_stat(src, 특기_화신, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
					if (pk::get_best_member_stat(src, 특기_신산, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				}
				break;

			case 계략_소화:
				if (pk::get_best_member_stat(src, 특기_화공, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_허실, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_화신, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_신산, 무장능력_지력) > dst_int) return pk::int_bool(100, false);

				b = sqrt(pk::max(10, src_int - 5)) * 11;

				n = pk::max(1, pk::min(99, b));
				break;

			case 계략_위보:
				if (dst is null) return pk::int_bool(0, false);

				if (dst.has_skill(특기_규율)) return pk::int_bool(0, true);
				if (dst.has_skill(특기_명경)) return pk::int_bool(0, true);

				if (pk::get_best_member_stat(src, 특기_언독, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_허실, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_신산, 무장능력_지력) > dst_int) return pk::int_bool(100, false);

				if (pk::get_best_member_stat(dst, 특기_간파, 무장능력_지력) > src_int) return pk::int_bool(0, true);
				if (pk::get_best_member_stat(dst, 특기_신산, 무장능력_지력) > src_int) return pk::int_bool(0, true);

				b = data_849b24(pk::get_person(dst.leader).character);
				c = src.attr.stat[부대능력_매력] / 20;

				d = src_int * 3 / 10 - dst_int / 5;
				d += b;
				d += c;

				e = a + d + 70;

				f = 100;
				f -= dst_int * 9 / 10;
				f *= src_int * src_int * 100;
				f /= dst_int * dst_int + src_int * src_int;
				f /= 55;
				f -= (100 - src_int) / 10;
				f += a;
				f += b;
				f += c;
				if (src_int < dst_int) f -= (dst_int - src_int) / 3;
				if (f < 1) f = 1;

				n = pk::max(1, pk::min(99, e, f));
				break;

			case 계략_교란:
				if (dst is null) return pk::int_bool(0, false);

				if (dst.has_skill(특기_침착)) return pk::int_bool(0, true);
				if (dst.has_skill(특기_명경)) return pk::int_bool(0, true);

				if (pk::get_best_member_stat(src, 특기_기략, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_허실, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_신산, 무장능력_지력) > dst_int) return pk::int_bool(100, false);

				if (pk::get_best_member_stat(dst, 특기_간파, 무장능력_지력) > src_int) return pk::int_bool(0, true);
				if (pk::get_best_member_stat(dst, 특기_신산, 무장능력_지력) > src_int) return pk::int_bool(0, true);

				b = data_849b14(pk::get_person(dst.leader).character);
				c = dst.attr.stat[부대능력_방어] / 20;

				d = src_int * 3 / 10 - dst_int / 5;
				d += b;
				d += c;

				e = a + d + 70;

				f = 100;
				f -= dst_int * 9 / 10;
				f *= src_int * src_int * 100;
				f /= dst_int * dst_int + src_int * src_int;
				f /= 45;
				f -= (100 - src_int) / 10;
				f += a;
				f += b;
				f += c;
				if (src_int < dst_int) f -= (dst_int - src_int) / 3;
				if (f < 1) f = 1;

				n = pk::max(1, pk::min(99, e, f));
				break;

			case 계략_진정:
				if (dst is null) return pk::int_bool(0, false);

				if (pk::get_best_member_stat(src, 특기_허실, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_신산, 무장능력_지력) > dst_int) return pk::int_bool(100, false);

				b = sqrt(pk::max(10, src_int - 5)) * 11 + data_849b04(pk::get_person(dst.leader).character);

				n = pk::max(1, pk::min(99, b));
				break;

			case 계략_복병:
				if (dst is null) return pk::int_bool(0, false);

				if (pk::get_best_member_stat(src, 특기_허실, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_신산, 무장능력_지력) > dst_int) return pk::int_bool(100, false);

				if (pk::get_best_member_stat(dst, 특기_간파, 무장능력_지력) > src_int) return pk::int_bool(0, true);
				if (pk::get_best_member_stat(dst, 특기_신산, 무장능력_지력) > src_int) return pk::int_bool(0, true);

				b = data_849af4(pk::get_person(dst.leader).character);
				c = int(src.attr.stat[부대능력_공격] - dst.attr.stat[부대능력_방어]) / 10;

				d = src_int * 3 / 10;
				d -= dst_int / 3;
				d += b;
				d += c;

				e = a + d + 80;

				f = 100;
				f -= dst_int * 9 / 10;
				f *= src_int * src_int * 100;
				f /= dst_int * dst_int + src_int * src_int;
				f /= 50;
				f -= (100 - src_int) / 10;
				f += a;
				f += b;
				f += c;
				if (src_int < dst_int) f -= (dst_int - src_int) / 3;
				if (f < 1) f = 1;

				n = pk::max(1, pk::min(99, e, f));
				break;

			case 계략_동토:
				if (dst is null) return pk::int_bool(0, false);

				if (pk::get_best_member_stat(src, 특기_궤계, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_허실, 무장능력_지력) > dst_int) return pk::int_bool(100, false);
				if (pk::get_best_member_stat(src, 특기_신산, 무장능력_지력) > dst_int) return pk::int_bool(100, false);

				if (pk::get_best_member_stat(dst, 특기_간파, 무장능력_지력) > src_int) return pk::int_bool(0, true);
				if (pk::get_best_member_stat(dst, 특기_신산, 무장능력_지력) > src_int) return pk::int_bool(0, true);

				b = data_849b14(pk::get_person(dst.leader).character);
				c = src_int * 3 / 10 - dst_int * 4 / 10 + b;
				e = a + c + 70;

				f = 100;
				f -= dst_int * 9 / 10;
				f *= src_int * src_int * 100;
				f /= dst_int * dst_int + src_int * src_int;
				f /= 55;
				f -= (100 - src_int) / 10;
				f += a;
				f += b;
				if (src_int < dst_int) f -= (dst_int - src_int) / 3;
				if (f < 1) f = 1;

				n = pk::max(1, pk::min(99, e, f));
				break;

			case 계략_요술:
				if (dst is null) return pk::int_bool(0, false);

				b = data_849b14(pk::get_person(dst.leader).character);
				c = 100 - dst_int * 9 / 10;

				d = src_int / 10;
				d += a;
				d += b;
				d += c;

				if (dst_int > src_int)
				{
					b -= (100 - src_int) / 10;
					b += c;
					d = a + b - 5;
				}

				n = pk::max(1, pk::min(99, d));
				return pk::int_bool(n + pk::core::strategy_chance(src.leader), false);

			case 계략_낙뢰:
				b = src_int * 0.6f;
				c = a + b + 15;
				n = pk::max(1, pk::min(99, c));
				return pk::int_bool(n + pk::core::strategy_chance(src.leader), false);
			}

			if (src.has_skill(특기_경국) and dst !is null and !pk::has_female_member(dst))
				n *= 2;

			return pk::int_bool(n + pk::core::strategy_chance(src.leader), false);
		}

		/** 시설에 따른 화계 상수 */
		int func_5af510(int facility_id)
		{
			switch (facility_id)
			{
			case 시설_도시:
			case 시설_관문:
			case 시설_성채:
			case 시설_석벽:
			case 시설_석병팔진:
			case 시설_동작대:
				return 10;

			case 시설_항구:
			case 시설_연노로:
			case 시설_투석대:
			case 시설_태고대:
			case 시설_군악대:
			case 시설_본거지1:
			case 시설_유적:
			case 시설_묘:
			case 시설_농장:
			case 시설_공방:
			case 시설_조선:
			case 시설_조폐:
			case 시설_곡창:
			case 시설_암시장:
			case 시설_군둔농:
			case 시설_농장2단:
			case 시설_농장3단:
				return 30;

			case 시설_진:
				return 35;

			case 시설_요새:
				return 25;

			case 시설_궁노:
			case 시설_본거지2:
			case 시설_시장:
			case 시설_병영:
			case 시설_대장간:
			case 시설_마구간:
			case 시설_부절태:
			case 시설_군사부:
			case 시설_인재부:
			case 시설_외교부:
			case 시설_계략부:
			case 시설_연병소:
			case 시설_대시장:
			case 시설_어시장:
			case 시설_시장2단:
			case 시설_시장3단:
			case 시설_병영2단:
			case 시설_병영3단:
			case 시설_대장간2단:
			case 시설_대장간3단:
			case 시설_마구간2단:
			case 시설_마구간3단:
				return 40;

			case 시설_토루:
			case 시설_제방:
				return 20;

			case 시설_불씨:
			case 시설_화염종:
			case 시설_화구:
			case 시설_화염구:
			case 시설_화선:
			case 시설_업화구:
			case 시설_업화종:
				return 100;

			case 시설_낙석:
			case 시설_얕은여:
			case 시설_산악:
			case 시설_장성:
				return 0;
			}
			return 0;
		}

		/** 지형에 따른 화계 상수 */
		int data_849a60(int terrain_id)
		{
			switch (terrain_id)
			{
			case 지형_초원: return 40;
			case 지형_땅: return 40;
			case 지형_모래: return 40;
			case 지형_습지: return 10;
			case 지형_독천: return 10;
			case 지형_숲: return 50;
			case 지형_내: return 0;
			case 지형_강: return 0;
			case 지형_바다: return 0;
			case 지형_황무지: return 35;
			case 지형_대로: return 30;
			case 지형_벼랑길: return 15;
			case 지형_다리: return 20;
			case 지형_여울: return 10;
			case 지형_물가: return 0;
			case 지형_산: return 0;
			case 지형_도시: return 0;
			case 지형_관문: return 0;
			case 지형_항구: return 0;
			case 지형_샛길: return 10;
			}
			return 0;
		}

		/** 성격에 따른 복병 상수 */
		int data_849af4(int character)
		{
			switch (character)
			{
			case 성격_소심: return 3;
			case 성격_냉정: return -2;
			case 성격_대담: return 0;
			case 성격_저돌: return 1;
			}
			return 0;
		}

		/** 성격에 따른 진정 상수 */
		int data_849b04(int character)
		{
			switch (character)
			{
			case 성격_소심: return -5;
			case 성격_냉정: return 5;
			case 성격_대담: return 0;
			case 성격_저돌: return -10;
			}
			return 0;
		}

		/** 성격에 따른 교란 상수 */
		int data_849b14(int character)
		{
			switch (character)
			{
			case 성격_소심: return -2;
			case 성격_냉정: return 0;
			case 성격_대담: return 1;
			case 성격_저돌: return 3;
			}
			return 0;
		}

		/** 성격에 따른 위보 상수 */
		int data_849b24(int character)
		{
			switch (character)
			{
			case 성격_소심: return 3;
			case 성격_냉정: return 1;
			case 성격_대담: return 0;
			case 성격_저돌: return -2;
			}
			return 0;
		}
	}

	Main main;
}