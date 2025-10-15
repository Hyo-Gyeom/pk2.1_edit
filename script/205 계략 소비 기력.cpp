namespace 계략_소비_기력
{
	class Main
	{
		Main()
		{
			pk::set_func(205, pk::func205_t(callback));
		}

		int callback(pk::unit@ src, int strategy_id)
		{
			if (src.has_skill(특기_백출))
				return 1;

			switch (strategy_id)
			{
			case 계략_화계: return 10;
			case 계략_소화: return 10;
			case 계략_위보: return 15;
			case 계략_교란: return 15;
			case 계략_진정: return 10;
			case 계략_복병: return 10;
			case 계략_동토: return 20;
			case 계략_요술: return 50;
			case 계략_낙뢰: return 50;
			}

			return 0;
		}
	}

	Main main;
}