//=============================================================================
//
// ゲーム画面のヘッダー
// Author:Hamada Ryuuga
//
//=============================================================================
#ifndef _GAME_H_		//このマクロが定義されてなかったら
#define _GAME_H_		//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "mode.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CParticleManager;
class CPlayer;
class CPause; 
class CCamera;

//=============================================================================
// ゲームクラス
//=============================================================================
class CGame : public CMode
{
public:
	CGame();
	~CGame();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// Getter
	static CParticleManager* GetParticleManager() { return m_pPaticleManager; }
	static CPlayer* GetPlayer() { return m_pPlayer; };
	static CPause* GetPause() { return m_pPause; };

private:
	static CPlayer* m_pPlayer;
	static CParticleManager* m_pPaticleManager;	// パーティクルマネジャー
	static CPause *m_pPause;
	CCamera* m_pCamera[2];

	int m_gameCount;
	int m_speedUp;
};
#endif