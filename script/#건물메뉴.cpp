namespace 건물메뉴
{
	class Main
	{
		Main()
		{
			pk::menu_item item;
			int parent;

			item.menu = 100;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("도시1"); });
			parent = pk::add_menu_item(item);

			item.menu = 100;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("도시2"); });
			parent = pk::add_menu_item(item);

			item.menu = 101;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("군사1"); });
			parent = pk::add_menu_item(item);

			item.menu = 101;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("군사2"); });
			parent = pk::add_menu_item(item);

			item.menu = 0;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("2단메뉴"); });
			parent = pk::add_menu_item(item);

			item.menu = parent;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("메뉴 복귀"); });
			// handler 함수에서 false 값을 리턴할 경우 메뉴로 복귀합니다.
			item.handler = cast<pk::menu_item_handler_t@>(function() { return false; });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("메뉴 종료"); });
			// true 값을 리턴할 경우 종료.
			item.handler = null;
			pk::add_menu_item(item);
		}

		pk::point src_pos_;
	}

	Main main;
}