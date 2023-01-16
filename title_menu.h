//=============================================================================
//
// 制作 ( タイトル )
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _TITLE_MENU_H_
#define _TITLE_MENU_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CObject2d;

//=============================================================================
// タイトルメニュークラス
//=============================================================================
class CTitleMenu :public CObject
{
public:
	CTitleMenu(CTaskGroup::EPriority list = CTaskGroup::LEVEL_2D_BG);
	~CTitleMenu() override;
	HRESULT Init() override;
	void Uninit() override;
	void PopUpdate() override;
	void NormalUpdate() override;
	void EndUpdate() override;

private:
	//std::map<int, CObject2d> m_List;

};
#endif