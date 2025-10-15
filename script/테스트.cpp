namespace 테스트
{
	class Main
	{
		Main()
		{
			// unit_property_test();
			// tactics_menu_test();
			// debate_test();
			// duel_test();
			// cutin_xml_test();
			// pk::bind(112, pk::trigger112_t(scene_test));
			// sort_test();
			// pk::bind(120, pk::trigger120_t(drawing_test));
			// numberpad_test();
			// pk::bind(102, pk::trigger102_t(effect_test));
			// pk::bind(111, pk::trigger111_t(mission_test));
			// get_movable_pos_test();
		}

		void unit_property_test()
		{
			pk::menu_item item;
			item.menu = 1;
			item.init = cast<pk::unit_menu_item_init_t@>(function(unit, src_pos) { @main.unit_ = unit; });
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return "prop"; });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) {
				pk::printf("{} {} {} {}\n", main.unit_.order, main.unit_.target, main.unit_.target_type, string(main.unit_.target_pos));
				return true;
			});
			pk::add_menu_item(item);
		}

		void tactics_menu_test()
		{
			pk::menu_item item;

			item.menu = 113;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("전법1"); });
			pk::add_menu_item(item);

			item.menu = 113;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("전법2"); });
			pk::add_menu_item(item);

			item.menu = 113;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("전법3"); });
			pk::add_menu_item(item);

			item.menu = 113;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("전법4"); });
			pk::add_menu_item(item);

			item.menu = 113;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("전법5"); });
			pk::add_menu_item(item);

			item.menu = 113;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("전법6"); });
			pk::add_menu_item(item);
		}

		void debate_test()
		{
			pk::menu_item item;
			item.menu = 2;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("설전1"); });
			item.handler = cast<pk::menu_item_handler_t@>(function() {
				pk::scene(function() {
					pk::int_int_bool result = pk::debate(pk::get_person(무장_아회남), pk::get_person(851), true, false, true);
					pk::printf("{} {} {}", result.first, result.second, result.third);
				});
				return false;
			});
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("설전2"); });
			item.handler = cast<pk::menu_item_handler_t@>(function() {
				pk::scene(function() {
					pk::int_int_bool result = pk::debate(pk::get_person(무장_아회남), pk::get_person(851), false, true, true);
					pk::printf("{} {} {}", result.first, result.second, result.third);
				});
				return false;
			});
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("설전3"); });
			item.handler = cast<pk::menu_item_handler_t@>(function() {
				pk::scene(function() {
					pk::int_int_bool result = pk::debate(pk::get_person(851), pk::get_person(무장_아회남), true, false, true);
					pk::printf("{} {} {}", result.first, result.second, result.third);
				});
				return false;
			});
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("설전4"); });
			item.handler = cast<pk::menu_item_handler_t@>(function() {
				pk::scene(function() {
					pk::int_int_bool result = pk::debate(pk::get_person(851), pk::get_person(무장_아회남), false, true, true);
					pk::printf("{} {} {}", result.first, result.second, result.third);
				});
				return false;
			});
			pk::add_menu_item(item);
		}

		void duel_test()
		{
			pk::menu_item item;
			item.menu = 2;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("일기토1"); });
			item.handler = cast<pk::menu_item_handler_t@>(function() {
				pk::scene(function() {
					pk::int_bool result = pk::duel(null, null, pk::get_person(무장_아회남), null, null, pk::get_person(851), null, null, true, false);
					pk::printf("{} {}", result.first, result.second);
				});
				return false;
			});
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("일기토2"); });
			item.handler = cast<pk::menu_item_handler_t@>(function() {
				pk::scene(function() {
					pk::int_bool result = pk::duel(null, null, pk::get_person(무장_아회남), null, null, pk::get_person(851), null, null, false, true);
					pk::printf("{} {}", result.first, result.second);
				});
				return false;
			});
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("일기토3"); });
			item.handler = cast<pk::menu_item_handler_t@>(function() {
				pk::scene(function() {
					pk::int_bool result = pk::duel(null, null, pk::get_person(851), null, null, pk::get_person(무장_아회남), null, null, true, false);
					pk::printf("{} {}", result.first, result.second);
				});
				return false;
			});
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("일기토4"); });
			item.handler = cast<pk::menu_item_handler_t@>(function() {
				pk::scene(function() {
					pk::int_bool result = pk::duel(null, null, pk::get_person(851), null, null, pk::get_person(무장_아회남), null, null, false, true);
					pk::printf("{} {}", result.first, result.second);
				});
				return false;
			});
			pk::add_menu_item(item);
		}

		void cutin_xml_test()
		{
			pk::load_xml("""
			<pk trace="true">
				<무장 번호="866">
					<컷인 값="!SAMPLE/cutin.png"/>
					<컷인 값="!SAMPLE/cutin2.png"/>
					<컷인 나이="10" 값="!SAMPLE/cutin.png"/>
					<컷인 나이="10" 값="!SAMPLE/cutin2.png"/>
				</무장>
			</pk>
			""");
		}

		void scene_test(pk::force@ force)
		{
			if (force.is_player())
			{
				pk::scene_t@ scene = function() {
					pk::council_t@ council = function() {
						int winner = pk::debate(pk::get_person(850), pk::get_person(0), true, false).first;
						pk::message_box(pk::u8encode("111"));
					};

					pk::council(null, null, null, null, null, null, null, council);

					pk::diplomacy_t@ diplomacy = function() {
						int winner = pk::duel(null, null, pk::get_person(850), null, null, pk::get_person(0), null, null, true, false).first;
						pk::message_box(pk::u8encode("111"));
					};

					pk::diplomacy(null, null, null, diplomacy);
				};

				pk::scene(scene);
			}
		}

		void sort_test()
		{
			pk::menu_item item;
			item.menu = 2;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("리스트 정렬"); });
			item.handler = cast<pk::menu_item_handler_t@>(function() {
				pk::list<pk::city@> list;
				list.add(pk::get_city(0));
				list.add(pk::get_city(1));
				list.sort(function(a, b) {
					return a.get_id() > b.get_id();
				});
				return true;
			});
			pk::add_menu_item(item);
		}

		void drawing_test()
		{
			pk::draw_text(pk::u8encode("테스트"), pk::point(200, 200), -1, FONT_BIG, 0xffff0000);
			pk::draw_text(pk::u8encode("색상\x1b[1x테스트"), pk::point(200, 250), -1, FONT_BIG);
			pk::draw_text("\xff\x01" + pk::u8encode("テスト", LANG_JP), pk::point(100, 200), -1, FONT_BIG, 0xffff0000);
			pk::draw_text("\xff\x02" + pk::u8encode("테스트", LANG_KR), pk::point(100, 250), -1, FONT_BIG, 0xffff0000);
			pk::draw_text("\xff\x03" + pk::u8encode("测试", LANG_SC), pk::point(100, 300), -1, FONT_BIG, 0xffff0000);

			pk::draw_rect(pk::rectangle(pk::point(200, 300), pk::size(100, 100)), 0xffff0000);
			pk::draw_text("draw_rectangle", pk::point(200, 300));

			pk::draw_filled_rect(pk::rectangle(pk::point(200, 450), pk::size(100, 100)), 0xffff0000);
			pk::draw_text("draw_filled_rectangle", pk::point(200, 450));

			pk::draw_face(FACE_SMALL, 0, pk::rectangle(pk::point(400, 200), pk::size(32, 40)));
			pk::draw_face(FACE_SMALL, 0, pk::rectangle(pk::point(400, 250), pk::size(32, 40)), FACE_R);
			pk::draw_text("32x40", pk::point(400, 180));

			pk::draw_face(FACE_SMALL, 0, pk::rectangle(pk::point(460, 200), pk::size(64, 80)));
			pk::draw_face(FACE_SMALL, 0, pk::rectangle(pk::point(460, 300), pk::size(64, 80)), FACE_R);
			pk::draw_text("64x80", pk::point(460, 180));

			pk::draw_face(FACE_BIG, 0, pk::rectangle(pk::point(550, 200), pk::size(240, 240)));
			pk::draw_text("240x240", pk::point(550, 180));
		}

		void numberpad_test()
		{
			pk::menu_item item;
			item.menu = 2;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return "numberpad"; });
			item.handler = cast<pk::menu_item_handler_t@>(function() {
				int n = pk::numberpad(pk::u8encode("히히"), 0, 100, 50, function(line, original, cur) {
					if (line == 0)
						return pk::format("{} -> {}", original, cur);
					else
						return pk::format("line: {}", line);
				}).first;
				return true;
			});
			pk::add_menu_item(item);
		}

		/** 화면을 따라다니는 화염구 */
		void effect_test()
		{
			pk::create_effect(0x6b, pk::point(0, 0), function(handle, delta) {
				pk::vector4 v = pk::get_lookat() - pk::get_eye();
				v = pk::get_eye() + v.normalized() * pk::vector4(200, 200, 200);
				pk::matrix4 m;
				m.translate(v);
				handle.m = m;
				return false;
			});
		}

		/***/
		void mission_test(pk::force@ force)
		{
			if (!force.is_player() or pk::is_mission_running()) return;
			pk::start_mission(pk::u8encode("타이틀"), pk::u8encode("승리"), pk::u8encode("패배"), pk::u8encode("제한"), 10, pk::u8encode("중단하실?"), function() {
				pk::message_box(pk::u8encode("중단하였다."));
			});
		}

		/***/
		void get_movable_pos_test()
		{
			pk::menu_item item;
			item.menu = 1;
			item.init = cast<pk::unit_menu_item_init_t@>(function(unit, src_pos) { @main.unit_ = unit; });
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return "get_movable_pos"; });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) {
				main.movable_pos_ = pk::get_movable_pos(main.unit_);
				return true;
			});
			pk::add_menu_item(item);

			item.init = null;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return "remove"; });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) {
				main.movable_pos_.resize(0);
				return true;
			});
			pk::add_menu_item(item);

			pk::bind(120, cast<pk::trigger120_t@>(function() {
				for (int i = 0; i < main.movable_pos_.length; i++) {
					auto pos = pk::hex_pos_to_screen_pos(main.movable_pos_[i]);
					pk::draw_text("1", pos);
				}
			}));
		}

		pk::unit@ unit_;
		array<pk::point> movable_pos_;
	}

	Main main;
}