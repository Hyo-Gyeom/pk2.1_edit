/*
// 제작자: 
// 설명: 컴퓨터 세력에게 매턴 기교 점수(TP) 지급
// Update: 2025.10.29 / 초기 버전 작성
*/

namespace AI기교점수지급
{
    //---------------------------------------------------------------------------------------
    // 설정변수
    const bool 기교점수지급_활성화 = true;            // true: 기능 활성화, false: 기능 비활성화
    const int  턴당_기교점수 = 100;                   // 턴마다 지급할 기교 점수(TP)
    
    
    //---------------------------------------------------------------------------------------
    
    class Main
    {
        Main()
        {
            pk::bind(111, pk::trigger111_t(onTurnStart));
        }
        
        // 턴 시작 시 기교 점수 지급
        void onTurnStart(pk::force@ force)
        {
            if (!기교점수지급_활성화) return;
            
            // 컴퓨터 세력만 처리
            if (force.is_player()) return;
            
            // 기교 점수 지급
            pk::add_tp(force, 턴당_기교점수, force.get_pos());
        }
    }
    
    Main main;
}

