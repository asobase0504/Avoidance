//============================
//
// MULTIPLYÅ@H//Ç±ÇÍÇÕäJî≠èâä˙Ç…Ç¬Ç≠Ç¡ÇΩé∏îsçÏ
// Author:hamada ryuuga
//
//============================

#ifndef _MULTIPLY_H_
#define _MULTIPLY_H_

#include "object2d.h"


#
class CNumber;
class CMultiplyManager
{
public:

};

//#define MAXRATE (10)
//#define RATE (MAXRATE-1)

class CMultiply : public CObject2d
{
private:
	static const int KETA = 3;

public:
	CMultiply();
	 ~CMultiply();

	 HRESULT Init() override
	 {
		 return S_OK;
	 };
	 void Uninit()override;
	 void NormalUpdate() override;
	 void Draw() override {};

	void set(int Number, D3DXVECTOR3 Pos, bool extinction, bool b3D);
	static void FastSet(int Number, D3DXVECTOR3 Pos);

	static CMultiply* Create(int Number, D3DXVECTOR3 Pos ,bool extinction,bool b3D);
	void  SetRate(int Rate) ;
	void  SetRateFast(int Rate) { m_Rate = Rate; };
	static int*  GetRate() { return &m_Rate; };

private:


	static int m_Rate;		//î{ó¶
	static int m_RateWait;
	static bool m_Decrease;
	CNumber * ratio[KETA];
};

#endif