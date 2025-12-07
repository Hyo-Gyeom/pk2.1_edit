/*
@만든이: 삼도카페 호위장군조운
@설명: 출진 부대 병과 최적화  (161012 이후 사용자용)
	출진하는 AI 부대의 병종(병과)을 특기/적성/세력기교에 기초하여 가장 적합하게 변경함
	(특기 > 적성 >> 세력기교)

  참고) 목수개발/투석개발 기교를 완료한 상태에서 충차/정란 부대를 등용한 후
	충차나 정란 부대가 거점에 들어가면 병기 수가 늘어나지 않음(삼국지 순정에 있는 일종의 버그)

부대 구분
- 검병부대, 일반부대(창병,극병,노병,기병), 병기부대(충차,목수,정란,투석)

※ 변경 기본 사항 ※
 - 병기부대, 검병부대 -> 일반부대 (O)
 - 검병부대, 일반부대 -> 병기부대 (X)
 - 병종 전용 특기 (예. 질주) 보유 부대인 경우는 적성이 낮은 부대로 변경되는 경우도 있음
  때문에, 위임 군단인 경우 (설정에 따라) 재고가 부족한 무기로는 변경하지 않기 때문에
  기능이 제대로 동작하지 않는 것으로 오해할 수 있음.
   컴퓨터 세력 부대는 재고를 확인하지 않음
*/
/*
// 수정자 : 삼도카페 기마책사
// 수정내용('20.9.11) : 
//       컴퓨터세력_무기재고부족_허용 옵션 추가, 트리거우선순위 반영,
//       컴퓨터세력 부족병력/병량 보충 및 금보상 옵션 설정 추가 (기존 알고리즘에 유저옵션 기능만 상단에 추가)
//       컴퓨터세력_병기혜택_설정 사용 시 부대궤멸이 아닌 조건에서도 동작하는 버그 수정 (거점진입, 출진취소 등)
//       병과변경 시 set_amount_weapon 함수만 사용함으로 인해 변경전 병과 기준으로 이동범위조회 되어 이동불가지형으로 이동가능한 버그 수정
//       병과변경에 따른 병기 반환시점 조정 (유닛생성 직후 → 유닛출진 이동후, 거점상한 이상 증발되는 버그 수정)
//        ㄴ병기 반환시점 조정에 따라 병과변경 순간의 부대 병기수량 반영하던 것을 반환시점 조정에 맞추어 이동후 계산으로 변경
//        ㄴ병기 반환시점 조정에 따라 유저 조종에 의한 출진취소 시 병기반환하던 함수 불필요 (트리거 삭졔)
//       모든 트리거에 우선 순위 지정값 추가 (다른 패치와의 연동 시 우선순위 조정 필요)
// 수정내용('20.10.18) : 관문/항구 방어출진용 간접병기 출진이면 병과변경 안함
*/

namespace 조운_출진부대병과최적화
{
////////////////////////////////////////////////////////////////////////////
bool  게임시작시_안내메시지_표시 = false;
bool  병종변경확인창_한번만표시 = true;     // (기본값 false) false일 경우 게임로드 시마다 확인, true 일 경우 최초 변경 시 설정값 계속 유지
bool  병종변경_허용 = true;	                 // (기본값 true), false 일 경우, 모든 부대의 병종 변경 기능을 사용하지 않음. (원본과 동일)
////////////////////////////////////////////////////////////////////////////
///* 사용자 변경 가능 설정
bool  플레이어_위임군단_병종변경_허용 = true;	// (기본값 true), false 일 경우, 위임 군단에서는 출진 부대 병종 변경을 하지 않음 - 유저가 직접 출진시킬 경우도 자동 변경됨!
bool  플레이어_위임군단_무기재고부족_허용 = false;	// (기본값 false), true 일 경우, 거점 무기수와 상관없이 병종 변경, false 일 경우, 무기가 부족한 병종으로는 변경되지 않음, 참고) 
bool  컴퓨터세력_무기재고부족_허용 = false;	    // (기본값 true),  true 일 경우, 거점 무기수와 상관없이 병종 변경, false 일 경우, 무기가 부족한 병종으로는 변경되지 않음, 참고)
bool  컴퓨터세력_부족병력_보충  = false; 	    // (기본값 true),  true 일 경우, 출진병력이 기준병력 이하인 경우 기준병력으로 강제보충, false 일 경우, 병력 부족하더라도 강제보충 안함
float 컴퓨터세력_병력보충_기준  = 0.4f;        // (기본값 0.4f),  컴퓨터세력_부족병력_보충 true 일 경우, 거점병력과 상관없이 기준병력 이상으로 부대출진 허용, false 일 경우, 병력 부족하더라도 강제보충 안함
bool  컴퓨터세력_병기전환_금보상 = false; 	    // (기본값 true),  true 일 경우, 병기부대 → 일반부대 변경 시 부대 금 +400, false 일 경우, 금보상 안함
bool  컴퓨터세력_부족병량_보충  = false; 	    // (기본값 true),  true 일 경우, 부대 병량 부족 시 보충, false 일 경우, 병력 부족하더라도 강제보충 안함
bool  컴퓨터세력_병기혜택_설정  = true;       // (기본값 true),  true 일 경우, 공성병기 부대 궤멸 시 소속 거점으로 병기 반환, false 일 경우, 병기 반환 안함
bool  병기부대_출진허용 = true;	// (기본값 true), false 일 경우, 출진하는 모든 부대는 일반부대로만 출진
bool  병기부대_출진알림 = false;	// (기본값 false), 병기부대 출진 알림 여부 - 알림창(확인 클릭 필요)으로 표시
// ※ [병기부대_출진허용] 설정이 false 면 [병기부대_변경허용] 설정은 무조건 true 가 됨
bool  병기부대_변경허용 = false;	// (기본값 true), false 일 경우, 병기부대는 그대로 출진함
bool  일반부대_변경허용 = true;	// (기본값 true), 일반부대를 더 나은 일반부대로 변경할 것인지 여부
bool  검병부대_변경허용 = true;	// (기본값 true), 검병부대를 일반부대로 변경할 것인지 여부
bool  출진거점_표시 = false;	// (기본값 false), 부대가 출진하는 거점 표시 여부
bool  사정특기_충차에서_정란으로_변경허용 = true;	// (기본값 false), 사정 특기 부대 충차/목수 -> 정란/투석 으로 변경
int   병기부대_출진가능_적성 = 적성_A;	// (기본값 적성_B), 적성_C / 적성_B / 적성_A / 적성_S 형태로 입력, 지정 적성 미만은 출진하지 않음
int   일반부대_출진가능_적성차이 = 1;	// (기본값 1)
int   병기부대_출진가능_적성차이 = 1;	// (기본값 1)

bool  관문항구_방어출진부대_변경금지 = false;
//*/

class ChangeBetterWeapon
{
	string 출진부대병과최적화_버전 = "v20161017";

