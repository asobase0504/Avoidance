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

class CPlayerDied : public CObjectX
{
public: // 定数
	static const int MAX_LIFE;
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
	bool OnHitPlain();	// Polygonとの当たり判定

private: // メンバー変数
	int m_life;	// 寿命
	bool m_hit;
	static D3DXVECTOR3 m_posOrigin;	// 初期位置
};
#endif
#pragma once
