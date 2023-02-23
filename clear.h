//=============================================================================
//
// ステージクリア
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _CLEAR_H_		//このマクロが定義されてなかったら
#define _CLEAR_H_		//2重インクルード防止のマクロ定義

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
class CClear : public CObject
{
private:
	static const int START_TIME;
public:
	CClear();
	~CClear();

	HRESULT Init() override;
	void Uninit() override;
	void PopUpdate() override;
	void NormalUpdate() override;
	void EndUpdate() override;

	bool IsEnd() { return m_end; }

	static CClear* Create(const D3DXVECTOR3& inPos);

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