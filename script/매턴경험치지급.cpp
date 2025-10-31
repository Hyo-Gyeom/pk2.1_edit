/*
// 제작자: 사용자 요청
// 설명: 매턴 장수들에게 경험치를 자동으로 지급
// Update: 2025.10.29 / 초기 버전 작성
*/

namespace 매턴경험치지급
{
    //---------------------------------------------------------------------------------------
    // 설정변수
    const bool 경험치지급_활성화 = true;          // true: 기능 활성화, false: 기능 비활성화
    const int  경험치지급_적용대상 = 2;           // 0: 모든 세력, 1: 플레이어 세력만, 2: 컴퓨터 세력만
    const int 능력치_상승_필요경험치 = 70;    // 능력치 1 오르는데 필요한 경험치
    // 능력치 최소값 설정 (이 값 미만이면 경험치 지급 안 함)
    const int 능력치_최소값 = 10;
    
    // 능력치 최대값 설정 (이 값 이상이면 경험치 지급 안 함)
    const int 통솔_최대값 = 100;
    const int 무력_최대값 = 100;
    const int 지력_최대값 = 100;
    const int 정치_최대값 = 100;
    const int 매력_최대값 = 100;
    
    // 능력치 구간별 경험치 지급량
    // 구간1: 1~49, 구간2: 50~69, 구간3: 70~79, 구간4: 80~89, 구간5: 90~최대값
    const int 통솔_경험치_구간1 = 10;      // 낮은 능력치 (1~49)
    const int 통솔_경험치_구간2 = 7;       // 중간 능력치 (50~69)
    const int 통솔_경험치_구간3 = 5;       // 높은 능력치 (70~79)
    const int 통솔_경험치_구간4 = 3;       // 높은 능력치 (80~89)
    const int 통솔_경험치_구간5 = 1;       // 최고 능력치 (90~최대)
    
    const int 무력_경험치_구간1 = 10;
    const int 무력_경험치_구간2 = 7;
    const int 무력_경험치_구간3 = 5;
    const int 무력_경험치_구간4 = 3;
    const int 무력_경험치_구간5 = 1;
    
    const int 지력_경험치_구간1 = 10;
    const int 지력_경험치_구간2 = 7;
    const int 지력_경험치_구간3 = 5;
    const int 지력_경험치_구간4 = 3;
    const int 지력_경험치_구간5 = 3;
    
    const int 정치_경험치_구간1 = 10;
    const int 정치_경험치_구간2 = 7;
    const int 정치_경험치_구간3 = 5;
    const int 정치_경험치_구간4 = 3;
    const int 정치_경험치_구간5 = 3;
    
    const int 매력_경험치_구간1 = 10;
    const int 매력_경험치_구간2 = 7;
    const int 매력_경험치_구간3 = 5;
    const int 매력_경험치_구간4 = 3;
    const int 매력_경험치_구간5 = 3;
    
    // 병종 적성 경험치 (병종최적화 로직으로 선택된 병종만 지급)
    const int 적성_경험치 = 4;    // 선택된 병종에 지급할 적성 경험치
    

    
    
    //---------------------------------------------------------------------------------------
    
    class Main
    {
        Main()
        {
            pk::bind(111, pk::trigger111_t(onTurnStart));
        }
        
        // 턴 시작 시 호출 (매달)
        void onTurnStart(pk::force@ force)
        {
            if (!경험치지급_활성화) return;
            if (!pk::is_alive(force)) return;
            
            // 적용 대상 세력 확인
            if (!check_force_target(force)) return;
            
            // 해당 세력의 모든 장수에게 경험치 지급 (모든 신분 포함)
            pk::list<pk::person@> person_list = pk::get_person_list(force, 0xFFFFFFFF);
            for (uint i = 0; i < person_list.size; i++)
            {
                pk::person@ person = person_list[i];
                if (!pk::is_alive(person)) continue;
                
                // 경험치 지급
                give_exp_to_person(person);
            }
        }
        
        //---------------------------------------------------------------------------------------
        // 적용 대상 세력인지 확인
        bool check_force_target(pk::force@ force)
        {
            if (경험치지급_적용대상 == 0) 
                return true;  // 모든 세력
            else if (경험치지급_적용대상 == 1)
                return force.is_player();  // 플레이어 세력만
            else if (경험치지급_적용대상 == 2)
                return !force.is_player();  // 컴퓨터 세력만
            
            return false;
        }
        