    int 출진부대최적화_트리거_우선순위_부대변경 = 50;   // [출진무장최적화], [출진병력최적화] 패치보다 낮은 값으로 설정
    int 출진부대최적화_트리거_우선순위_병기반환 = 255;  // [출진무장최적화], [출진병력최적화] 패치보다 높은 값으로 설정
	bool bDataInitialized = false;

	float 가중치_허용_오차 = 0.2f;

	array<float> 병종_기본_가중치(병종_끝, 0.0f);
	array<float> 무기_기본_가중치(병기_끝, 0.0f);
	array<dictionary> 병종_특기_가중치(병종_끝);
	array<dictionary> 무기_특기_가중치(병기_끝);
	array<int> 적성별_병기_기본출진확률(적성_끝, 100);

	ForceSkillWeight forceSkillWeight;

	bool 병종변경_메시지표시 = true;
	int 병종변경_메시지표시_값 = 1;	// 0 : 메시지 박스로 표시, 1 : 말풍선으로 표시, 2 : 표시하지 않음
	bool 병종변경_메시지표시_확인 = false;

	bool 컴퓨터세력_병기_혜택 = 컴퓨터세력_병기혜택_설정;
    
    bool 컴퓨터세력_유저조종시_병종변경_허용 = true; // (기본값 false), true 일 경우, 컴퓨터세력 조종 모드에서 컴퓨터부대 최적화 동작, false 일 경우, 컴퓨터세력 조종 모드에서 컴퓨터부대 최적화 동작 안함
    bool 게임_재로드 = false;

	uint changeCount;

	uint countSiegeWeapon;

	uint changeCountFromSiegeToNormalWeapon;
	uint changeCountFromNormalToSiegeWeapon;

	uint cancelCount = 0;
	uint siegeWeaponCreationCount = 0;
	uint64 totalCountCreation = 0;

	RestoreWeaponInfo restoreWeaponInfo1;
	RestoreWeaponInfo restoreWeaponInfo2;

	ChangeBetterWeapon()
	{
		if (병종변경_허용)
			initialize();
	}

	void initialize()
	{
		if (bDataInitialized)
			return;

		// 가중치는 적성 1단계 = 1.0f 와 동일

		//병종_기본_가중치[병종_병기] = 0.0f;

		무기_기본_가중치[병기_검] = -2.0f;

		// 병종별 특기 가중치 설정
		dictionary 창병_특기 = { {특기_비장+"", 5.0f}, {특기_패왕+"", 5.0f}, {특기_신장+"", 4.5f}, {특기_용장+"", 5.0f}, {특기_투신+"", 5.0f}, {특기_창신+"", 5.0f}, {특기_창장+"", 4.0f}, {특기_맹장+"", 2.5f} };//창병
		dictionary 극병_특기 = { {특기_비장+"", 7.0f}, {특기_패왕+"", 5.0f}, {특기_신장+"", 4.5f}, {특기_용장+"", 5.0f}, {특기_투신+"", 5.0f}, {특기_극신+"", 5.0f}, {특기_극장+"", 4.0f}, {특기_맹장+"", 2.5f} };//극병
		dictionary 노병_특기 = { {특기_비장+"", 4.5f}, {특기_패왕+"", 4.5f}, {특기_신장+"", 4.5f}, {특기_용장+"", 4.5f}, {특기_궁신+"", 9.0f}, {특기_궁장+"", 5.0f}, {특기_보좌+"", 1.0f}, {특기_사수+"", 3.5f} };//노병
		dictionary 기병_특기 = { {특기_비장+"", 7.0f}, {특기_패왕+"", 5.0f}, {특기_신장+"", 4.5f}, {특기_용장+"", 5.0f}, {특기_기신+"", 9.0f}, {특기_기장+"", 4.0f}, {특기_백마+"", 4.0f}, {특기_질주+"", 7.0f}, {특기_맹장+"", 3.5f} };//기병
		dictionary 병기_특기 = { {특기_공신+"", 9.0f}, {특기_공성+"", 5.0f}, {특기_사정+"", 5.0f}, {특기_발명+"", 1.0f} };//병기

		//노병_특기.set( 특기_심공+"", -0.2f );
		노병_특기.set( 특기_맹장+"", -0.5f );
		
		병기_특기.set( 특기_연전+"", -0.5f );
		병기_특기.set( 특기_심공+"", -3.0f );
		병기_특기.set( 특기_구축+"", -5.0f );
		병기_특기.set( 특기_포박+"", -0.5f );
		병기_특기.set( 특기_보좌+"", -1.0f );
		병기_특기.set( 특기_급습+"", -0.5f );
		병기_특기.set( 특기_맹장+"", -1.5f );

		병종_특기_가중치[병종_창병] = 창병_특기;
		병종_특기_가중치[병종_극병] = 극병_특기;
		병종_특기_가중치[병종_노병] = 노병_특기;
		병종_특기_가중치[병종_기병] = 기병_특기;
		병종_특기_가중치[병종_병기] = 병기_특기;

		// 병과별 특기 가중치 설정
		//dictionary 충차_특기 = { };
		//dictionary 목수_특기 = { };

		// 사정 특기는 정란과 투석에만 이점이 있으므로, 병종_병기 가 아닌 병기_정란, 병기_투석 에만 설정함
		dictionary 정란_특기 = { {특기_사정+"", 4.0f} };	// 정란
		dictionary 투석_특기 = { {특기_사정+"", 4.0f} };	// 투석

		무기_특기_가중치[병기_정란] = 정란_특기;
		무기_특기_가중치[병기_투석] = 투석_특기;

		적성별_병기_기본출진확률[적성_C] = 15;
		적성별_병기_기본출진확률[적성_B] = 35;
		적성별_병기_기본출진확률[적성_A] = 75;

		pk::bind(103, 출진부대최적화_트리거_우선순위_부대변경, pk::trigger103_t(CallBack_GameStart));
		pk::bind(170, 출진부대최적화_트리거_우선순위_부대변경, pk::trigger170_t(CallBack_UnitCreated));

        // 기마책사 추가 (병기 반환 시점 조정용)
        pk::bind(172, 출진부대최적화_트리거_우선순위_병기반환, pk::trigger172_t(RestoreUnitWeapon));
		// ------------ 기마책사 수정 : 병기반환 시점을 유닛생성 직후에서 유닛출진 후로 변경하면서 출진취소 트리거 사용할 필요 없어졌음
        //pk::bind(171, 출진부대최적화_트리거_우선순위_부대변경, pk::trigger171_t(CallBack_UnitCreationCanceled));
		
        if (컴퓨터세력_병기_혜택)
			pk::bind(171, 출진부대최적화_트리거_우선순위_부대변경, pk::trigger171_t(CallBack_UnitDefeated));

		if (!병기부대_출진허용)
			병기부대_변경허용 = true;

		bDataInitialized = true;
        
	}

