namespace 도적부대생성
{
	class Main
	{
		Main()
		{
			pk::menu_item item;
			item.menu = 0;
			item.init = pk::building_menu_item_init_t(init);
			item.get_text = pk::menu_item_get_text_t(get_text);
			item.is_enabled = pk::menu_item_is_enabled_t(is_enabled);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		pk::building@ building_;

		void init(pk::building@ building)
		{
			@building_ = @building;
		}

		string get_text()
		{
			return pk::encode("도적");
		}

		bool is_enabled()
		{
			return get_empty_pos() != -1;
		}

		bool handler()
		{
			// 상황에 따라 721(오환두목) ~ 728(남만장수), 745(적장) 중 선택.(다른 무장도 가능함)
			pk::person@ bandit = pk::create_bandit(pk::get_person(무장_적장));

			if (!pk::is_alive(bandit))
				return true;

			// 군단 설정은 필수.(하지 않으면 부대가 정상적으로 표시되지 않음.)
			pk::set_district(bandit, 세력_도적);
			bandit.mibun = 신분_일반;
			bandit.birth = pk::get_year() - 30;
			bandit.death = pk::min(353, bandit.birth + 99);
			bandit.appearance = pk::get_year() - 10;
			// 능력치 수정.
			bandit.base_stat[무장능력_무력] = 100;
			bandit.tekisei[병종_창병] = int(pk::core["최대적성"]);
			// 능력치 반영.
			bandit.update();

			// pk::create_unit 함수의 첫 번째 파라미터는 출진한 거점을 의미하며 부대의 기력값을 초기화하는데만 사용됨.
			// 건물의 is_alive 여부 상관없이 get_energy 함수만 사용되므로 null을 제외한 아무 건물이나 입력해도 됨.
			pk::unit@ unit = pk::create_unit(building_, bandit, null, null, 5000, 병기_창, 병기_투함, 1000, 10000, get_empty_pos());

			// 부대 생성이 실패한 경우 무장 삭제
			if (!pk::is_alive(unit))
				pk::reset(bandit);

			// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
			unit.energy = 100;

			// 임무 설정(임무 별 파라미터는 문서 참고.)
			pk::set_order(unit, 부대임무_정복, building_.get_pos());

			// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
			if (!pk::run_order(unit))
				// 임무에 문제가 있다면 부대 삭제.
				pk::remove(unit);

			return true;
		}

		pk::point get_empty_pos()
		{
			auto arr = pk::range(building_.get_pos(), 1, 2);
			for (int i = 0; i < arr.length; i++)
			{
				pk::hex@ hex = pk::get_hex(arr[i]);
				if (!hex.has_building and !hex.has_unit)
					return arr[i];
			}
			return -1;
		}
	}

	Main main;
}