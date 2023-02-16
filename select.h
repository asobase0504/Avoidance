//=============================================================================
//
// マウスで選択可能なUI
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _SELECT_H_	// このマクロが定義されてなかったら
#define _SELECT_H_	// 2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object2d.h"

//=============================================================================
// マウスで選択可能なUI
//=============================================================================
class CSelect : public CObject2d
{
public:
	CSelect();
	~CSelect();

	HRESULT Init() override;
	void PopUpdate() override;
	void NormalUpdate() override;

	static CSelect* Create(const D3DXVECTOR3& inPos);

	void SetFunctionClick(std::function<void(CSelect*)> inClick) { m_click = inClick; }
	void SetFunctionSelection(std::function<void(CSelect*)> inSelection) { m_selection = inSelection; }
	void SetFunctionNoSelection(std::function<void(CSelect*)> inSelection) { m_noSelection = inSelection; }

private:
	std::function<void(CSelect*)> m_click;
	std::function<void(CSelect*)> m_selection;
	std::function<void(CSelect*)> m_noSelection;
};
#endif