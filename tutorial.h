//=============================================================================
//
// TUTORIAL画面のヘッダー
// Author : Hamada Ryuuga
//
//=============================================================================
#ifndef _TUTORIAL_H_		//このマクロが定義されてなかったら
#define _TUTORIAL_H_		//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "main.h"
#include "mode.h"
#include "object.h"
#include "object2d.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CPlayer;
class CEnemy;
class CBg;
class CMagicBox;
class CParticleManager;
class CMultiply;
class CScore;

//=============================================================================
// チュートリアル画面のクラス
//=============================================================================
class CTutorial :public CMode
{
public:
	CTutorial();
	~CTutorial();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// Getter
	static CParticleManager* GetParticleManager() { return m_pPaticleManager; }
	static CMagicBox*GetMagicBox() { return m_pMagicBox; };
	static CPlayer * GetPlayer() { return m_pPlayer; };

private:
	CObject2d* m_pObject2d[4];
	CEnemy* m_pEnemy;
	CBg* m_pBg[4];
	CScore* m_pScore;

	static CParticleManager* m_pPaticleManager;	// パーティクルマネジャー
	static CMagicBox* m_pMagicBox;
	static CPlayer * m_pPlayer;
	int m_magic;
	int m_nextTaskCount;	// 次のタスクまでのカウント

	// CLEARタスク一覧表
	bool m_isMoveClear;
	bool m_isAttackClear;
	bool m_isMagicClear;
	
};

#endif
