//============================
//
// プレイヤー設定ヘッター
// Author:hamada ryuuga
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

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CMotion;

class CPlayer : public CObjectX
{
public:
	static const int MAXLIFE = 300;
	static const float ATTENUATION;		// 減衰係数
	static const float SPEED;			// スピード
	static const float WIDTH;			// モデルの半径
	static const int MAX_PRAYER;		// 最大数
	static const int MAX_MOVE;			// アニメーションの最大数
	static const int INVINCIBLE;		// 無敵時間
	static const int MAX_MODELPARTS = 9;
	
public:
	CPlayer();
	~CPlayer();

	HRESULT Init()override;			// 初期化
	void Uninit()override;			// 破棄
	void NormalUpdate()override;	// 更新
	void Draw()override;			// 描画

	static CPlayer *Create();

private:
	
	void Move();		// 移動

	D3DXQUATERNION m_quaternion;
	float m_MoveSpeed;
private:
};
#endif
