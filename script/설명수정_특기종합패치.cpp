/*
@만든이: 기마책사
@Update: '19.11.26  / 최초적용: 특기 설명을 pk2.1 특기.xml 세팅값에 맞춰서 표기
@Update: '19.12.5   / 변경적용: 무장.xml 설정값 반영안되도록 수정

// 특기종합패치를 위해 기마책사님의 스크립트를 수정한 파일입니다 (삼도카페 unno)
// 하단에 기교 설명도 포함되어 있습니다

*/

namespace 설명수정_특기종합패치
{
	class Main
	{
        
        Main()
		{
            pk::bind(100, pk::trigger100_t(onGameInit));   // 특기 설명 수정을 위해 추가
        }
        
        //---------------------------------------------------------------------------------------
        
        void onGameInit()
        {
            backup_person_skill();
            
            change_pk_skill_desc();  // 특기 설명 수정
            
            reset_person_skill();
            
            change_pk_tech_desc();  // 기교 설명 수정
        }
        
        //---------------------------------------------------------------------------------------
        // pk2.1 특기.xml 설정값을 특기 설명에 반영
        void change_pk_skill_desc()
        {
            int id_sel;
            int val1;
            int val2;
            float rate;
            
	pk::get_skill(특기_비장).desc = pk::u8encode("육상에서 ＺＯＣ 무시、적보다 무력이 높으면 전법 크리티컬．");
	pk::get_skill(특기_도주).desc = pk::u8encode("육상에서 ＺＯＣ 무시．");

            id_sel = set_person_skill(특기_강행);
            if (id_sel >= 0) 
            {
                val1 = int(pk::core::skill_constant(id_sel, 특기_강행, 0));
                pk::get_skill(특기_강행).desc = pk::u8encode(pk::format("육상 부대 이동력 {} 증가、거점의 아군 부대 이동력 2 증가．", val1) );
            }
            
            id_sel = set_person_skill(특기_행군);
            if (id_sel >= 0) 
            {
                val1 = int(pk::core::skill_constant(id_sel, 특기_행군, 0));
                pk::get_skill(특기_행군).desc = pk::u8encode(pk::format("기병 부대 이동력 {} 증가、기병 전법 성공률 5％ 증가．", val1) );
            }
            
	pk::get_skill(특기_추진).desc = pk::u8encode("수상에서 ＺＯＣ 무시．");

            id_sel = set_person_skill(특기_조타);
            if (id_sel >= 0)
            {
                val1 = int(pk::core::skill_constant(id_sel, 특기_조타, 0));
                pk::get_skill(특기_조타).desc = pk::u8encode(pk::format("수상에서 이동력 {} 증가．", val1) );
            }
            
	pk::get_skill(특기_답파).desc = pk::u8encode("벼랑길 통행시 피해 없음．낙석과 불함정 피해를 경감．");

            id_sel = set_person_skill(특기_운반);
            if (id_sel >= 0) 
            {
                val1 = int(pk::core::skill_constant(id_sel, 특기_운반, 0));
                pk::get_skill(특기_운반).desc = pk::u8encode(pk::format("수송 부대 이동력 {} 증가．", val1) );
            }
            
	pk::get_skill(특기_해독).desc = pk::u8encode("독천 통행시 피해 없음．");

            id_sel = set_person_skill(특기_소탕);
            if (id_sel >= 0) 
            {
                val1 = int(pk::core::skill_constant(id_sel, 특기_소탕, 0));
                pk::get_skill(특기_소탕).desc = pk::u8encode(pk::format("공격한 적 부대의 기력 {} 감소、기력 소진시 추가 피해．", val1) );
            }
            
            id_sel = set_person_skill(특기_위풍);
            if (id_sel >= 0) 
            {
                val1 = int(pk::core::skill_constant(id_sel, 특기_위풍, 0));
                pk::get_skill(특기_위풍).desc = pk::u8encode(pk::format("공격한 적 부대의 기력 {} 감소、기력 소진시 추가 피해．", val1) );
            }
            
            id_sel = set_person_skill(특기_앙양);
            if (id_sel >= 0) 
            {
                val1 = int(pk::core::skill_constant(id_sel, 특기_앙양, 0));
                pk::get_skill(특기_앙양).desc = pk::u8encode(pk::format("적 부대 격파시 기력 {} 회복、전법 성공시 5 회복．", val1) );
            }
            
            id_sel = set_person_skill(특기_연전);
            if (id_sel >= 0) 
            {
                val1 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_연전, 0))));
                pk::get_skill(특기_연전).desc = pk::u8encode(pk::format("통상 공격시 {}％ 확률로 한 번 더 공격．", val1) );
            }
            
            id_sel = set_person_skill(특기_급습);
            if (id_sel >= 0) 
            {
                val1 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_급습, 0))));
                pk::get_skill(특기_급습).desc = pk::u8encode(pk::format("육상에서 공격시 {}％ 확률로 무피해、상태이상시 추가 피해．", val1) );
            }
            
            id_sel = set_person_skill(특기_강습);
            if (id_sel >= 0) 
            {
                val1 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_강습, 0))));
                pk::get_skill(특기_강습).desc = pk::u8encode(pk::format("수상에서 공격시 {}％ 확률로 무피해、상태이상시 추가 피해．", val1) );
            }
            
	pk::get_skill(특기_난전).desc = pk::u8encode("숲에서 공격시 크리티컬．");
	pk::get_skill(특기_공성).desc = pk::u8encode("거점、시설물 공격시 크리티컬．요새의 방어효과 무시．");

            id_sel = set_person_skill(특기_기각);
            if (id_sel >= 0) 
            {   
                val1 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_기각, 0))));
                pk::get_skill(특기_기각).desc = pk::u8encode(pk::format("일제 공격시 {}％ 확률로 적을 교란．", val1) );
            }
          
            id_sel = set_person_skill(특기_포박);
            if (id_sel >= 0) 
            {
                val1 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_포박, 0))));
                pk::get_skill(특기_포박).desc = pk::u8encode(pk::format("적 부대 격파시 포획 확률 {}％ 증가．", val1) );
            }
            
	pk::get_skill(특기_정묘).desc = pk::u8encode("적 부대 격파시 획득 기교 2 배．전법 성공률 5％ 증가．");

            id_sel = set_person_skill(특기_강탈);
            if (id_sel >= 0) 
            {
                val1 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_강탈, 0))));
                pk::get_skill(특기_강탈).desc = pk::u8encode(pk::format("적 부대 격파시 {}％ 확률로 보물 탈취．", val1) );
            }
            
            id_sel = set_person_skill(특기_심공);
            if (id_sel >= 0) 
            {
                rate = pk::min(100.f, pk::max(0.f, float(pk::core::skill_constant(id_sel, 특기_심공, 0))));
                val1 = int(100.f * rate);
                pk::get_skill(특기_심공).desc = pk::u8encode(pk::format("공격시 적 피해의 {}％를 자신 부대로 흡수．", val1) );
            }

	pk::get_skill(특기_구축).desc = pk::u8encode("통상 공격시 크리티컬．");
	pk::get_skill(특기_사정).desc = pk::u8encode("정란、투석의 사거리 한 칸 증가．투석 전법시 지력 사용 가능．");
	pk::get_skill(특기_백마).desc = pk::u8encode("기병 부대가 사격 가능．기마사격시 크리티컬．");
	pk::get_skill(특기_보좌).desc = pk::u8encode("주장이면 지원 공격에서、부장이면 능력 기여에서 관계 향상．");
	pk::get_skill(특기_창장).desc = pk::u8encode("적보다 무력이 높으면 창병 전법 성공시 크리티컬．");
	pk::get_skill(특기_극장).desc = pk::u8encode("적보다 무력이 높으면 극병 전법 성공시 크리티컬．");
	pk::get_skill(특기_궁장).desc = pk::u8encode("적보다 무력이 높으면 궁병 전법 성공시 크리티컬．");
	pk::get_skill(특기_기장).desc = pk::u8encode("적보다 무력이 높으면 기병 전법 성공시 크리티컬．");
	pk::get_skill(특기_수장).desc = pk::u8encode("적보다 무력이 높으면 수군 전법 성공시 크리티컬．");
	pk::get_skill(특기_용장).desc = pk::u8encode("적보다 무력이 높으면 모든 전법 성공시 크리티컬．");
	pk::get_skill(특기_신장).desc = pk::u8encode("적보다 무력이 높으면 육상 전법、통상 공격시 크리티컬．");
	pk::get_skill(특기_투신).desc = pk::u8encode("창병 전법과 극병 전법 성공시 크리티컬．태고대 효과 증가．");
	pk::get_skill(특기_창신).desc = pk::u8encode("창병 전법 성공시 크리티컬．");
	pk::get_skill(특기_극신).desc = pk::u8encode("극병 전법 성공시 크리티컬．");
	pk::get_skill(특기_궁신).desc = pk::u8encode("궁병 전법 성공시 크리티컬．");
	pk::get_skill(특기_기신).desc = pk::u8encode("기병 전법 성공시 크리티컬．");
	pk::get_skill(특기_공신).desc = pk::u8encode("병기 전법 성공시 크리티컬．");
	pk::get_skill(특기_수신).desc = pk::u8encode("수군 전법 성공시 크리티컬．");
	pk::get_skill(특기_패왕).desc = pk::u8encode("모든 전법 성공시 크리티컬．");
            
            id_sel = set_person_skill(특기_질주);
            if (id_sel >= 0) 
            {
                val1 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_질주, 0))));
                pk::get_skill(특기_질주).desc = pk::u8encode(pk::format("적보다 공격력이 높으면 기병 전법 성공시 {}％ 교란．", val1) );
            }

	pk::get_skill(특기_사수).desc = pk::u8encode("노병 부대가 숲으로 사격 가능、노병 전법 성공률 5％ 증가．");

            id_sel = set_person_skill(특기_맹장);
            if (id_sel >= 0)
            {
                val1 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_맹장, 0))));
                pk::get_skill(특기_맹장).desc = pk::u8encode(pk::format("전법으로 적 부대를 이동시키면 {}％ 확률로 적장을 상해．", val1) );
            }

            id_sel = set_person_skill(특기_불굴);
            if (id_sel >= 0) 
            {
                val1 = int(pk::core::skill_constant(id_sel, 특기_불굴, 0));
                val2 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_불굴, 1))));
                pk::get_skill(특기_불굴).desc = pk::u8encode(pk::format("병력이 {} 보다 적으면 {}％ 확률로 통상 공격을 막음．", val1, val2) );
            }
            
            id_sel = set_person_skill(특기_금강);
            if (id_sel >= 0) 
            {
                val1 = int(pk::core::skill_constant(id_sel, 특기_금강, 0));
                val2 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_금강, 1))));
                pk::get_skill(특기_금강).desc = pk::u8encode(pk::format("피해량이 {} 보다 작으면 {}％ 확률로 통상 공격을 막음．", val1, val2) );
            }

	pk::get_skill(특기_철벽).desc = pk::u8encode("일제 공격을 당하지 않음．요새 시설에서 방어력 증가．");

            id_sel = set_person_skill(특기_노발);
            if (id_sel >= 0) 
            {
                val1 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_노발, 0))));
                pk::get_skill(특기_노발).desc = pk::u8encode(pk::format("적 전법을 당하면 기력 {} 회복、자신 전법을 실패하면 10 회복．", val1) );
            }
            
	pk::get_skill(특기_등갑).desc = pk::u8encode("불 이외의 공격은 피해를 반감．(단、불에 의한 피해는 2 배)");
	pk::get_skill(특기_강운).desc = pk::u8encode("전사、포박、부상을 당하지 않음．");
	pk::get_skill(특기_혈로).desc = pk::u8encode("자신부대와 인접부대가 포박을 당하지 않음．");
	pk::get_skill(특기_호위).desc = pk::u8encode("자신부대가 전사、부상、보병의 이동 전법을 당하지 않음．");
	pk::get_skill(특기_매복).desc = pk::u8encode("적보다 지력이 높으면「복병」을 반드시 성공．");
	pk::get_skill(특기_화공).desc = pk::u8encode("적보다 지력이 높으면「화계」를 반드시 성공．불 강화．");
	pk::get_skill(특기_언독).desc = pk::u8encode("적보다 지력이 높으면「위보」를 반드시 성공．");
	pk::get_skill(특기_기략).desc = pk::u8encode("적보다 지력이 높으면「교란」을 반드시 성공．");
	pk::get_skill(특기_궤계).desc = pk::u8encode("적보다 지력이 높으면「동토」를 반드시 성공．");
	pk::get_skill(특기_허실).desc = pk::u8encode("적보다 지력이 높으면 모든 계략을 반드시 성공．");
	pk::get_skill(특기_묘계).desc = pk::u8encode("자신과 떨어진 부대에 모든 계략 성공시 크리티컬．");
	pk::get_skill(특기_비책).desc = pk::u8encode("자신과 인접한 부대에 모든 계략 성공시 크리티컬．");
	pk::get_skill(특기_간파).desc = pk::u8encode("적보다 지력이 높으면 모든 계략을 반드시 간파．");
	pk::get_skill(특기_통찰).desc = pk::u8encode("적의 계략에 당하지 않음．");
	pk::get_skill(특기_화신).desc = pk::u8encode("적보다 지력이 높으면「화계」를 반드시 성공．불에 우대．");
	pk::get_skill(특기_신산).desc = pk::u8encode("적보다 지력이 높으면 모든 계략을 반드시 성공．계략에 우대．");
	pk::get_skill(특기_백출).desc = pk::u8encode("모든 계략의 소비 기력이 1 로 감소．");
	pk::get_skill(특기_귀모).desc = pk::u8encode("계략 실행거리 한 칸 증가、자신과 떨어진 부대에 성공률 2 배．");
	pk::get_skill(특기_연환).desc = pk::u8encode("계략 성공시 옆 칸에 반복、자신과 인접한 부대에 성공률 2 배．");
	pk::get_skill(특기_심모).desc = pk::u8encode("모든 계략 성공시 크리티컬．");
	pk::get_skill(특기_반계).desc = pk::u8encode("계략을 간파하면 반사．상대의 계략 특기를 무시．");
	pk::get_skill(특기_경국).desc = pk::u8encode("여성 무장이 없는 부대에 계략 성공률 2 배．");
	pk::get_skill(특기_요술).desc = pk::u8encode("「요술」을 사용 가능．");
	pk::get_skill(특기_귀문).desc = pk::u8encode("「요술、낙뢰」를 사용 가능．");
	pk::get_skill(특기_규율).desc = pk::u8encode("「위보、동토」를 당하지 않음．");
	pk::get_skill(특기_침착).desc = pk::u8encode("「교란、복병」을 당하지 않음．");
	pk::get_skill(특기_명경).desc = pk::u8encode("「위보、교란」을 당하지 않음．");
            
            id_sel = set_person_skill(특기_주악);
            if (id_sel >= 0) 
            {
                val1 = int(pk::core::skill_constant(id_sel, 특기_주악, 0));
                pk::get_skill(특기_주악).desc = pk::u8encode(pk::format("매 차례 부대의 기력 {} 회복．", val1) );
            }
            
	pk::get_skill(특기_시상).desc = pk::u8encode("군악대에 기력 회복량 2 배．");
	pk::get_skill(특기_축성).desc = pk::u8encode("군사시설의 건설 속도 2 배．");
	pk::get_skill(특기_둔전).desc = pk::u8encode("관문、항구、요새 시설에서 병량 소비량 절감．");
	pk::get_skill(특기_명성).desc = pk::u8encode("징병 모집량 50％ 증가．");
	pk::get_skill(특기_능리).desc = pk::u8encode("창、극、노 생산량 100％ 증가．");
	pk::get_skill(특기_번식).desc = pk::u8encode("군마 생산량 100％ 증가．");
	pk::get_skill(특기_발명).desc = pk::u8encode("공성병기 생산 기간 반감．");
	pk::get_skill(특기_조선).desc = pk::u8encode("함선 생산 기간 반감．");
	pk::get_skill(특기_지도).desc = pk::u8encode("기교 연구시 비용 반감．매 계절 거점 무장의 경험치 증가．");
	pk::get_skill(특기_안력).desc = pk::u8encode("재야 무장을 반드시 발견．매 월 거점 무장의 공적 증가．");
	pk::get_skill(특기_논객).desc = pk::u8encode("동맹、정전 협정、포로 교환에서 설전으로 해결 가능．");
	pk::get_skill(특기_부호).desc = pk::u8encode("도시의 금 수입 50％ 증가．");
	pk::get_skill(특기_미도).desc = pk::u8encode("도시의 병량 수입 50％ 증가．");
	pk::get_skill(특기_징세).desc = pk::u8encode("도시의 금 수입 50％ 감소．매 차례 수입 획득．");
	pk::get_skill(특기_징수).desc = pk::u8encode("도시의 병량 수입 50％ 감소．매 월 수입 획득．");
	pk::get_skill(특기_친오).desc = pk::u8encode("「양평、북평、계」에서「오환」발생과 치안 하락이 없음．");
	pk::get_skill(특기_친강).desc = pk::u8encode("「무위、안정、천수」에서「강」발생과 치안 하락이 없음．");
	pk::get_skill(특기_친월).desc = pk::u8encode("「회계、오、시상」에서「산월」발생과 치안 하락이 없음．");
	pk::get_skill(특기_친만).desc = pk::u8encode("「운남、건녕、강주」에서「남만」발생과 치안 하락이 없음．");
	pk::get_skill(특기_위압).desc = pk::u8encode("도적과 이민족의 발생을 억제．치안이 낮아도 징병량 유지．");
	pk::get_skill(특기_인정).desc = pk::u8encode("거점 무장의 충성도가 내려가지 않음．도시를 축복．");
	pk::get_skill(특기_풍수).desc = pk::u8encode("거점에서 메뚜기、역병이 발생하지 않음．도시를 축복．");
	pk::get_skill(특기_기원).desc = pk::u8encode("거점에서 풍작이 자주 일어남．도시를 축복．");

            id_sel = set_person_skill(특기_내조);
            if (id_sel >= 0)
            {
                val1 = pk::min(100, pk::max(0, int(pk::core::skill_constant(id_sel, 특기_내조, 0))));
                pk::get_skill(특기_내조).desc = pk::u8encode(pk::format("결혼하면 부부의 모든 능력이 {} 증가．", val1) );
            }
            
        }

                    void change_pk_tech_desc()
        {
	pk::get_tech(기교_창병단련).desc = pk::u8encode("창병 부대의 피해 증가.");
	pk::get_tech(기교_병량습격).desc = pk::u8encode("창병으로 공격시 적 병량을 탈취한다.");
	pk::get_tech(기교_기습).desc = pk::u8encode("숲에서 창병이 반격을 받지 않음.");
	pk::get_tech(기교_정예창병).desc = pk::u8encode("창병의 공격、방어、이동을 강화.");

	pk::get_tech(기교_극병단련).desc = pk::u8encode("극병 부대의 피해 증가.");
	pk::get_tech(기교_화살방패).desc = pk::u8encode("화살 공격을 30％의 확률로 막는다.");
	pk::get_tech(기교_큰방패).desc = pk::u8encode("통상 공격을 30％의 확률로 막는다.");
	pk::get_tech(기교_정예극병).desc = pk::u8encode("극병의 공격、방어、이동을 강화.");

	pk::get_tech(기교_노병단련).desc = pk::u8encode("노병 부대의 피해 증가.");
	pk::get_tech(기교_응사).desc = pk::u8encode("노병이 화살 공격을 받았을 때 반격한다.");
	pk::get_tech(기교_강노).desc = pk::u8encode("노병의 사정거리가 한 칸 늘어난다.");
	pk::get_tech(기교_정예노병).desc = pk::u8encode("노병의 공격、방어、이동을 강화.");

	pk::get_tech(기교_기병단련).desc = pk::u8encode("기병 부대의 피해 증가.");
	pk::get_tech(기교_양마산출).desc = pk::u8encode("기병 이동력 4 증가. 마구간 효율 향상.");
	pk::get_tech(기교_기사).desc = pk::u8encode("기병으로 화살공격을 할 수 있다.");
	pk::get_tech(기교_정예기병).desc = pk::u8encode("기병의 공격、방어、이동을 강화.");

	pk::get_tech(기교_숙련병).desc = pk::u8encode("기력 상한 20 증가.");
	pk::get_tech(기교_난소행군).desc = pk::u8encode("샛길、여울 통행 가능. 벼랑길 피해 없음.");
	pk::get_tech(기교_군제개혁).desc = pk::u8encode("병력 상한 3000 증가. 대장간 효율 향상.");
	pk::get_tech(기교_운제).desc = pk::u8encode("거점、시설에 대한 공격력 증가.");

	pk::get_tech(기교_차축강화).desc = pk::u8encode("공성병기 이동력 4 증가.");
	pk::get_tech(기교_석조건축).desc = pk::u8encode("장애물「석벽」. 시설「석병팔진」연구.");
	pk::get_tech(기교_투석개발).desc = pk::u8encode("병기「투석、투함」. 시설「투석대」연구.");
	pk::get_tech(기교_벽력).desc = pk::u8encode("투석 공격시 주위 한 칸도 공격.");

	pk::get_tech(기교_공병육성).desc = pk::u8encode("매 차례 내구 회복량 증가.");
	pk::get_tech(기교_시설강화).desc = pk::u8encode("시설「요새、연노로」연구.");
	pk::get_tech(기교_성벽강화).desc = pk::u8encode("시설「성채」연구. 거점의 내구 증가.");
	pk::get_tech(기교_방어강화).desc = pk::u8encode("거점、시설의 반격 강화.");

	pk::get_tech(기교_목수개발).desc = pk::u8encode("병기「목수」연구.");
	pk::get_tech(기교_신화계).desc = pk::u8encode("「화계」범위 3 칸으로 확장.");
	pk::get_tech(기교_화약연성).desc = pk::u8encode("함정「화염종、화염구」연구.");
	pk::get_tech(기교_폭약연성).desc = pk::u8encode("함정「업화종、업화구」연구. 불 강화.");

	pk::get_tech(기교_목우유마).desc = pk::u8encode("수송부대 이동력 3 증가.");
	pk::get_tech(기교_항관확장).desc = pk::u8encode("관문、항구의 용량 증가. 시장 효율 향상.");
	pk::get_tech(기교_법령정비).desc = pk::u8encode("도시의 치안저하를 완화. 농장 효율 향상.");
	pk::get_tech(기교_인심장악).desc = pk::u8encode("무장의 충성저하를 완화. 병영 효율 향상.");
        }
        
        //-----------------------------------
        // 특기를 가진 무장 검색 (미사용)
        int find_person_skill(int skill_id)
        {
            int person_id = -1;
            for (int i = 0; i <= 무장_노식; i++)
            {
                pk::person@ person = pk::get_person(i);
                if (person !is null and person.skill == skill_id)
                {
                    person_id = i;
                    break;
                }
            }
            return person_id;
        }
        
        //-----------------------------------
        int skill_id_backup;
        int person_id_t = 무장_무관;
                
        // NPC 무장 특기 임의 지정
        int set_person_skill(int skill_id)
        {
            pk::person@ person = pk::get_person(person_id_t);
            if (person !is null) person.skill = skill_id;
            return person_id_t;
        }
        
        // NPC 무장 특기 백업
        void backup_person_skill()
        {
            pk::person@ person = pk::get_person(person_id_t);
            if (person !is null) skill_id_backup = person.skill;
            return;
        }
        
        // NPC 무장 특기 삭제
        void reset_person_skill()
        {
            pk::person@ person = pk::get_person(person_id_t);
            if (person !is null) person.skill = skill_id_backup;
            return;
        }
    }
    
	Main main;
}

