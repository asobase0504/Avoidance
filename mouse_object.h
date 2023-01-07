//=============================================================================
//
// マウスカーソルの上に貼る
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _MOUSE_OBJECT_H_	// このマクロ定義がされてなかったら
#define _MOUSE_OBJECT_H_	// 二重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object2d.h"

//=============================================================================
// マウスカーソルの上に貼るobjectのクラス
//=============================================================================
class CMouseObject : public CObject2d
{
public:
	static CMouseObject *CMouseObject::Create();

	CMouseObject(CTaskGroup::EPriority list = CTaskGroup::EPriority::LEVEL_2D_UI);
	~CMouseObject() override;

	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
private:
};

#endif
