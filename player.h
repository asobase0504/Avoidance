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
public:
	static const float ATTENUATION;		// 減衰係数
	static const float SPEED;			// スピード
	
public:
	CPlayer();
	~CPlayer();

	HRESULT Init()override;			// 初期化
	void Uninit()override;			// 破棄
	void NormalUpdate()override;	// 更新
	void Draw()override;			// 描画

	static CPlayer *Create();

private:
	
	void Move();	// 移動
	void Jump();	// 跳躍

	D3DXQUATERNION m_quaternion;
	float m_MoveSpeed;
private:
};
#endif
