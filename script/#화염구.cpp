namespace 화염구
{
	const int ENERGY_COST = 1;

	class Main
	{
		Main()
		{
			pk::menu_item item;
			item.menu = 115;
			item.init = pk::unit_menu_item_init_t(init);
			item.get_text = pk::menu_item_get_text_t(get_text);
			item.get_desc = pk::menu_item_get_desc_t(get_desc);
			item.is_enabled = pk::menu_item_is_enabled_t(is_enabled);
			item.get_targets = pk::unit_menu_item_get_targets_t(get_targets);
			item.get_chance = pk::unit_menu_item_get_chance_t(get_chance);
			item.handler = pk::unit_menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		pk::unit@ unit_;
		pk::point src_pos_;
		pk::point dst_pos_;

		void init(pk::unit@ unit, pk::point src_pos)
		{
			@unit_ = @unit;
			src_pos_ = src_pos;
		}

		string get_text()
		{
			return pk::u8encode(pk::format("화염구 ({})", ENERGY_COST));
		}

		string get_desc()
		{
			if (unit_.energy < ENERGY_COST)
				return pk::u8encode("기력이 부족합니다.");
			else if (get_targets().length == 0)
				return pk::u8encode("범위내에 대상이 없습니다.");
			else
				return pk::u8encode("화염구를 떨어트립니다.");
		}

		bool is_enabled()
		{
			if (unit_.energy < ENERGY_COST) return false;
			return get_targets().length > 0;
		}

		pk::array<pk::point_int> get_targets()
		{
			pk::array<pk::point_int> targets;
			auto range = pk::range(src_pos_, 1, 2);
			for (int i = 0; i < range.length; i++)
			{
				auto unit = pk::get_unit(range[i]);
				if (unit !is null and pk::is_enemy(unit_, unit))
					targets.insertLast(pk::point_int(range[i], 1));
			}
			return targets;
		}

		int get_chance(pk::point dst_pos)
		{
			return 100;
		}

		bool handler(pk::point dst_pos)
		{
			pk::unit@ target = pk::get_unit(dst_pos);

			// 실행 부대 기력 감소
			pk::add_energy(unit_, -ENERGY_COST);

			// 실행 부대 계략 실행 음성, 효과 생성
			if (pk::is_in_screen(src_pos_))
			{
				pk::play_voice(0x35, pk::get_person(unit_.leader));
				pk::play_se(0x39, src_pos_);
				pk::create_effect(0x3a, src_pos_);
				pk::wait(1, 1000);
			}

			// 대미지 계산
			int troops_damage = target.troops * 0.9f;
			if (troops_damage < pk::get_max_troops(target) * 0.1f)
				troops_damage = target.troops;

			// 목표 부대 애니메이션
			if (pk::is_in_screen(dst_pos))
			{
				dst_pos_ = dst_pos;
				// 운석 효과 생성
				auto handle = pk::create_effect(0x6b, dst_pos, function(handle, delta) {
					// 운석 떨어지는 소리
					if (delta == -1) {
						pk::play_se(0x56, main.dst_pos_);
					}
					// 애니메이션이 끝났다면 삭제
					else if (handle.elapsed > 500) {
						pk::create_effect(0xe, main.dst_pos_);
						pk::play_se(0x71, main.dst_pos_);
						return true;
					}

					auto v = pk::hex_pos_to_world_pos(main.dst_pos_);
					v.y = v.y + 120 - handle.elapsed / 500.f * 120;
					auto m = pk::matrix4();
					m.translate(v);
					handle.m = m;
					return false;
				});

				// 대미지 애니메이션
				pk::damage_info info;
				info.troops_damage = troops_damage;
				info.dst_pos = dst_pos;
				pk::unit_anim(target, 32, { info }, -1, 500, pk::u8encode("아"));

				// 부대 애니메이션이 끝나거나 클릭으로 생략할 때 까지 대기
				pk::wait(1);

				// 생략한경우를 위한 애니메이션 종료, 효과 삭제
				pk::stop_unit_anim();
				pk::remove(handle);
			}

			// 대미지 적용
			pk::add_troops(target, -troops_damage, true);

			// 목표 부대 괴멸, 살아있다면 혼란
			if (target.troops == 0)
			{
				if (pk::is_in_screen(dst_pos))
					pk::create_death_effect(target, unit_);
				pk::kill(target, unit_);
			}
			else
			{
				pk::set_status(target, unit_, 부대상태_혼란, pk::rand(3) + 1);
			}

			// 화재 발생
			pk::create_fire(dst_pos, pk::rand(2) + 2, unit_, true);

			// 기교점수
			pk::add_tp(pk::get_force(unit_.get_force_id()), 30, dst_pos);
			// 능력경험치
			pk::add_stat_exp(unit_, 무장능력_지력, 4);
			// 공적
			pk::add_kouseki(unit_, 100);
			// 행동완료
			pk::action_done(unit_);
			if (int(pk::option["San11Option.FreeOperate"]) == 1)
				pk::set_action_done(unit_, false);

			return true;
		}
	}

	Main main;
}