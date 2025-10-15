namespace 월별_치안_감소
{
	class Main
	{
		Main()
		{
			pk::set_func(154, pk::func154_t(callback));
		}

		int callback(pk::city@ city)
		{
			if (!pk::is_first_month_of_quarter(pk::get_month()))
				return 0;
			pk::force@ force = pk::get_force(city.get_force_id());

			if (force is null)
				return 0;

			// 법령정비 연구 시 75% 확률로 감소하지 않음
			if (pk::has_tech(force, 기교_법령정비) and pk::rand_bool(75))
				return 0;

			
			// 친이민족 특기 조건 추가
			pk::building@ building = pk::city_to_building(city);

			// 친이민족 특기가 지역에 있으면 감소하지 않음 (특기종합패치)
			if ((city.get_id() == 도시_양평 or city.get_id() == 도시_북평 or city.get_id() == 도시_계) and (building.has_skill(특기_친오) or building.has_skill(특기_위압)))
				return 0;
			if ((city.get_id() == 도시_무위 or city.get_id() == 도시_안정 or city.get_id() == 도시_천수) and (building.has_skill(특기_친강) or building.has_skill(특기_위압)))
				return 0;
			if ((city.get_id() == 도시_회계 or city.get_id() == 도시_오 or city.get_id() == 도시_시상) and (building.has_skill(특기_친월) or building.has_skill(특기_위압)))
				return 0;
			if ((city.get_id() == 도시_운남 or city.get_id() == 도시_건녕 or city.get_id() == 도시_강주) and (building.has_skill(특기_친만) or building.has_skill(특기_위압)))
				return 0;

			int n = 90;
			pk::person@ taishu = pk::get_person(city.get_taishu_id());
			if (pk::is_alive(taishu))
				n = n - taishu.stat[무장능력_매력];
			n = pk::max(n, 1) / 10 + pk::rand(3);
			n = pk::min(n, 5);
			return -n;
		}
	}

	Main main;
}