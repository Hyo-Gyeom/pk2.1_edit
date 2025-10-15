namespace 부대의_공격_대미지
{
	class Main
	{
		Main()
		{
			pk::set_func(209, pk::func209_t(callback));
		}

		void callback(pk::damage_info& info, pk::unit@ attacker, int tactics_id, const pk::point &in target_pos, int type, int critical, bool ambush)
		{
			int force_id = attacker.get_force_id();
			pk::force@ force = pk::get_force(force_id);
			int weapon_id = attacker.weapon;
			int command = func_5aee10(attacker);
			pk::unit@ target_unit = pk::get_unit(target_pos);
			pk::building@ target_building = pk::get_building(target_pos);
			int atk = 0;
			int troops_atk = 0;
			int buffed = 0;

			// 공격 능력치
			if (ambush)
				atk = pk::max(attacker.attr.stat[부대능력_지력] - 5, 1);
			else
				atk = attacker.attr.stat[부대능력_공격];

			// 병력 공격력, 전법 결과
			if (pk::is_valid_tactics_id(tactics_id))
			{
				troops_atk = pk::get_tactics(tactics_id).troops_atk;
				info.tactics_hit = pk::rand_bool(cast<pk::func202_t>(pk::get_func(202))(attacker, attacker.get_pos(), pk::get_hex_object(target_pos), tactics_id));
			}
			else if (pk::is_neighbor_pos(attacker.get_pos(), target_pos))
			{
				troops_atk = 10;
			}

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
				if (func_5aedc0(target_unit.get_pos(), 1, pk::get_facility(facility_id).max_range, target_unit.get_force_id()))
					info.debuffer = facility_id;
			}

			// 크리티컬
			info.critical = false;
			if (critical == 0)
			{
				if (pk::is_valid_tactics_id(tactics_id) and not info.tactics_hit)
					info.critical = false;
				else if (type == 0)
					info.critical = cast<pk::func201_t@>(pk::get_func(201))(attacker, pk::get_hex_object(target_pos), tactics_id, !pk::is_neighbor_pos(attacker.get_pos(), target_pos));
			}
			else if (critical == 1)
			{
				info.critical = true;
			}

			float 병기상성_강 = float(pk::core["병기상성.강"]); // 1.15f
			float 병기상성_약 = float(pk::core["병기상성.약"]); // 0.85f
			float 정예기교_공격력 = 1.15f;
			float 단련기교_공격력 = 1.10f;
			float 크리티컬_공격력 = 1.15f;

			if (target_unit !is null)
			{
				info.dst_troops = target_unit.troops;

				float troops_damage = func_5aee60(atk, command, troops_atk, buffed, target_unit.attr.stat[부대능력_방어], func_5aee10(target_unit));

				switch (weapon_id)
				{
				case 병기_창:
					if (pk::is_valid_tactics_id(tactics_id))
					{
						if (target_unit.weapon == 병기_극)
							troops_damage *= 병기상성_약;
						else if (target_unit.weapon == 병기_군마)
							troops_damage *= 병기상성_강;
					}

					if (pk::has_tech(force, 기교_정예창병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_창병단련))
						troops_damage *= 단련기교_공격력;
					break;

				case 병기_극:
					if (pk::is_valid_tactics_id(tactics_id))
					{
						if (target_unit.weapon == 병기_창)
							troops_damage *= 병기상성_강;
						else if (target_unit.weapon == 병기_군마)
							troops_damage *= 병기상성_약;
					}

					if (pk::has_tech(force, 기교_정예극병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_극병단련))
						troops_damage *= 단련기교_공격력;
					break;

				case 병기_노:
					if (pk::is_valid_tactics_id(tactics_id) and pk::equipment_id_to_heishu(target_unit.weapon) == 병종_병기)
						troops_damage *= 병기상성_약;

					if (pk::has_tech(force, 기교_정예노병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_노병단련))
						troops_damage *= 단련기교_공격력;
					break;

				case 병기_군마:
					if (pk::is_valid_tactics_id(tactics_id))
					{
						if (target_unit.weapon == 병기_창)
							troops_damage *= 병기상성_약;
						else if (target_unit.weapon == 병기_극)
							troops_damage *= 병기상성_강;
					}

					if (pk::has_tech(force, 기교_정예기병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_기병단련))
						troops_damage *= 단련기교_공격력;
					break;

				case 병기_정란:
				case 병기_주가:
				case 병기_누선:
				case 병기_투함:
					if (pk::is_valid_tactics_id(tactics_id) and pk::equipment_id_to_heishu(target_unit.weapon) == 병종_병기)
						troops_damage *= 0.8f;
					break;
				}

				if (type == 0)
				{
					if (info.critical)
						troops_damage *= 크리티컬_공격력;
					
					info.troops_damage = troops_damage;
					if (func_5af230(info, attacker, target_unit, tactics_id))
						troops_damage = 0;

					if (attacker.has_skill(특기_소탕))
						info.energy_damage = pk::max(info.energy_damage, int(pk::core::skill_constant(attacker, 특기_소탕))); // 5

					if (attacker.has_skill(특기_위풍))
						info.energy_damage = pk::max(info.energy_damage, int(pk::core::skill_constant(attacker, 특기_위풍))); // 20
					
					// 위풍, 소탕 특기가 기력 떨어진 부대에 10% 추가 피해 (특기종합패치)
					if (target_unit.energy == 0)
					{
						if (attacker.has_skill(특기_위풍)) 
							troops_damage *= 1.10f;
						if (attacker.has_skill(특기_소탕))
							troops_damage *= 1.05f;
					}					
					
					if (attacker.has_skill(특기_맹장)) 
						troops_damage *= 1.10f;

					info.food_damage = func_5aecc0(attacker, target_unit);
					info.food_heal = info.food_damage;
				}
				else
				{
					if (type == 1)
						troops_damage *= 0.9f;
					else if (type == 2)
						troops_damage *= 0.4f;

					// 반격이기 때문에 target_unit은 공격을 실행한 부대를 뜻함.
					if (pk::is_in_water(target_unit))
					{
						if (target_unit.has_skill(특기_강습) and pk::rand_bool(int(pk::core::skill_constant(target_unit, 특기_강습))))
						{
							troops_damage = 0;
							info.def_skill = 특기_강습;
						}
					}
					else
					{
						if (target_unit.has_skill(특기_급습) and pk::rand_bool(int(pk::core::skill_constant(target_unit, 특기_급습))))
						{
							troops_damage = 0;
							info.def_skill = 특기_급습;
						}
					}
				}

				pk::force@ target_force = pk::get_force(target_unit.get_force_id());
				if (pk::has_tech(target_force, 기교_숙련병))
					troops_damage *= 0.9f;

				if (pk::has_tech(force, 기교_숙련병))
					troops_damage *= 1.1f;

				if (target_unit.has_skill(특기_등갑))
					troops_damage = troops_damage / 2;
				
				if (target_unit.has_skill(특기_철벽))
					troops_damage = troops_damage * 0.8f;

				if (info.debuffer == 시설_진)
					troops_damage *= 0.90f;
				else if (info.debuffer == 시설_요새)
					troops_damage *= 0.80f;
				else if (info.debuffer == 시설_성채)
					troops_damage *= 0.70f;
	
				if (attacker.is_player())
					troops_damage *= float(pk::core["대미지패널티"][pk::get_scenario().difficulty]);
						
				info.troops_damage = troops_damage;
			}
			else if (target_building !is null and pk::is_general_type(target_building))
			{
				info.dst_troops = pk::get_troops(target_building);

				int facility_id = target_building.facility;
				int troops_atk, dst_atk, dst_def, dst_troops;
				func_5af0e0(troops_atk, dst_atk, dst_def, dst_troops, target_building);
				float troops_damage = func_5aee60(atk, command, troops_atk, buffed, dst_def, dst_troops);

				if (attacker.weapon == 병기_정란)
				{
					troops_damage *= 1.5f;
				}
				else if (attacker.weapon == 병기_투석)
				{
					troops_damage *= 1.5f;
				}
				else
				{
					troops_damage = func_5aee60(atk, pk::max(command * 0.75f, 1.f), troops_atk, buffed, dst_def, dst_troops);
					if (facility_id == 시설_도시)
						troops_damage *= 0.50f;
					else if (facility_id == 시설_관문)
						troops_damage *= 0.30f;
					else if (facility_id == 시설_항구)
						troops_damage *= 0.40f;
				}

				if (type == 0)
				{
					if (info.critical)
						troops_damage *= 크리티컬_공격력;
				}
				else
				{
					if (type == 1)
						troops_damage *= 0.9f;
					else if (type == 2)
						troops_damage *= 0.4f;

					// 반격이기 때문에 target_unit은 공격을 실행한 부대를 뜻함.
					if (pk::is_in_water(target_unit))
					{
						if (target_unit.has_skill(특기_강습) and pk::rand_bool(int(pk::core::skill_constant(target_unit, 특기_강습))))
						{
							troops_damage = 0;
							info.def_skill = 특기_강습;
						}
					}
					else
					{
						if (target_unit.has_skill(특기_급습) and pk::rand_bool(int(pk::core::skill_constant(target_unit, 특기_급습))))
						{
							troops_damage = 0;
							info.def_skill = 특기_급습;
						}
					}
				}
								
				if (attacker.is_player())
					troops_damage *= float(pk::core["대미지패널티"][pk::get_scenario().difficulty]);

				info.troops_damage = troops_damage;
			}

			if (target_building !is null)
			{
				int facility_id = target_building.facility;
				int hp_atk = 0;
				float hp_damage = 0;

				pk::trace("start info.troops_damage : " + info.troops_damage);

				hp_atk = 5;
				if (pk::is_valid_tactics_id(tactics_id))
					hp_atk = pk::get_tactics(tactics_id).hp_atk;
				else if (pk::is_neighbor_pos(attacker.get_pos(), target_pos))
					hp_atk = 15;

				if (attacker.weapon == 병기_충차 or attacker.weapon == 병기_목수)
					hp_damage = func_5af050(atk, command, hp_atk, buffed);
				else
					hp_damage = func_5aeff0(atk, command, hp_atk, buffed);

				if (facility_id >= 시설_불씨 and facility_id <= 시설_업화종)
				{
					hp_damage *= 2.0f;
				}
				else if (facility_id >= 시설_시장 and facility_id <= 시설_조선3단)
				{
					hp_damage *= 1.1f;
				}
				else if (facility_id == 시설_제방)
				{
					hp_damage *= 0.7f;
				}
				else if (attacker.weapon != 병기_충차 and attacker.weapon != 병기_목수)
				{

					switch (facility_id)
					{			
					case 시설_도시:
						break;
					case 시설_항구:
						break;
					case 시설_관문:
						info.troops_damage *= 0.5f;
						hp_damage *= 0.5f;
						break;
					default:
						info.troops_damage *= 0.9f;
						hp_damage *= 0.7f;
						break;
					}

				}

				if (facility_id == 시설_제방 and not target_building.completed)
					hp_damage = 0;
				
				pk::trace("start info.troops_damage : " + info.troops_damage);
				pk::trace("start hp_damage  : " + hp_damage);

				// 태수 능력
				pk::building@ building = target_building;
				if (building !is null) 
				{
					if (facility_id == 시설_도시 or facility_id == 시설_항구 or facility_id == 시설_관문)
					{	
						pk::city@ city = pk::building_to_city(target_building);
						pk::person@ taishu = pk::get_person(city.taishu);

						float taishu_troops_damage = 0;
						float taishu_hp_damage = 0;
						int attacker_atk = attacker.attr.stat[부대능력_공격];	

						if (pk::is_alive(taishu))
						{
				        		float taishu_status = taishu.stat[무장능력_통솔] * 2
                        							+ taishu.stat[무장능력_무력]
					                		        + taishu.stat[무장능력_지력];
							// 능력치 최고값 125 * 4 최대 250(500를 기준으로 능력치에 따라 병력공격력 감소
							taishu_troops_damage =  taishu_status / 2.f;
							// 능력치 최고값 125 * 4 각 병기 125, 250을 기준으로 능력치에 따라 내구공격력 감소
							if (attacker.weapon == 병기_충차 or attacker.weapon == 병기_목수)
								taishu_hp_damage =  taishu_status / 4.f;
							else
								taishu_hp_damage =  taishu_status / 2.f;

							pk::trace("attacker_atk : " + attacker_atk);
							pk::trace("taishu_troops_damage : " + taishu_troops_damage);

							info.troops_damage += attacker_atk - taishu_troops_damage;
							hp_damage += (attacker_atk * 0.5) - taishu_hp_damage;

							pk::trace("add taishu troops : " + info.troops_damage);
							pk::trace("add taishu_hp : " + hp_damage);

							// 부대 공격력 능력 적용
							if (0 >= info.troops_damage)
							{						
								// 최소 데미지 넘지 못 할 경우 최소데미지
								info.troops_damage = pk::max(200.f, float(info.troops_damage));
								hp_damage = pk::max(50.f, float(hp_damage));

								pk::trace("- info.troops_damage : " + info.troops_damage);
								pk::trace("- hp_damage : " + hp_damage);
							}

							// 방어 스킬 적용
							if (building.has_skill(특기_공신) or building.has_skill(특기_신산))
							{
								info.troops_damage *= 0.77f;
								hp_damage *= 0.77f;
							}
							else if (building.has_skill(특기_공성) or building.has_skill(특기_화신))
							{
								info.troops_damage *= 0.85f;
								hp_damage *= 0.85f;
							}

							if (building.has_skill(특기_통찰))
							{
								info.troops_damage *= 0.85f;
								hp_damage *= 0.85f;
							}
							else if (target_unit.has_skill(특기_명경) or target_unit.has_skill(특기_규율) or target_unit.has_skill(특기_침착))
							{
								info.troops_damage *= 0.91f; 
								hp_damage *= 0.91f;
							}

							if (building.has_skill(특기_불굴) or building.has_skill(특기_금강) or building.has_skill(특기_둔전))
							{
								info.troops_damage *= 0.91f; 
								hp_damage *= 0.91f;
							}

							if (building.has_skill(특기_철벽) or building.has_skill(특기_정묘))
							{
								info.troops_damage *= 0.85f;
								hp_damage *= 0.85f; 
							}

							if (building.has_skill(특기_위압))
							{
								info.troops_damage *= 0.85f;
								hp_damage *= 0.85f; 
							}

							if (building.has_skill(특기_축성))
							{
								info.troops_damage *= 0.91f; 
								hp_damage *= 0.85f; 
							}
						}
					}
				}
				pk::trace("add target troops skill atk : " + info.troops_damage);
				pk::trace("add target hp skill atk : " + hp_damage);

				// 부대 스킬 공격력
				if (attacker.has_skill(특기_공신) or attacker.has_skill(특기_신산))
				{
					info.troops_damage *= 1.3f;
					hp_damage *= 1.3f;
				}
				else if (attacker.has_skill(특기_공성) or attacker.has_skill(특기_화신))
				{
					info.troops_damage *= 1.2f;
					hp_damage *= 1.2f;
				}
				if (attacker.has_skill(특기_사정) or attacker.has_skill(특기_발명))
				{
					info.troops_damage *= 1.2f;				
					hp_damage *= 1.2f;
				}
						
				if (attacker.has_skill(특기_패왕) or attacker.has_skill(특기_비장) or attacker.has_skill(특기_신장) or attacker.has_skill(특기_용장) or attacker.has_skill(특기_궁신))
				{
					info.troops_damage *= 1.3f;
					hp_damage *= 1.3f;
				} 
				else if (attacker.has_skill(특기_창신) or attacker.has_skill(특기_극신) or attacker.has_skill(특기_궁장))
				{
					info.troops_damage *= 1.2f;
					hp_damage *= 1.2f;
				}
				else if (attacker.has_skill(특기_창장) or attacker.has_skill(특기_극장)) 
				{
					info.troops_damage *= 1.1f;
					hp_damage *= 1.1f;
				}
				if (attacker.has_skill(특기_사수) or attacker.has_skill(특기_백마))
					info.troops_damage *= 1.2f;
			
				if (attacker.has_skill(특기_위풍) or attacker.has_skill(특기_구축) or attacker.has_skill(특기_질주))
				{
					info.troops_damage *= 1.2f;				
					hp_damage *= 1.2f;
				}
				
				if (attacker.has_skill(특기_난전) or attacker.has_skill(특기_맹장) or attacker.has_skill(특기_급습) or attacker.has_skill(특기_위압))
				{
					info.troops_damage *= 1.2f;				
					hp_damage *= 1.2f;
				}

				if (attacker.has_skill(특기_정묘) or attacker.has_skill(특기_기각))
				{
					info.troops_damage *= 1.2f;				
					hp_damage *= 1.2f;
				}			
			
				pk::trace("add attacker troops skill atk : " + info.troops_damage);
				pk::trace("add attacker hp skill atk : " + hp_damage);						
				if (attacker.has_tech(기교_강노) or attacker.has_tech(기교_기사))
				{
					info.troops_damage *= 1.2f;
					hp_damage *= 1.2f;
				}

				if (attacker.has_tech(기교_숙련병))
				{	
					info.troops_damage *= 1.1f;
					hp_damage *= 1.1f;					
				}
				if (attacker.has_tech(기교_난소행군))
				{	
					info.troops_damage *= 1.2f;
					hp_damage *= 1.2f;					
				}

				if (attacker.has_tech(기교_공병육성))
				{	
					info.troops_damage *= 1.2f;
					hp_damage *= 1.2f;					
				}

				if (attacker.has_tech(기교_군제개혁))
				{
					if (attacker.weapon >= 병기_검 and attacker.weapon <= 병기_군마)
					{
						info.troops_damage *= 1.2f;
						hp_damage *= 1.2f;
					}
				}

				if (attacker.has_tech(기교_운제))
				{
					info.troops_damage *= 1.4f;
					hp_damage *= 1.2f;
				}

				if (attacker.weapon >= 병기_충차 and attacker.weapon <= 병기_목수)
				{										
					if (attacker.has_tech(기교_투석개발))					
					{
						info.troops_damage *= 1.4f;
						hp_damage *= 1.4f;
					}
		
					if (attacker.has_tech(기교_벽력))
					{
						info.troops_damage *= 1.2f;
						hp_damage *= 1.2f;
					}
				}

				pk::trace("add attacker troops tech atk : " + info.troops_damage);
				pk::trace("add attacker hp tech atk : " + hp_damage);
		
				// 기교연구로 항관확장 익혔을 경우 거점 피해량 설정.
               			if (pk::has_tech(target_building, 기교_항관확장)) 
				{
					if (facility_id == 시설_항구 or facility_id == 시설_관문)
					{
						info.troops_damage *= 0.5f;
						hp_damage *= 0.5f;
					}
				}
				
				// 기교연구로 화살방패 익혔을 경우 거점 피해량 설정
               			if (pk::has_tech(target_building, 기교_화살방패)) 
				{
					info.troops_damage = info.troops_damage * 0.85f;
					hp_damage = hp_damage * 0.85f;					
				}

				// 기교연구로 숙련병 익혔을 경우 거점 피해량 설정
               			if (pk::has_tech(target_building, 기교_숙련병)) 
				{
					info.troops_damage = info.troops_damage * 0.91f;
					hp_damage = hp_damage * 0.91f;					
				}

				// 기교연구로 석조건축 익혔을 경우 거점 피해량 설정
               			if (pk::has_tech(target_building, 기교_석조건축)) 
				{
					info.troops_damage = info.troops_damage * 0.77f;
					hp_damage = hp_damage * 0.77f;					
				}

				// 기교연구로 공병육성 익혔을 경우 거점 피해량 설정
               			if (pk::has_tech(target_building, 기교_공병육성)) // 기교_공병육성
				{
					info.troops_damage = info.troops_damage * 0.85f;
					hp_damage = hp_damage * 0.85f;
				}

				// 기교연구로 시설강화 시 설정
				if (pk::has_tech(target_building, 기교_시설강화))
				{
					info.troops_damage = info.troops_damage * 0.85f;
					hp_damage = hp_damage * 0.85f;	
				}
            		   	// 기교연구로 성벽강화 시 설정
				if (pk::has_tech(target_building, 기교_성벽강화))
				{
					info.troops_damage = info.troops_damage * 0.85f;
					hp_damage = hp_damage * 0.85f;
				}

            			 // 기교연구로 방어강화 시 설정               			
				if (pk::has_tech(target_building, 기교_방어강화))
				{
					info.troops_damage = info.troops_damage * 0.85f;
					hp_damage = hp_damage * 0.85f;					
				}

				pk::trace("add target troops tech atk : " + info.troops_damage);
				pk::trace("add target hp tech atk : " + hp_damage);

				if (info.critical)
					hp_damage *= 크리티컬_공격력;

				if (attacker.is_player())
				{
					hp_damage *= float(pk::core["대미지패널티"][pk::get_scenario().difficulty]);
					info.troops_damage *= float(pk::core["대미지패널티"][pk::get_scenario().difficulty]);
				}
				
				pk::trace("last troops atk : " + info.troops_damage);
				pk::trace("last hp atk : " + hp_damage);
				pk::trace("===========================================");

				info.hp_damage = hp_damage;
			}

			info.tactics = tactics_id;
			info.src_pos = attacker.get_pos();
			info.dst_pos = target_pos;
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
		bool func_5aedc0(const pk::point &in pos, int min, int max, int force_id)
		{
			array<pk::point> range = pk::range(pos, min, max);
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
		float func_5aeff0(int src_atk, int src_troops, int tactics_atk, int buffed)
		{
			pk::trace("src_troops : " +src_troops);
			pk::trace("src_atk : " + src_atk);
			pk::trace("tactics_atk : " + tactics_atk);
			pk::trace("buffed : " + buffed);

			src_troops = pk::max(src_troops, 1);
			float a = sqrt(src_atk * src_atk / 15.f);
			float b = sqrt(src_troops);
			float c = tactics_atk * 4 + 100;
			float d = buffed + 10;
			return a * b * c * d / 10000;
		}

		/***/
		float func_5af050(int src_atk, int src_troops, int tactics_atk, int buffed)
		{
			src_troops = pk::max(src_troops, 1);
			float a = sqrt(src_atk * src_atk / 15.f);
			float b = sqrt(src_troops);
			float c = tactics_atk * 4 + 100;
			float d = buffed + 10;
			return a * pk::min(b, 40.f) * c * d / 10000 + (src_troops / 25) + b;
		}

		/***/
		void func_5af0e0(int &out src_atk, int &out dst_atk, int &out dst_def, int &out dst_troops, pk::building@ building)
		{
			src_atk = 10;
			pk::person@ taishu = pk::get_person(pk::get_taishu_id(building));
			if (pk::is_alive(taishu))
			{
				dst_atk = pk::max(taishu.stat[무장능력_무력] - 25, 40);
				dst_def = pk::max(taishu.stat[무장능력_통솔] - 25, 40);
				dst_troops = pk::min(pk::get_troops(building), pk::get_command(taishu));
			}
			else
			{
				dst_atk = 30;
				dst_def = 30;
				dst_troops = pk::min(pk::get_troops(building), 7000);
				if (dst_troops != 0)
					dst_troops = pk::max(dst_troops, 500);
			}
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