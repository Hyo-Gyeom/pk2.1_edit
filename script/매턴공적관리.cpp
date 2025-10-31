/*
// 제작자: 사용자 요청
// 설명: 매턴 장수들의 공적과 스탯을 확인하여 자동으로 증감
// Update: 2025.10.29 / 초기 버전 작성
*/

namespace 매턴공적관리
{
    //---------------------------------------------------------------------------------------
    // 설정변수
    const bool 공적관리_활성화 = true;          // true: 기능 활성화, false: 기능 비활성화
    const int  공적관리_적용대상 = 2;           // 0: 모든 세력, 1: 플레이어 세력만, 2: 컴퓨터 세력만
    
    
    //---------------------------------------------------------------------------------------
    
    class Main
    {
        Main()
        {
            pk::bind(111, pk::trigger111_t(onTurnStart));
        }
        
        // 턴 시작 시 공적 관리
        void onTurnStart(pk::force@ force)
        {
            if (!공적관리_활성화) return;
            if (!check_force_target(force)) return;
            
            // 세력의 모든 장수 처리
            pk::list<pk::person@> person_list = pk::get_person_list(force, 0xFFFFFFFF);
            for (uint i = 0; i < person_list.size; i++)
            {
                pk::person@ person = person_list[i];
                if (pk::is_alive(person))
                {
                    manage_kouseki(person);
                }
            }
        }
        
        // 세력 필터링
        bool check_force_target(pk::force@ force)
        {
            if (공적관리_적용대상 == 0) 
                return true;  // 모든 세력
            else if (공적관리_적용대상 == 1)
                return force.is_player();  // 플레이어 세력만
            else if (공적관리_적용대상 == 2)
                return !force.is_player();  // 컴퓨터 세력만
            
            return false;
        }
        
        // 장수의 공적 관리
        void manage_kouseki(pk::person@ person)
        {
            int current_kouseki = person.kouseki;
            int kouseki_change = 0;
            
            // 통솔+무력 합이 100 미만이고 지력이 70 미만이면 공적 절반 감소
            int tongsol = person.stat[무장능력_통솔];
            int muryok = person.stat[무장능력_무력];
            int jiryok = person.stat[무장능력_지력];
            
            if ((tongsol + muryok) < 100 && jiryok < 70)
            {
                // 현재 공적의 절반 감소
                kouseki_change = -(current_kouseki / 2);
            }
            
            // 공적 적용
            if (kouseki_change != 0)
            {
                pk::add_kouseki(person, kouseki_change);
            }
        }
    }
    
    Main main;
}

