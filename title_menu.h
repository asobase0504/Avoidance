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
#include "object2d.h"

//=============================================================================
// タイトルクラス
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
	float m_rateSizeX;
	float m_rateSizeY;
	int m_count;
};
#endif