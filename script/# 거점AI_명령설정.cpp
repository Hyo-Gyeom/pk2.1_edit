/*
// 제작자: 삼도카페 기마책사
// Update: '20.10.21   / 최초적용: 거점 AI 명령설정, [255 거점 AI 행동 순서.cpp] 에 의한 명령 실행 시 스크립트 동작
*/

namespace 거점AI_명령설정
{
    //---------------------------------------------------------------------------------------
    // 유저 설정 (true = on, false = off)
    const bool 플레이어_위임군단_무장이동_인재설정연동 = true;   // 위임군단 설정 메뉴의 인재 허가/금지에 따라 이동/소환 명령 연동하기
    const bool 플레이어_위임군단_무장이동_항상금지설정 = true;   // 인재설정연동=false 일 경우에만 동작, 위임군단의 이동/소환 명령 사용금지 설정
    
    const bool AI거점_방어출진금지_병력기준설정 = true;        // 거점 병력이 기준병력 미만이 될 경우 전투부대 방어 출진금지
    const int  AI거점_방어출진금지_병력기준값_도시거점 = 5000;  // 거점병력 설정된 기준값 미만이 될 경우 방어 출진금지됨
    const int  AI거점_방어출진금지_병력기준값_관문항구 = 3000;  // 거점병력 설정된 기준값 미만이 될 경우 방어 출진금지됨
    
    const bool AI거점_공략출진금지_병력기준설정 = true;        // 거점 병력이 기준병력 미만이 될 경우 전투부대 공략 출진금지
    const int  AI거점_공략출진금지_병력기준값_도시거점 = 5000;  // 거점병력 설정된 기준값 미만이 될 경우 공략 출진금지됨
    const int  AI거점_공략출진금지_병력기준값_관문항구 = 3000;  // 거점병력 설정된 기준값 미만이 될 경우 공략 출진금지됨
    
    //---------------------------------------------------------------------------------------
    
	class Main
	{
        
		Main()
		{
			@prev_callback_256 = cast<pk::func256_t@>(pk::get_func(256));
			pk::reset_func(256);
			pk::set_func(256, pk::func256_t(callback_256));
            
            @prev_callback_257 = cast<pk::func257_t@>(pk::get_func(257));
			pk::reset_func(257);
			pk::set_func(257, pk::func257_t(callback_257));
            
            pk::bind(111, pk::trigger111_t(onTurnStart));
		}

        pk::func256_t@ prev_callback_256;
        pk::func257_t@ prev_callback_257;
        int temp_ap;
        
        array<array<bool>> arr_cmd_canceled(건물_거점끝, array<bool>(28, false));
        array<int> arr_building_ap(건물_거점끝, 0);
        
        //---------------------------------------------------------------------------------------
        
        // 256함수 : AI 명령실행 전 설정변경
		void callback_256(pk::ai_context@ context, pk::building@ building, int cmd)
		{
            if (!pk::is_campaign() and building !is null and building.get_id() < 건물_거점끝)
            {
                int base_id = building.get_id();
                pk::district@ district = pk::get_district(building.get_district_id());
                arr_building_ap[base_id] = context.ap;  // 행동력 저장
                
                // 플레이어 위임군단 무장이동금지 설정
                if ((cmd == 거점AI_이동 or cmd == 거점AI_소환) and !arr_cmd_canceled[base_id][cmd])
                {
                    if (building.is_player() and !pk::is_player_controlled(building))
                    {
                        if (플레이어_위임군단_무장이동_인재설정연동 and !district.personnel)
                            cancel_cmd_256(context, building, cmd);   // 위임군단 인재금지 시 무장이동/소환 취소
                        else if (플레이어_위임군단_무장이동_항상금지설정) 
                            cancel_cmd_256(context, building, cmd);
                    }
                }
                // 거점 방어 출진 금지설정
                if (cmd == 거점AI_방어 and !arr_cmd_canceled[base_id][cmd])
                {
                    if (AI거점_방어출진금지_병력기준설정)
                    {
                        int min_troops;
                        if (base_id < 건물_도시끝)
                            min_troops = pk::max(0, AI거점_방어출진금지_병력기준값_도시거점);
                        else if (건물_관문시작 <= base_id and base_id < 건물_항구끝)
                            min_troops = pk::max(0, AI거점_방어출진금지_병력기준값_관문항구);
                        
                        if (pk::get_troops(building) < min_troops) 
                            cancel_cmd_256(context, building, cmd);
                    }
                }
                // 거점 공략 출진 금지설정
                if (cmd == 거점AI_공격 and !arr_cmd_canceled[base_id][cmd])
                {
                    if (AI거점_공략출진금지_병력기준설정)
                    {
                        int min_troops;
                        if (base_id < 건물_도시끝)
                            min_troops = pk::max(0, AI거점_공략출진금지_병력기준값_도시거점);
                        else if (건물_관문시작 <= base_id and base_id < 건물_항구끝)
                            min_troops = pk::max(0, AI거점_공략출진금지_병력기준값_관문항구);
                        
                        if (pk::get_troops(building) < min_troops) 
                            cancel_cmd_256(context, building, cmd);
                    }
                }
            }
            
			if (prev_callback_256 !is null)
				prev_callback_256(context, building, cmd);
		}

