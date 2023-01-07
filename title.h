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

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CObject2d;
class CCamera;
class CLight;
class CParticleManager;

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
	void Draw() override;

	CParticleManager* GetPaticleManager() { return m_pPaticleManager; }

private:
	CCamera* m_pCamera;
	CLight* m_pLight;
	CParticleManager* m_pPaticleManager;	// パーティクルマネジャー
	CObject2d* m_mouseCursor;

};
#endif