//=============================================================================
//
// 制作 ( タイトル )
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "main.h"
#include "object2d.h"
#include "mode.h"

//=============================================================================
// タイトルクラス
//=============================================================================
class CTitle :public CMode
{
	// 遷移先
	enum ESelectMode
	{
		MODE_GAME = 0,		// ゲーム画面	
		MODE_TUTORIAL,		// チュートリアル画面
		MODE_RANKING,		// ランキング画面
		MODE_END,			// 終了
		MODE_MAX
	};

public:
	CTitle();
	~CTitle() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:
};
#endif