//=============================================================================
//
// プレイヤー追従の敵
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _ENEMY_HOMING_H_
#define _ENEMY_HOMING_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "enemy.h"

//-----------------------------------------------------------------------------
// エネミークラス
//-----------------------------------------------------------------------------
class CEnemyHoming : public CEnemy
{
private: // 定数
	static const D3DXVECTOR3 SCALE;
	static const D3DXVECTOR3 MOVE_POWER;
	static const int MOVE_START_TIME;
	static const int MOVE_END_TIME;

public: // パブリック関数
	CEnemyHoming();
	~CEnemyHoming();

	HRESULT Init() override;		// 初期化
	void Uninit() override;			// 破棄
	void PopUpdate() override;		// 更新
	void NormalUpdate() override;	// 更新
	void EndUpdate() override;		// 終了更新
	void Draw() override;	// 更新

	void SeeTarget();

	void SetMove(const D3DXVECTOR3& inMove) override;

private: // プライベート関数

private: // メンバー変数
};
#endif