	void resetVals()
	{
		병종변경_메시지표시_확인 = false;
		병종변경_메시지표시 = !병종변경_메시지표시_확인;
		병종변경_메시지표시_값 = 1;

		changeCount = changeCountFromSiegeToNormalWeapon = changeCountFromNormalToSiegeWeapon = countSiegeWeapon = 0;

		cancelCount = 0;
		totalCountCreation = 0;
		siegeWeaponCreationCount = 0;

		forceSkillWeight.initialize();
	}

	void notify()
	{
		if (병종변경_허용)
		{
			string question = "";

			question = "\x1b[1x출진 부대 병과 최적화\x1b[0x가 \x1b[1x적용\x1b[0x되고 있습니다.";
			if (플레이어_위임군단_병종변경_허용)
				question += pk::format("(위임군단 \x1b[1x변경허용\x1b[0x, 재고부족시 {}\x1b[0x)", (플레이어_위임군단_무기재고부족_허용 ? "\x1b[1x변경허용" : "\x1b[16x변경불가"));
			question += pk::format(" \x1b[28x{}", 출진부대병과최적화_버전);

			pk::choose({ pk::encode(" 확인 ") }, pk::encode(question), pk::get_person(무장_무관));
		}
	}

	void CallBack_GameStart()
	{
        if (!게임_재로드)
        {
            resetVals();
            게임_재로드 = true;
            if (!병종변경확인창_한번만표시) 
                게임_재로드 = false;
        }
        
        if(게임시작시_안내메시지_표시)
            pk::scene(pk::scene_t(notify));
	}

	// 부대 출진이 취소되었을 경우 (위임군단 또는 컴퓨터 세력 부대 조종 시) ... 기마책사 수정 : 컴퓨터 세력도 포함시킴
	// NOTE 플레이어가 직접 출진->취소를 한 경우에만 호출
	void CallBack_UnitCreationCanceled(pk::unit@ unit, int type)
	{
		if (!pk::is_alive(unit)) return;
        if (type == 1) return;  // 진입 제외

        if (!unit.is_player() or (플레이어_위임군단_병종변경_허용 and isPlayerDelegatedUnit(unit)))
        {
            pk::building@ base = pk::get_building(pk::get_service(unit));
            pk::building@ buliding_t = pk::get_building(unit.pos);
            if (pk::is_alive(base) && pk::is_alive(buliding_t) && pk::is_valid_building_id(base.get_id())
                && base.get_force_id() == unit.get_force_id() && buliding_t.get_id() == base.get_id())
            {
                string base_name = pk::decode(pk::get_name(base));
                string unit_name = pk::decode(pk::get_name(pk::get_person(unit.leader)));
                if (디버그모드) pk::info(pk::format("{}대 출진취소: {} {}({}), {}({})", unit_name, base_name, 병기_이름[restoreWeaponInfo1.weapon], pk::get_weapon_amount(base, restoreWeaponInfo1.weapon), 병기_이름[restoreWeaponInfo2.weapon], pk::get_weapon_amount(base, restoreWeaponInfo2.weapon) ));
                
                if (unit.get_id() == restoreWeaponInfo1.getUnitId()) restoreWeaponInfo1.restoreTo(base);
                if (unit.get_id() == restoreWeaponInfo2.getUnitId()) restoreWeaponInfo2.restoreTo(base);
                
            }
		}
	}

	void CallBack_UnitDefeated(pk::unit@ unit, int type)
	{
		if (!pk::is_alive(unit) || !pk::is_normal_force(unit.get_force_id())) return;

		int weapon = pk::get_ground_weapon_id(unit);
		if (!unit.is_player() && isSiegeWeapon(weapon))
		{
			pk::building@ base = pk::get_building(pk::get_service(unit));
            
			if (pk::is_alive(base) && pk::is_valid_building_id(base.get_id()) && base.get_force_id() == unit.get_force_id())
			{
                // ---------- 기마책사 추가 : 제거된 유닛 위치가 출진거점(출진취소 상황), 세력 거점으로 진입인 경우 제외
                pk::building@ buliding_t = pk::get_building(unit.pos);
                if (type == 1) return;  // 진입
                if (pk::is_alive(buliding_t) and buliding_t.get_id() == base.get_id()) return; // 출진취소
                // ------------------------------
                
                auto force = pk::get_force(unit.get_force_id());
				if (!pk::is_alive(force)) return;

				int amount = pk::get_max_troops(unit);
				pk::add_troops(base, amount, true);
				pk::add_food(base, amount*2, true);
				pk::add_weapon_amount(base, weapon, 1);
                
                if (디버그모드)
                {
                    string unit_name = pk::decode(pk::get_name(pk::get_person(unit.leader)));
                    string base_name = pk::decode(pk::get_name(base));
                    pk::info(pk::format("출진부대최적화: 컴퓨터세력_병기혜택: {}대 소속거점 {}(으)로 병력({}),병량({}),{}(1) 반환", unit_name, base_name, amount, amount*2, 병기_이름[weapon]));
                }
			}
		}
	}

