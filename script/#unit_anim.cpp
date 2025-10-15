namespace unit_anim
{
	/**
	이 샘플은 부대 애니메이션을 실행하기 위한 최소한의 코드만을 보여줍니다.
	애니메이션으로 인해 이동될 좌표에 다른 부대나 건물이 있을 경우(좌표가 겹치는 경우) 튕길 수도 있으므로 실제 사용하실 때는 확인하는 코드가 필요합니다.
	*/
	class Main
	{
		Main()
		{
			pk::menu_item item;
			int parent;

			item.menu = 1;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("애니메이션1"); });
			parent = pk::add_menu_item(item);

			item.menu = parent;
			item.init = pk::unit_menu_item_init_t(init);
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("근접공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(0, dst_pos); });
			pk::add_menu_item(item);

			// 노병이 아닐 경우 튕길 수 있습니다.
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("활공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(2); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(1, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("돌출공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(2, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("나선첨공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(3, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("이단첨공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(4, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("갈퀴공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim2(5, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("횡치공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(6, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("선풍공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(7, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("화시공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(2); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(8, dst_pos); });
			pk::add_menu_item(item);

			// 다중 목표 지정 예제입니다. 최대 12개의 목표를 가질 수 있습니다. 다중목표 지정은 1번 일반 활공격을 제외한 나머지 활, 투석 공격만 가능합니다.
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("화시공격(다중목표)"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(0); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim3(8, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("관시공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(2); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(9, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("난사공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(2); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(10, dst_pos); });
			pk::add_menu_item(item);

			// 돌격, 돌파, 돌진 애니메이션은 기병이 아닐 경우 튕깁니다.
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("돌격공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim4(11, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("돌파공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim5(12, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("돌진공격"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim5(13, dst_pos); });
			pk::add_menu_item(item);

			item.menu = 1;
			item.init = null;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("애니메이션2"); });
			item.get_targets = null;
			item.handler = null;
			parent = pk::add_menu_item(item);

			item.menu = parent;
			item.init = pk::unit_menu_item_init_t(init);
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("화시공격(병기)"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(2); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(14, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("파쇄공격(병기)"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(15, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("방사공격(병기)"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(16, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("투석공격(병기)"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(2); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(17, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("근접공격(함선)"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(18, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("활공격(함선)"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(2); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(19, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("화시공격(함선)"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(2); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(20, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("투석공격(함선)"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(2); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(21, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("격돌공격(함선)"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim1(22, dst_pos); });
			pk::add_menu_item(item);

			item.menu = 1;
			item.init = null;
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("애니메이션3"); });
			item.get_targets = null;
			item.handler = null;
			parent = pk::add_menu_item(item);

			item.menu = parent;
			item.init = pk::unit_menu_item_init_t(init);
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("돌출당함(충돌), ..."); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_attack_direction(); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim6(23, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("근접공격당함, ..."); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return get_attack_direction(); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim6(24, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("일제공격당함"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return get_attack_direction(); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim6(25, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("활공격당함"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return get_attack_direction(); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim6(26, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("돌출당함, ..."); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return get_attack_direction(); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim7(27, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("돌격당함, ..."); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return get_attack_direction(); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim7(28, dst_pos, 2); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("돌출당함(1타겟), ..."); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return get_attack_direction(); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim8(29, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("갈퀴당함"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return get_attack_direction(); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim9(30, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("돌파당함"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return get_attack_direction(); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim6(31, dst_pos); });
			pk::add_menu_item(item);

			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::u8encode("투석당함"); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return get_attack_direction(); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.anim6(32, dst_pos); });
			pk::add_menu_item(item);
		}

		void init(pk::unit@ unit, pk::point src_pos)
		{
			@unit_ = unit;
			src_pos_ = src_pos;
		}

		bool anim1(int id, pk::point dst_pos)
		{
			pk::damage_info info;
			info.src_pos = src_pos_;
			info.dst_pos = dst_pos;
			pk::unit_anim(unit_, id, { info });
			pk::wait(1);
			return true;
		}

		bool anim2(int id, pk::point dst_pos)
		{
			pk::damage_info info;
			info.src_pos = src_pos_;
			info.src_move_pos = pk::get_neighbor_pos(src_pos_, pk::get_opposite_direction(pk::get_direction(src_pos_, dst_pos)));
			info.dst_pos = dst_pos;
			pk::unit_anim(unit_, id, { info });
			pk::wait(1);
			pk::set_pos(unit_, info.src_move_pos);
			return true;
		}

		bool anim3(int id, pk::point dst_pos)
		{
			array<pk::damage_info> damage_arr;
			auto ring = pk::ring(dst_pos, 1);
			for (int i = 0; i < ring.length; i++)
			{
				if (pk::is_enabled_terrain(pk::get_hex(ring[i]).terrain))
				{
					pk::damage_info damage;
					damage.src_pos = src_pos_;
					damage.dst_pos = ring[i];
					damage_arr.insertLast(damage);
				}
			}
			pk::unit_anim(unit_, id, damage_arr);
			pk::wait(1);
			return true;
		}

		bool anim4(int id, pk::point dst_pos)
		{
			pk::damage_info info;
			info.src_pos = src_pos_;
			info.src_move_pos = dst_pos;
			info.dst_pos = dst_pos;
			pk::unit_anim(unit_, id, { info });
			pk::wait(1);
			pk::set_pos(unit_, info.src_move_pos);
			return true;
		}

		bool anim5(int id, pk::point dst_pos)
		{
			pk::damage_info info;
			info.src_pos = src_pos_;
			info.src_move_pos = pk::get_neighbor_pos(dst_pos, pk::get_direction(src_pos_, dst_pos));
			info.dst_pos = dst_pos;
			pk::unit_anim(unit_, id, { info });
			pk::wait(1);
			pk::set_pos(unit_, info.src_move_pos);
			return true;
		}

		bool anim6(int id, pk::point dst_pos)
		{
			pk::damage_info info;
			info.src_pos = dst_pos;
			info.dst_pos = src_pos_;
			info.troops_damage = 1;
			pk::unit_anim(unit_, id, { info });
			pk::wait(1);
			return true;
		}

		bool anim7(int id, pk::point dst_pos, int distance = 1)
		{
			pk::damage_info info;
			info.src_pos = dst_pos;
			info.dst_pos = src_pos_;
			info.dst_move_pos = pk::get_neighbor_pos(src_pos_, pk::get_direction(dst_pos, src_pos_), distance);
			info.troops_damage = 1;
			pk::unit_anim(unit_, id, { info });
			pk::wait(1);
			pk::set_pos(unit_, info.dst_move_pos);
			return true;
		}

		bool anim8(int id, pk::point dst_pos)
		{
			pk::damage_info info;
			info.src_pos = dst_pos;
			info.dst_pos = src_pos_;
			info.dst_move_pos = src_pos_;
			info.troops_damage = 1;
			pk::unit_anim(unit_, id, { info }, pk::get_neighbor_pos(src_pos_, pk::get_direction(dst_pos, src_pos_)));
			pk::wait(1);
			pk::set_pos(unit_, info.dst_move_pos);
			return true;
		}

		bool anim9(int id, pk::point dst_pos, int distance = 1)
		{
			pk::damage_info info;
			info.src_pos = dst_pos;
			info.dst_pos = src_pos_;
			info.dst_move_pos = dst_pos;
			info.troops_damage = 1;
			pk::unit_anim(unit_, id, { info });
			pk::wait(1);
			pk::set_pos(unit_, info.dst_move_pos);
			return true;
		}

		array<pk::point_int> get_targets(int max)
		{
			array<pk::point_int> targets;
			auto range = pk::range(src_pos_, max == 0 ? 0 : 1, max);
			for (int i = 0; i < range.length; i++)
			{
				if (pk::is_enabled_terrain(pk::get_hex(range[i]).terrain))
					targets.insertLast(pk::point_int(range[i], 0));
			}
			return targets;
		}

		array<pk::point_int> get_attack_direction()
		{
			array<pk::point_int> targets;
			auto ring = pk::ring(src_pos_, 1);
			for (int i = 0; i < ring.length; i++)
				targets.insertLast(pk::point_int(ring[i], 3));
			return targets;
		}

		pk::unit@ unit_;
		pk::point src_pos_;
	}

	Main main;
}