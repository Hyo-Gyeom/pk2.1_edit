namespace 군량탄원
{
	class Main
	{
		Main()
		{
			pk::bind(112, pk::trigger112_t(callback));
		}

		pk::force@ force_;
		pk::person@ kunshu_;
		pk::city@ city_;

		void callback(pk::force@ force)
		{
			if (force.is_player())
			{
				@force_ = force;
				@kunshu_ = pk::get_person(force.kunshu);
				@city_ = pk::building_to_city(pk::get_building(kunshu_.service));

				if (city_ is null) return;
				if (city_.food < 25000) return;
				if (pk::rand(100) > 1 + (city_.inago ? pk::rand(2) : 0)) return;

				pk::scene(pk::scene_t(scene));
			}
		}

		void scene()
		{
			pk::move_screen(city_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(2);
			pk::fade(255);

			string kunshu_name = pk::u8decode(pk::get_name(kunshu_));
			pk::person@ farmer = pk::get_person(무장_농민);

			switch (pk::rand(3))
			{
			case 0: pk::message_box(pk::u8encode(pk::format("\x1b[1x{}\x1b[0x님의 자비가 없이는 살아갈 수 없습니다. 뭔가 먹을 것을 주십시오.", kunshu_name)), farmer); break;
			case 1: pk::message_box(pk::u8encode(pk::format("내일 먹을 것도 없는 형편이라... \x1b[1x{}\x1b[0x님, 도와주십시오.", kunshu_name)), farmer); break;
			case 2: pk::message_box(pk::u8encode("부탁입니다...뭔가...뭔가 먹을 것을..."), farmer); break;
			}

			array<int> food = {
				0,
				city_.food / 5,
				city_.food / 4,
				city_.food / 3
			};

			array<string> answers = {
				pk::u8encode("주지 않는다"),
				pk::u8encode(pk::format("군량 {}을 준다", food[1])),
				pk::u8encode(pk::format("군량 {}을 준다", food[2])),
				pk::u8encode(pk::format("군량 {}을 준다", food[3])),
			};

			int n = pk::choose(pk::u8encode("군량을 주겠습니까?"), answers);
			int tp;

			switch (n)
			{
			case 0:
				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::u8encode("우리에게도 여유가 없소. 용서해 주시오."), kunshu_); break;
				case 1: pk::message_box(pk::u8encode("거짓말하지 마시오. 안색도 좋은 것 같은데."), kunshu_); break;
				}
				switch (pk::rand(3))
				{
				case 0: pk::message_box(pk::u8encode("...사람도 아니다..."), farmer); break;
				case 1: pk::message_box(pk::u8encode("아아... 저더러 죽으라는 말씀이군요..."), farmer); break;
				case 2: pk::message_box(pk::u8encode(pk::format("이런... \x1b[1x{}\x1b[0x님만을 의지했건만...", kunshu_name)), farmer); break;
				}
				tp = pk::rand(6) * 500;
				pk::message_box(pk::u8encode(pk::format("기교 포인트가 \x1b[1x{}\x1b[0x만큼 감소했습니다.", tp)));
				pk::add_tp(force_, -tp, city_.get_pos());
				break;

			case 1:
			case 2:
			case 3:
				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::u8encode(pk::format("알았소. 서둘러 군량 {}을 준비시키겠소.", food[n])), kunshu_); break;
				case 1: pk::message_box(pk::u8encode(pk::format("그것 참 안됐군. 군량 {}을 주겠소.", food[n])), kunshu_); break;
				}
				switch (pk::rand(3))
				{
				case 0: pk::message_box(pk::u8encode("감사합니다. 이 은혜는 잊지 않겠습니다."), farmer); break;
				case 1: pk::message_box(pk::u8encode(pk::format("아아, \x1b[1x{}\x1b[0x님은 생명의 은인입니다. 이 은혜는 꼭 갚겠습니다.", kunshu_name)), farmer); break;
				case 2: pk::message_box(pk::u8encode("감사합니다... 이것으로 어떻게든 연명할 수 있을 것 같습니다."), farmer); break;
				}
				tp = (pk::rand(n) + 1) * 100;
				pk::message_box(pk::u8encode(pk::format("기교 포인트가 \x1b[1x{}\x1b[0x만큼 증가했습니다.", tp)));
				pk::add_tp(force_, tp, city_.get_pos());
				pk::add_food(city_, -food[n], true);
				break;
			}

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		}
	}

	Main main;
}