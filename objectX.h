//=============================================================================
//
// オブジェクトX処理 [objectX.h]
// Author : KADO TAKUMA
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "object.h"

//=============================================================================
// クラスの定義
//=============================================================================
class CObjectX : public CObject
{
public:
	//-------------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//-------------------------------------------------------------------------
	explicit CObjectX(CTaskGroup::EPriority nPriority = CTaskGroup::LEVEL_3D_1);
	~CObjectX() override;

	//-------------------------------------------------------------------------
	// メンバー関数
	//-------------------------------------------------------------------------
	HRESULT Init() override;		// 初期化
	void Uninit() override;			// 終了
	void NormalUpdate() override;	// 更新

	/* 描画 */
	void Draw() override;
	void Draw(const D3DXQUATERNION& quaternion);	// 回転をクオータニオン
	void Draw(D3DXMATRIX mtxParent);				// 親子関係
	void Projection();								// 平行投影処理

	/* 生成処理 */
	static CObjectX *Create(D3DXVECTOR3 pos, CTaskGroup::EPriority nPriority = CTaskGroup::LEVEL_3D_1);
	void LoadModel(const char *aFileName);		// モデルの読み込み処理

	/* ワールドマトリックス */
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }	// 設定
	const D3DXMATRIX& GetMtxWorld() { return m_mtxWorld; }				// 取得

	/* 頂点位置 */
	void CalculationVtx();				// 頂点最大小値の計算
	void SetMaxVtx(const D3DXVECTOR3& Maxvtx) { m_MaxVtx = Maxvtx; }	// 頂点最大値設定
	const D3DXVECTOR3& GetMaxVtx() { return m_MaxVtx; }				// 頂点最大値取得
	void SetMinVtx(const D3DXVECTOR3& Minvtx) { m_MaxVtx = Minvtx; }	// 頂点最大値設定
	const D3DXVECTOR3& GetMinVtx() { return m_MinVtx; }				// 頂点最小値取得

	/* 親子関係 */
	void SetParent(CObjectX* inParent) { m_pParent = inParent; }	// 親モデルの情報
	CObjectX* GetParent() const { return m_pParent; }			// 親モデルの情報

	/* マテリアル関係 */
	void SetMaterialDiffuse(unsigned int index, const D3DXCOLOR& inColor);

	/* 当たり判定 */
	void SetCollisionFlag(bool inFlag) { m_isCollision = inFlag; }	// 当たり判定の有無を設定
	bool IsCollision() { return m_isCollision; }	// 当たり判定の有無を取得
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pSize);	// 当たり判定 (左右, 奥, 手前)
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *inMaxVtx, D3DXVECTOR3 *inMinVtx);	// 当たり判定 (左右, 奥, 手前)
	bool UpCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pSize, D3DXVECTOR3 *pMove);	// 当たり判定 (上側)
	bool UpCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *inMaxVtx, D3DXVECTOR3 *inMinVtx, D3DXVECTOR3 *pMove);	// 当たり判定 (上側)

private:
	//-------------------------------------------------------------------------
	// メンバー変数
	//-------------------------------------------------------------------------
	D3DXVECTOR3 m_MinVtx;		// モデルの頂点最小値
	D3DXVECTOR3 m_MaxVtx;		// モデルの頂点最大値
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	LPD3DXMESH m_pMesh;			// メッシュ情報へのポインタ
	D3DXMATERIAL* m_material;	// マテリアル情報へのポインタ
	DWORD m_NumMat;				// マテリアル情報の数

	CObjectX *m_pParent;		// 親モデルの情報
	bool m_isCollision;			// 当たり判定が必要か
};
#endif