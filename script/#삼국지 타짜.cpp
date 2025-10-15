namespace 삼국지_타짜
{
	class Main
	{
		Main()
		{
			pk::menu_item item;
			item.menu = 2;
			item.get_text = pk::menu_item_get_text_t(get_text);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		string get_text()
		{
			return pk::u8encode("타짜");
		}

		bool handler()
		{
			pk::scene(pk::scene_t(scene));
			return true;
		}

		pk::person@ 손권 = pk::get_person(무장_손권);
		pk::person@ 노숙 = pk::get_person(무장_노숙);
		pk::person@ 유비 = pk::get_person(무장_유비);
		pk::person@ 제갈량 = pk::get_person(무장_제갈량);
		pk::person@ 유기 = pk::get_person(무장_유기);

		void scene()
		{
			int pause = int(pk::option["MessagePause"]);
			pk::option["MessagePause"] = 100;
			pk::move_screen(pk::get_city(도시_건업).get_pos(), 2000);
			pk::diplomacy(손권, null, 노숙, pk::diplomacy_t(scene1));
			pk::move_screen(pk::get_city(도시_강릉).get_pos(), 2000);
			pk::council(유비, 노숙, 제갈량, null, null, null, null, pk::council_t(scene2));
			pk::cutin(18);
			pk::council(유비, 노숙, 제갈량, null, 유기, null, null, pk::council_t(scene3));
			pk::option["MessagePause"] = pause;
		}

		void scene1()
		{
			pk::anim(100, 노숙, 0x5b);
			pk::message_box(pk::u8encode("맡겨만 주쇼잉. 나가 유비 패거리 그 시벌롬들을 확 젓어불고 올랑게"), 노숙);
		}

		void scene2()
		{
			pk::message_box(pk::u8encode("(싸늘하다)"), 제갈량);
			pk::message_box(pk::u8encode("(가슴에 노숙의 시선이 날아와 꽂힌다)"), 제갈량);
			pk::message_box(pk::u8encode("(하지만 걱정하지 마라)"), 제갈량);
			pk::message_box(pk::u8encode("(내 계산은 주유보다 빠르니까)"), 제갈량);
			pk::anim(0, 제갈량, 0x3a);
			pk::message_box(pk::u8encode("저희 형주를 살펴보신 소감은\n어떻습니까\n자경께서 보시기에 불만족스럽지는 않..."), 제갈량);
			pk::anim(0, 노숙, 0x82);
			pk::message_box(pk::u8encode("동작그만, 방 안 빼냐?"), 노숙);
			pk::anim(0, 제갈량, 0x4f);
			pk::message_box(pk::u8encode("뭐야"), 제갈량);
			pk::message_box(pk::u8encode("아예 형주에 눌러 앉을 생각이지?"), 노숙);
			pk::anim(0, 노숙, 0x5c);
			pk::message_box(pk::u8encode("오나라가 빙다리 핫바지로 보이냐 이 새끼야?"), 노숙);
			pk::anim(0, 유비, 0x37);
			pk::message_box(pk::u8encode("형주가 내 땅이 될 수 없다는 증거 있어?"), 유비);
			pk::anim(0, 노숙, 0x81);
			pk::message_box(pk::u8encode("증거? 증거 있지"), 노숙);
			pk::anim(0, 노숙, 0x37);
			pk::message_box(pk::u8encode("너는 우리 오나라에 \"동맹맺어 조조군을 막자\"고 바람을 넣었을 것이여"), 노숙);
			pk::message_box(pk::u8encode("그런데 정작 동맹이란 니들은 뒤에서 꿀만 빨고 오나라가 조빠지게 싸워서 조조를 물리쳤으니"), 노숙); // 늬, 좆
			pk::anim(0, 노숙, 0x41);
			pk::message_box(pk::u8encode("당연히 형주는 오나라땅 되는 거 아니겠어?"), 노숙); // 겄
			pk::anim(0, 제갈량, 0x3f);
			pk::message_box(pk::u8encode("시나리오 쓰고 있네 우리 아니었음 항복했을 미친 새끼가"), 제갈량);
			pk::anim(0, 노숙, 0x81);
			pk::message_box(pk::u8encode("으허허허허허허허허허허허허허허허허"), 노숙);
			pk::anim(0, 제갈량, 0x37);
			pk::message_box(pk::u8encode("유황숙의 조카 유기의 땅을 되찾아 준 거라고 하면 어쩔건데?"), 제갈량);
			pk::anim(0, 노숙, 0x3d);
			pk::message_box(pk::u8encode("유기는 지금 강하에 있지 형주에는 없어. 여기에 있지도 않은 조카 핑계로 형주를 먹겠다는거 아녀?"), 노숙);
			pk::anim(0, 제갈량, 0x41);
			pk::message_box(pk::u8encode("씨발 천하의 노숙이 혀바닥이 왜이렇게 길어...\n후달리냐?"), 제갈량); // 혓
			pk::anim(0, 노숙, 0x81);
			pk::message_box(pk::u8encode("후달려?\n어허허허허허허허허허"), 노숙);
			pk::anim(0, 노숙, 0x3f);
			pk::message_box(pk::u8encode("오냐 내 돈 모두하고 내 손모가질 건다"), 노숙);
			pk::message_box(pk::u8encode("준비됐어? 까볼까?"), 노숙);
			pk::anim(0, 노숙, 0x3a);
			pk::message_box(pk::u8encode("자 지금부터 확인들어가겠습니다잉"), 노숙);
			pk::message_box(pk::u8encode("따라란~ 따라란 따 쿵작작 쿵작작\n다라리다라리"), 노숙);
		}

		void scene3()
		{
			pk::message_box(pk::u8encode("..."), 유기);
			pk::anim(0, 유비, 0x81);
			pk::message_box(pk::u8encode("유기네."), 유비);
			pk::message_box(pk::u8encode("유기야?"), 노숙);
			pk::anim(0, 노숙, 0x5c);
			pk::message_box(pk::u8encode("...내가 봤어\n이 씨발놈 강하에 있는 걸 똑똑히 봤다니께?"), 노숙);
			pk::anim(0, 제갈량, 0x44);
			pk::message_box(pk::u8encode("\"확실하지 않으면 승부를 걸지마라.\"\n이런 거 안배웠어?"), 제갈량);
			pk::anim(0, 유비, 0x37);
			pk::message_box(pk::u8encode("뭐해 형님 오나라로 안 돌아가고"), 유비);
		}
	}

	Main main;
};