/*
// 제작자: 사용자 요청
// 설명: 부대 출진 시 지휘관의 적성에 따라 병종을 자동으로 변경
// Update: 2025.10.28 / 초기 버전 작성
*/

namespace 출진병종최적화
{
    //---------------------------------------------------------------------------------------
    // 설정변수
    const bool 병종최적화_활성화 = true;         // true: 기능 활성화, false: 기능 비활성화
    const bool 병종최적화_화면표시 = true;       // true: 메시지 표시, false: 메시지 숨김
    const int  병종최적화_적용대상 = 0;          // 0: 컴퓨터AI 와 플레이어_위임군단 모두,  1: 플레이어_위임군단만, 2: 컴퓨터AI만,  3: 모두 미적용
    
    // 적성 기준치 (이 값 미만의 적성은 고려하지 않음)
    const int 적성_최소기준 = 0;  // 0:적성C, 1:적성B, 2:적성A, 3:적성S
    
    // 병종 변경 우선순위 (높을수록 우선, 같으면 적성으로 결정)
    const int 창병_우선순위 = 0;
    const int 극병_우선순위 = 0;
    const int 노병_우선순위 = 0;
    const int 기병_우선순위 = 0;
    const int 병기_우선순위 = 0;  // 충차, 정란, 투석 등
    
    //---------------------------------------------------------------------------------------
    
    class Main
    {
        Main()
        {
            pk::bind(170, 병종최적화_트리거_우선순위, pk::trigger170_t(onUnitCreate));
            pk::bind(172, 병종최적화_트리거_우선순위, pk::trigger172_t(onUnitPosChange));
        }
        
        int 병종최적화_트리거_우선순위 = 50;  // 다른 스크립트보다 먼저 실행 (출진무장최적화는 100)
        
        pk::array<int> pending_unit_ids;  // 병종 변경 대기 중인 부대 ID 목록
        pk::array<int> pending_best_heishu;  // 각 부대의 목표 병종
        
        // 부대 생성 시 호출
        void onUnitCreate(pk::unit@ unit_t, int type)
        {
            if (pk::is_campaign()) return;
            if (!병종최적화_활성화) return;
            
            // 유효한 부대인지 확인
            if (!check_valid_unit(unit_t, type)) return;
            
            // 적용 대상 세력인지 확인
            if (!check_force_option(unit_t)) return;
            
            // 전투부대만 처리 (수송부대 제외)
            if (unit_t.type != 부대종류_전투) return;
            
            // 지휘관 가져오기
            pk::person@ leader = pk::get_person(unit_t.leader);
            if (!pk::is_alive(leader)) return;
            
            // 현재 병기 ID와 병종
            int current_weapon_id = unit_t.weapon;
            int current_heishu = pk::equipment_id_to_heishu(current_weapon_id);
            
            // 최적의 병종 찾기
            int best_heishu = find_best_heishu(leader, current_heishu, current_weapon_id);
            
            if (best_heishu != current_heishu && best_heishu >= 0)
            {
                // 부대를 대기 목록에 추가 (이동 시 변경)
                pending_unit_ids.insertLast(unit_t.get_id());
                pending_best_heishu.insertLast(best_heishu);
            }
        }
        
        // 부대 이동 시 호출
        void onUnitPosChange(pk::unit@ unit_t, const pk::point &in pos)
        {
            if (pk::is_campaign()) return;
            if (!pk::is_alive(unit_t)) return;
            
            // 대기 목록에 있는 부대인지 확인
            int unit_id = unit_t.get_id();
            int idx = -1;
            for (uint i = 0; i < pending_unit_ids.length; i++)
            {
                if (pending_unit_ids[i] == unit_id)
                {
                    idx = int(i);
                    break;
                }
            }
            
            if (idx < 0) return;  // 대기 목록에 없음
            
            pk::person@ leader = pk::get_person(unit_t.leader);
            if (!pk::is_alive(leader)) 
            {
                // 대기 목록에서 제거 (ID를 -1로 표시)
                pending_unit_ids[idx] = -1;
                return;
            }
            
            // 목표 병종 가져오기
            int best_heishu = pending_best_heishu[idx];
            int current_heishu = pk::equipment_id_to_heishu(unit_t.weapon);
            
            if (best_heishu != current_heishu && best_heishu >= 0)
            {
                change_unit_heishu(unit_t, leader, current_heishu, best_heishu);
            }
            
            // 대기 목록에서 제거 (ID를 -1로 표시)
            pending_unit_ids[idx] = -1;
        }
        
