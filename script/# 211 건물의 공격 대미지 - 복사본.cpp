namespace 건물의_공격_대미지
{
	class Main
	{
		Main()
		{
			pk::set_func(211, pk::func211_t(callback));
		}

		void callback(pk::damage_info& info, pk::building@ attacker, pk::hex_object@ target)
		{
			// 사용하지 않음
			if (target.is_instance(pk::building::type_id))
				pk::assert(false);

			int facility_id = attacker.facility;
			pk::unit@ target_unit = pk::hex_object_to_unit(target);
			int troops_atk = 0, atk = 0, def = 0, troops = 0;
			
			// 군사 백명당 1 보조
			info.troops_damage += int(attacker.troops / 1000);

			// 태수의 능력
			if (attacker !is null) 
			{
				pk::city@ city = pk::building_to_city(attacker);
				if (city !is null)
				{
					pk::person@ taishu = pk::get_person(city.taishu);
					if (pk::is_alive(taishu))
					{
					        float taishu_status = taishu.stat[무장능력_통솔] * 2
                	        					+ taishu.stat[무장능력_무력] * 2
						                        + taishu.stat[무장능력_지력] * 2
									+ taishu.stat[무장능력_정치]
									+ taishu.stat[무장능력_매력];
						// 능력치 최고값 125 * 8 최대 500을 기준으로 능력치에 따라 병력공격력 증가 
						info.troops_damage +=  taishu_status / 2.f;	
					}
				}

				// 특기 감지
				if (attacker.has_skill(특기_공신) or attacker.has_skill(특기_신산))
					info.troops_damage *= 1.3f;
				else if (attacker.has_skill(특기_공성) or attacker.has_skill(특기_화신)) 
					info.troops_damage *= 1.2f;

				if (attacker.has_skill(특기_사수) or target_unit.has_skill(특기_정묘))
					info.troops_damage *= 1.2f;

				if (attacker.has_skill(특기_위압))
					info.troops_damage *= 1.2f;
			}

			switch (facility_id)
			{
				case 시설_도시:
					break;
				case 시설_항구:
					break;
				case 시설_관문:
					info.troops_damage *= 2.f;
					break;
				default:
					info.troops_damage = func_5af370(attacker, target_unit);
					break;
			}

			if (target_unit.has_skill(특기_공신) or target_unit.has_skill(특기_신산))
				info.troops_damage *= 0.7f;
			else if (target_unit.has_skill(특기_공성) or target_unit.has_skill(특기_화신))
				info.troops_damage *= 0.8f;

			if (target_unit.has_skill(특기_등갑))
				info.troops_damage *= 0.5f;			

			if (target_unit.has_skill(특기_통찰))
				info.troops_damage *= 0.8f;
			else if (target_unit.has_skill(특기_명경))
				info.troops_damage *= 0.9f;
			else if (target_unit.has_skill(특기_규율) or target_unit.has_skill(특기_침착))
				info.troops_damage *= 0.95f;

			if (target_unit.has_skill(특기_철벽) or target_unit.has_skill(특기_통찰) or target_unit.has_skill(특기_정묘))
				info.troops_damage *= 0.8f;
			else if (target_unit.has_skill(특기_불굴) or target_unit.has_skill(특기_금강))
				info.troops_damage *= 0.9f;
			
			pk::force@ target_force = pk::get_force(target_unit.get_force_id());
			if (pk::has_tech(target_force, 기교_숙련병))
				info.troops_damage *= 0.9f;
			
			// 항관확장 기교 연구 시 관문, 항구의 공격력 2배
			if (attacker.has_tech(기교_항관확장) and (facility_id == 시설_항구 or facility_id == 시설_관문))
				info.troops_damage *= 2.f;

			// 숙련병 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.1배
			if (attacker.has_tech(기교_숙련병) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				info.troops_damage *= 1.1f;

			// 군제개혁 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.1배
			if (attacker.has_tech(기교_군제개혁) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				info.troops_damage *= 1.2f;

			// 투석개발 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.4배
			if (attacker.has_tech(기교_투석개발) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				info.troops_damage *= 1.4f;

			// 벽력 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.4배
			if (attacker.has_tech(기교_벽력) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				info.troops_damage *= 1.3f;
				
			// 공병육성 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.2배
			if (attacker.has_tech(기교_공병육성) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				info.troops_damage *= 1.2f;
			// 시설강화 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.2배
			if (attacker.has_tech(기교_시설강화) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				info.troops_damage *= 1.2f;
			// 성벽강화 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.2배
			if (attacker.has_tech(기교_성벽강화) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				info.troops_damage *= 1.2f;
			// 방어강화 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.2배
			if (attacker.has_tech(기교_방어강화) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				info.troops_damage *= 1.2f;
			
		
//			if (attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
//				info.troops_damage = pk::max(info.troops_damage, 500);						

			if (pk::equipment_id_to_heishu(target_unit.weapon) == 병종_병기)
				info.troops_damage *= 0.5f;
			
			if (attacker.is_player())
				info.troops_damage *= float(pk::core["대미지패널티"][pk::get_scenario().difficulty]);
				
	
			info.src_pos = attacker.get_pos();
			info.dst_pos = target.get_pos();
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
	
		/**
			군사시설의 공격력 계산
		*/
		int func_5af370(pk::building@ attacker, pk::unit@ target)
		{
			int atk = 0;
			int bonus = 0;

			switch (attacker.facility)
			{
			case 시설_진:
				atk = 500;
				bonus = 300;
				break;
			case 시설_요새:
				atk = 700;
				bonus = 300;
				break;
			case 시설_성채:
				atk = 1000;
				bonus = 300;
				break;
			case 시설_궁노:
				atk = 500;
				bonus = 300;
				break;
			case 시설_연노로:
				atk = 700;
				bonus = 400;
				break;
			case 시설_투석대:
				atk = 1000;
				bonus = 600;
				break;
			default:
				// 시설.자동공격 대미지
				atk = 300;
				bonus = 300;
				break;
			}

			// 체력 비율에 따라 공격력 보정
   			float hp_ratio = float(attacker.hp) / float(pk::get_max_hp(attacker));

			if (hp_ratio >= (2.f / 3.f))
				atk = atk - int(target.attr.stat[부대능력_방어] * 2.f) + pk::rand(bonus);
			else if (hp_ratio >= (1.f / 3.f))
				atk = atk - int(target.attr.stat[부대능력_방어] * 3.f) + pk::rand(bonus);
			else 
				atk = atk - int(target.attr.stat[부대능력_방어] * 4.f) + pk::rand(bonus);
	
			return atk;
/**
			float max_hp = pk::max(pk::get_max_hp(attacker), 1);
			float hp = pk::max(attacker.hp, max_hp / 2);

			return hp / max_hp * (152 - int(target.attr.stat[부대능력_방어])) * atk / 132 + pk::rand(bonus);
*/			
		}
	}

	Main main;
}