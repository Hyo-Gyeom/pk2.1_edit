namespace 월별_본거지_발생_확률
{
	class Main
	{
		Main()
		{
			pk::set_func(158, pk::func158_t(callback));
		}

		int callback(pk::city@ city)
		{
			// 게임 시작 후 6개월까지 발생하지 않음.
			if (pk::get_elapsed_months() <= 6)
				return 0;

			// 중복 발생 금지
			int city_id = city.get_id();
			// 이 콜백은 도시 별로 호출되기 때문에 맵 상에 건물이 많을 수록 리스트로 접근하면 속도가 느려지므로 배열로 변환.
			array<pk::building@> arr = pk::list_to_array(pk::get_building_list());
			for (int i = 0; i < arr.length; i++)
			{
				pk::building@ building = arr[i];
				if (pk::get_city_id(building.get_pos()) == city_id and (building.facility == 시설_본거지1 or building.facility == 시설_본거지2))
					return 0;
			}

			// 치안이 기준치 이상일 경우 발생하지 않음
			if (city.public_order >= 80 - (pk::city_to_building(city).has_skill(특기_위압) ? 20 : 0))
				return 0;

			// 기준치 미만일 경우 확률 계산
			return (100 - city.public_order) / 3;
		}
	}

	Main main;
}