//=============================================================================
//
// エネミー(一方向にすすむ)設定ヘッダー
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _ENEMY_LASER_H_
#define _ENEMY_LASER_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "enemy.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CLine;

//-----------------------------------------------------------------------------
// エネミークラス
//-----------------------------------------------------------------------------
class CEnemyLaser : public CEnemy
{
private: // 定数
	static const D3DXVECTOR3 SCALE;
	static const D3DXVECTOR3 MOVE_POWER;
	static const int MOVE_START_TIME;

public: // パブリック関数
	CEnemyLaser();
	~CEnemyLaser();

	HRESULT Init() override;		// 初期化
	void Uninit() override;			// 破棄
	void PopUpdate() override;		// 更新
	void NormalUpdate() override;	// 更新
	void EndUpdate() override;		// 終了更新
	void Draw() override;	// 更新

	void SetMove(const D3DXVECTOR3& inMove) override;

private: // プライベート関数

private: // メンバー変数

	int m_startCnt;
	float m_moveScale;
	CLine* m_guideLine;
};
#endif