        //---------------------------------------------------------------------------------------
        
        // 257함수 : AI 명령실행 후 설정변경
		void callback_257(pk::ai_context@ context, pk::building@ building, int cmd)
		{
            int base_id = building.get_id();
            
            // 명령 취소완료 후 행동력 복원처리
            if (arr_cmd_canceled[base_id][cmd] and context.ap == 0)
            {
                context.ap = temp_ap;   // 행동력 복원
                arr_cmd_canceled[base_id][cmd] = false;
                
                if (디버그모드) pk::info(pk::format("---거점AI: {}, 행동력({}), {} 취소완료", pk::decode(pk::get_name(building)), context.ap, get_cmd_info(cmd) ));
            }
            
            // 행동력 감소한 경우, 명령실행한 것으로 간주
            if (context.ap < arr_building_ap[base_id])
            {
                if (디버그모드) pk::info(pk::format("---거점AI: {}, 행동력({}→{}), {} 실행완료", pk::decode(pk::get_name(building)), arr_building_ap[base_id], context.ap , get_cmd_info(cmd) ));
            }
            
			if (prev_callback_257 !is null)
				prev_callback_257(context, building, cmd);
		}
        
        
        // 명령 취소를 위한 행동력 강제변경 함수
        void cancel_cmd_256(pk::ai_context@ context, pk::building@ building, int cmd)
        {
            temp_ap = context.ap;
            context.ap = 0;     // 행동력 백업 후 0 으로 처리
            
            int base_id = building.get_id();
            arr_cmd_canceled[base_id][cmd] = true;
            
            if (디버그모드) pk::info(pk::format("---거점AI: {}, 행동력({}), {} 취소설정", pk::decode(pk::get_name(building)), temp_ap, get_cmd_info(cmd) ));
        }
        
        // 턴시작 시 명령금지설정 정보 초기화
        void onTurnStart(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            
            for (int i = 0; i < 건물_거점끝; i++)
            {
                for (int j = 0; j < 28; j++)
                    arr_cmd_canceled[i][j] = false;
            }
            
        }
        
        //---------------------------------------------------------------------------------------
        
        string get_cmd_info(int cmd)
        {
            string str_cmd;
            switch(cmd)
            {
                case 1  : str_cmd = "개발"          ; break;
                case 2  : str_cmd = "징병"          ; break;
                case 3  : str_cmd = "생산"          ; break;
                case 4  : str_cmd = "순찰"          ; break;
                case 5  : str_cmd = "거래"          ; break;
                case 6  : str_cmd = "훈련"          ; break;
                case 7  : str_cmd = "방어"          ; break;
                case 8  : str_cmd = "공격"          ; break;
                case 9  : str_cmd = "설치"          ; break;
                case 10 : str_cmd = "관문수송"       ; break;
                case 11 : str_cmd = "도시수송"       ; break;
                case 12 : str_cmd = "이동"          ; break;
                case 13 : str_cmd = "소환"          ; break;
                case 14 : str_cmd = "인재탐색"       ; break;
                case 15 : str_cmd = "무장등용"       ; break;
                case 16 : str_cmd = "포상"          ; break;
                case 17 : str_cmd = "이호경식"       ; break;
                case 18 : str_cmd = "구호탄랑"       ; break;
                case 19 : str_cmd = "유언비어"       ; break;
                case 20 : str_cmd = "타세력무장등용"   ; break;
                case 21 : str_cmd = "철거"          ; break;
                case 22 : str_cmd = "계략"          ; break;
                case 23 : str_cmd = "최소인재탐색"    ; break;
                case 24 : str_cmd = "최소무장등용"    ; break;
                case 25 : str_cmd = "최소타세력무장등용"; break;
                case 26 : str_cmd = "거래2"         ; break;
                case 27 : str_cmd = "흡수합병"       ; break;
            }
            return str_cmd;
        }
        
        //---------------------------------------------------------------------------------------

        bool 디버그모드 = false;

	}

	Main main;
}