        //---------------------------------------------------------------------------------------
        // 최적의 병종 찾기
        int find_best_heishu(pk::person@ leader, int current_heishu, int current_weapon_id)
        {
            if (!pk::is_alive(leader)) return -1;
            
            // 1단계: 특화 특기가 있는지 먼저 확인 (최우선)
            int heishu_by_skill = find_heishu_by_special_skill(leader);
            if (heishu_by_skill >= 0)
            {
                return heishu_by_skill;
            }
            
            // 2단계: 투신 특기가 있으면 창병/극병 중 적성 높은 쪽 선택
            if (pk::has_skill(leader, 특기_투신))
            {
                int tekisei_창병 = leader.tekisei[병종_창병];
                int tekisei_극병 = leader.tekisei[병종_극병];
                
                // 적성이 같으면 창병, 아니면 높은 쪽
                if (tekisei_창병 >= tekisei_극병)
                    return 병종_창병;
                else
                    return 병종_극병;
            }
            
            // 3단계: 현재 공성병기(충차, 정란, 투석, 목수)인 경우 처리
            if (current_weapon_id >= 병기_충차 && current_weapon_id <= 병기_목수)
            {
                // 다른 병종의 최대 적성 찾기
                int max_other_tekisei = 0;
                for (int i = 병종_창병; i <= 병종_기병; i++)
                {
                    int tekisei = leader.tekisei[i];
                    if (tekisei > max_other_tekisei)
                        max_other_tekisei = tekisei;
                }
                
                // 다른 적성이 A(2) 이하면 공성병기 유지
                if (max_other_tekisei <= 2)
                {
                    return current_heishu;  // 현재 병종 유지
                }
                // 아니면 4단계로 진행 (높은 병종으로 변경)
            }
            
            // 4단계: 투신도 없고 특화 특기도 없으면 적성으로 결정
            // 적성이 같으면 기병 우선
            int best_heishu = 병종_창병;
            int best_tekisei = leader.tekisei[병종_창병];
            
            // 각 병종별로 적성 비교 (창병, 극병, 노병, 기병)
            for (int i = 병종_창병; i <= 병종_기병; i++)
            {
                int tekisei = leader.tekisei[i];
                
                // 적성이 높으면 변경, 같으면 기병 우선
                if (tekisei > best_tekisei || (tekisei == best_tekisei && i == 병종_기병))
                {
                    best_tekisei = tekisei;
                    best_heishu = i;
                }
            }
            
            return best_heishu;
        }
        
        // 특화 특기로 병종 결정 (신, 장 특기가 있으면 해당 병종 반환)
        int find_heishu_by_special_skill(pk::person@ leader)
        {
            if (!pk::is_alive(leader)) return -1;
            
            // 창병 특화 특기
            if (pk::has_skill(leader, 특기_창신) || pk::has_skill(leader, 특기_창장))
                return 병종_창병;
            
            // 극병 특화 특기
            if (pk::has_skill(leader, 특기_극신) || pk::has_skill(leader, 특기_극장))
                return 병종_극병;
            
            // 노병 특화 특기
            if (pk::has_skill(leader, 특기_궁신) || pk::has_skill(leader, 특기_궁장))
                return 병종_노병;
            
            // 기병 특화 특기
            if (pk::has_skill(leader, 특기_기신) || pk::has_skill(leader, 특기_기장) || 
                pk::has_skill(leader, 특기_질주) || pk::has_skill(leader, 특기_백마))
                return 병종_기병;
            
            // 특화 특기가 없음
            return -1;
        }
        
        // 병종 점수 계산
        int get_heishu_score(pk::person@ leader, int heishu, int current_heishu)
        {
            if (!pk::is_alive(leader)) return 0;
            
            // 적성값 (0~3: C~S)
            int tekisei = leader.tekisei[heishu];
            
            // 최소 기준 미달 시 0점
            if (tekisei < 적성_최소기준) return 0;
            
            // 기본 점수 = 적성 * 100
            int score = tekisei * 100;
            
            // 병종별 우선순위 보너스
            if (heishu == 병종_창병) score += 창병_우선순위;
            else if (heishu == 병종_극병) score += 극병_우선순위;
            else if (heishu == 병종_노병) score += 노병_우선순위;
            else if (heishu == 병종_기병) score += 기병_우선순위;
            else if (heishu == 병종_병기) score += 병기_우선순위;
            
            // 현재 병종이면 약간의 보너스 (불필요한 변경 방지)
            if (heishu == current_heishu) score += 10;
            
            return score;
        }
        
