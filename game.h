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
class CStage;
class CCountdown;
class CPlayer;

//=============================================================================
// ゲームクラス
//=============================================================================
class CGame : public CMode
{
private:
	static const int FALL_TIME;
public:
	CGame();
	~CGame();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:
	void StageClear();
	void NextStage();
	void RetryStage();
	void PlayerDeath();
	void BackStaging();

private:
	CStage* m_stage;		// 今のステージ
	CStage* m_stageNext;	// 次のステージ
	CPlayer* m_player;
	CCountdown* m_countdown;// カウントダウン
	int m_section;			// 1面のステージ数

	int m_fallCount;		// 落ちる時間

	bool m_isDeathStop;
};
#endif