        // 장수에게 경험치 지급
        void give_exp_to_person(pk::person@ person)
        {
            if (person is null) return;
            if (!pk::is_alive(person)) return;
            
            // 능력 경험치 지급 (구간별 차등)
            give_stat_exp(person, 무장능력_통솔, 통솔_최대값, 통솔_경험치_구간1, 통솔_경험치_구간2, 통솔_경험치_구간3, 통솔_경험치_구간4, 통솔_경험치_구간5);
            give_stat_exp(person, 무장능력_무력, 무력_최대값, 무력_경험치_구간1, 무력_경험치_구간2, 무력_경험치_구간3, 무력_경험치_구간4, 무력_경험치_구간5);
            give_stat_exp(person, 무장능력_지력, 지력_최대값, 지력_경험치_구간1, 지력_경험치_구간2, 지력_경험치_구간3, 지력_경험치_구간4, 지력_경험치_구간5);
            give_stat_exp(person, 무장능력_정치, 정치_최대값, 정치_경험치_구간1, 정치_경험치_구간2, 정치_경험치_구간3, 정치_경험치_구간4, 정치_경험치_구간5);
            give_stat_exp(person, 무장능력_매력, 매력_최대값, 매력_경험치_구간1, 매력_경험치_구간2, 매력_경험치_구간3, 매력_경험치_구간4, 매력_경험치_구간5);
            
            // 병종 적성 경험치 (병종최적화 로직으로 최적 병종 찾아서 지급)
            int best_heishu = find_best_heishu_for_exp(person);
            if (best_heishu >= 0 && 적성_경험치 > 0)
            {
                pk::add_heishu_exp(person, best_heishu, 적성_경험치);
            }
            
            // 763~799번 무장 특수 처리 (통솔, 무력만)
            int person_id = person.get_id();
            if (person_id >= 763 && person_id <= 799)
            {
                // 통솔 처리
                int current_exp_tousui = person.stat_exp[무장능력_통솔];
                if (current_exp_tousui >= 능력치_상승_필요경험치)
                {
                    if (person.stat[무장능력_통솔] < 통솔_최대값)
                    {
                        person.stat[무장능력_통솔] = person.stat[무장능력_통솔] + 1;
                    }
                    person.stat_exp[무장능력_통솔] = 0;
                }
                
                // 무력 처리
                int current_exp_buryoku = person.stat_exp[무장능력_무력];
                if (current_exp_buryoku >= 능력치_상승_필요경험치)
                {
                    if (person.stat[무장능력_무력] < 무력_최대값)
                    {
                        person.stat[무장능력_무력] = person.stat[무장능력_무력] + 1;
                    }
                    person.stat_exp[무장능력_무력] = 0;
                }
            }
        }
        
        // 능력치 구간별 경험치 지급
        void give_stat_exp(pk::person@ person, int stat_type, int max_value, int exp1, int exp2, int exp3, int exp4, int exp5)
        {
            int current_stat = person.stat[stat_type];
            
            // 최소값 미만이거나 최대값 이상이면 지급 안 함
            if (current_stat < 능력치_최소값 || current_stat >= max_value) return;
            
            // 현재 능력치의 경험치가 필요경험치 * 30 이상이면 지급 안 함
            int current_exp = person.stat_exp[stat_type];
            if (current_exp >= 능력치_상승_필요경험치 * 30) return;
            
            int exp_amount = 0;
            
            // 구간별 경험치 결정
            if (current_stat <= 49)
                exp_amount = exp1;      // 구간1: 1~49
            else if (current_stat <= 69)
                exp_amount = exp2;      // 구간2: 50~69
            else if (current_stat <= 79)
                exp_amount = exp3;      // 구간3: 70~79
            else if (current_stat <= 89)
                exp_amount = exp4;      // 구간4: 80~89
            else
                exp_amount = exp5;      // 구간5: 90~최대
            
            if (exp_amount > 0)
            {
                // pk::add_stat_exp 대신 직접 경험치 추가
                person.stat_exp[stat_type] = person.stat_exp[stat_type] + exp_amount;
            }
        }
        
        // 병종최적화와 동일한 로직으로 최적 병종 찾기
        int find_best_heishu_for_exp(pk::person@ person)
        {
            if (person is null) return -1;
            
            // 1단계: 특화 특기 확인
            if (pk::has_skill(person, 특기_창신) || pk::has_skill(person, 특기_창장))
                return 병종_창병;
            
            if (pk::has_skill(person, 특기_극신) || pk::has_skill(person, 특기_극장))
                return 병종_극병;
            
            if (pk::has_skill(person, 특기_궁신) || pk::has_skill(person, 특기_궁장))
                return 병종_노병;
            
            if (pk::has_skill(person, 특기_기신) || pk::has_skill(person, 특기_기장) || 
                pk::has_skill(person, 특기_질주) || pk::has_skill(person, 특기_백마))
                return 병종_기병;
            
            // 2단계: 투신 특기
            if (pk::has_skill(person, 특기_투신))
            {
                int tekisei_창병 = person.tekisei[병종_창병];
                int tekisei_극병 = person.tekisei[병종_극병];
                
                if (tekisei_창병 >= tekisei_극병)
                    return 병종_창병;
                else
                    return 병종_극병;
            }
            
            // 3단계: 공성 적성 확인 (다른 적성이 모두 A 이하고 공성이 B 이상이면 공성 우선)
            int tekisei_병기 = person.tekisei[병종_병기];
            if (tekisei_병기 >= 1)  // B 이상
            {
                // 다른 병종의 최대 적성 찾기
                int max_other_tekisei = 0;
                for (int i = 병종_창병; i <= 병종_기병; i++)
                {
                    int tekisei = person.tekisei[i];
                    if (tekisei > max_other_tekisei)
                        max_other_tekisei = tekisei;
                }
                
                // 다른 적성이 A(2) 이하면 공성 적성 성장
                if (max_other_tekisei <= 2)
                    return 병종_병기;
            }
            
            // 4단계: 적성으로 결정 (적성 같으면 기병 우선)
            int best_heishu = 병종_창병;
            int best_tekisei = person.tekisei[병종_창병];
            
            for (int i = 병종_창병; i <= 병종_기병; i++)
            {
                int tekisei = person.tekisei[i];
                
                if (tekisei > best_tekisei || (tekisei == best_tekisei && i == 병종_기병))
                {
                    best_tekisei = tekisei;
                    best_heishu = i;
                }
            }
            
            return best_heishu;
        }
    }
    
    Main main;
}

