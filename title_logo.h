//=============================================================================
//
// 制作 ( タイトル )
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _TITLE_LOGO_H_
#define _TITLE_LOGO_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object2d.h"

//=============================================================================
// タイトルクラス
//=============================================================================
class CTitleLogo :public CObject2d
{
public:
	CTitleLogo(CTaskGroup::EPriority list = CTaskGroup::LEVEL_2D_BG);
	~CTitleLogo() override;
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