//=============================================================================
//
// エネミー(一方向にすすむ)設定ヘッダー
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _ENEMY_ONEWAY_H_
#define _ENEMY_ONEWAY_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "enemy.h"

//-----------------------------------------------------------------------------
// エネミークラス
//-----------------------------------------------------------------------------
class CEnemyOneWay : public CEnemy
{
private: // 定数
	static const D3DXVECTOR3 SCALE;
	static const D3DXVECTOR3 MOVE_POWER;

public: // パブリック関数
	CEnemyOneWay();
	~CEnemyOneWay();

	HRESULT Init() override;		// 初期化
	void Uninit() override;			// 破棄
	void NormalUpdate() override;	// 更新

private: // プライベート関数

private: // メンバー変数
};
#endif
