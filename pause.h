//=============================================================================
// 
// ポーズ画面の作成
// Author : Yuda Kaito
// 
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CMouseObject;
class CObject2d;
class CSelect;

//=============================================================================
// ポーズクラス
//=============================================================================
class CPause :public CObject
{
public:
	CPause();
	~CPause();

	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;

	// Setter
	void Set() { m_onPause = !m_onPause; };

	// Getter
	bool Get() { return m_onPause; };

private:
	bool m_onPause;
	CObject2d* m_bg;

	CMouseObject* m_mouseCursor;

	CSelect* m_back;
	CSelect* m_retry;
	CSelect* m_titleBack;
};
#endif 
