//============================
//
// プレイヤー設定ヘッター
// Author:Yuda Kaito
//
//============================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "main.h"
#include "motion.h"
#include "renderer.h"
#include "objectX.h"

class CPlayer : public CObjectX
{
private: // 定数
	static const float SPEED;			// スピード
	static const float ATTENUATION;		// 減衰係数
	static const float JUMPING_POWER;	// 跳躍力
	static const float GRAVITY;			// 重力

public: // パブリック関数
	CPlayer();
	~CPlayer();

	HRESULT Init()override;			// 初期化
	void Uninit()override;			// 破棄
	void NormalUpdate()override;	// 更新
	void Draw()override;			// 描画

	static CPlayer *Create();

private: // プライベート関数
	void Move();	// 移動
	void Jump();	// 跳躍

private: // メンバー変数

	D3DXQUATERNION m_quaternion;	// クオータニオン
	int m_jumpCount;				// ジャンプ回数
};
#endif
