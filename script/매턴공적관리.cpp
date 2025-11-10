/*
// 제작자:
// 설명: 매턴 장수들의 스탯을 확인하여 전투에 쓸모 없는 스탯을 가졌을 경우 자동으로 반감
// 낮은 전투능력치를 가진 장수가 높은 관직에 오르는 것을 방지하기 위함
// Update: 2025.10.29 / 초기 버전 작성
*/

namespace 매턴공적관리
{
    //---------------------------------------------------------------------------------------
    // 설정변수
    const bool 공적관리_활성화 = true;          // true: 기능 활성화, false: 기능 비활성화
    const int  공적관리_적용대상 = 2;           // 0: 모든 세력, 1: 플레이어 세력만, 2: 컴퓨터 세력만
    const int  통솔_무력_합_기준 = 100;           // 통솔과 무력의 합이 이 값 미만일 경우 공적 반감
    const int  지력_기준 = 70;                   // 지력이 이 값 미만일 경우 공적 반감
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
            
            // 통솔+무력 합이 기준 미만이고 지력이 기준 미만이면 공적 절반 감소
            int tongsol = person.stat[무장능력_통솔];
            int muryok = person.stat[무장능력_무력];
            int jiryok = person.stat[무장능력_지력];
            
            if ((tongsol + muryok) < 통솔_무력_합_기준 && jiryok < 지력_기준)
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
