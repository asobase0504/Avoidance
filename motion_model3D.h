//=============================================================================
//
// 3Dモーションキャラクタークラス(motion_model3D.h)
// Author : 唐﨑結斗
// Author : Yuda Kaito
// 概要 : 3Dプレイヤー生成を行う
//
//=============================================================================
#ifndef _MOTION_MODEL3D_H_			// このマクロ定義がされてなかったら
#define _MOTION_MODEL3D_H_			// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "objectX.h"
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMotion;

//=============================================================================
// 3Dプレイヤークラス
// Author : 唐﨑結斗
// 概要 : 3Dプレイヤー生成を行うクラス
//=============================================================================
class CMotionModel3D : public CObjectX
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CMotionModel3D *Create();			// 3Dプレイヤーの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMotionModel3D(CTaskGroup::EPriority nPriority = CTaskGroup::LEVEL_3D_1);
	~CMotionModel3D();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void NormalUpdate() override;		// 更新
	void Draw() override;		// 描画

	// Setter
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }			// 過去位置
	void SetMotion(const char *pName);									// モーション情報の設定
	void SetUpdateStop(bool isStop) { m_isUpdateStop = isStop; }
	void SetMaxMinVtx();

	// Getter
	D3DXVECTOR3 GetPosOld() { return m_posOld; }		// 過去位置
	CMotion *GetMotion() { return m_pMotion; }			// モーション情報の取得

	bool SegmentCollision(CObjectX* inObjectX);
	float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0);

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CMotion			*m_pMotion;			// モーションインスタンス
	D3DXVECTOR3		m_posOld;			// 過去位置
	bool			m_isUpdateStop;		// 更新を停止させる。
};

#endif