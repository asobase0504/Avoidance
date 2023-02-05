//=============================================================================
//
// モードクラスのヘッダー
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _MODE_H_		//このマクロが定義されてなかったら
#define _MODE_H_		//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "task.h"

//=============================================================================
// ゲームクラス
//=============================================================================
class CMode : public CTask
{
public:
	CMode() : CTask(CTaskGroup::LEVEL_SYSTEM, CTaskGroup::EPushMethod::PUSH_CURRENT) {}
	~CMode() override {}

	virtual HRESULT Init() override = 0;
	virtual void Uninit() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override {};

private:
};
#endif