namespace 거점_AI_행동_순서
{
	class Main
	{
		Main()
		{
			pk::set_func(255, pk::func255_t(callback));
		}

		void callback(pk::ai_context@ context, pk::building@ building)
		{
			int building_id = building.get_id();
			pk::ai_context_base@ base = context.base[building_id];

			// 거점 주변 3칸 안에 적 있음
			if (base.enemies_around3)
			{
				context.push_cmd(거점AI_방어);
				if (building_id >= 건물_도시시작 and building_id < 건물_도시끝)
				{
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_생산);
					context.push_cmd(거점AI_철거);
				}
				return;
			}

			// 명령 가능한 무장 없음
			if (base.num_idle_officers == 0)
				return;

			// 도시
			if (building_id >= 건물_도시시작 and building_id < 건물_도시끝)
			{
				switch (base.status)
				{
				case 거점상태_후방:
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_최소인재탐색);
					context.push_cmd(거점AI_최소무장등용);
					context.push_cmd(거점AI_최소타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_생산);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_거래2);
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_철거);
					context.push_cmd(거점AI_개발);
					context.push_cmd(거점AI_흡수합병);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_경계:
					context.push_cmd(거점AI_공격);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_최소인재탐색);
					context.push_cmd(거점AI_최소무장등용);
					context.push_cmd(거점AI_최소타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_거래2);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_생산);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_철거);
					context.push_cmd(거점AI_개발);
					context.push_cmd(거점AI_흡수합병);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_전방:
					context.push_cmd(거점AI_공격);
					context.push_cmd(거점AI_설치);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_최소인재탐색);
					context.push_cmd(거점AI_최소무장등용);
					context.push_cmd(거점AI_최소타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_거래2);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_생산);
					context.push_cmd(거점AI_철거);
					context.push_cmd(거점AI_개발);
					context.push_cmd(거점AI_흡수합병);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_전투:
					context.push_cmd(거점AI_방어);
					context.push_cmd(거점AI_철거);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_징병);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_거래);
					context.push_cmd(거점AI_순찰);
					context.push_cmd(거점AI_생산);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_거래2);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;
				}
			}
			// 관문, 항구
			else
			{
				switch (base.status)
				{
				case 거점상태_후방:
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_경계:
					context.push_cmd(거점AI_공격);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_전방:
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_공격);
					context.push_cmd(거점AI_설치);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_관문수송);
					context.push_cmd(거점AI_계략);
					context.push_cmd(거점AI_인재탐색);
					context.push_cmd(거점AI_무장등용);
					context.push_cmd(거점AI_타세력무장등용);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;

				case 거점상태_전투:
					context.push_cmd(거점AI_방어);
					context.push_cmd(거점AI_훈련);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_이동);
					context.push_cmd(거점AI_도시수송);
					context.push_cmd(거점AI_포상);
					context.push_cmd(거점AI_계략);
					break;
				}
			}
		}
	}

	Main main;
}