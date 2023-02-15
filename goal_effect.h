//=============================================================================
//
// ゴールの演出
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _GOAL_EFFECT_H_
#define _GOAL_EFFECT_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "objectX.h"

//-----------------------------------------------------------------------------
// エネミークラス
//-----------------------------------------------------------------------------
class CGoalEffect : public CObjectX
{
private: // 定数
	static const D3DXVECTOR3 SCALE;
	static const D3DXVECTOR3 MOVE_POWER;

public: // パブリック関数
	CGoalEffect();
	~CGoalEffect();

	HRESULT Init() override;		// 初期化
	void Uninit() override;			// 破棄
	void NormalUpdate() override;	// 更新
	void Draw() override;			// 描画

	void SetMove(const D3DXVECTOR3& inMove) override;

	static CGoalEffect* Create(const D3DXVECTOR3& inPos);

private: // プライベート関数
private: // メンバー変数
	int m_life;
};
#endif
