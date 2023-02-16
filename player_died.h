//=============================================================================
//
// プレイヤーの死亡エフェクト
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _PLAYER_DIED_H_
#define _PLAYER_DIED_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "objectX.h"

//-----------------------------------------------------------------------------
// プレイヤーの死亡エフェクト
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
class CPlayerDied : public CObjectX
{
public: // 定数
	static const unsigned int MAX_LIFE;			// 生存時間
	static const unsigned int AGGREGATE_TIME;	// 集合する時間
	static const float SPATTER_SPEED;			// 飛び散る速度
	static const float ROT_RANDOM_LIMIT;		// 加算角度の上限
	static const float GRAVITY;					// 重力

public: // パブリック関数
	CPlayerDied();
	~CPlayerDied();

	HRESULT Init() override;		// 初期化
	void Uninit() override;			// 破棄
	void NormalUpdate() override;	// 更新
	void EndUpdate() override;		// 更新
	void Draw() override;			// 描画

	static CPlayerDied* Create(const D3DXVECTOR3& inPos);	// 生成
	static void SetOriginPos(const D3DXVECTOR3& inPos) { m_posOrigin = inPos; }

private: // メンバー関数
	bool OnHitPlain();	// Polygonとの当たり判定

private: // メンバー変数
	int m_life;	// 寿命
	bool m_hit;
	static D3DXVECTOR3 m_posOrigin;	// 初期位置
	D3DXVECTOR3 m_scaleOrigin;	// 初期位置
};
#endif
#pragma once
