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

//=============================================================================
// ポーズクラス
//=============================================================================
class CPause :public CObject
{
public:
	//選択できる種類
	enum ESelectType
	{
		MODE_NONE = 0,	// 選択していない
		MODE_BACK,		// ゲーム画面に戻る
		MODE_RETURN,	// ゲームをやり直す
		MODE_END,		// タイトルに戻る
		MODE_MAX
	};

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
	ESelectType m_nextMode;
	CObject2d* m_bg;
	CObject2d* m_selectText[4];
	CMouseObject* m_mouseCursor;
};

#endif 
