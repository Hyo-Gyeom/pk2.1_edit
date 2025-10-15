namespace Shift_우클릭_메뉴
{
	class Main
	{
		Main()
		{
			pk::add_menu_item(OptionMenu("San11Option.AlwaysActive", "항상 활성화").create_menu_item());
			pk::add_menu_item(OptionMenu("San11Option.FreeOperate", "컴퓨터세력 조종").create_menu_item());
			pk::add_menu_item(OptionMenu("San11Option.EnableInfiniteAction", "부대 무한행동").create_menu_item());
			// pk::add_menu_item(create_menu_item(function() { return pk::u8encode("전투 테스트"); }, function() { return main.battle_test_ ? 1249 : 889; }, function() { main.battle_test_ = !main.battle_test_; return true; }));
			pk::add_menu_item(OptionMenu("San11Option.ShowHex", "칸 지형").create_menu_item());
			pk::add_menu_item(OptionMenu("San11Option.ShowHexInfo", "칸 정보").create_menu_item());
			pk::add_menu_item(create_menu_item(function() { return pk::u8encode("지형 편집"); }, function() { return pk::core::is_editing_terrain() ? 1249 : 889; }, function() { pk::core::toggle_terrain_editor(); return true; }));
			pk::add_menu_item(OptionMenu("San11Option.ShowArea", "구역").create_menu_item());
			pk::add_menu_item(create_menu_item(function() { return pk::u8encode("지형 텍스처 리로드"); }, function() { return 889; }, function() { pk::core::load_terrain_texture(); return true; }));
			pk::add_menu_item(create_menu_item(function() { return pk::u8encode("FPS"); }, function() { return pk::core::is_show_fps() ? 1249 : 889; }, function() { pk::core::toggle_show_fps(); return true; }));
			pk::bind(107, pk::trigger107_t(new_day));
		}

		bool battle_test_ = false;

		void new_day()
		{
			if (battle_test_)
			{
				for (int i = 0; i < 도시_끝; i++)
				{
					pk::city@ city = pk::get_city(i);
					if (!pk::is_alive(city) or city.get_force_id() == -1)
						continue;

					pk::add_troops(city, 100000);
					pk::add_weapon_amount(city, 1, 100000);
					pk::add_weapon_amount(city, 2, 100000);
					pk::add_weapon_amount(city, 3, 100000);
					pk::add_weapon_amount(city, 4, 100000);
					pk::add_gold(city, 100000);
					pk::add_food(city, 100000);
				}
			}
		}
	}

	Main main;

	pk::menu_item create_menu_item(pk::menu_item_get_text_t@ get_text, pk::menu_item_get_image_id_t@ get_image_id, pk::menu_item_handler_t@ handler)
	{
		pk::menu_item item;
		item.menu = 2;
		item.get_text = get_text;
		item.get_image_id = get_image_id;
		item.handler = handler;
		return item;
	}

	class OptionMenu
	{
		string key_;
		string text_;
		int value_;

		OptionMenu(string key, string text)
		{
			key_ = key;
			text_ = text;
			pk::bind(100, pk::trigger100_t(title_screen));
			pk::bind(102, pk::trigger102_t(game_init));
		}

		void title_screen()
		{
			value_ = int(pk::option[key_]);
			pk::printf("{} {}\n", key_, value_);
		}

		void game_init()
		{
			pk::option[key_] = value_;
		}

		pk::menu_item create_menu_item()
		{
			pk::menu_item item;
			item.menu = 2;
			item.get_text = pk::menu_item_get_text_t(get_text);
			item.get_image_id = pk::menu_item_get_image_id_t(get_image_id);
			item.handler = pk::menu_item_handler_t(handler);
			return item;
		}

		string get_text()
		{
			return pk::u8encode(text_);
		}

		int get_image_id()
		{
			return value_ != 0 ? 1249 : 889;
		}

		bool handler()
		{
			value_ = 1 - value_;
			pk::option[key_] = value_;
			return true;
		}
	}
}