	void CallBack_UnitCreated(pk::unit@ unit, int type)
	{
		++totalCountCreation;

		if (!pk::is_alive(unit)) return;

		if (type != 0 || !IsAllowUnitWeaponChange(unit, 플레이어_위임군단_병종변경_허용)) return;

		/* NOTE
		부대 이동력을 0으로 만들어서 부대를 출진하지 못하도록 해 두었을 경우에는 병종 변경을 하지 않음
		이동력 2~4 일 경우 평지에서 1칸이므로, 1칸을 이동하지 못하는 경우 부대 생성이 자동으로 취소되는데,
		부대 출진 여부를 판단하는 기준이 "부대 생성"이기 때문.
		*/
		int weapon_mov = unit.attr.stat[부대능력_이동];
		if (weapon_mov < 2) return;

		auto force = pk::get_force(unit.get_force_id());
		if (!pk::is_alive(force) || !pk::is_normal_force(force.get_id()))
			return;

        // ---------- 기마책사 추가 : 컴퓨터 유닛이 해당 턴이 아닌 경우 제외 (컴퓨터세력 조종 등)
        if (!컴퓨터세력_유저조종시_병종변경_허용 and !unit.is_player() and unit.get_force_id() != pk::get_current_turn_force_id()) 
            return;   
        // ------------------------------
		auto base = pk::get_building(pk::get_service(unit));
		if (!pk::is_alive(base) || !pk::is_valid_building_id(base.get_id()))
			return;

		int weapon = pk::get_ground_weapon_id(unit);
		int currTekisei = getTekiseiEx(unit, weapon);

		if (isSiegeWeapon(weapon))
			++siegeWeaponCreationCount;

		int skill;
		string strSkillNames = "";
		array<string> skillNames(0);
		for (int i=0; i<3; i++)
		{
			auto person = pk::get_person(unit.member[i]);
			if (!pk::is_alive(person)) continue;
			skill = person.skill;
			if (skill >= 0 && skill < 특기_끝) skillNames.insertLast(pk::format("\x1b[17x{}\x1b[0x", pk::decode(pk::get_skill(skill).name)));
		}

		if (skillNames.length() == 0)
			strSkillNames = "\x1b[17x없음\x1b[0x";
		else
			strSkillNames = join(skillNames, ",");

		bool bTargetToChange = isTargetToChange(weapon);
		bool bWeaponChanged = false;
		if (bTargetToChange)
			bWeaponChanged = changeWeapon(unit, 병기부대_출진허용);

		if (pk::get_scenario().difficulty == 난이도_특급 && !unit.is_player())
		{
			if ((int(unit.troops) < int(pk::get_max_troops(unit) * 컴퓨터세력_병력보충_기준)) && 컴퓨터세력_부족병력_보충)
                unit.troops = int(pk::get_max_troops(unit) * 컴퓨터세력_병력보충_기준);
		}

		string strFrom = 출진거점_표시 ? pk::format("\x1b[2x{}\x1b[0x에서 ", pk::decode(pk::get_name(base))) : "";
		int newWeapon = pk::get_ground_weapon_id(unit);
		string strUnitInfo = strFrom + pk::format("\x1b[2x{}군 {}\x1b[0x({})", pk::decode(pk::get_name(force)), pk::decode(pk::get_name(unit)), strSkillNames);
		if (bWeaponChanged)
		{
			++changeCount;

			int newTekisei = getTekiseiEx(unit, newWeapon);

			// 병기 부대 -> 일반 부대인 경우
			if (컴퓨터세력_병기전환_금보상 && isSiegeWeapon(weapon) && !isSiegeWeapon(newWeapon))
			{
				++changeCountFromSiegeToNormalWeapon;
				pk::add_gold(unit, 400);
			}
			else
			{
				if (isSiegeWeapon(newWeapon)) ++changeCountFromNormalToSiegeWeapon;
			}

			if (!병종변경_메시지표시_확인)
			{
				병종변경_메시지표시_확인 = true;

				string question = pk::encode("출진하는 부대의 병종(병과) 변경 내용을 표시하시겠습니까?");
				array<string> answer = { pk::encode("알림창으로 표시"), pk::encode("말풍선으로 표시"), pk::encode(" 표시하지 않음  ") };
				병종변경_메시지표시_값 = pk::choose(answer, question, pk::get_person(무장_무관));

				switch (병종변경_메시지표시_값)
				{
				case 0: 병종변경_메시지표시 = true; break;
				case 1: 병종변경_메시지표시 = true; break;
				case 2: 병종변경_메시지표시 = false; break;
				}
			}

			string clr = isSiegeWeapon(weapon) ? "\x1b[24x" : "\x1b[0x";
			string clr2 = isSiegeWeapon(newWeapon) || (newTekisei < currTekisei) ? "\x1b[16x" : (weapon == 병기_검 || newTekisei > currTekisei) ? "\x1b[1x" : "\x1b[0x";
			string strWeaponInfo = pk::format("{}\x1b[0x → {} \x1b[0x출진!", clr + 병과정보(weapon, currTekisei), clr2 + 병과정보(newWeapon, newTekisei));
			

            // ----- 기마책사 추가 : 거점에 무장이 있으면 메세지 안뜨도록
            pk::person@ leader = pk::get_person(unit.leader);
            pk::building@ service = pk::get_building(leader.service);
            bool is_in_base = (pk::is_alive(service) and leader.location == pk::get_hex_object_id(service));
            
			if (병종변경_메시지표시_값 == 0) 
				pk::message_box(pk::encode(strUnitInfo + " " + strWeaponInfo), leader);
			else if (병종변경_메시지표시_값 == 1 and !is_in_base) 
				pk::balloon(pk::encode(strUnitInfo + " \n" + strWeaponInfo), leader);
				pk::history_log(unit.get_pos(), force.color, pk::encode(strUnitInfo + " " + strWeaponInfo));
		
		}

		// 낚시 방지용. 병량 30% 증가
		if (컴퓨터세력_부족병량_보충 && !unit.is_player() && unit.food < int(unit.troops * 1.3f))
            pk::set_food(unit, int(unit.troops * 1.3f + 0.5f));

		if (isSiegeWeapon(newWeapon))
		{
			++countSiegeWeapon;
			currTekisei = getTekiseiEx(unit, newWeapon);
			if (병기부대_출진알림) pk::message_box(pk::encode(pk::format(strUnitInfo + "가 \x1b[24x{} \x1b[0x(으)로 출진했습니다!", 병과정보(newWeapon, currTekisei))));
		}
	}

