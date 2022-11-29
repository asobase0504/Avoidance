//============================
//
// プレイヤー設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "motion.h"
#include "renderer.h"
#include "motion_model3D.h"

class CMotion;

class CPlayer : public CMotionModel3D
{
public:
	//modelデータの構造体//
	struct MODELDATAPLAYER
	{
		int key;		// 時間管理
		int nowKey;		// 今のキー
		int loop;		// ループするかどうか[0:ループしない / 1 : ループする]
		int num_key;  	// キー数
		/*MyKeySet KeySet[MAX_KEY];*/
	};

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

	HRESULT Init()override;	// 初期化
	void Uninit()override;	// 破棄
	void NormalUpdate()override;	// 更新
	void Draw()override;	// 描画

	static CPlayer *Create();

private:
	
	void Move();		// 移動
	void TitleMove();
	void ResetMove();
	void TutorialMove();	//動きセット
	int m_Pow;
	float m_MoveSpeed;
private:
};
#endif
