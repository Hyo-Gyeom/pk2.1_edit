namespace 부대메뉴
{
	class Main
	{
		Main()
		{
			pk::menu_item item;
			int parent;

			item.menu = 113;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("전법1"); });
			parent = pk::add_menu_item(item);

			item.menu = 113;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("전법2"); });
			parent = pk::add_menu_item(item);

			item.menu = 115;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("계략1"); });
			parent = pk::add_menu_item(item);

			item.menu = 115;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("계략2"); });
			parent = pk::add_menu_item(item);

			item.menu = 1;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("2단메뉴"); });
			parent = pk::add_menu_item(item);

			item.menu = parent;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("일반"); });
			pk::add_menu_item(item);

			item.init = cast<pk::unit_menu_item_init_t@>(function(unit, src_pos) { main.src_pos_ = src_pos; });
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("목표지정"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return { pk::point_int(main.src_pos_, 0) }; });
			pk::add_menu_item(item);
		}

		pk::point src_pos_;
	}

	Main main;
}