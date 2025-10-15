namespace 거점정복공격
{
	class Main
	{
		Main()
		{
			pk::bind(174, pk::trigger174_t(attack_ground));
		}

		void attack_ground(pk::unit@ unit)
		{
			if (!pk::is_alive(unit) or !pk::is_player_controlled(unit))
				return;

			if (unit.type == 부대종류_전투 and unit.order == 부대임무_공격 and unit.target_type == 부대임무대상_거점)
				unit.order = 부대임무_정복;
		}
	}
	Main main;
}