/*
// 제작자:
// 설명: 건물 메뉴에서 랜덤 신무장을 생성하는 기능
// Update:
*/

namespace 무장생성
{
	class Main
	{
		pk::building@ building_;  // 현재 선택된 건물

		Main()
		{
			// 건물 메뉴에 "신무장 생성" 항목 추가
			pk::menu_item item;
			item.menu = 0;
			item.init = pk::building_menu_item_init_t(init);
			item.get_text = pk::menu_item_get_text_t(get_text);
			item.is_enabled = pk::menu_item_is_enabled_t(is_enabled);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		// 메뉴 초기화: 현재 건물 저장
		void init(pk::building@ building)
		{
			@building_ = @building;
		}

		// 메뉴에 표시될 텍스트
		string get_text()
		{
			return pk::u8encode("신무장 생성");
		}

		// 메뉴 활성화 조건: 플레이어 세력이고 빈 무장 슬롯이 있을 때
		bool is_enabled()
		{
			return building_.is_player() and get_empty_person() !is null;
		}

		// 메뉴 실행: 신무장 생성 및 현재 건물로 이동
		bool handler()
		{
			auto person = get_empty_person();
			init(person);  // 무장 정보 랜덤 생성
			pk::sort(pk::person::type_id);
			pk::move(person, building_);  // 생성된 무장을 현재 건물로 이동
			return true;
		}

		/**
		 * 빈 무장 슬롯 찾기
		 * @return 사용 가능한 빈 무장, 없으면 null
		 * 
		 * @details
		 * 빈 무장칸은 아래의 순서대로 확인할 것을 권장함.
		 * 1.  850 ~  999(신무장)
		 * 2.  832 ~  849(사용하지 않는 고대 무장)
		 * 3.  670 ~  699(사용하지 않는 사실 무장)
		 * 4.  800 ~  831(고대 무장)
		 * 5.    0 ~  670(역사 무장)
		 * 6. 1000 ~ 1099(적장). pk::create_bandit 함수에서 사용하는 영역임.
		 * 7.  700 ~  799(이벤트 무장). 신분에 상관없이 pk::is_alive 함수가 항상 true를 리턴하는 영역임.
		 * 
		 * 사망한 무장을 부활시키는 스크립트가 있다면 "case 신분_사망:" 부분은 제외.
		 */
		pk::person@ get_empty_person()
		{
			// 신무장 범위(850~999)에서 빈 슬롯 검색
			for (int i = 신무장_시작; i < 신무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				switch (person.mibun)
				{
				case 신분_없음:      // 데이터 없음
				case 신분_미등장:    // 등장 전
				case 신분_사망:      // 사망
					return person;
				}
			}
			return null;
		}

		/**
		 * 무장 정보 랜덤 생성
		 * @param person 초기화할 무장 객체
		 */
		void init(pk::person@ person)
		{
			pk::random random(pk::rand());
			person.init();
			
			// 기본 정보
			person.sex = random(성별_끝 - 1);                                          // 성별
			person.sei = pk::get_family_name(random(성씨_끝 - 1)).name;               // 성
			person.mei = pk::get_family_name(random(성씨_끝 - 1)).name;               // 이름
			person.face = person.sex == 성별_남 ? random(2000, 2099) : random(2100, 2173);  // 얼굴 (남: 2000~2099, 여: 2100~2173)
			person.appearance = pk::get_year();                                       // 등장 연도
			person.birth = pk::get_year() - 17 - random(4);                          // 출생 연도 (현재-17~20세)
			person.death = pk::min(353, person.birth + 70 + random(20));             // 사망 연도 (70~89세)
			person.ketsuen = person.get_id();                                         // 혈연 ID
			person.aishou = random(0, 150);                                           // 궁합
			person.mibun = 신분_재야;                                                 // 신분: 재야
			
			// 병종 적성
			for (int i = 0; i < 병종_끝; i++)
				person.tekisei[i] = random(0, int(pk::core["최대적성"]));
			
			// 능력치
			for (int i = 0; i < 무장능력_끝; i++)
			{
				person.base_stat[i] = random(1, int(pk::core["무장.최대능력치"]));    // 기초 능력치
				person.stat_aging[i] = random(능력성장_끝 - 1);                      // 능력 성장 타입
			}
			
			// 개성 설정
			person.birthplace = random(주_끝 - 1);                                   // 출신지
			person.skill = random(특기_끝) - 1;                                      // 특기
			person.wadai = random(화제_끝 - 1);                                      // 화제
			person.giri = random(의리_끝 - 1);                                       // 의리
			person.ambition = random(야망_끝 - 1);                                   // 야망
			person.promotion = random(기용_끝 - 1);                                  // 기용
			person.character = random(성격_끝 - 1);                                  // 성격
			person.voice = person.sex == 성별_남 ? random(음성_소심남, 음성_저돌남) : random(음성_냉정녀, 음성_대담녀);  // 음성
			person.tone = person.sex == 성별_남 ? random(말투_장비, 말투_보통남) : random(말투_고대만족, 말투_보통녀);   // 말투
			person.kanshitsu = random(한실_끝 - 1);                                  // 한실 관계
			person.strategic_tendency = random(전략경향_끝 - 1);                     // 전략 경향
			person.local_affiliation = random(지역집착_끝 - 1);                      // 지역 집착
			
			person.update();  // 무장 정보 업데이트
		}
	}

	Main main;
}