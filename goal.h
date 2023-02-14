//=============================================================================
//
// プレイヤー設定ヘッター
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _GOAL_H_
#define _GOAL_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "motion.h"
#include "objectX.h"

class CGoal : public CObjectX
{
private: // 定数

public: // パブリック関数
	CGoal();
	~CGoal();

	HRESULT Init() override;		// 初期化
	void Uninit() override;			// 破棄
	void NormalUpdate() override;	// 更新
	void EndUpdate() override;		// 更新
	void Draw() override;			// 描画

	static CGoal* Create();	// 生成

	void Goal(bool goal) { m_isGoal = goal; }
	bool IsGoal() { return m_isGoal; }

	void SetTime(int inTime);
	int GetTime() { return m_goalTime; }

private: // プライベート関数

private: // メンバー変数
	int m_endCnt;
	bool m_isGoal;
	int m_goalTime;
};
#endif
