namespace 무장생성
{
	class Main
	{
		pk::building@ building_;

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

		void init(pk::building@ building)
		{
			@building_ = @building;
		}

		string get_text()
		{
			return pk::u8encode("신무장 생성");
		}

		bool is_enabled()
		{
			return building_.is_player() and get_empty_person() !is null;
		}

		bool handler()
		{
			auto person = get_empty_person();
			init(person);
			pk::sort(pk::person::type_id);
			pk::move(person, building_);
			return true;
		}

		/**
		@details
			빈 무장칸은 아래의 순서대로 확인할 것을 권장함.
			1.  850 ~  999(신무장)
			2.  832 ~  849(사용하지 않는 고대 무장)
			3.  670 ~  699(사용하지 않는 사실 무장)
			4.  800 ~  831(고대 무장)
			5.    0 ~  670(역사 무장)
			6. 1000 ~ 1099(적장). pk::create_bandit 함수에서 사용하는 영역임.
			7.  700 ~  799(이벤트 무장). 신분에 상관없이 pk::is_alive 함수가 항상 true를 리턴하는 영역임.

			사망한 무장을 부활시키는 스크립트가 있다면 "case 신분_사망:" 부분은 제외.
		*/
		pk::person@ get_empty_person()
		{
			for (int i = 신무장_시작; i < 신무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				switch (person.mibun)
				{
				case 신분_없음:
				case 신분_미등장:
				case 신분_사망:
					return person;
				}
			}
			return null;
		}

		void init(pk::person@ person)
		{
			pk::random random(pk::rand());
			person.init();
			person.sex = random(성별_끝 - 1);
			person.sei = pk::get_family_name(random(성씨_끝 - 1)).name;
			person.mei = pk::get_family_name(random(성씨_끝 - 1)).name;
			person.face = person.sex == 성별_남 ? random(2000, 2099) : random(2100. 2173);
			person.appearance = pk::get_year();
			person.birth = pk::get_year() - 17 - random(4);
			person.death = pk::min(353, person.birth + 70 + random(20));
			person.ketsuen = person.get_id();
			person.aishou = random(0, 150);
			person.mibun = 신분_재야;
			for (int i = 0; i < 병종_끝; i++)
				person.tekisei[i] = random(0, int(pk::core["최대적성"]));
			for (int i = 0; i < 무장능력_끝; i++)
			{
				person.base_stat[i] = random(1, int(pk::core["무장.최대능력치"]));
				person.stat_aging[i] = random(능력성장_끝 - 1);
			}
			person.birthplace = random(주_끝 - 1);
			person.skill = random(특기_끝) - 1;
			person.wadai = random(화제_끝 - 1);
			person.giri = random(의리_끝 - 1);
			person.ambition = random(야망_끝 - 1);
			person.promotion = random(기용_끝 - 1);
			person.character = random(성격_끝 - 1);
			person.voice = person.sex == 성별_남 ? random(음성_소심남, 음성_저돌남) : random(음성_냉정녀, 음성_대담녀);
			person.tone = person.sex == 성별_남 ? random(말투_장비, 말투_보통남) : random(말투_고대만족, 말투_보통녀);
			person.kanshitsu = random(한실_끝 - 1);
			person.strategic_tendency = random(전략경향_끝 - 1);
			person.local_affiliation = random(지역집착_끝 - 1);
			person.update();
		}
	}

	Main main;
}