        // 병종 변경 실행
        void change_unit_heishu(pk::unit@ unit_t, pk::person@ leader, int old_heishu, int new_heishu)
        {
            if (!pk::is_alive(unit_t)) return;
            if (!pk::is_alive(leader)) return;
            
            // 새로운 병종의 기본 무기 ID 가져오기
            int new_weapon_id = get_default_weapon_for_heishu(new_heishu, leader);
            if (new_weapon_id < 0) return;
            
            // 병종 변경
            int old_weapon_id = pk::get_ground_weapon_id(unit_t);
            int old_weapon_count = (old_weapon_id >= 병기_충차 && old_weapon_id <= 병기_목수) ? 1 : unit_t.troops;
            int new_weapon_count = (new_weapon_id >= 병기_충차 && new_weapon_id <= 병기_목수) ? 1 : unit_t.troops;
            
            // 기존 병기 제거, 새 병기 설정
            pk::set_weapon_amount(unit_t, 0, old_weapon_id, 0);
            pk::set_weapon_amount(unit_t, 0, new_weapon_id, new_weapon_count);
            
            // 부대 병과정보 및 능력치 업데이트 (필수!)
            unit_t.update();
            
            // 병력을 최대값으로 설정
            unit_t.troops = pk::get_max_troops(unit_t);
            
            // 병력 변경 후 다시 업데이트
            unit_t.update();
            
            // 메시지 표시
            if (병종최적화_화면표시)
            {
                string leader_name = pk::decode(pk::get_name(leader));
                string old_heishu_name = get_heishu_name(old_heishu);
                string new_heishu_name = get_heishu_name(new_heishu);
                int tekisei = leader.tekisei[new_heishu];
                string tekisei_name = get_tekisei_name(tekisei);
                
                pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x의 \x1b[2x{}\x1b[0x 적성에 맞춰\n병종을 \x1b[17x{}\x1b[0x에서 \x1b[2x{}\x1b[0x(으)로 변경합니다", leader_name, tekisei_name, old_heishu_name, new_heishu_name)), leader);
            }
        }
        
        //---------------------------------------------------------------------------------------
        // 병종별 기본 무기 ID 반환
        int get_default_weapon_for_heishu(int heishu, pk::person@ leader)
        {
            if (heishu == 병종_창병) return 병기_창;      // 창
            if (heishu == 병종_극병) return 병기_극;      // 극
            if (heishu == 병종_노병) return 병기_노;      // 노
            if (heishu == 병종_기병) return 병기_군마;    // 군마
            if (heishu == 병종_병기)
            {
                // 투석 기교 연구 완료시 투석, 아니면 정란
                pk::force@ force = pk::get_force(leader.get_force_id());
                if (pk::is_alive(force) && pk::has_tech(force, 기교_투석개발))
                    return 병기_투석;
                else
                    return 병기_정란;
            }
            if (heishu == 병종_수군) return 병기_주가;    // 주가
            return -1;
        }
        
        // 병종 이름 반환
        string get_heishu_name(int heishu)
        {
            if (heishu == 병종_창병) return "창병";
            if (heishu == 병종_극병) return "극병";
            if (heishu == 병종_노병) return "노병";
            if (heishu == 병종_기병) return "기병";
            if (heishu == 병종_병기) return "병기";
            if (heishu == 병종_수군) return "수군";
            return "알수없음";
        }
        
        // 적성 이름 반환
        string get_tekisei_name(int tekisei)
        {
            if (tekisei == 0) return "C";
            if (tekisei == 1) return "B";
            if (tekisei == 2) return "A";
            if (tekisei == 3) return "S";
            return "?";
        }
        
        //---------------------------------------------------------------------------------------
        // 유효한 부대인지 확인
        bool check_valid_unit(pk::unit@ unit_t, int type)
        {
            if (!pk::is_alive(unit_t)) return false;
            if (type != 0) return false;  // 신규 부대가 아닌 경우 제외
            
            if (unit_t.attr.stat[부대능력_이동] < 2) return false;  // 이동력 2 미만 제외
            
            int force_id = unit_t.get_force_id();
            pk::force@ force = pk::get_force(force_id);
            if (!pk::is_alive(force) or !pk::is_normal_force(force)) return false;
            
            return true;
        }
        
        // 적용 대상 세력인지 확인
        bool check_force_option(pk::unit@ unit_t)
        {
            // 0: 컴퓨터AI와 플레이어_위임군단 모두
            if (병종최적화_적용대상 == 0)
            {
                // 플레이어 직접 조작 부대는 제외
                if (pk::is_player_controlled(unit_t)) return false;
                return true;
            }
            // 1: 플레이어_위임군단만
            else if (병종최적화_적용대상 == 1)
            {
                if (unit_t.is_player() && !pk::is_player_controlled(unit_t)) 
                    return true;
            }
            // 2: 컴퓨터AI만
            else if (병종최적화_적용대상 == 2)
            {
                if (!unit_t.is_player()) 
                    return true;
            }
            // 3: 모두 미적용
            else if (병종최적화_적용대상 == 3)
            {
                return false;
            }
            
            return false;
        }
    }
    
    Main main;
}

