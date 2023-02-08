//=============================================================================
//
// カウントダウンUI
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _COUNTDOWN_H_		//このマクロが定義されてなかったら
#define _COUNTDOWN_H_		//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CObject2d;
class CNumber;

//=============================================================================
// カウントダウンUI
//=============================================================================
class CCountdown : public CObject
{
private:
	static const int START_TIME;
public:
	CCountdown();
	~CCountdown();

	HRESULT Init() override;
	void Uninit() override;
	void PopUpdate() override;
	void NormalUpdate() override;

	bool IsEnd() { return m_end; }

	static CCountdown* Create(const D3DXVECTOR3& inPos);
private:
	CObject2d* m_side[4];
	CObject2d* m_sideTrace[4];
	CObject2d* m_stopLogo;
	int m_count;
	bool m_end;

	std::function<void()> m_pop;
	std::function<void()> m_countdown;
};
#endif