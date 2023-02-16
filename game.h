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
class CObject2d;
class CSelect;

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

	void RetryStage();

private:
	void StageClear();
	void NextStage();
	void PlayerDeath();

private:
	CStage* m_stage;		// 今のステージ
	CStage* m_stageNext;	// 次のステージ
	int m_stageSection;
	std::vector<std::string> m_stagePath;

	CPlayer* m_player;
	CCountdown* m_countdown;	// カウントダウン
	int m_section;				// 1面のステージ数

	int m_fallCount;			// 落ちる時間

	bool m_isDeathStop;

	CSelect* m_nextText;
	CSelect* m_retryText;
	CSelect* m_backText;

	CObject2d* m_mouseCursor;
};
#endif