	// 병종(병과) 변경 함수
	// bForcedChangeSiegeWeapon : 병기는 강제로 다른 병종으로 변경할 것인지
	bool changeWeapon(pk::unit@ unit, bool bForcedChangeSiegeWeapon=false)
	{
        
		if (unit.type != 부대종류_전투) return false;

		int weapon = pk::get_ground_weapon_id(unit);

		auto force = pk::get_force(unit.get_force_id());
		if (!(pk::is_alive(force) && pk::is_normal_force(force.get_id()))) return false;

		forceSkillWeight.setForce(unit.get_force_id());

		bool bPlayerDelegatedUnit = isPlayerDelegatedUnit(unit);

		// 원본
        //bool isCheckStock = bPlayerDelegatedUnit && !플레이어_위임군단_무기재고부족_허용;
        // ------------ 기마책사 수정 : 컴퓨터세력_무기재고부족_허용 여부 설정 기능 추가
        bool isCheckStock = (!unit.is_player())? (!컴퓨터세력_무기재고부족_허용) : (bPlayerDelegatedUnit && !플레이어_위임군단_무기재고부족_허용) ;
        
		int currTekisei = getTekiseiEx(unit, weapon);

		int newWeapon  = getSuitableWeapon(unit, bForcedChangeSiegeWeapon, false, isCheckStock);
		int newTekisei = getTekiseiEx(unit, newWeapon);

		if (weapon == newWeapon) return false;
        
        // ------------ 기마책사 추가 : 관문/항구 방어출진용 간접병기 출진이면 병과변경 안함
        if (관문항구_방어출진부대_변경금지 and isGateDefenceUnit(unit)) return false;
        
        // ------------ 기마책사 수정 : 병기가 '검'인 경우 병장기량 계산조건 추가
		int weaponCount    = isSiegeWeapon(weapon)    ? 1 : ((weapon    == 병기_검)? 0 : unit.troops);
		int newWeaponCount = isSiegeWeapon(newWeapon) ? 1 : ((newWeapon == 병기_검)? 0 : unit.troops);
        
        // ------------ 기마책사 추가 : 거점재고 재확인
        auto base = pk::get_building(pk::get_service(unit));
        if (isCheckStock and !isEnoughStocksOfWeapon(base, newWeapon, newWeaponCount)) return false;
        
        string base_name = pk::decode(pk::get_name(base));
        string unit_name = pk::decode(pk::get_name(pk::get_person(unit.leader)));
        string stock_opt = (isCheckStock)? "재고제한" : "재고무시";
        if (디버그모드)
            pk::info(pk::format("출진부대최적화: {} 병력({}), 재고:{}({}),{}({}), 옵션:{}", base_name, pk::get_troops(base), 병기_이름[weapon], pk::get_weapon_amount(base, weapon), 병기_이름[newWeapon],  pk::get_weapon_amount(base, newWeapon), stock_opt));
        // ------------------------------------
        
        
        // ------------ 기마책사 수정 : 주석처리
		// 충차/목수 → 정란/투석인 경우
		//if (isSiegeWeapon(weapon) && isSiegeWeapon(newWeapon))
		//	weaponCount = newWeaponCount = 0;

		// 아래 코드를 사용하여 무기값을 바꾼 후에는 getWeapon() 은 바꾸기 이전 값으로만 리턴되므로, '부대속성_주무기' 값을 이용해야 함.
		// NOTE 차후 개발하는 모든 코드에서 getWeapon() 대신 queryValue(부대속성_주무기) 사용
		pk::set_weapon_amount(unit, 0, pk::get_ground_weapon_id(unit), 0);
		pk::set_weapon_amount(unit, 0, newWeapon, newWeaponCount);
        
        // ------ 기마책사 추가 : 부대 병과정보 및 능력치 업데이트 (매우 중요!!!!)
        // 누락 시 변경 전 병과 기준으로 이동범위 확인이 조회되는 버그 발생 -> 이동불가지형으로 이동가능 -> 강제종료 유발 가능
        unit.update(); 
        
		restoreWeaponInfo1.clear();
		restoreWeaponInfo2.clear();
		
		if (pk::is_alive(base) && pk::is_valid_building_id(base.get_id()) && base.get_force_id() == force.get_id())
		{
            // ------------ 기마책사 수정 : RestoreUnitWeapon 함수 추가하면서 주석처리함 (유닛 이동 후 병기 반환 실행하도록 변경)
			/* NOTE
			// 컴퓨터일 이후 출진할 부대의 병종 선택에 영향을 줄 수 있기 때문에 병종의 무기를 출진 거점에 추가하거나 거점에서 차감하지 않음. .
			// 예외. 공성 병기인 경우에만 추가 - 생산 노력이 많이 들기 때문.
			
			if (!unit.is_player() && isSiegeWeapon(weapon)) pk::add_weapon_amount(base, weapon, weaponCount);

			if (플레이어_위임군단_병종변경_허용 && bPlayerDelegatedUnit && !플레이어_위임군단_무기재고부족_허용)
			{
				if (weapon != 병기_검) pk::add_weapon_amount(base, weapon, weaponCount);
				pk::add_weapon_amount(base, newWeapon, -newWeaponCount);

				restoreWeaponInfo1.setData(unit.get_id(), weapon, -weaponCount);
				restoreWeaponInfo2.setData(unit.get_id(), newWeapon, newWeaponCount);
			}
            */
            
            // ------------ 기마책사 수정 : 출진이동 트리거용 RestoreUnitWeapon 함수 추가 연동
            unit_id = unit.get_id();
            clear_weapon_info(unit_id);
            isOptimized[unit_id] = true;
            wpn_amount[unit_id] = unit.troops;
            wpn_id_add[unit_id] = weapon;
            wpn_id_sub[unit_id] = newWeapon;
            
            if (디버그모드)
                pk::info(pk::format("--- {} {}대({}) 출진변경: {}→{}", base_name, unit_name, unit.troops, 병기_이름[weapon], 병기_이름[newWeapon]));
		
		}
        
		return true;
	}
    
    
    ////---- 기마책사 수정 : 출진병력최적화.cpp 와 같은 타이밍, 트리거 우선순위 주의 필요
    int unit_id;
    array<bool> isOptimized(부대_끝, false);
    array<int> wpn_amount(부대_끝, 0);
    array<int> wpn_id_add(부대_끝, -1);
    array<int> wpn_id_sub(부대_끝, -1);
    
    void RestoreUnitWeapon(pk::unit@ unit, const pk::point &in pos)
    {
        if (!pk::is_alive(unit))	       return;
        if (unit.type == 부대종류_수송)		   return;	// 수송 부대 제외
        if (pk::get_hex(pos).has_building) return;	// 실제로 출진하지 않은 경우(출진 취소) 제외

        pk::building@ building = pk::get_building(pk::get_service(unit));
        if (!pk::is_alive(building))	   return;
        if (building.pos != unit.pos)	   return;	// 부대의 이동 전 위치가 소속 거점이 아닌 경우 제외
        string base_name = pk::decode(pk::get_name(building));

        int uid = unit.get_id();
        if (isOptimized[uid])
        {
            // 유닛 이동 간 병력변동 있을 수 있어 이동 완료시점 병력에 따라 차감 무기량 계산, 반환무기는 이동전 병력 기준으로 계산
            int wpn_amount_add = isSiegeWeapon(wpn_id_add[uid]) ? 1 : ((wpn_id_add[uid] == 병기_검)? 0 : wpn_amount[uid]);
            int wpn_amount_sub = isSiegeWeapon(wpn_id_sub[uid]) ? 1 : ((wpn_id_sub[uid] == 병기_검)? 0 : wpn_amount[uid]);
            int wpn_before_add = pk::get_weapon_amount(building, wpn_id_add[uid]);
            int wpn_before_sub = pk::get_weapon_amount(building, wpn_id_sub[uid]);
            wpn_amount_sub = pk::min(wpn_amount_sub, pk::get_weapon_amount(building, wpn_id_sub[uid]));
            
            pk::add_weapon_amount(building, wpn_id_add[uid],  wpn_amount_add, false);    // 거점에 변경 전 병기 반환
            pk::add_weapon_amount(building, wpn_id_sub[uid], -wpn_amount_sub, false);    // 거점에 변경 후 병기 차감
            
            int wpn_after_add = pk::get_weapon_amount(building, wpn_id_add[uid]);
            int wpn_after_sub = pk::get_weapon_amount(building, wpn_id_sub[uid]);
            if (디버그모드)
            {
                pk::info(pk::format("--- {} 출진 후 병력({})", base_name, pk::get_troops(building)));
                pk::info(pk::format("--- {} 병기반환 {}({}→{})", base_name, 병기_이름[wpn_id_add[uid]], wpn_before_add, wpn_after_add));
                pk::info(pk::format("--- {} 병기차감 {}({}→{})", base_name, 병기_이름[wpn_id_sub[uid]], wpn_before_sub, wpn_after_sub));
            }
            
            // 부대 병기 정보 초기화
            clear_weapon_info(uid);
        }
    }
    
