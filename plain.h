//=============================================================================
//
// プレイヤー設定ヘッター
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _PLAIN_H_
#define _PLAIN_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "motion.h"
#include "objectX.h"

class CPlain : public CObjectX
{
public: // パブリック関数
	CPlain();
	~CPlain();

	HRESULT Init() override;		// 初期化
	void Uninit() override;			// 破棄
	void NormalUpdate() override;	// 更新
	void EndUpdate() override;		// 更新
	void Draw() override;			// 描画

	static CPlain* Create();	// 生成

private: // プライベート関数

private: // メンバー変数
	int m_endCnt;
	bool begin;
};
#endif
