//=============================================================================
//
// マウスカーソルの上に貼る
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _MOUSE_EFFECT_H_	// このマクロ定義がされてなかったら
#define _MOUSE_EFFECT_H_	// 二重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object2d.h"

//=============================================================================
// マウスカーソルの上に貼るobjectのクラス
//=============================================================================
class CMouseEffect : public CObject2d
{
public:
	static CMouseEffect *CMouseEffect::Create();

	CMouseEffect(CTaskGroup::EPriority list = CTaskGroup::EPriority::LEVEL_2D_UI);
	~CMouseEffect() override;

	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
private:
	int m_life;
};

#endif