    // 부대 병기 정보 초기화
    void clear_weapon_info(int uid)
    {
        wpn_amount[uid] = 0;
        wpn_id_add[uid] = -1;
        wpn_id_sub[uid] = -1;
        isOptimized[uid] = false;
    }
    
    // 관문/항구 방어부대인지 확인
    bool isGateDefenceUnit(pk::unit@ unit)
    {
        if (!pk::is_alive(unit)) return false;
        
        int base_id = pk::get_service(unit);
        pk::building@ base = pk::get_building(base_id);
        if (!pk::is_alive(base)) return false;
        if (!pk::enemies_around(base)) return false;    // 교전아님
        
        int weapon_id = unit.weapon;
        if (건물_관문시작 <= base_id and base_id < 건물_항구끝)
        {
            if (weapon_id == 병기_노 or weapon_id == 병기_정란 or weapon_id == 병기_투석) return true;
            if (weapon_id == 병기_군마 and unit.has_tech(기교_기사)) return true;
        }
        return false;
    }
    
    
    bool 디버그모드 = false;
    ////----
    
    
    
	// 무기에 따른 모든 가중치 (특기, 기교) 구함
	float getWeaponWeight(pk::unit@ unit, int weapon, int tekisei=0)
	{
		if (!pk::is_valid_equipment_id(weapon)) return 0.0f;
		//int weapon_type = pk::equipment_id_to_heishu(weapon);
		return getWeaponWeightExceptForceSkill(unit, weapon, tekisei) + forceSkillWeight.getWeight(unit.get_force_id(), weapon);
	}

	// 세력 기교 제외한 무기에 따른 가중치 계산
	float getWeaponWeightExceptForceSkill(pk::unit@ unit, int weapon, int tekisei=0)
	{
		if (!pk::is_valid_equipment_id(weapon)) return 0.0f;
		int weapon_type = pk::equipment_id_to_heishu(weapon);
		return 무기_기본_가중치[weapon] + getWeaponTypeWeight(unit, weapon_type, tekisei) + getWeaponSkillWeight(unit, weapon, tekisei);
	}

	// 병종에 따른 가중치 구함
	// weapon_type : 병종, heishu
	float getWeaponTypeWeight(pk::unit@ unit, int weapon_type, int tekisei=0)
	{
		if (weapon_type < 0 || weapon_type >= 병종_끝) return 0.0f;

		float weight = 병종_기본_가중치[weapon_type];
		array<string> 특기_키_배열 = 병종_특기_가중치[weapon_type].getKeys();
		int len = 특기_키_배열.length();
		for (int i=0; i<len; i++)
		{
			int skill = parseInt(특기_키_배열[i]);
			if (unit.has_skill(skill))
				weight += float(병종_특기_가중치[weapon_type][특기_키_배열[i]]);
		}
		return weight;
	}

	// 무기의 특기 가중치 구함
	float getWeaponSkillWeight(pk::unit@ unit, int weapon, int tekisei=0)
	{
		if (weapon < 병기_검 || weapon >= 병기_끝) return 0.0f;

		float weight = 0.0f;
		array<string> 특기_키_배열 = 무기_특기_가중치[weapon].getKeys();
		int len = 특기_키_배열.length();
		for (int i=0; i<len; i++)
		{
			int skill = parseInt(특기_키_배열[i]);
			if (unit.has_skill(skill))
				weight += float(무기_특기_가중치[weapon][특기_키_배열[i]]);
		}
		return weight;
	}

	// 부대 소속 무장 중 가장 효율이 좋은 무기를 구함.
	// useSiegeWeapon 이 includeSiegeWeapon 에 우선
	// @useSiegeWeapon : 공성병기 사용 여부
	// @includeSiegeWeapon : 공성병기를 검색할 것인지 여부
	// @checkStock : 무기 재고 확인 여부
	int getSuitableWeapon(pk::unit@ unit, bool useSiegeWeapon=true, bool includeSiegeWeapon=false, bool checkStock=false)
	{
		// 원본
        /*if (!useSiegeWeapon) */includeSiegeWeapon = false;

		int weapon = pk::get_ground_weapon_id(unit);

		if (사정특기_충차에서_정란으로_변경허용 && (weapon == 병기_충차 || weapon == 병기_목수) && unit.has_skill(특기_사정))
			weapon = forceSkillWeight.getWeapon(병기_정란);

		int tekisei = getTekiseiEx(unit, weapon);

		float weight = tekisei + getWeaponWeight(unit, weapon) + 가중치_허용_오차;

		if (isSiegeWeapon(weapon))
		{
			// 모든 적성이 C 면 무조건 노병 출진
			if (tekisei == 적성_C && getTopTekisei(unit, true) == 적성_C)
				return 병기_노;

			if (tekisei >= 병기부대_출진가능_적성)
				weight += 병기부대_출진가능_적성차이;

			// 병기 출진을 허용하지 않은 경우 '내부적으로 검병으로 계산'
			if (!useSiegeWeapon)
			{
				//weapon = 병기_검;
				//weight = 무기_기본_가중치[weapon];
                weight = 무기_기본_가중치[병기_검];
			}
		}
		else
			weight += 일반부대_출진가능_적성차이;

		array<int> weapons = getBetterWeapons(unit, weapon, weight, includeSiegeWeapon);

		if (weapons.length() == 0)
		{
			if (isSiegeWeapon(weapon))
			{
				if (tekisei > 적성_A) tekisei = 적성_A;
				int prob = pk::min(100, 적성별_병기_기본출진확률[tekisei] + int(unit.attr.stat[부대능력_공격]/5 + unit.attr.stat[부대능력_방어]/4));

				//if (tekisei >= 병기부대_출진가능_적성 && pk::rand_bool(prob))
				//  return weapon;
                
                //----- 기마책사 수정 : 공성병기 사용가능 설정 시에만
                if (!useSiegeWeapon) weapon = 병기_검;
				if (tekisei >= 병기부대_출진가능_적성 and pk::rand_bool(prob) and useSiegeWeapon) return weapon;

				weapons = getBetterWeapons(unit, weapon, tekisei + 가중치_허용_오차, includeSiegeWeapon);
			}
			else
				return weapon;
		}

		int requiredStocks = checkStock ? unit.troops : 0;
		int candidateWeapon;
		float errorVal = 가중치_허용_오차;

		auto base = pk::get_building(pk::get_service(unit));

		for (int i=weapons.length()-1; i>=0; i--)
		{
			if (!isEnoughStocksOfWeapon(base, weapons[i], requiredStocks)
				|| canIgnoreDeviation(unit, weapon, weapons[i], errorVal))
				weapons.removeAt(i);
		}

		int iCandidateCount = weapons.length();
		if (iCandidateCount == 0)
			return weapon;

		candidateWeapon = weapons[pk::rand(iCandidateCount, unit.attr.stat[부대능력_공격], unit.attr.stat[부대능력_방어], datetime().get_second(), 0, 0, 0, 0)];

        //----- 기마책사 추가 : 공성병기 사용불가 설정 시 검병으로 최종출진 처리
        if (isSiegeWeapon(candidateWeapon) and !useSiegeWeapon) return 병기_검;
        
		return candidateWeapon;
	}

