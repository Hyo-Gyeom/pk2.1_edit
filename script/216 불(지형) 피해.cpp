/*
// 제작자:
// 불 지형에서 부대가 입는 피해를 계산
// Update: 2025.11.09 / 숲 지형 2배 피해 추가, 기본 피해량 100~199로 증가
*/

namespace 불_피해
{
	class Main
	{
		Main()
		{
			pk::set_func(216, pk::func216_t(callback));
		}

	int callback(pk::unit@ unit, const pk::point &in pos)
	{
		if (unit.has_skill(특기_화신) or unit.has_skill(특기_화공))
			return 0;
		
		int n = 100 + pk::rand(100);
		if (unit.has_skill(특기_등갑))
			n = n * 2;
		
		// 숲 지형일 경우 2배 데미지
		if (pk::get_hex(pos).terrain == 지형_숲)
			n = n * 2;
		
		return n;
	}
	}

	Main main;
}