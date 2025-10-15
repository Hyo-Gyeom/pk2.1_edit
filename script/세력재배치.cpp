namespace 세력재배치
{
	class Main
	{
		Main()
		{
			pk::bind(102, pk::trigger102_t(callback));
		}

		void callback()
		{
			if (pk::get_scenario().loaded or pk::get_scenario().no != 7) return;
			if (pk::choose(pk::u8encode("세력을 재배치 하시겠습니까?"), { pk::u8encode("예"), pk::u8encode("아니오") }) == 1) return;

			pk::list<pk::force@> force_list = pk::get_force_list();
			array<int> city_count;
			array<int> index;

			force_list.shuffle();
			for (int i = 0; i < force_list.count; i++)
				city_count.insertLast(pk::get_city_list(force_list[i]).count);

			index = Partitioner(pk::get_scenario().seed).partition(city_count);

			// 도시 재설정
			for (int i = 0; i < force_list.count; i++)
			{
				pk::list<pk::city@> a = pk::get_city_list(force_list[i]); // 불필요한 도시
				pk::list<pk::city@> b; // 필요한 도시

				for (int j = 0; j < 도시_끝; j++)
				{
					if (index[j] == i)
					{
						pk::city@ city = pk::get_city(j);
						int k = a.index_of(city);
						if (k >= 0)
							a.remove_at(k);
						else
							b.add(city);
					}
				}

				a.shuffle();
				b.shuffle();

				for (int j = 0; j < a.count; j++)
				{
					pk::building@ c = pk::city_to_building(a[j]);
					pk::building@ d = pk::city_to_building(b[j]);

					// 황제 재설정(영웅집결 시나리오만 적용)
					if (j == 0 and i == 0 and pk::get_scenario().no == 7)
					{
						int building_id = d.get_id();
						array<int> emperors = { 무장_영제, 무장_소제, 무장_헌제 };
						for (int k = 0; k < emperors.length; k++)
						{
							pk::person@ emperor = pk::get_person(emperors[k]);
							pk::set_service(emperor, building_id);
							emperor.location = building_id;
							emperor.mibun = 신분_사망;
						}
						int n = pk::rand(3);
						pk::get_person(emperors[n]).mibun = -1;
						pk::get_scenario().emperor = emperors[n];
					}

					swap(c, d);
				}
			}

			// 항구, 관문 재설정
			for (int i = 0; i < 도시_끝; i++)
			{
				pk::city@ city = pk::get_city(i);
				int force_id = city.get_force_id();
				for (int j = 0; j < 5; j++)
				{
					if (city.gate[j] != -1)
					{
						pk::building@ building = pk::get_building(city.gate[j]);
						if (force_id == -1)
						{
							pk::neutralize(building);
						}
						else
						{
							pk::set_district(building, city.get_district_id());
							pk::set_gold(building, city.gold / 10);
							pk::set_food(building, city.food / 10);
							pk::set_troops(building, city.troops / 10);
							pk::set_energy(building, city.energy);
							pk::reset_weapon_amount(building);
							pk::set_weapon_amount(building, pk::rand(병기_군마) + 1, city.troops / 10);
						}
					}
				}
			}
		}
	}

	class Partitioner
	{
		array<int> result_(도시_끝);
		array<int> start_;
		pk::random random_;

		Partitioner(int seed)
		{
			random_.seed = seed;
		}

		/**
		@param count
			도시 수 배열
		@return
			도시 아이디 별로 city_count 키(인덱스) 값이 들어감
		*/
		array<int> partition(const array<int> &in city_count)
		{
			bool retry = true;

			while (retry)
			{
				start_.resize(도시_끝);
				for (int i = 0; i < 도시_끝; i++)
				{
					result_[i] = -1;
					start_[i] = i;
				}

				shuffle(start_);

				retry = false;
				for (int i = 0; i < city_count.length; i++)
				{
					if (spread(start_[0], city_count[i], i) != 0)
					{
						retry = true;
						break;
					}
				}
			}

			return result_;
		}

		/** city_id 도시를 기준으로 n 개의 도시만큼 선택
		@return
			남은 개수
		*/
		int spread(int city_id, int n, int index)
		{
			// 끝났거나 이미 다른 세력의 도시인 경우 리턴.
			if (n <= 0 or result_[city_id] != -1)
				return n;

			result_[city_id] = index;
			n -= 1;

			int p = start_.find(city_id);
			if (p >= 0)
				start_.removeAt(p);

			// 이웃도시 섞기.
			pk::city@ city = pk::get_city(city_id);
			array<int> neighbor_city_id(6);
			for (int i = 0; i < 6; i++)
				neighbor_city_id[i] = city.neighbor[i];
			shuffle(neighbor_city_id);

			for (int i = 0; i < 6; i++)
			{
				if (neighbor_city_id[i] != -1 && result_[neighbor_city_id[i]] == -1)
					n = spread(neighbor_city_id[i], n, index);
				if (n == 0)
					break;
			}

			// 남은 도시 수를 리턴
			return n;
		}

		void shuffle(array<int>& arr)
		{
			int n = arr.length();
			for (int i = 0; i < n; i++)
			{
				int j = random_(n - 1);
				int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	};

	void swap(pk::building@ a, pk::building@ b)
	{
		SwapContext ctx_a;
		SwapContext ctx_b;
		ctx_a.load(a);
		ctx_b.load(b);
		ctx_a.save(b);
		ctx_b.save(a);
		if (a.get_force_id() == -1)
			pk::neutralize(a);
		if (b.get_force_id() == -1)
			pk::neutralize(b);
	}

	class SwapContext
	{
		int district_id = -1;
		pk::list<pk::person@> person_list;
		int gold = 0;
		int food = 0;
		int public_order = 0;
		int troops = 0;
		int energy = 0;
		array<int> weapon_amount(병기_끝, 0);

		void load(pk::building@ building)
		{
			pk::city@ city = pk::building_to_city(building);
			district_id = building.get_district_id();
			person_list = pk::get_person_list(building, pk::mibun_flags(신분_군주, 신분_도독, 신분_태수, 신분_일반));
			gold = pk::get_gold(building);
			food = pk::get_food(building);
			public_order = city !is null ? city.public_order : 0;
			troops = pk::get_troops(building);
			energy = pk::get_energy(building);
			for (int i = 0; i < 병기_끝; i++)
				weapon_amount[i] = pk::get_weapon_amount(building, i);
		}

		void save(pk::building@ building)
		{
			pk::city@ city = pk::building_to_city(building);
			pk::set_district(building, district_id);
			for (int i = 0; i < person_list.count; i++)
				pk::move(person_list[i], building);
			pk::set_gold(building, gold);
			pk::set_food(building, food);
			if (city !is null)
				pk::set_public_order(city, public_order);
			pk::set_troops(building, troops);
			pk::set_energy(building, energy);
			for (int i = 0; i < 병기_끝; i++)
				pk::set_weapon_amount(building, i, weapon_amount[i]);
		}
	};

	Main main;
}