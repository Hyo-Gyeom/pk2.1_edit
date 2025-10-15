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
			pk::person@ taishu = pk::get_person(pk::get_taishu_id(attacker));
			int troops_atk = 0, atk = 0, def = 0, troops = 0;
			
			float base_atk = 0.f;
			float total_atk = 0.f;
									
			switch (facility_id)
			{
				case 시설_도시:
					break;
				case 시설_항구:
					break;
				case 시설_관문:
					break;
				default:
					base_atk = func_5af370(attacker, target_unit);
					break;
			}

			// 도시, 항구, 관문
			if (facility_id == 시설_도시 or facility_id == 시설_항구 or facility_id == 시설_관문)
			{	
				int taishu_dam = 0;
				int tro_dam = 0;

				// 군사 백명당 1 기본 공격력 1부대를 한번에 공격할 수 없으니 최대 3만으로 설정
				if (attacker.troops != 0)
					tro_dam = int(attacker.troops / 100);
				// 병력으로 베이스 공격력 설정
				base_atk = pk::min(tro_dam, 300);

				// 태수 능력 반영
				if (pk::is_alive(taishu))
				{	
					// 능력치 최고값 125 * 8 최대 400(1000 * 0.4)을 기준으로 능력치에 따라 병력공격력 증가 	
			        	float taishu_status = taishu.stat[무장능력_통솔] * 4
               	        					+ taishu.stat[무장능력_무력] 
				                	        + taishu.stat[무장능력_지력]
								+ taishu.stat[무장능력_정치]
								+ taishu.stat[무장능력_매력];
					
					taishu_status *= 0.4f;

					// 타겟 부대 방어능력 적용
					int target_def = target_unit.attr.stat[부대능력_방어];

					if (target_def >= taishu_status)
						base_atk += 50.f;
					else
						base_atk += taishu_status - target_def;

				}

				//  병력이 필요한 곳에서만 발동
				if (attacker.has_tech(기교_정예노병))
					base_atk *= 1.3f;
				if (attacker.has_tech(기교_강노))
					base_atk *= 1.2f;
				if (attacker.has_tech(기교_노병단련))
					base_atk *= 1.15f;
				if (attacker.has_tech(기교_공병육성))
					base_atk *= 1.2f;
				if (attacker.has_tech(기교_숙련병))
					base_atk *= 1.1f;
				if (attacker.has_tech(기교_군제개혁))
					base_atk *= 1.2f;
			}
			
			// 항관확장 기교 연구 시 관문, 항구의 공격력 2배
			if (attacker.has_tech(기교_항관확장) and (facility_id == 시설_항구 or facility_id == 시설_관문))
				base_atk *= 2.f;

			// 투석개발 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.2배
			if (attacker.has_tech(기교_투석개발) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				base_atk *= 1.2f;
			// 벽력 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.2배
			if (attacker.has_tech(기교_벽력) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				base_atk *= 1.2f;				

			// 시설강화 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.2배
			if (attacker.has_tech(기교_시설강화) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				base_atk *= 1.2f;
			// 성벽강화 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.2배
			if (attacker.has_tech(기교_성벽강화) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				base_atk *= 1.2f;
			// 방어강화 기교 연구 시 성, 관문, 항구, 진, 요새, 성채의 공격력 1.2배
			if (attacker.has_tech(기교_방어강화) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_투석대)
				base_atk *= 1.2f;

/**
			상대방 기교
*/
			pk::force@ target_force = pk::get_force(target_unit.get_force_id());
			if (target_unit.weapon == 병기_창)
			{
				if (pk::has_tech(target_force, 기교_창병단련))
					base_atk *= 0.87f;
				if (pk::has_tech(target_force, 기교_정예창병))
					base_atk *= 0.77f;
			}
			else if (target_unit.weapon == 병기_극)
			{
				if (pk::has_tech(target_force, 기교_극병단련))
					base_atk *= 0.87f;
				if (pk::has_tech(target_force, 기교_정예극병))
					base_atk *= 0.77f;
			}
			else if (target_unit.weapon == 병기_노)
			{
				if (pk::has_tech(target_force, 기교_노병단련))
					base_atk *= 0.87f;
				if (pk::has_tech(target_force, 기교_정예노병))
					base_atk *= 0.77f;
			}
			else if (target_unit.weapon == 병기_군마)
			{
				if (pk::has_tech(target_force, 기교_기병단련))
					base_atk *= 0.87f;
				if (pk::has_tech(target_force, 기교_정예기병))
					base_atk *= 0.77f;
			}

			if (pk::has_tech(target_force, 기교_화살방패))
				base_atk *= 0.85f;			

			if (pk::has_tech(target_force, 기교_숙련병))
				base_atk *= 0.91f;
			if (pk::has_tech(target_force, 기교_난소행군))
				base_atk *= 0.85f;
			if (pk::has_tech(target_force, 기교_군제개혁))
				base_atk *= 0.85f;

			if (pk::has_tech(target_force, 기교_공병육성))
				base_atk *= 0.85f;

			if (pk::has_tech(target_force, 기교_목수개발))
				base_atk *= 0.85f;
			
/**
			건물 공격 스킬
*/
			// 태수 있으면 공격 스킬
			// float attacker_skill_atk = base_atk;
			if (pk::is_alive(taishu))
			{	
				// 특기 감지
				if (attacker.has_skill(특기_공신) or attacker.has_skill(특기_신산))
					base_atk *= 1.3f;
				else if (attacker.has_skill(특기_공성) or attacker.has_skill(특기_화신)) 
					base_atk *= 1.2f;

				if (attacker.has_skill(특기_사수) or attacker.has_skill(특기_정묘) or attacker.has_skill(특기_사정))
					base_atk *= 1.2f;

				if (attacker.has_skill(특기_위압) or attacker.has_skill(특기_위풍))
					base_atk *= 1.2f;

				if (attacker.has_skill(특기_통찰))
					base_atk *= 1.2f;
				else if (attacker.has_skill(특기_명경) or attacker.has_skill(특기_규율) or attacker.has_skill(특기_침착)) 
					base_atk*= 1.1f;
			}
/**
			타겟 방어 스킬			
*/
			if (target_unit.has_skill(특기_공신) or target_unit.has_skill(특기_신산))
				base_atk *= 0.77f;
			else if (target_unit.has_skill(특기_공성) or target_unit.has_skill(특기_화신))
				base_atk *= 0.85f;

			if (target_unit.has_skill(특기_위압))
				base_atk *= 0.85f;

			if (target_unit.has_skill(특기_통찰))
				base_atk *= 0.85f;
			else if (target_unit.has_skill(특기_명경) or target_unit.has_skill(특기_규율) or target_unit.has_skill(특기_침착))
				base_atk *= 0.91f;

			if (target_unit.has_skill(특기_철벽) or target_unit.has_skill(특기_정묘))
				base_atk *= 0.85f;
			else if (target_unit.has_skill(특기_불굴) or target_unit.has_skill(특기_금강))
				base_atk *= 0.91f;

			if (target_unit.has_skill(특기_호위) or target_unit.has_skill(특기_답파))
				base_atk *= 0.85f;

			info.troops_damage += pk::max(0.f, base_atk);
			
			if (facility_id == 시설_도시 or facility_id == 시설_항구 or facility_id == 시설_관문)
			{
				// 기본 데미지 추가
				if (400.f >= info.troops_damage)
					info.troops_damage += 400.f + pk::rand(100);		
				else if (300.f >= info.troops_damage)
					info.troops_damage += 500.f + pk::rand(100);
				else					
					info.troops_damage += pk::rand(100);
			}
		
			if (pk::equipment_id_to_heishu(target_unit.weapon) == 병종_병기 or target_unit.has_skill(특기_등갑))
				info.troops_damage *= 0.5f;
			
			// 난이도 패널티
			if (attacker.is_player())
				info.troops_damage *= float(pk::core["대미지패널티"][pk::get_scenario().difficulty]);

			// 컴퓨터끼리는 감소
			else if (!attacker.is_player() and !target_unit.is_player())
			{		
				info.troops_damage *= 0.5;
			}

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
			pk::unit@ target_unit = pk::hex_object_to_unit(target);
			int atk = 0;
			int bonus = 0;

			switch (attacker.facility)
			{
			case 시설_진:
				atk = 300;
				bonus = 100;
				break;
			case 시설_요새:
				atk = 400;
				bonus = 150;
				break;
			case 시설_성채:
				atk = 500;
				bonus = 200;
				break;
			case 시설_궁노:
				atk = 400;
				bonus = 200;
				break;
			case 시설_연노로:
				atk = 500;
				bonus = 300;
				break;
			case 시설_투석대:
				atk = 600;
				bonus = 400;
				break;
			default:
				// 시설.자동공격 대미지
				atk = 100;
				bonus = 100;
				break;
			}

			// 체력 비율에 따라 공격력 보정
   			float hp_ratio = float(attacker.hp) / float(pk::get_max_hp(attacker));

			if (hp_ratio >= 0.5f)
				atk = atk - target_unit.attr.stat[부대능력_방어] + pk::rand(bonus);
			else
				atk = atk - int(target_unit.attr.stat[부대능력_방어] * 2.f) + pk::rand(bonus);
			
			if (0 >= atk)	
				atk = pk::rand(bonus);

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