	// 가중치 무시 범위인지
	bool canIgnoreDeviation(pk::unit@ unit, int weapon, int weapon2, float errVal=0.3f)
	{
		// 현재 무기와 큰 차이가 없는 경우 AI 의 병과 선택 존중
		float currWpW = getTekiseiEx(unit, weapon) + getWeaponWeight(unit, weapon);
		float candiWpW = getTekiseiEx(unit, weapon2) + getWeaponWeight(unit, weapon2);

		float diffVal = candiWpW - currWpW;
		return diffVal >= 0 && diffVal <= errVal;
	}

	bool isSiegeWeapon(int weapon) { return (weapon >= 병기_충차 && weapon <= 병기_목수); }

	int getTekiseiEx(pk::unit@ unit, int weapon)
	{
		return !pk::is_valid_equipment_id(weapon) ? 0 : pk::get_tekisei(unit, weapon);
	}

	bool IsDelegate(int district_no) { return pk::get_district(district_no).get_id() != 1; }

	// 병종을 바꿀 대상인지
	// allowPlayerDelegatedUnit : 위임군단 병종 변경 허용 여부
	bool IsAllowUnitWeaponChange(pk::unit@ unit, bool allowPlayerDelegatedUnit=false)
	{
		// 이민족, 도적 제외
		if (!pk::is_normal_force(unit.get_force_id())) return false;

		// 수송 부대 제외
		if (unit.type != 부대종류_전투) return false;

		if (!unit.is_player()) return true;

		if (allowPlayerDelegatedUnit && isPlayerDelegatedUnit(unit)) return true;

		return false;
	}

	// 플레이어 위임 군단 부대 여부
	bool isPlayerDelegatedUnit(pk::unit@ unit)
	{
		return unit.is_player() && !pk::is_player_controlled(unit);
	}

	// 거점에 병기(무기) 재고가 충분한지 여부
	bool isEnoughStocksOfWeapon(pk::building@ building, int weapon, int count)
	{
		if (weapon == 병기_검) return true;

		if (pk::is_alive(building) && pk::is_valid_building_id(building.get_id()))
		{
			if (pk::get_weapon_amount(building, weapon) >= count) return true;
		}
		return false;
	}

	bool isDefenceUnit(pk::unit@ unit, int startWeapon=병기_창)
	{
		if (startWeapon == 병기_검) return true;
		else if (isSiegeWeapon(startWeapon)) return false;

		if (int(unit.troops) == pk::get_max_troops(unit))
			if (unit.food > int(unit.troops * 2))
				return false;
		else
		{
			if (unit.gold >= 1000) return true;
		}

		if (startWeapon == 병기_군마 && unit.gold >= 1500) return true;

		return int(unit.troops) < int(unit.food * 2);
	}

	bool isTargetToChange(int weapon)
	{
		bool bVal = true;
		switch (weapon)
		{
		case 병기_검:

			bVal = 검병부대_변경허용;

			break;

		case 병기_충차:
		case 병기_목수:
		case 병기_정란:
		case 병기_투석:

			bVal = 병기부대_변경허용;

			break;

		default:	// 창병, 극병, 노병, 기병

			bVal = 일반부대_변경허용;

			break;
		}
		return bVal;
	}

	int getTopTekisei(pk::unit@ unit, bool bIncludeSiegeWeapon=false)
	{
		int start = 병기_창;
		int end = bIncludeSiegeWeapon ? 병기_충차 : 병기_군마;
		int best = 0;

		for (int i=start; i<=end; i++)
		{
			best = pk::max(getTekiseiEx(unit, i), best);
		}
		return best;
	}

	bool hasBetterWeapon(pk::unit@ unit, int weapon, float weight, bool bIncludeSiegeWeapon=false, bool bIgnoreForceSkill=false)
	{
		int start = 병기_창;
		int end = bIncludeSiegeWeapon ? 병기_정란 : 병기_군마;

		int candidateWeapon;
		for (int i=start; i<=end; i++)
		{
			candidateWeapon = forceSkillWeight.getWeapon(i);
			if (weapon == i)
				continue;

			float compVal = getTekiseiEx(unit, i) + getWeaponWeight(unit, i);
			if (compVal > weight)
				return true;
		}
		return false;
	}

	array<int> getBetterWeapons(pk::unit@ unit, int weapon, float weight, bool bIncludeSiegeWeapon=false)
	{
		int start = 병기_창;
		int end = bIncludeSiegeWeapon ? 병기_정란 : 병기_군마;

		array<int> candidates(0);

		int candidateWeapon;
		for (int i=start; i<=end; i++)
		{
			candidateWeapon = forceSkillWeight.getWeapon(i);
			if (weapon == candidateWeapon) continue;

			float skillWeight = getWeaponWeight(unit, candidateWeapon);
			int candidateTekisei = getTekiseiEx(unit, candidateWeapon);
			float compVal = candidateTekisei + skillWeight;

			if (compVal > weight)
			{
				int count = candidates.length();
				if (count == 0)
				{
					candidates.insertLast(candidateWeapon);
					continue;
				}

				for (int j=0; j<count; j++)
				{
					float valueCandidate = getTekiseiEx(unit, candidates[j]) + getWeaponWeight(unit, candidates[j]);
					if (compVal > valueCandidate)
					{
						candidates.resize(0);
						candidates.insertAt(j, candidateWeapon);
						break;
					}
					else if (compVal == valueCandidate)
					{
						candidates.insertAt(j+1, candidateWeapon);
						break;
					}
				}
			}
		}

		return candidates;
	}

	// note 배열을 직접 접근하지 말고 함수를 사용하자!
	array<string> 병기_이름 = {"검", "창", "극", "노", "군마", "충차", "정란", "투석", "목수", "주가", "누선", "투함" };

	string 병기이름(int equipment)
	{
		if (equipment < 0 || equipment >= 병기_끝) return "모름 > " + equipment;
		return 병기_이름[equipment];
	}

	array<string> 병종_이름 = {"창병", "극병", "노병", "기병", "병기", "수군"};

	string 병종이름By무기(int equipment)
	{
		if (equipment == 병기_검) return "검병";
		return 병종이름(pk::equipment_id_to_heishu(equipment));
	}

	string 병종이름(int heishu)
	{
		if (heishu < 0 || heishu >= 병종_끝) return "모름";
		return 병종_이름[heishu];
	}

	string 병과이름(int equipment)
	{
		switch(equipment)
		{
		case 병기_군마: return "기병";
		case 병기_검:
		case 병기_창:
		case 병기_극:
		case 병기_노:
			return 병기_이름[equipment] + "병";
		}
		return 병기_이름[equipment];
	}

