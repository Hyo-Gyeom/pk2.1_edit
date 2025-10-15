namespace 월별_충성도_감소
{
	class Main
	{
		Main()
		{
			pk::set_func(155, pk::func155_t(callback));
		}

		int callback(pk::building@ building, pk::person@ person)
		{
			if (person.mibun == 신분_군주)
				return 0;

			// 거점에 인정 특기 보유자가 있다면 일반 무장은 감소하지 않음
			if (person.mibun != 신분_포로 and pk::is_valid_person_id(building.who_has_skill(특기_인정)))
				return 0;

			int kunshu_id = pk::get_kunshu_id(person);
			pk::person@ kunshu = pk::get_person(kunshu_id);

			// 군주가 없다면 감소하지 않음(멸망한 세력의 포로)
			if (!pk::is_alive(kunshu))
				return 0;

			// 일반 무장은 분기별 첫 달에만 감소
			if (person.mibun != 신분_포로 and !pk::is_first_month_of_quarter(pk::get_month()))
				return 0;

			// 군주를 친애하거나 부부, 의형제, 부모자식 관계일 경우 감소하지 않음
			if (pk::is_like(person, kunshu_id) or pk::is_fuufu(person, kunshu_id) or pk::is_gikyoudai(person, kunshu_id) or pk::is_oyako(person, kunshu_id))
				return 0;

			// 일반 무장이고 군주와의 궁합 25 이하(좋음)
			if (person.mibun != 신분_포로 and pk::get_aishou_distance(person, kunshu_id) <= 25)
			{
				// 의리가 보통 이상이거나 야망이 보통 이하
				if (person.giri >= 의리_보통 or person.ambition <= 야망_보통)
				{
					// 군주를 혐오하지 않으면 감소하지 않음
					if (!pk::is_dislike(person, kunshu_id))
						return 0;
				}
			}

			// 인심장악 기교가 연구된 경우 66% 확률로 감소하지 않음
			pk::force@ force = pk::get_force(person.get_force_id());
			if (pk::is_alive(force) and pk::has_tech(force, 기교_인심장악) and pk::rand(3) >= 1)
				return 0;

			int n = pk::rand(3);
			pk::city@ city = pk::building_to_city(building);
			if (pk::is_alive(city) and pk::has_facility(city, 시설_부절태))
				n = n + 2;
			if (kunshu.giri == 의리_매우낮음 and kunshu.ambition == 야망_매우높음)
				n = n + (의리_매우높음 - person.giri) / 2;
			n = n + pk::rand(3);
			return -n;
		}
	}

	Main main;
}