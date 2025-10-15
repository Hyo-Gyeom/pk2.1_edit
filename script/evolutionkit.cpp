namespace evolutionkit
{
	class Main
	{
		Main()
		{
			// pk::reset_func(101);
			// pk::set_func(101, pk::func101_t(func101));
			pk::reset_func(110);
			pk::set_func(110, pk::func110_t(func110));
			pk::reset_func(152);
			pk::set_func(152, pk::func152_t(func152));
			pk::reset_func(153);
			pk::set_func(153, pk::func153_t(func153));
			//pk::reset_func(157);
			//pk::set_func(157, pk::func157_t(func157));
			pk::reset_func(168);
			pk::set_func(168, pk::func168_t(func168));
			pk::reset_func(169);
			pk::set_func(169, pk::func169_t(func169));
			pk::reset_func(216);
			pk::set_func(216, pk::func216_t(func216));
			pk::reset_func(217);
			pk::set_func(217, pk::func216_t(func217));
			pk::reset_func(218);
			pk::set_func(218, pk::func216_t(func218));
			pk::reset_func(250);
			pk::set_func(250, pk::func250_t(func250));
			pk::reset_func(251);
			pk::set_func(251, pk::func251_t(func251));
			pk::reset_func(252);
			pk::set_func(252, pk::func252_t(func252));
			pk::reset_func(253);
			pk::set_func(253, pk::func253_t(func253));
			pk::reset_func(254);
			pk::set_func(254, pk::func254_t(func254));
	
			pk::reset_func(255);
			pk::set_func(255, pk::func255_t(func255));

			@func256_ = cast<pk::func256_t@>(pk::get_func(256));
			pk::reset_func(256);
			pk::set_func(256, pk::func256_t(func256));

			xml();
		}

		/** 컴퓨터 징병 후 치안 감소 반감 */
		int func101(pk::city@ city, const pk::detail::arrayptr<pk::person@> &in actors, int troops)
		{
			int n = cast<pk::func101_t@>(pk::get_func(101, true))(city, actors, troops);
			if (!city.is_player()) n /= 2;
			return n;
		}

		/** 컴퓨터 보물 발견 확률 2배 */
		int func110(pk::building@ building, pk::person@ actor, pk::item@ item)
		{
			int n = pk::max((61 - item.value) / 20, 1);
			if (!actor.is_player())
				n *= 2;
			return n;
		}

		/** 관문, 항구 금 수입 감소 */
		int func152(pk::building@ building, int city_revenue)
		{
			return city_revenue * 0.01f;
		}

		/** 관문, 항구 병량 수입 감소 */
		int func153(pk::building@ building, int city_harvest)
		{
			return city_harvest * 0.01f;
		}

		/***/
		void func168(pk::building@ base, pk::unit@ attacker)
		{
			pk::person@ leader;

			if (pk::is_alive(attacker))
				@leader = pk::get_person(attacker.leader);

			int n = 70;
			int charisma = 20;

			if (pk::is_alive(leader))
				charisma = leader.stat[무장능력_매력];

			pk::set_gold(base, pk::get_gold(base) * n / 100);
			pk::set_food(base, pk::get_food(base) * n / 100);
			pk::set_troops(base, pk::get_troops(base) * n / 100);
			for (int i = 0; i < 병기_끝; i++)
				pk::set_weapon_amount(base, i, pk::get_weapon_amount(base, i) * n / 100);

			pk::city@ city = pk::building_to_city(base);

			if (!pk::is_alive(city))
				return;

			int city_id = city.get_id();
			int city_force_id = city.get_force_id();
			pk::list<pk::building@> city_devs;
			pk::list<pk::building@> buildings = pk::get_building_list();

			for (int i = 0; i < buildings.count; i++)
			{
				pk::building@ building = buildings[i];
				if (pk::is_alive(building) and pk::get_city_id(building.get_pos()) == city_id)
				{
					switch (pk::get_facility_type(building))
					{
					case 시설종류_내정시설:
						if (building.completed)
						{
							if (building.facility != 시설_동작대)
								city_devs.add(building);
						}
						else
						{
							if (building.facility == 시설_동작대)
							{
								pk::item@ item = pk::get_item(보물_동작);
								if (pk::is_alive(item))
									pk::kill(item);
							}
							else
							{
								pk::kill(building, false);
							}
						}
						break;

					case 시설종류_군사시설:
						if (pk::is_valid_force_id(city_force_id) and city_force_id == building.get_force_id())
						{
							int distance = pk::get_distance(city.get_pos(), building.get_pos());
							int n = 0;

							if (distance <= 4)
								n = 75;
							else if (distance <= 8)
								n = 50;
							else
								n = 30;

							if (pk::rand_bool(n))
								pk::set_district(building, pk::get_district(attacker.get_district_id()), 0);
							else if (pk::rand_bool(20))
								continue;
							else
								pk::kill(building, false);
						}
						break;
					}
				}
			}

			n = city_devs.count;
			if (pk::is_valid_normal_force_id(attacker.get_force_id()))
				n = n - pk::min(charisma / 20, n);
			city_devs.shuffle();
			for (int i = 0; i < n; i++)
				pk::kill(city_devs[i], false);
		}

		/** 수송 부대는 아래에서 설정한 검병 설정과 상관 없이 내, 물가, 산 지형을 사용하지 못하도록함. */
		bool func169(pk::unit@ unit, int terrain_id)
		{
			if (unit.type == 부대종류_수송 and (terrain_id == 지형_내 or terrain_id == 지형_물가 or terrain_id == 지형_산)) return false;
			return cast<pk::func169_t@>(pk::get_func(169, true))(unit, terrain_id);
		}

		/***/
		int func216(pk::unit@ unit, const pk::point &in pos)
		{
			if (unit.has_skill(특기_화신) or unit.has_skill(특기_화공))
				return 0;
			int n = pk::max(unit.troops * 0.01f, 50.f);
			n += pk::rand(n);
			if (unit.has_skill(특기_등갑))
				n *= 2;
			return n;
		}

		/***/
		int func217(pk::unit@ unit, const pk::point &in pos)
		{
			if (unit.has_skill(특기_해독))
				return 0;
			int n = pk::max(unit.troops * 0.04f, 200.f);
			n += pk::rand(n);
			return n;
		}

		/***/
		int func218(pk::unit@ unit, const pk::point &in pos)
		{
			if (unit.has_skill(특기_답파) or unit.has_tech(기교_난소행군))
				return 0;
			int n = pk::max(unit.troops * 0.02f, 100.f);
			n += pk::rand(n);
			return n;
		}

		/***/
		void func250(pk::ai_context_base@ base_info, int src, const pk::list<pk::building@> &in dst_list)
		{
			int n = 0;
			int empty_dst_count = 0;			

			for (int i = 0; i < dst_list.count; i++)
			{
				pk::building@ building = dst_list[i];
				if (pk::is_alive(building))
				{
					int dst_force_id = building.get_force_id();
					if (pk::is_valid_force_id(dst_force_id))
					{
						if (isEnemy(src, building.get_id()))
							n++;
					}
					else
					{
						empty_dst_count++;
					}
				}
			}

			if (empty_dst_count > 0)
				n++;

			base_info.num_enemy_routes = n;
		}

		bool isEnemy(int src, int dst)
		{
			pk::building@ src_building = pk::get_building(src);
			pk::building@ dst_building = pk::get_building(dst);
			int src_force_id = src_building.get_force_id();
			int dst_force_id = dst_building.get_force_id();
			int route;

			if (pk::is_reachable_neighbor(src, dst, src_force_id))
				return true;

			if (pk::is_reachable_neighbor(dst, src, dst_force_id))
			{
				if (dst < 건물_도시끝 and dst_building.get_troops() * 2 < src_building.get_troops())
					return false;
				return true;
			}

			route = pk::get_route(src, dst, src_force_id);
			if (route != 경로_없음)
				return true;

			route = pk::get_route(dst, src, dst_force_id);
			if (route == 경로_벼랑길 or route == 경로_샛길 or route == 경로_여울)
			{
				if (dst < 건물_도시끝 and dst_building.get_troops() * 2 < src_building.get_troops())
					return false;
			}
			return route != 경로_없음;
		}

		/***/
		void func251(pk::ai_context_base@ base_info, pk::building@ base)
		{
			if (base_info.status == 거점상태_후방 and base_info.num_idle_officers > 0 and base.get_id() > 건물_관문시작)
				base_info.ap_needed = 20;
		}

		/***/
		void func252(pk::ai_context@ context)
		{
			@context_ = @context;
			context.bases.sort(function(a, b)
			{
				if ((a.get_id() >= 건물_관문시작 and b.get_id() < 건물_관문시작) or (b.get_id() >= 건물_관문시작 and a.get_id() < 건물_관문시작))
					return a.get_id() > b.get_id();
				return main.context_.base[a.get_id()].status < main.context_.base[b.get_id()].status;
			});
		}

		pk::ai_context@ context_;

		/***/
		int func253(pk::building@ src, int src_rank, pk::building@ dst, int dst_troops)
		{
			uint dst_max_troops = pk::get_max_troops(dst);

			// 빈 거점
			if (dst.get_force_id() == -1)
			{
				pk::city@ city = pk::building_to_city(dst);
				// 관문이나 항구일 경우
				if (city is null)
					return dst_max_troops / 5;
				// 도시일 경우 이웃 도시에 적 세력이 존재한다면
				for (int i = 0; i < 6; i++)
				{
					if (city.neighbor[i] != -1 and pk::is_enemy(city, pk::get_city(city.neighbor[i])))
						return dst_max_troops / 2;
				}
				// 안전한 도시일 경우
				return dst_max_troops / 4;
			}

			// 병력 수 재계산(다른 세력 부대 병력 수까지 포함)
			int src_force_id = src.get_force_id();
			int dst_id = dst.get_id();
			dst_troops = pk::get_troops(dst);
			array<pk::unit@> arr = pk::list_to_array(pk::get_unit_list());
			for (int i = 0; i < arr.length; i++)
			{
				pk::unit@ unit = arr[i];
				if (unit.get_force_id() != src_force_id and pk::get_building_id(unit.get_pos()) == dst_id)
					dst_troops += unit.troops;
			}

			if (dst_troops > dst_max_troops)
				return dst_troops + dst_troops / 4;
			else if (dst_troops * 2 > dst_max_troops)
				return dst_max_troops + dst_max_troops / 4;
			else
				return pk::max(dst_troops, dst_max_troops / 4) * 3;
		}

		/***/
		int func254(pk::building@ building, int troops)
		{
			return troops * 1.25f;
		}

		/***/
		void func255(pk::ai_context@ context, pk::building@ building)
		{
			int building_id = building.get_id();
			pk::ai_context_base@ base = context.base[building_id];

			if (base.enemies_around3)
			{
				context.push_cmd(거점AI_방어);
				if (building_id >= 건물_도시시작 and building_id < 건물_도시끝)
				{
					// 플레이어가 공격중일 때만 방어 강화
					if (base.num_player_units > 0)
					{
						context.push_cmd(거점AI_징병);
						context.push_cmd(거점AI_순찰);
						context.push_cmd(거점AI_순찰);
						context.push_cmd(거점AI_징병);
						context.push_cmd(거점AI_순찰);
						context.push_cmd(거점AI_생산);
						context.push_cmd(거점AI_생산);
						context.push_cmd(거점AI_철거);
					}
					else
					{
						context.push_cmd(거점AI_징병);
						context.push_cmd(거점AI_순찰);
						context.push_cmd(거점AI_생산);
						context.push_cmd(거점AI_철거);
					}
				}
				return;
			}

			if (base.num_idle_officers == 0)
				return;

			if (building_id >= 건물_도시시작 and building_id < 건물_도시끝)
			{
				uint gold = pk::get_gold(building);
				uint officers_needed = pk::max(base.officers_needed, 10);
				uint total_weapons = 0;
				for (int i = 병기_창; i <= 병기_군마; i++)
					total_weapons = total_weapons + pk::get_weapon_amount(building, i);

				switch (base.status)
				{
				case 거점상태_후방:
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_생산);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_최소인재탐색);
					context.push_cmd(거점AI_최소무장등용);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_거래2);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_철거);
					context.push_cmd(거점AI_개발);
					context.push_cmd(거점AI_흡수합병);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_경계:
					context.push_cmd(거점AI_공격);
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_생산);
					context.push_cmd(거점AI_최소인재탐색);
					context.push_cmd(거점AI_최소무장등용);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_거래2);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_철거);
					context.push_cmd(거점AI_개발);
					context.push_cmd(거점AI_흡수합병);
					context.push_cmd(거점AI_관문수송);

					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_전방:
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_생산);
					context.push_cmd(거점AI_공격);
					context.push_cmd(거점AI_최소인재탐색);
					context.push_cmd(거점AI_최소무장등용);
					context.push_cmd(거점AI_설치);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_철거);
					context.push_cmd(거점AI_흡수합병);
					context.push_cmd(거점AI_개발);
					// 무장과 금이 충분한 경우 내정시설 2개씩 건설
					if (base.num_officers >= officers_needed and gold > 1500)
						context.push_cmd(거점AI_개발);
					// 병기가 병력보다 모자르다면 병기생산 우선
					if (total_weapons < pk::get_troops(building) - 5000)
						context.push_cmd(거점AI_생산);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_거래2);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_전투:
					context.push_cmd(거점AI_방어);
					// 적 군사시설만 있음
					if (base.num_enemy_units == 0)
					{
						context.push_cmd(거점AI_흡수합병);
						context.push_cmd(거점AI_개발);
						if (base.num_officers >= officers_needed and gold > 1500)
							context.push_cmd(거점AI_개발);
					}
					// 적 부대 있음
					else
					{
						// 자세력 부대 수와 남아 있는 무장 수의 합이 적 세력 부대 수보다 적을 때만 철거
						if (base.num_own_units + base.num_idle_officers < base.num_enemy_units)
						{
							context.push_cmd(거점AI_철거);
						}
						else if (base.num_own_units > base.num_enemy_units and pk::get_troops(building) > 15000)
						{
							context.push_cmd(거점AI_흡수합병);
							context.push_cmd(거점AI_개발);
						}
					}
					// 병기가 병력보다 모자르다면 병기생산 우선
					if (total_weapons < pk::get_troops(building) - 5000)
						context.push_cmd(거점AI_생산);

					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_생산);
					context.push_cmd(거점AI_거래2);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_계략);
					break;
				}
			}
			else
			{
				switch (base.status)
				{
				case 거점상태_후방:
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_경계:
					context.push_cmd(거점AI_공격);
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_전방:
					context.push_cmd(거점AI_공격);
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_설치);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_전투:
					context.push_cmd(거점AI_방어);
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;
				}
			}
		}

		/** 컴퓨터 행동력 최대 */
		void func256(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (!building.is_player())
			{
				if (pk::get_scenario().ai_table.force[building.get_force_id()].troops_rank > 49)
				{
					pk::get_district(building.get_district_id()).ap = 255;
					context.ap = 255;
				}
			}

			if (func256_ !is null)
				func256_(context, building, cmd);
		}

		pk::func256_t@ func256_;

		void xml()
		{
			// 경로
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<평원>
					<거리 창양항="3"/>
					<난소행군거리 창양항="3"/>
				</평원>
				<소패>
					<이웃 번호="3" 경로="일반" x="-1" y="-1"/>
					<거리 복양="1"/>
					<난소행군거리 복양="1"/>
				</소패>
				<수춘>
					<이웃 번호="4" 경로="샛길" x="129" y="110"/>
					<이웃 번호="5" 거점="끝"/>
					<거리 강하="2" 강도항="2"/>
					<난소행군거리 강도항="2"/>
				</수춘>
				<복양>
					<이웃 번호="1" 경로="일반" x="-1" y="-1"/>
					<거리 소패="1"/>
					<난소행군거리 소패="1"/>
				</복양>
				<낙양>
					<거리 완="2"/>
					<난소행군거리 완="2"/>
				</낙양>
				<완>
					<이웃 번호="4" 거점="호로관" 경로="일반" x="-1" y="-1"/>
					<거리 낙양="2" 호로관="1"/>
					<난소행군거리 낙양="2" 호로관="1"/>
				</완>
				<건업>
					<이웃 번호="1" 경로="일반" x="-1" y="-1"/>
					<거리 회계="1"/>
				</건업>
				<회계>
					<이웃 번호="0" 경로="일반" x="-1" y="-1"/>
					<거리 건업="1"/>
				</회계>
				<강하>
					<이웃 번호="0" 경로="샛길" x="129" y="110"/>
					<거리 수춘="2"/>
				</강하>
				<호로관>
					<이웃 번호="3" 거점="완" 경로="일반" x="-1" y="-1"/>
					<거리 완="1"/>
					<난소행군거리 완="1"/>
				</호로관>
				<고당항>
					<이웃 번호="2" 거점="끝"/>
					<거리 창양항="2"/>
					<난소행군거리 창양항="2"/>
				</고당항>
				<창양항>
					<이웃 번호="1" 거점="끝"/>
					<이웃 번호="2" 거점="끝"/>
					<거리 평원="3" 고당항="2" 임제항="2"/>
					<난소행군거리 평원="3" 고당항="2" 임제항="2"/>
				</창양항>
				<임제항>
					<이웃 번호="3" 거점="끝"/>
					<거리 창양항="2"/>
					<난소행군거리 창양항="2"/>
				</임제항>
				<해릉항>
					<이웃 번호="1" 거점="끝"/>
					<거리 강도항="2"/>
					<난소행군거리 강도항="2"/>
				</해릉항>
				<강도항>
					<이웃 번호="0" 거점="끝"/>
					<이웃 번호="1" 거점="끝"/>
					<이웃 번호="2" 거점="끝"/>
					<거리 수춘="2" 해릉항="2"/>
					<난소행군거리 수춘="2" 해릉항="2"/>
				</강도항>
				<유수항>
					<이웃 번호="3" 거점="끝"/>
					<거리 호림항="2"/>
					<난소행군거리 호림항="2"/>
				</유수항>
				<호림항>
					<이웃 번호="2" 거점="끝"/>
					<이웃 번호="3" 거점="끝"/>
					<거리 유수항="2" 환구항="2"/>
					<난소행군거리 유수항="2" 환구항="2"/>
				</호림항>
				<환구항>
					<이웃 번호="1" 거점="끝"/>
					<거리 호림항="2"/>
					<난소행군거리 호림항="2"/>
				</환구항>
			</pk>
			""");

			// 구역
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<구역 번호="59">
					<도시 값="오"/>
				</구역>
			</pk>
			""");

			// 기타
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<건설명령버그수정 값="true"/>
				<전투경험치패널티 값="false"/>
				<기사공격숲제외 값="false"/>
				<낙석사용 값="true"/>
				<대미지패널티 초급="" 상급="0.9" 특급="0.75"/>
				<무장.능력상승필요경험치 값="70"/>
				<무장.최대능력치 값="125"/>
				<비활성상태입력무시 값="true"/>
				<업화종버그수정 값="true"/>
				<위임군단제어 값="true"/>
				<일기토.부가무력으로표시 값="true"/>
				<적부대출진알림 값="false"/>
				<점프리스트단축키사용 값="true"/>
				<초특기연전 값="true"/>
				<컴퓨터논객활성화 값="true"/>
				<타일기반선택 값="true"/>
				<폭약연성버그수정 값="true"/>
				<반투명무장로그 값="true"/>
				<반투명세력로그 값="true"/>
			</pk>
			""");

			// 맵
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<!-- 호관 -->
				<hex x="92" y="56" 지형="미정"/>
				<hex x="92" y="58" 지형="미정"/>

				<!-- 호로관 -->
				<hex x="90" y="76" 지형="미정"/>
				<hex x="90" y="78" 지형="미정"/>

				<!-- 동관 -->
				<hex x="61" y="76" 지형="미정"/>
				<hex x="61" y="78" 지형="미정"/>

				<!-- 함곡관 -->
				<hex x="68" y="77" 지형="미정"/>
				<hex x="68" y="79" 지형="미정"/>

				<!-- 무관 -->
				<hex x="68" y="85" 지형="미정"/>
				<hex x="68" y="87" 지형="미정"/>

				<!-- 앙평관 -->
				<hex x="26" y="83" 지형="미정"/>
				<hex x="26" y="84" 지형="미정"/>
				<hex x="28" y="83" 지형="미정"/>
				<hex x="28" y="84" 지형="미정"/>

				<!-- 검각 -->
				<hex x="13" y="96" 지형="미정"/>
				<hex x="13" y="97" 지형="미정"/>
				<hex x="15" y="96" 지형="미정"/>
				<hex x="15" y="97" 지형="미정"/>

				<!-- 가맹관 -->
				<hex x="22" y="102" 지형="미정"/>
				<hex x="22" y="103" 지형="미정"/>
				<hex x="24" y="102" 지형="미정"/>
				<hex x="24" y="103" 지형="미정"/>

				<!-- 사수관 -->
				<hex x="11" y="118" 지형="미정"/>
				<hex x="11" y="120" 지형="미정"/>

				<!-- 면죽관 -->
				<hex x="7" y="124" 지형="미정"/>
				<hex x="7" y="125" 지형="미정"/>
				<hex x="9" y="124" 지형="미정"/>
				<hex x="9" y="125" 지형="미정"/>

				<!-- 무위 -->
				<hex x="7" y="53" 구역="21"/>
				<hex x="7" y="54" 구역="21"/>
				<hex x="7" y="55" 구역="21"/>
				<hex x="8" y="53" 구역="21"/>
				<hex x="8" y="54" 구역="21"/>
				<hex x="8" y="55" 구역="21"/>
				<hex x="8" y="56" 구역="21"/>
				<hex x="8" y="57" 구역="21"/>
				<hex x="9" y="52" 구역="21"/>
				<hex x="9" y="53" 구역="21"/>
				<hex x="9" y="54" 구역="21"/>
				<hex x="9" y="55" 구역="21"/>
				<hex x="9" y="56" 구역="21"/>
				<hex x="10" y="52" 구역="21"/>
				<hex x="10" y="53" 구역="21"/>
				<hex x="10" y="54" 구역="21"/>
				<hex x="10" y="55" 구역="21"/>
				<hex x="10" y="56" 구역="21"/>
				<hex x="11" y="51" 구역="21"/>
				<hex x="11" y="52" 구역="21"/>
				<hex x="11" y="53" 구역="21"/>
				<hex x="11" y="54" 구역="21"/>
				<hex x="11" y="55" 구역="21"/>
				<hex x="12" y="51" 구역="21"/>
				<hex x="12" y="52" 구역="21"/>
				<hex x="12" y="53" 구역="21"/>
				<hex x="12" y="54" 구역="21"/>
				<hex x="12" y="55" 구역="21"/>
				<hex x="13" y="50" 구역="21"/>
				<hex x="13" y="51" 구역="21"/>
				<hex x="13" y="52" 구역="21"/>
				<hex x="13" y="53" 구역="21"/>
				<hex x="13" y="54" 구역="21"/>
				<hex x="13" y="55" 구역="21"/>
				<hex x="14" y="51" 구역="21"/>
				<hex x="14" y="52" 구역="21"/>
				<hex x="14" y="53" 구역="21"/>
				<hex x="14" y="54" 구역="21"/>
				<hex x="14" y="55" 구역="21"/>
				<hex x="14" y="56" 구역="21"/>

				<!-- 천수 -->
				<hex x="9" y="57" 구역="20"/>
				<hex x="10" y="57" 구역="20"/>
				<hex x="10" y="58" 구역="20"/>
				<hex x="11" y="56" 구역="20"/>
				<hex x="11" y="57" 구역="20"/>
				<hex x="12" y="56" 구역="20"/>
				<hex x="12" y="57" 구역="20"/>
				<hex x="12" y="58" 구역="20"/>
				<hex x="13" y="56" 구역="20"/>
				<hex x="13" y="57" 구역="20"/>
				<hex x="13" y="58" 구역="20"/>
				<hex x="14" y="57" 구역="20"/>
				<hex x="14" y="58" 구역="20"/>

				<!-- 안정 -->
				<hex x="15" y="55" 구역="19"/>
				<hex x="15" y="56" 구역="19"/>
				<hex x="15" y="57" 구역="19"/>

				<!-- 호로관 -->
				<hex x="90" y="84" 구역="43"/>
				<hex x="90" y="85" 구역="43"/>
				<hex x="90" y="86" 구역="43"/>
				<hex x="90" y="87" 구역="43"/>
				<hex x="91" y="83" 구역="43"/>
				<hex x="91" y="84" 구역="43"/>
				<hex x="91" y="85" 구역="43"/>
				<hex x="91" y="86" 구역="43"/>
				<hex x="92" y="83" 구역="43"/>
				<hex x="92" y="84" 구역="43"/>
				<hex x="93" y="83" 구역="43"/>

				<!-- 완 -->
				<hex x="89" y="84" 구역="16"/>
				<hex x="89" y="85" 구역="16"/>
				<hex x="89" y="86" 구역="16"/>
				<hex x="89" y="87" 구역="16"/>

				<!-- 소패 -->
				<hex x="121" y="76" 구역="9"/>
				<hex x="121" y="77" 구역="9"/>
				<hex x="121" y="78" 구역="9"/>
				<hex x="121" y="79" 구역="9"/>
				<hex x="121" y="80" 구역="9"/>
				<hex x="121" y="81" 구역="9"/>
				<hex x="122" y="77" 구역="9"/>
				<hex x="122" y="78" 구역="9"/>
				<hex x="122" y="79" 구역="9"/>
				<hex x="122" y="80" 구역="9"/>
				<hex x="122" y="81" 구역="9"/>
				<hex x="123" y="77" 구역="9"/>
				<hex x="123" y="78" 구역="9"/>
				<hex x="123" y="79" 구역="9"/>
				<hex x="123" y="80" 구역="9"/>
				<hex x="124" y="78" 구역="9"/>
				<hex x="124" y="79" 구역="9"/>
				<hex x="124" y="80" 구역="9"/>
				<hex x="124" y="81" 구역="9"/>

				<!-- 복양 -->
				<hex x="119" y="77" 구역="11"/>
				<hex x="119" y="78" 구역="11"/>
				<hex x="119" y="79" 구역="11"/>
				<hex x="119" y="80" 구역="11"/>
				<hex x="120" y="77" 구역="11"/>
				<hex x="120" y="78" 구역="11"/>
				<hex x="120" y="79" 구역="11"/>
				<hex x="120" y="80" 구역="11"/>

				<!-- 진류 -->
				<hex x="119" y="81" 구역="12"/>
				<hex x="119" y="82" 구역="12"/>
				<hex x="120" y="81" 구역="12"/>
				<hex x="120" y="82" 구역="12"/>

				<!-- 건업 -->
				<hex x="169" y="115" 구역="22"/>
				<hex x="169" y="116" 구역="22"/>
				<hex x="169" y="117" 구역="22"/>
				<hex x="169" y="118" 구역="22"/>
				<hex x="169" y="119" 구역="22"/>
				<hex x="169" y="120" 구역="22"/>
				<hex x="170" y="113" 구역="22"/>
				<hex x="170" y="114" 구역="22"/>
				<hex x="170" y="115" 구역="22"/>
				<hex x="170" y="116" 구역="22"/>
				<hex x="170" y="117" 구역="22"/>
				<hex x="170" y="118" 구역="22"/>
				<hex x="170" y="119" 구역="22"/>
				<hex x="170" y="120" 구역="22"/>
				<hex x="171" y="111" 구역="22"/>
				<hex x="171" y="112" 구역="22"/>
				<hex x="171" y="113" 구역="22"/>
				<hex x="171" y="114" 구역="22"/>
				<hex x="171" y="115" 구역="22"/>
				<hex x="171" y="116" 구역="22"/>
				<hex x="171" y="117" 구역="22"/>
				<hex x="171" y="118" 구역="22"/>
				<hex x="171" y="119" 구역="22"/>

				<!-- 건업 -->
				<hex x="169" y="103" 구역="22"/>
				<hex x="169" y="104" 구역="22"/>
				<hex x="170" y="103" 구역="22"/>
				<hex x="170" y="104" 구역="22"/>
				<hex x="170" y="105" 구역="22"/>
				<hex x="171" y="101" 구역="22"/>
				<hex x="171" y="102" 구역="22"/>
				<hex x="171" y="103" 구역="22"/>
				<hex x="171" y="104" 구역="22"/>
				<hex x="171" y="105" 구역="22"/>
				<hex x="171" y="106" 구역="22"/>
				<hex x="171" y="107" 구역="22"/>
				<hex x="171" y="108" 구역="22"/>
				<hex x="171" y="109" 구역="22"/>
				<hex x="171" y="110" 구역="22"/>
				<hex x="172" y="102" 구역="22"/>
				<hex x="172" y="103" 구역="22"/>
				<hex x="172" y="104" 구역="22"/>
				<hex x="172" y="105" 구역="22"/>
				<hex x="172" y="106" 구역="22"/>
				<hex x="172" y="107" 구역="22"/>
				<hex x="172" y="108" 구역="22"/>
				<hex x="173" y="102" 구역="22"/>
				<hex x="173" y="103" 구역="22"/>
				<hex x="173" y="104" 구역="22"/>
				<hex x="174" y="103" 구역="22"/>

				<!-- 회계 -->
				<hex x="172" y="115" 구역="24"/>
				<hex x="172" y="116" 구역="24"/>
				<hex x="172" y="117" 구역="24"/>
				<hex x="172" y="118" 구역="24"/>
				<hex x="173" y="115" 구역="24"/>
				<hex x="173" y="116" 구역="24"/>
				<hex x="173" y="117" 구역="24"/>
				<hex x="174" y="116" 구역="24"/>
				<hex x="174" y="117" 구역="24"/>

				<!-- 장사 -->
				<hex x="122" y="161" 구역="31"/>
				<hex x="122" y="162" 구역="31"/>
				<hex x="122" y="163" 구역="31"/>
				<hex x="122" y="164" 구역="31"/>
				<hex x="123" y="157" 구역="31"/>
				<hex x="123" y="158" 구역="31"/>
				<hex x="123" y="159" 구역="31"/>
				<hex x="123" y="160" 구역="31"/>
				<hex x="123" y="161" 구역="31"/>
				<hex x="123" y="162" 구역="31"/>
				<hex x="123" y="163" 구역="31"/>
				<hex x="123" y="164" 구역="31"/>
				<hex x="123" y="165" 구역="31"/>
				<hex x="123" y="166" 구역="31"/>
				<hex x="124" y="158" 구역="31"/>
				<hex x="124" y="159" 구역="31"/>
				<hex x="124" y="160" 구역="31"/>
				<hex x="124" y="161" 구역="31"/>
				<hex x="124" y="162" 구역="31"/>
				<hex x="124" y="163" 구역="31"/>
				<hex x="124" y="164" 구역="31"/>
				<hex x="124" y="165" 구역="31"/>
				<hex x="124" y="166" 구역="31"/>
				<hex x="124" y="167" 구역="31"/>
				<hex x="125" y="157" 구역="31"/>
				<hex x="125" y="158" 구역="31"/>
				<hex x="125" y="159" 구역="31"/>
				<hex x="125" y="160" 구역="31"/>
				<hex x="125" y="161" 구역="31"/>
				<hex x="125" y="162" 구역="31"/>
				<hex x="125" y="163" 구역="31"/>
				<hex x="125" y="164" 구역="31"/>
				<hex x="125" y="165" 구역="31"/>
				<hex x="125" y="166" 구역="31"/>
				<hex x="125" y="167" 구역="31"/>
				<hex x="126" y="157" 구역="31"/>
				<hex x="126" y="158" 구역="31"/>
				<hex x="126" y="159" 구역="31"/>
				<hex x="126" y="160" 구역="31"/>
				<hex x="126" y="161" 구역="31"/>
				<hex x="126" y="162" 구역="31"/>
				<hex x="126" y="163" 구역="31"/>
				<hex x="126" y="164" 구역="31"/>
				<hex x="126" y="165" 구역="31"/>
				<hex x="126" y="166" 구역="31"/>
				<hex x="126" y="167" 구역="31"/>
				<hex x="126" y="168" 구역="31"/>
				<hex x="127" y="157" 구역="31"/>
				<hex x="127" y="158" 구역="31"/>
				<hex x="127" y="159" 구역="31"/>
				<hex x="127" y="160" 구역="31"/>
				<hex x="127" y="161" 구역="31"/>
				<hex x="127" y="162" 구역="31"/>
				<hex x="127" y="163" 구역="31"/>
				<hex x="127" y="164" 구역="31"/>
				<hex x="127" y="165" 구역="31"/>
				<hex x="127" y="166" 구역="31"/>
				<hex x="127" y="167" 구역="31"/>
				<hex x="126" y="168" 구역="31"/>
				<hex x="128" y="165" 구역="31"/>
				<hex x="128" y="166" 구역="31"/>
				<hex x="128" y="167" 구역="31"/>
				<hex x="129" y="166" 구역="31"/>

				<!-- 영안 -->
				<hex x="32" y="122" 구역="35"/>
				<hex x="32" y="123" 구역="35"/>
				<hex x="32" y="124" 구역="35"/>
				<hex x="32" y="125" 구역="35"/>
				<hex x="32" y="126" 구역="35"/>
				<hex x="32" y="127" 구역="35"/>
				<hex x="33" y="121" 구역="35"/>
				<hex x="33" y="122" 구역="35"/>
				<hex x="33" y="123" 구역="35"/>
				<hex x="33" y="124" 구역="35"/>
				<hex x="33" y="125" 구역="35"/>
				<hex x="33" y="126" 구역="35"/>
				<hex x="34" y="121" 구역="35"/>
				<hex x="34" y="122" 구역="35"/>
				<hex x="34" y="123" 구역="35"/>
				<hex x="34" y="124" 구역="35"/>
				<hex x="34" y="125" 구역="35"/>
				<hex x="34" y="126" 구역="35"/>
				<hex x="35" y="120" 구역="35"/>
				<hex x="35" y="121" 구역="35"/>
				<hex x="35" y="122" 구역="35"/>
				<hex x="35" y="123" 구역="35"/>
				<hex x="35" y="124" 구역="35"/>
				<hex x="36" y="120" 구역="35"/>
				<hex x="36" y="121" 구역="35"/>
				<hex x="36" y="122" 구역="35"/>
				<hex x="36" y="123" 구역="35"/>
				<hex x="36" y="124" 구역="35"/>
				<hex x="37" y="119" 구역="35"/>
				<hex x="37" y="120" 구역="35"/>
				<hex x="37" y="121" 구역="35"/>
				<hex x="37" y="122" 구역="35"/>
				<hex x="37" y="123" 구역="35"/>
				<hex x="38" y="120" 구역="35"/>
				<hex x="38" y="121" 구역="35"/>
				<hex x="38" y="122" 구역="35"/>
				<hex x="38" y="123" 구역="35"/>
				<hex x="39" y="120" 구역="35"/>
				<hex x="39" y="121" 구역="35"/>
				<hex x="39" y="122" 구역="35"/>
				<hex x="40" y="120" 구역="35"/>
				<hex x="40" y="121" 구역="35"/>
				<hex x="40" y="122" 구역="35"/>
				<hex x="40" y="123" 구역="35"/>
				<hex x="41" y="115" 구역="35"/>
				<hex x="41" y="116" 구역="35"/>
				<hex x="41" y="117" 구역="35"/>
				<hex x="41" y="118" 구역="35"/>
				<hex x="41" y="119" 구역="35"/>
				<hex x="41" y="120" 구역="35"/>
				<hex x="41" y="121" 구역="35"/>
				<hex x="41" y="122" 구역="35"/>
				<hex x="41" y="123" 구역="35"/>
				<hex x="42" y="115" 구역="35"/>
				<hex x="42" y="116" 구역="35"/>
				<hex x="42" y="117" 구역="35"/>
				<hex x="42" y="118" 구역="35"/>
				<hex x="42" y="119" 구역="35"/>
				<hex x="42" y="120" 구역="35"/>
				<hex x="42" y="121" 구역="35"/>
				<hex x="42" y="122" 구역="35"/>
				<hex x="42" y="123" 구역="35"/>
				<hex x="42" y="124" 구역="35"/>
				<hex x="43" y="114" 구역="35"/>
				<hex x="43" y="115" 구역="35"/>
				<hex x="43" y="116" 구역="35"/>
				<hex x="43" y="117" 구역="35"/>
				<hex x="43" y="118" 구역="35"/>
				<hex x="43" y="119" 구역="35"/>
				<hex x="43" y="120" 구역="35"/>
				<hex x="43" y="121" 구역="35"/>
				<hex x="43" y="122" 구역="35"/>
				<hex x="43" y="123" 구역="35"/>
				<hex x="44" y="115" 구역="35"/>
				<hex x="44" y="116" 구역="35"/>
				<hex x="44" y="117" 구역="35"/>
				<hex x="44" y="118" 구역="35"/>
				<hex x="44" y="119" 구역="35"/>
				<hex x="44" y="120" 구역="35"/>
				<hex x="44" y="121" 구역="35"/>
				<hex x="44" y="122" 구역="35"/>
				<hex x="44" y="123" 구역="35"/>
				<hex x="45" y="115" 구역="35"/>
				<hex x="45" y="116" 구역="35"/>
				<hex x="45" y="117" 구역="35"/>
				<hex x="45" y="118" 구역="35"/>
				<hex x="45" y="119" 구역="35"/>
				<hex x="45" y="120" 구역="35"/>
				<hex x="45" y="121" 구역="35"/>
				<hex x="45" y="122" 구역="35"/>

				<!-- 상용 -->
				<hex x="40" y="102" 구역="18"/>
				<hex x="40" y="103" 구역="18"/>
				<hex x="40" y="104" 구역="18"/>
				<hex x="40" y="105" 구역="18"/>
				<hex x="40" y="106" 구역="18"/>
				<hex x="40" y="107" 구역="18"/>
				<hex x="40" y="108" 구역="18"/>
				<hex x="41" y="101" 구역="18"/>
				<hex x="41" y="102" 구역="18"/>
				<hex x="41" y="103" 구역="18"/>
				<hex x="41" y="104" 구역="18"/>
				<hex x="41" y="105" 구역="18"/>
				<hex x="41" y="106" 구역="18"/>
				<hex x="41" y="107" 구역="18"/>
				<hex x="41" y="108" 구역="18"/>
				<hex x="42" y="98" 구역="18"/>
				<hex x="42" y="99" 구역="18"/>
				<hex x="42" y="100" 구역="18"/>
				<hex x="42" y="101" 구역="18"/>
				<hex x="42" y="102" 구역="18"/>
				<hex x="42" y="103" 구역="18"/>
				<hex x="42" y="104" 구역="18"/>
				<hex x="42" y="105" 구역="18"/>
				<hex x="42" y="106" 구역="18"/>
				<hex x="42" y="107" 구역="18"/>
				<hex x="42" y="108" 구역="18"/>
				<hex x="42" y="109" 구역="18"/>
				<hex x="43" y="100" 구역="18"/>
				<hex x="43" y="101" 구역="18"/>
				<hex x="43" y="102" 구역="18"/>
				<hex x="43" y="103" 구역="18"/>
				<hex x="43" y="104" 구역="18"/>
				<hex x="43" y="105" 구역="18"/>
				<hex x="43" y="106" 구역="18"/>
				<hex x="43" y="107" 구역="18"/>
				<hex x="43" y="108" 구역="18"/>
				<hex x="43" y="109" 구역="18"/>
				<hex x="44" y="103" 구역="18"/>
				<hex x="44" y="104" 구역="18"/>
				<hex x="44" y="105" 구역="18"/>
				<hex x="44" y="106" 구역="18"/>
				<hex x="44" y="107" 구역="18"/>
				<hex x="44" y="108" 구역="18"/>
				<hex x="44" y="109" 구역="18"/>
				<hex x="45" y="105" 구역="18"/>
				<hex x="45" y="106" 구역="18"/>
				<hex x="45" y="107" 구역="18"/>
				<hex x="45" y="108" 구역="18"/>
				<hex x="46" y="106" 구역="18"/>
				<hex x="46" y="107" 구역="18"/>
				<hex x="46" y="108" 구역="18"/>
				<hex x="47" y="106" 구역="18"/>
				<hex x="47" y="107" 구역="18"/>
				<hex x="48" y="107" 구역="18"/>
			</pk>
			""");

			// 방어구역
			pk::load_xml("""
			<pk>
				<방어구역 번호="0">
					<좌표 x="174" y="24"/>
					<좌표 x="175" y="24"/>
					<좌표 x="176" y="25"/>
					<좌표 x="177" y="24"/>
					<좌표 x="178" y="25"/>
				</방어구역>
				<방어구역 번호="1">
					<좌표 x="147" y="31"/>
					<좌표 x="148" y="32"/>
					<좌표 x="149" y="31"/>
					<좌표 x="150" y="32"/>
					<좌표 x="151" y="31"/>
					<좌표 x="152" y="27"/>
					<좌표 x="152" y="28"/>
					<좌표 x="152" y="29"/>
					<좌표 x="152" y="30"/>
					<좌표 x="152" y="31"/>
				</방어구역>
				<방어구역 번호="2">
					<좌표 x="123" y="28"/>
					<좌표 x="124" y="28"/>
					<좌표 x="125" y="27"/>
				</방어구역>
				<방어구역 번호="3">
					<좌표 x="144" y="37"/>
					<좌표 x="145" y="37"/>
					<좌표 x="146" y="37"/>
					<좌표 x="147" y="32"/>
					<좌표 x="147" y="33"/>
					<좌표 x="147" y="34"/>
					<좌표 x="147" y="35"/>
					<좌표 x="147" y="36"/>
				</방어구역>
				<방어구역 번호="4">
					<좌표 x="125" y="61"/>
					<좌표 x="126" y="61"/>
					<좌표 x="126" y="62"/>
					<좌표 x="127" y="60"/>
					<좌표 x="127" y="61"/>
					<좌표 x="128" y="60"/>
					<좌표 x="128" y="61"/>
					<좌표 x="129" y="59"/>
					<좌표 x="129" y="60"/>
					<좌표 x="134" y="56"/>
					<좌표 x="135" y="55"/>
					<좌표 x="135" y="56"/>
					<좌표 x="136" y="55"/>
					<좌표 x="136" y="56"/>
					<좌표 x="136" y="57"/>
					<좌표 x="137" y="54"/>
					<좌표 x="137" y="55"/>
					<좌표 x="137" y="56"/>
					<좌표 x="138" y="54"/>
					<좌표 x="138" y="55"/>
					<좌표 x="138" y="56"/>
					<좌표 x="139" y="53"/>
					<좌표 x="139" y="54"/>
					<좌표 x="139" y="55"/>
					<좌표 x="140" y="52"/>
					<좌표 x="140" y="53"/>
					<좌표 x="140" y="54"/>
					<좌표 x="140" y="55"/>
				</방어구역>
				<방어구역 번호="5">
					<좌표 x="86" y="52"/>
					<좌표 x="87" y="51"/>
					<좌표 x="87" y="52"/>
					<좌표 x="88" y="51"/>
					<좌표 x="88" y="52"/>
					<좌표 x="89" y="50"/>
					<좌표 x="89" y="51"/>
					<좌표 x="90" y="50"/>
					<좌표 x="90" y="51"/>
				</방어구역>
				<방어구역 번호="6">
					<좌표 x="100" y="66"/>
					<좌표 x="101" y="63"/>
					<좌표 x="101" y="64"/>
					<좌표 x="101" y="65"/>
					<좌표 x="102" y="63"/>
					<좌표 x="102" y="64"/>
					<좌표 x="102" y="65"/>
					<좌표 x="103" y="62"/>
					<좌표 x="103" y="63"/>
					<좌표 x="103" y="64"/>
					<좌표 x="104" y="62"/>
					<좌표 x="104" y="63"/>
					<좌표 x="104" y="64"/>
				</방어구역>
				<방어구역 번호="7">
					<좌표 x="163" y="45"/>
					<좌표 x="164" y="45"/>
					<좌표 x="165" y="44"/>
					<좌표 x="166" y="45"/>
					<좌표 x="167" y="44"/>
					<좌표 x="168" y="45"/>
					<좌표 x="168" y="46"/>
					<좌표 x="168" y="47"/>
					<좌표 x="168" y="48"/>
					<좌표 x="168" y="49"/>
					<좌표 x="168" y="50"/>
					<좌표 x="168" y="51"/>
					<좌표 x="169" y="45"/>
					<좌표 x="169" y="46"/>
					<좌표 x="169" y="47"/>
					<좌표 x="169" y="48"/>
					<좌표 x="169" y="49"/>
					<좌표 x="169" y="50"/>
					<좌표 x="170" y="46"/>
					<좌표 x="170" y="47"/>
					<좌표 x="170" y="48"/>
					<좌표 x="170" y="49"/>
					<좌표 x="170" y="50"/>
				</방어구역>
				<방어구역 번호="8">
					<좌표 x="162" y="73"/>
					<좌표 x="162" y="74"/>
					<좌표 x="163" y="68"/>
					<좌표 x="163" y="72"/>
					<좌표 x="163" y="73"/>
					<좌표 x="163" y="74"/>
					<좌표 x="164" y="69"/>
					<좌표 x="164" y="72"/>
					<좌표 x="164" y="73"/>
					<좌표 x="164" y="74"/>
					<좌표 x="164" y="75"/>
					<좌표 x="165" y="69"/>
					<좌표 x="165" y="70"/>
					<좌표 x="165" y="71"/>
					<좌표 x="165" y="72"/>
					<좌표 x="165" y="73"/>
					<좌표 x="165" y="74"/>
					<좌표 x="165" y="75"/>
					<좌표 x="166" y="70"/>
					<좌표 x="166" y="71"/>
					<좌표 x="166" y="72"/>
					<좌표 x="166" y="73"/>
					<좌표 x="166" y="74"/>
					<좌표 x="166" y="75"/>
					<좌표 x="167" y="70"/>
					<좌표 x="167" y="71"/>
					<좌표 x="167" y="72"/>
					<좌표 x="167" y="73"/>
					<좌표 x="167" y="74"/>
					<좌표 x="168" y="71"/>
					<좌표 x="168" y="72"/>
					<좌표 x="168" y="73"/>
					<좌표 x="168" y="74"/>
				</방어구역>
				<방어구역 번호="9">
					<좌표 x="135" y="70"/>
					<좌표 x="135" y="71"/>
					<좌표 x="136" y="70"/>
					<좌표 x="137" y="69"/>
					<좌표 x="138" y="69"/>
				</방어구역>
				<방어구역 번호="11">
					<좌표 x="111" y="67"/>
					<좌표 x="112" y="68"/>
					<좌표 x="127" y="66"/>
					<좌표 x="127" y="67"/>
					<좌표 x="127" y="68"/>
					<좌표 x="128" y="66"/>
					<좌표 x="128" y="67"/>
					<좌표 x="128" y="68"/>
					<좌표 x="129" y="65"/>
					<좌표 x="129" y="66"/>
					<좌표 x="130" y="65"/>
					<좌표 x="130" y="66"/>
					<좌표 x="131" y="65"/>
					<좌표 x="131" y="66"/>
				</방어구역>
				<방어구역 번호="12">
					<좌표 x="101" y="73"/>
					<좌표 x="101" y="74"/>
					<좌표 x="101" y="75"/>
					<좌표 x="101" y="76"/>
					<좌표 x="102" y="73"/>
					<좌표 x="102" y="74"/>
					<좌표 x="102" y="75"/>
					<좌표 x="102" y="76"/>
					<좌표 x="102" y="77"/>
					<좌표 x="103" y="72"/>
					<좌표 x="103" y="73"/>
					<좌표 x="103" y="76"/>
					<좌표 x="103" y="77"/>
					<좌표 x="104" y="73"/>
					<좌표 x="104" y="74"/>
					<좌표 x="104" y="77"/>
					<좌표 x="105" y="73"/>
					<좌표 x="105" y="74"/>
					<좌표 x="105" y="76"/>
					<좌표 x="106" y="74"/>
					<좌표 x="106" y="75"/>
					<좌표 x="106" y="76"/>
					<좌표 x="107" y="74"/>
					<좌표 x="107" y="75"/>
				</방어구역>
				<방어구역 번호="13">
					<좌표 x="89" y="93"/>
					<좌표 x="89" y="94"/>
					<좌표 x="89" y="95"/>
					<좌표 x="89" y="96"/>
					<좌표 x="89" y="97"/>
					<좌표 x="89" y="98"/>
					<좌표 x="102" y="86"/>
					<좌표 x="103" y="85"/>
					<좌표 x="103" y="86"/>
					<좌표 x="104" y="86"/>
					<좌표 x="104" y="87"/>
					<좌표 x="105" y="86"/>
					<좌표 x="105" y="87"/>
					<좌표 x="106" y="87"/>
				</방어구역>
				<방어구역 번호="14">
					<좌표 x="111" y="113"/>
					<좌표 x="112" y="114"/>
					<좌표 x="113" y="114"/>
					<좌표 x="114" y="115"/>
					<좌표 x="115" y="114"/>
					<좌표 x="116" y="114"/>
					<좌표 x="117" y="112"/>
					<좌표 x="117" y="113"/>
				</방어구역>
				<방어구역 번호="15">
					<좌표 x="69" y="75"/>
					<좌표 x="70" y="76"/>
					<좌표 x="71" y="71"/>
					<좌표 x="72" y="71"/>
					<좌표 x="72" y="72"/>
					<좌표 x="73" y="70"/>
					<좌표 x="73" y="71"/>
					<좌표 x="74" y="71"/>
					<좌표 x="74" y="72"/>
					<좌표 x="75" y="71"/>
					<좌표 x="75" y="72"/>
					<좌표 x="81" y="69"/>
					<좌표 x="81" y="70"/>
					<좌표 x="81" y="71"/>
					<좌표 x="82" y="69"/>
					<좌표 x="82" y="70"/>
					<좌표 x="82" y="71"/>
					<좌표 x="83" y="68"/>
					<좌표 x="83" y="69"/>
					<좌표 x="84" y="68"/>
					<좌표 x="84" y="69"/>
					<좌표 x="85" y="68"/>
					<좌표 x="85" y="69"/>
					<좌표 x="86" y="69"/>
					<좌표 x="86" y="70"/>
					<좌표 x="86" y="71"/>
					<좌표 x="86" y="72"/>
					<좌표 x="86" y="73"/>
					<좌표 x="86" y="74"/>
					<좌표 x="87" y="69"/>
					<좌표 x="87" y="70"/>
					<좌표 x="87" y="71"/>
					<좌표 x="87" y="72"/>
					<좌표 x="87" y="73"/>
				</방어구역>
				<방어구역 번호="16">
					<좌표 x="71" y="104"/>
					<좌표 x="72" y="105"/>
					<좌표 x="73" y="104"/>
				</방어구역>
				<방어구역 번호="22">
					<좌표 x="169" y="103"/>
					<좌표 x="169" y="104"/>
					<좌표 x="170" y="104"/>
					<좌표 x="170" y="105"/>
					<좌표 x="171" y="104"/>
					<좌표 x="171" y="105"/>
					<좌표 x="171" y="106"/>
					<좌표 x="171" y="107"/>
					<좌표 x="171" y="108"/>
					<좌표 x="172" y="105"/>
					<좌표 x="172" y="106"/>
					<좌표 x="172" y="107"/>
					<좌표 x="172" y="108"/>
				</방어구역>
				<방어구역 번호="24">
					<좌표 x="189" y="121"/>
					<좌표 x="189" y="122"/>
					<좌표 x="189" y="123"/>
					<좌표 x="190" y="121"/>
					<좌표 x="190" y="122"/>
					<좌표 x="190" y="123"/>
					<좌표 x="190" y="124"/>
					<좌표 x="191" y="120"/>
					<좌표 x="191" y="121"/>
					<좌표 x="191" y="122"/>
					<좌표 x="191" y="123"/>
					<좌표 x="191" y="124"/>
					<좌표 x="192" y="120"/>
					<좌표 x="192" y="121"/>
					<좌표 x="192" y="122"/>
					<좌표 x="192" y="123"/>
					<좌표 x="192" y="124"/>
					<좌표 x="192" y="125"/>
					<좌표 x="193" y="119"/>
					<좌표 x="193" y="120"/>
					<좌표 x="193" y="121"/>
					<좌표 x="193" y="122"/>
					<좌표 x="193" y="123"/>
					<좌표 x="193" y="124"/>
					<좌표 x="193" y="125"/>
					<좌표 x="194" y="119"/>
					<좌표 x="194" y="120"/>
					<좌표 x="194" y="121"/>
					<좌표 x="194" y="122"/>
					<좌표 x="194" y="123"/>
					<좌표 x="194" y="124"/>
					<좌표 x="194" y="125"/>
					<좌표 x="195" y="119"/>
					<좌표 x="195" y="120"/>
					<좌표 x="195" y="121"/>
					<좌표 x="195" y="122"/>
					<좌표 x="195" y="123"/>
					<좌표 x="196" y="120"/>
					<좌표 x="196" y="121"/>
					<좌표 x="196" y="122"/>
				</방어구역>
				<방어구역 번호="25">
					<좌표 x="138" y="121"/>
					<좌표 x="139" y="120"/>
					<좌표 x="140" y="120"/>
					<좌표 x="147" y="111"/>
					<좌표 x="148" y="111"/>
					<좌표 x="148" y="112"/>
					<좌표 x="149" y="111"/>
					<좌표 x="149" y="112"/>
					<좌표 x="150" y="112"/>
					<좌표 x="150" y="113"/>
				</방어구역>
				<방어구역 번호="26">
					<좌표 x="111" y="138"/>
					<좌표 x="111" y="139"/>
					<좌표 x="111" y="140"/>
					<좌표 x="111" y="141"/>
					<좌표 x="111" y="142"/>
					<좌표 x="111" y="143"/>
					<좌표 x="112" y="138"/>
					<좌표 x="112" y="139"/>
					<좌표 x="112" y="140"/>
					<좌표 x="112" y="141"/>
					<좌표 x="112" y="142"/>
					<좌표 x="112" y="143"/>
					<좌표 x="113" y="137"/>
					<좌표 x="113" y="138"/>
					<좌표 x="113" y="139"/>
					<좌표 x="113" y="140"/>
					<좌표 x="113" y="141"/>
					<좌표 x="113" y="142"/>
					<좌표 x="114" y="138"/>
					<좌표 x="114" y="139"/>
					<좌표 x="114" y="140"/>
					<좌표 x="114" y="141"/>
					<좌표 x="114" y="142"/>
					<좌표 x="114" y="143"/>
					<좌표 x="115" y="138"/>
					<좌표 x="115" y="139"/>
					<좌표 x="115" y="140"/>
					<좌표 x="115" y="141"/>
					<좌표 x="115" y="142"/>
					<좌표 x="115" y="143"/>
					<좌표 x="116" y="139"/>
					<좌표 x="116" y="140"/>
					<좌표 x="116" y="141"/>
					<좌표 x="116" y="142"/>
					<좌표 x="116" y="143"/>
					<좌표 x="116" y="144"/>
					<좌표 x="119" y="140"/>
					<좌표 x="119" y="141"/>
					<좌표 x="119" y="142"/>
				</방어구역>
				<방어구역 번호="27">
					<좌표 x="101" y="121"/>
					<좌표 x="102" y="122"/>
					<좌표 x="103" y="122"/>
					<좌표 x="104" y="122"/>
					<좌표 x="105" y="121"/>
					<좌표 x="122" y="121"/>
					<좌표 x="123" y="120"/>
					<좌표 x="124" y="120"/>
					<좌표 x="125" y="119"/>
					<좌표 x="126" y="119"/>
					<좌표 x="127" y="116"/>
					<좌표 x="127" y="117"/>
					<좌표 x="127" y="118"/>
				</방어구역>
				<방어구역 번호="28">
					<좌표 x="83" y="100"/>
					<좌표 x="84" y="100"/>
					<좌표 x="85" y="99"/>
				</방어구역>
				<방어구역 번호="29">
					<좌표 x="64" y="110"/>
					<좌표 x="64" y="111"/>
					<좌표 x="65" y="109"/>
					<좌표 x="65" y="110"/>
					<좌표 x="65" y="111"/>
					<좌표 x="66" y="109"/>
					<좌표 x="66" y="110"/>
					<좌표 x="66" y="111"/>
					<좌표 x="66" y="112"/>
					<좌표 x="67" y="109"/>
					<좌표 x="67" y="110"/>
					<좌표 x="67" y="111"/>
					<좌표 x="67" y="112"/>
					<좌표 x="68" y="110"/>
					<좌표 x="68" y="111"/>
					<좌표 x="68" y="112"/>
					<좌표 x="68" y="113"/>
					<좌표 x="69" y="110"/>
					<좌표 x="69" y="111"/>
					<좌표 x="69" y="112"/>
					<좌표 x="69" y="113"/>
				</방어구역>
				<방어구역 번호="30">
					<좌표 x="75" y="129"/>
					<좌표 x="92" y="130"/>
					<좌표 x="92" y="131"/>
					<좌표 x="92" y="132"/>
					<좌표 x="93" y="129"/>
					<좌표 x="93" y="130"/>
					<좌표 x="93" y="131"/>
					<좌표 x="94" y="129"/>
					<좌표 x="94" y="130"/>
					<좌표 x="94" y="131"/>
					<좌표 x="94" y="132"/>
					<좌표 x="95" y="129"/>
					<좌표 x="95" y="130"/>
					<좌표 x="95" y="131"/>
					<좌표 x="95" y="132"/>
					<좌표 x="96" y="130"/>
					<좌표 x="96" y="131"/>
					<좌표 x="96" y="132"/>
					<좌표 x="96" y="133"/>
					<좌표 x="97" y="130"/>
					<좌표 x="97" y="131"/>
					<좌표 x="97" y="132"/>
					<좌표 x="97" y="133"/>
				</방어구역>
				<방어구역 번호="32">
					<좌표 x="78" y="160"/>
					<좌표 x="79" y="160"/>
					<좌표 x="80" y="161"/>
				</방어구역>
				<방어구역 번호="37">
					<좌표 x="13" y="101"/>
					<좌표 x="14" y="101"/>
					<좌표 x="14" y="102"/>
					<좌표 x="15" y="101"/>
					<좌표 x="15" y="102"/>
					<좌표 x="16" y="102"/>
					<좌표 x="16" y="103"/>
					<좌표 x="16" y="104"/>
					<좌표 x="16" y="105"/>
					<좌표 x="17" y="102"/>
					<좌표 x="17" y="103"/>
					<좌표 x="17" y="104"/>
					<좌표 x="17" y="105"/>
				</방어구역>
				<방어구역 번호="40">
					<좌표 x="14" y="177"/>
					<좌표 x="14" y="178"/>
					<좌표 x="14" y="179"/>
				</방어구역>
			</pk>
			""");

			// 병기
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<병기 번호="0">
					<이동력비용 내="10" 물가="10" 산="10"/>
				</병기>
				<병기 번호="1">
					<이동력비용 내="10" 물가="10" 산="10"/>
				</병기>
				<병기 번호="2">
					<이동력비용 내="10" 물가="10" 산="10"/>
				</병기>
				<병기 번호="3">
					<이동력비용 내="10" 물가="10" 산="10"/>
				</병기>
			</pk>
			""");

			// 보물
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<보물 번호="7">
					<능력치 무력="7"/>
				</보물>
				<보물 번호="8">
					<능력치 무력="6"/>
				</보물>
				<보물 번호="9">
					<능력치 무력="5"/>
				</보물>
				<보물 번호="10">
					<능력치 무력="5"/>
				</보물>
				<보물 번호="11">
					<능력치 무력="8"/>
				</보물>
				<보물 번호="12">
					<능력치 무력="6"/>
				</보물>
				<보물 번호="13">
					<능력치 무력="6"/>
				</보물>
				<보물 번호="14">
					<능력치 무력="3"/>
				</보물>
				<보물 번호="15">
					<능력치 무력="2"/>
				</보물>
				<보물 번호="16">
					<능력치 무력="2"/>
				</보물>
				<보물 번호="17">
					<능력치 무력="2"/>
				</보물>
				<보물 번호="18">
					<능력치 무력="2"/>
				</보물>
				<보물 번호="19">
					<능력치 무력="2"/>
				</보물>
				<보물 번호="20">
					<능력치 무력="2"/>
				</보물>
				<보물 번호="21">
					<능력치 무력="4"/>
				</보물>
				<보물 번호="22">
					<능력치 무력="4"/>
				</보물>
				<보물 번호="23">
					<능력치 무력="4"/>
				</보물>
				<보물 번호="24">
					<능력치 무력="1"/>
				</보물>
				<보물 번호="25">
					<능력치 무력="1"/>
				</보물>
				<보물 번호="26">
					<능력치 무력="2"/>
				</보물>
				<보물 번호="27">
					<능력치 무력="2"/>
				</보물>
				<보물 번호="28">
					<능력치 무력="1"/>
				</보물>
				<보물 번호="29">
					<능력치 무력="1"/>
				</보물>
				<보물 번호="30">
					<능력치 통솔="10"/>
				</보물>
				<보물 번호="31">
					<능력치 통솔="2"/>
				</보물>
				<보물 번호="32">
					<능력치 통솔="2"/>
				</보물>
				<보물 번호="33">
					<능력치 통솔="8"/>
				</보물>
				<보물 번호="34">
					<능력치 통솔="8"/>
				</보물>
				<보물 번호="35">
					<능력치 통솔="6"/>
				</보물>
				<보물 번호="36">
					<능력치 통솔="5"/>
				</보물>
				<보물 번호="37">
					<능력치 정치="3"/>
				</보물>
				<보물 번호="38">
					<능력치 통솔="7"/>
				</보물>
				<보물 번호="39">
					<능력치 통솔="4"/>
				</보물>
				<보물 번호="40">
					<능력치 지력="2"/>
				</보물>
			</pk>
			""");

			// 시설
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<시설 번호="0">
					<최대범위 값="3"/>
					<자동공격 값="true"/>
					<기력상승 값="true"/>
					<공격력상승 값="true"/>
				</시설>
				<시설 번호="1">
					<최대범위 값="3"/>
					<자동공격 값="true"/>
					<기력상승 값="true"/>
					<공격력상승 값="true"/>
				</시설>
				<시설 번호="2">
					<최대범위 값="3"/>
					<자동공격 값="true"/>
					<기력상승 값="true"/>
					<공격력상승 값="true"/>
				</시설>
				<시설 번호="3">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
					<군사시설금지 값="1"/>
				</시설>
				<시설 번호="4">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
					<군사시설금지 값="1"/>
				</시설>
				<시설 번호="5">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
					<군사시설금지 값="1"/>
				</시설>
				<시설 번호="6">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
					<군사시설금지 값="1"/>
				</시설>
				<시설 번호="7">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
					<군사시설금지 값="1"/>
				</시설>
				<시설 번호="8">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
				</시설>
				<시설 번호="9">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
				</시설>
				<시설 번호="10">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
					<군사시설금지 값="1"/>
				</시설>
				<시설 번호="11">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
					<군사시설금지 값="1"/>
				</시설>
				<시설 번호="12">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
					<군사시설금지 값="1"/>
				</시설>
				<시설 번호="13">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
					<최대범위 값="2"/>
					<군사시설금지 값="1"/>
				</시설>
				<시설 번호="14">
					<자동공격 값="true"/>
				</시설>
				<시설 번호="15">
					<자동공격 값="true"/>
				</시설>
				<시설 번호="16">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
				</시설>
				<시설 번호="17">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
				</시설>
				<시설 번호="18">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
				</시설>
				<시설 번호="19">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
				</시설>
				<시설 번호="20">
					<설치가능지형 내="true" 여울="true" 물가="true"/>
				</시설>
				<시설 번호="21">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
				</시설>
				<시설 번호="22">
					<설치가능지형 황무지="true" 산="true" 물가="true"/>
				</시설>
			</pk>
			""");

			// 인공지능
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<인공지능.개발.개발지탐색향상 값="true"/>
				<인공지능.개발.공방강제 값="true"/>
				<인공지능.개발.암시장사용 값="false"/>
				<인공지능.개발.최대시설가중치>
					<전체 병영="127" 대장간="127" 마구간="127" 공방="0" 조선="0" 연병소="0" 대시장="1" 어시장="1"/>
				</인공지능.개발.최대시설가중치>
				<인공지능.개발.최소시설가중치>
					<전체 시장="16"/>
				</인공지능.개발.최소시설가중치>
				<인공지능.개발.최소시설수>
					<소패 공방="0"/>
					<오 조선="0"/>
					<시상 조선="0"/>
					<강릉 조선="0"/>
					<장사 조선="0"/>
				</인공지능.개발.최소시설수>
				<인공지능.개발.특색가중치 마구간="127" 대장간="127" 공방="1" 조선="0"/>
				<인공지능.개발.특수가중치 부절태="127" 인재부="0" 외교부="1" 계략부="1" 연병소="1"/>
				<인공지능.공격.가중치.경로.육로 값="14"/>
				<인공지능.공격.가중치.빈도시 값="80"/>
				<인공지능.공격.같은주우선 값="false"/>
				<인공지능.공격.공격임무사용 값="false"/>
				<인공지능.공격.국력기준 값="0"/>
				<인공지능.공격.기력기준 값="50"/>
				<인공지능.공격.능력우선부대출진 값="true"/>
				<인공지능.공격.도시공략세력확인 값="false"/>
				<인공지능.공격.병량제한 값="false"/>
				<인공지능.공격.보유도시수비례 값="false"/>
				<인공지능.공격.손씨양주우선 값="false"/>
				<인공지능.공격.숙련병기력기준 값="100"/>
				<인공지능.공격.우호도기준 값="80"/>
				<인공지능.공격.최소시설필요 값="false"/>
				<인공지능.공격.플레이어우선 값="true"/>
				<인공지능.기교연구.차축강화우선 값="false"/>
				<인공지능.무장등용.소요무장수한정 값="false"/>
				<인공지능.무장등용.최소무장수한정 값="false"/>
				<인공지능.설치.국력기준 값="0"/>
				<인공지능.설치.보유금기준 값="5000"/>
				<인공지능.수송.비전투거점한정 값="false"/>
				<인공지능.순찰.치안기준 값="90"/>
				<인공지능.순찰.국력기준 값="0"/>
				<인공지능.인재탐색.소요무장수한정 값="false"/>
				<인공지능.인재탐색.최소무장수한정 값="false"/>
				<인공지능.징병.국력기준 값="0"/>
				<인공지능.징병.치안기준 값="89"/>
				<인공지능.타세력무장등용.소요무장수한정 값="false"/>
			</pk>
			""");

			// 적성
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<적성 번호="1">
					<필요경험치 값="0"/>
					<전법성공확률 값="1"/>
				</적성>
				<적성 번호="2">
					<필요경험치 값="50"/>
					<전법성공확률 값="2"/>
				</적성>
				<적성 번호="3">
					<필요경험치 값="100"/>
					<전법성공확률 값="3"/>
				</적성>
				<적성 번호="4">
					<이름 값="S1"/>
					<필요경험치 값="115"/>
					<전법성공확률 값="5"/>
				</적성>
				<적성 번호="5">
					<이름 값="S2"/>
					<필요경험치 값="130"/>
					<전법성공확률 값="6"/>
				</적성>
				<적성 번호="6">
					<이름 값="S3"/>
					<필요경험치 값="150"/>
					<전법성공확률 값="7"/>
				</적성>
				<적성 번호="7">
					<이름 값="S4"/>
					<필요경험치 값="170"/>
					<전법성공확률 값="8"/>
				</적성>
				<적성 번호="8">
					<이름 값="S5"/>
					<필요경험치 값="200"/>
					<전법성공확률 값="9"/>
				</적성>
				<적성 번호="9">
					<이름 값="S6"/>
					<필요경험치 값="210"/>
					<전법성공확률 값="10"/>
				</적성>
				<적성 번호="10">
					<이름 값="S7"/>
					<필요경험치 값="220"/>
					<전법성공확률 값="11"/>
				</적성>
				<적성 번호="11">
					<이름 값="S8"/>
					<필요경험치 값="230"/>
					<전법성공확률 값="12"/>
				</적성>
				<적성 번호="12">
					<이름 값="S9"/>
					<필요경험치 값="240"/>
					<전법성공확률 값="13"/>
				</적성>
				<적성 번호="13">
					<이름 값="X"/>
					<필요경험치 값="250"/>
					<전법성공확률 값="15"/>
				</적성>
			</pk>
			""");

			// 전법
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<전법 번호="12">
					<지형 산="true"/>
				</전법>
				<전법 번호="14">
					<지형 산="true"/>
				</전법>
				<전법 번호="15">
					<지형 내="true" 물가="true" 산="true"/>
				</전법>
				<전법 번호="16">
					<지형 내="true" 물가="true"/>
				</전법>
				<전법 번호="18">
					<지형 내="true" 물가="true" 산="true"/>
				</전법>
			</pk>
			""");

			// 지형
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<지형 번호="4">
					<화계가능 값="true"/>
				</지형>
				<지형 번호="6">
					<이동가능 값="false"/>
					<화계가능 값="false"/>
					<피해종류 값="벼랑길"/>
				</지형>
				<지형 번호="13">
					<화계가능 값="true"/>
				</지형>
				<지형 번호="14">
					<이동가능 값="false"/>
					<화계가능 값="false"/>
					<피해종류 값="벼랑길"/>
				</지형>
				<지형 번호="15">
					<이동가능 값="false"/>
					<화계가능 값="false"/>
					<피해종류 값="벼랑길"/>
				</지형>
				<지형 번호="19">
					<화계가능 값="true"/>
				</지형>
			</pk>
			""");

			// 카메라
			pk::load_xml("""
			<pk utf8="true" trace="true">
				<camera.far 값="1152"/>
				<camera.min_x 값="0"/>
				<camera.max_x 값="5120"/>
				<camera.max_y 값="5120"/>
				<camera.min_angle 값="-89.9999"/>
				<camera.max_angle 값="89.9999"/>
				<camera.zoom_in 값="32"/>
				<camera.zoom_out0 값="752"/>
				<camera.zoom_out1 값="852"/>
				<camera.zoom_out2 값="952"/>
				<camera.through 값="true"/>
			</pk>
			""");
		}
	}

	Main main;
}