//=============================================================================
//
// エネミー設定ヘッダー
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "motion.h"
#include "objectX.h"

//-----------------------------------------------------------------------------
// エネミークラス
//-----------------------------------------------------------------------------
class CEnemy : public CObjectX
{
public: // パブリック関数
	CEnemy();
	~CEnemy();

	HRESULT Init() override;		// 初期化
	void Uninit() override;			// 破棄
	void PopUpdate() override;		// 更新
	void NormalUpdate() override;	// 更新
	void EndUpdate() override;		// 更新

	static CEnemy* Create();	// 生成
	bool OnHitPlain();	// Plainとの当たり判定

private: // プライベート関数

private: // メンバー変数
};
#endif