	string 병기정보(int weapon, int tekisei)
	{
		return 병과정보(weapon, tekisei) + " (" + 병종이름By무기(weapon) + ")";
	}

	string 병과정보(int weapon, int tekisei)
	{
		return 병과이름(weapon) + (weapon == 병기_검 ? "" : 적성이름(tekisei));
	}

	string 적성이름(int tekisei)
	{
        string str_tekisei = "";
        switch(tekisei)
        {
            case 적성_C: str_tekisei = "C"; break;
            case 적성_B: str_tekisei = "B"; break;
            case 적성_A: str_tekisei = "A"; break;
            case 적성_S: str_tekisei = "S"; break;
        }
        return str_tekisei;
        
		//int limit = int(pk::core["최대적성"]);
		//if (tekisei < 0 || tekisei >= limit) return "";
		//return string(pk::core["적성"][tekisei]["이름"]);
	}
    
    

};

class RestoreWeaponInfo
{
	private int unitNo;
	int weapon;
	int count;

	RestoreWeaponInfo()
	{
		clear();
	}

	int getUnitId()
	{
		return unitNo;
	}

	void setData(int unit_no, int weapon, int count)
	{
		unitNo = unit_no;
		this.weapon = weapon;
		this.count = count;
	}

	void restoreTo(int base_no)
	{
		restoreTo(pk::get_building(base_no));
	}

	void clear()
	{
		setData(-1, 병기_검, 0);
	}

	void restoreTo(pk::building@ base)
	{
		if (unitNo < 0 || count == 0) return;

		if (pk::is_valid_building_id(base.get_id()))
		{
			if (weapon != 병기_검) pk::add_weapon_amount(base, weapon, count);
		}

		clear();
	}
};

// 세력의 기교에 의한 병기 가중치 데이터 클래스
class ForceSkillWeight
{
	int forceNo;

	array<int> 병기_배열(병기_끝);
	array<float> 병기_가중치_배열(병기_끝, 0.0f);

	ForceSkillWeight()
	{
	}

	void initialize()
	{
		forceNo = -1;

		for (int i=병기_창; i<=병기_목수; i++) 병기_가중치_배열[i] = 0.0f;

		for (int i = 0; i < 병기_끝; i++) { 병기_배열[i] = i; }

		setForce(forceNo);
	}

	void setForce(int forceNo)
	{
		if (this.forceNo == forceNo) return;

		auto force = pk::get_force(forceNo);
		if (pk::is_alive(force) && pk::is_valid_force_id(forceNo))
		{
			for (int i=병기_창; i<=병기_목수; i++) 병기_가중치_배열[i] = 0.0f;

			병기_가중치_배열[병기_창] += (pk::has_tech(force, 기교_창병단련) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_창] += (pk::has_tech(force, 기교_병량습격) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_창] += (pk::has_tech(force, 기교_기습) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_창] += (pk::has_tech(force, 기교_정예창병) ? 0.7f : 0.0f);

			병기_가중치_배열[병기_극] += (pk::has_tech(force, 기교_극병단련) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_극] += (pk::has_tech(force, 기교_화살방패) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_극] += (pk::has_tech(force, 기교_큰방패) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_극] += (pk::has_tech(force, 기교_정예극병) ? 0.7f : 0.0f);

			병기_가중치_배열[병기_노] += (pk::has_tech(force, 기교_노병단련) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_노] += (pk::has_tech(force, 기교_응사) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_노] += (pk::has_tech(force, 기교_강노) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_노] += (pk::has_tech(force, 기교_정예노병) ? 0.7f : 0.0f);

			병기_가중치_배열[병기_군마] += (pk::has_tech(force, 기교_기병단련) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_군마] += (pk::has_tech(force, 기교_양마산출) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_군마] += (pk::has_tech(force, 기교_기사) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_군마] += (pk::has_tech(force, 기교_정예기병) ? 0.7f : 0.0f);

			float techWeight = (pk::has_tech(force, 기교_차축강화) ? 0.2f : 0.0f);

			병기_가중치_배열[병기_충차] += techWeight;
			병기_가중치_배열[병기_정란] += techWeight;
			병기_가중치_배열[병기_투석] += techWeight + (pk::has_tech(force, 기교_투석개발) ? 0.2f : 0.0f) + (pk::has_tech(force, 기교_벽력) ? 0.1f : 0.0f);
			병기_가중치_배열[병기_목수] += techWeight + (pk::has_tech(force, 기교_목수개발) ? 0.1f : 0.0f);

			병기_배열[병기_충차] = pk::has_tech(force, 기교_목수개발) ? 병기_목수 : 병기_충차;
			병기_배열[병기_정란] = pk::has_tech(force, 기교_투석개발) ? 병기_투석 : 병기_정란;
		}

		this.forceNo = forceNo;
	}

	float getWeight(int weapon)
	{
		return 병기_가중치_배열[weapon];
	}

	float getWeight(int forceNo, int weapon)
	{
		setForce(forceNo);
		return getWeight(weapon);
	}

	int getWeapon(int weapon)
	{
		return 병기_배열[weapon];
	}
    
};

ChangeBetterWeapon zgChangeBetterWeapon;

}	// end of namesapce

/*
namespace cwt
{
	// note 배열을 직접 접근하지 말고 함수를 사용하자!
	array<string> 병기_이름 = {"검", "창", "극", "노", "군마", "충차", "정란", "투석", "목수", "주가", "누선", "투함" };

	string 병기이름(int equipment)
	{
		if (equipment < 0 || equipment >= 병기_끝) return "모름 > " + equipment;
		return 병기_이름[equipment];
	}

	array<string> 병종_이름 = {"창병", "극병", "노병", "기병", "병기", "수군"};

	string 병종이름By무기(int equipment)
	{
		if (equipment == 병기_검) return "검병";
		return 병종이름(pk::equipment_id_to_heishu(equipment));
	}

	string 병종이름(int heishu)
	{
		if (heishu < 0 || heishu >= 병종_끝) return "모름";
		return 병종_이름[heishu];
	}

	string 병과이름(int equipment)
	{
		switch(equipment)
		{
		case 병기_군마: return "기병";
		case 병기_검:
		case 병기_창:
		case 병기_극:
		case 병기_노:
			return 병기_이름[equipment] + "병";
		}
		return 병기_이름[equipment];
	}

	string 병기정보(int weapon, int tekisei)
	{
		return 병과정보(weapon, tekisei) + " (" + 병종이름By무기(weapon) + ")";
	}

	string 병과정보(int weapon, int tekisei)
	{
		return 병과이름(weapon) + (weapon == 병기_검 ? "" : 적성이름(tekisei));
	}

	array<string> 적성_이름 = {"C", "B", "A", "S", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9", "SS"};

	string 적성이름(int tekisei)
	{
		int limit = 적성_이름.length();
		if (tekisei < 0 || tekisei >= limit) return "";
		return 적성_이름[tekisei];
	}
}*/