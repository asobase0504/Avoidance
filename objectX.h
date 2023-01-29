//=============================================================================
//
// オブジェクトX処理 [objectX.h]
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "main.h"
#include "object.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CObjectPolygon3D;

struct RI
{
	char inR;
	char outR;
	char inI;
	char outI;
};


union FeaturePair
{
	RI ri;

	int key;
};

struct ClipVertex
{
	ClipVertex()
	{
		f.key = ~0;
	}

	D3DXVECTOR3 v;
	FeaturePair f;
};

//-----------------------------------------------------------------------------
// クラスの定義
//-----------------------------------------------------------------------------
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

	/* 描画 */
	void Draw() override;
	void DrawMaterial();
	void DrawOutLine();
	void Projection();					// 平行投影処理

	/* 生成処理 */
	static CObjectX *Create(D3DXVECTOR3 pos, CTaskGroup::EPriority nPriority = CTaskGroup::LEVEL_3D_1);
	void LoadModel(const char *aFileName);		// モデルの読み込み処理

	/* ワールドマトリックス */
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }	// 設定
	const D3DXMATRIX& GetMtxWorld() { return m_mtxWorld; }				// 取得

	/* 大きさ倍率 */
	void SetScale(const D3DXVECTOR3& inScale);
	const D3DXVECTOR3& GetScale() const { return m_scale; }

	/* 回転系統 */
	void SetRot(const D3DXVECTOR3& inRot) override;
	void SetMtxRot(const D3DXVECTOR3& inRot);
	void SetMtxRot(const D3DXMATRIX& inRot) { m_mtxRot = inRot; }
	void SetMtxQuaternion(const D3DXQUATERNION& inQuaternion) { D3DXMatrixRotationQuaternion(&m_mtxRot, &inQuaternion); }
	const D3DXMATRIX& GetMtxRot() { return m_mtxRot; }

	/* 頂点位置 */
	void CalculationVtx();	// 頂点最大小値の計算
	void SetMaxVtx(const D3DXVECTOR3& Maxvtx) { m_MaxVtx = Maxvtx; }	// 頂点最大値設定
	const D3DXVECTOR3& GetMaxVtx() { return m_MaxVtx; }					// 頂点最大値取得
	void SetMinVtx(const D3DXVECTOR3& Minvtx) { m_MaxVtx = Minvtx; }	// 頂点最大値設定
	const D3DXVECTOR3& GetMinVtx() { return m_MinVtx; }					// 頂点最小値取得

	/* 親子関係 */
	void SetParent(CObjectX* inParent) { m_pParent = inParent; }	// 親モデルの設定
	CObjectX* GetParent() const { return m_pParent; }				// 親モデルの取得

	/* マテリアル関係 */
	void SetMaterialDiffuse(unsigned int index, const D3DXCOLOR& inColor);
	const D3DXCOLOR& GetMaterialDiffuse(unsigned int index) { return m_materialDiffuse.at(index); }

	/* 透明度 */
	void SetColorAlpha(float inAlpha) { m_colorAlpha = inAlpha; }

	/* 当たり判定 */
	void SetCollisionFlag(bool inFlag) { m_isCollision = inFlag; }	// 当たり判定の有無を設定

	bool IsCollision() { return m_isCollision; }	// 当たり判定の有無を取得
	bool OBBAndOBB(CObjectX* inObjectX,D3DXVECTOR3* outPos);
	bool OBBAndOBB(CObjectX* inObjectX);
	bool RayAndAABB(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inNormal, D3DXVECTOR3* outPos = nullptr);
	bool SegmentAndAABB(const D3DXVECTOR3& inPos, const D3DXVECTOR3& inNormal);

private:
	float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = nullptr);
	void ComputeIncidentFace(const D3DXVECTOR3& itx_pos, const D3DXMATRIX& itx_mtxRot, const D3DXVECTOR3& e, D3DXVECTOR3 n, ClipVertex* out);
	void ComputeReferenceEdgesAndBasis(const D3DXVECTOR3& eR, const D3DXVECTOR3& rtx_pos, const D3DXMATRIX& rtx_mtxRot, D3DXVECTOR3 n, int axis, char* out, D3DXMATRIX* basis, D3DXVECTOR3* e);
	int Clip(const D3DXVECTOR3& rPos, const D3DXVECTOR3& e, char* clipEdges, const D3DXMATRIX& basis, ClipVertex* incident, ClipVertex* outVerts, float* outDepths);

private:
	//-------------------------------------------------------------------------
	// メンバー変数
	//-------------------------------------------------------------------------
	D3DXVECTOR3 m_scale;		// 大きさ倍率
	D3DXVECTOR3 m_MinVtx;		// モデルの頂点最小値
	D3DXVECTOR3 m_MaxVtx;		// モデルの頂点最大値
	D3DXVECTOR3 m_MinVtxOrigin;	// モデルの頂点最小値
	D3DXVECTOR3 m_MaxVtxOrigin;	// モデルの頂点最大値
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXMATRIX m_mtxRot;		// 回転行列

	LPD3DXMESH m_pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;	// マテリアル情報へのポインタ
	std::unordered_map<unsigned int,D3DXCOLOR> m_materialDiffuse;	// マテリアルのDiffuse
	DWORD m_NumMat;				// マテリアル情報の数

	CObjectX *m_pParent;		// 親モデルの情報
	bool m_isCollision;			// 当たり判定が必要か
	float m_colorAlpha;			// 透明度

	//=========================================
	//ハンドル一覧
	//=========================================
	IDirect3DTexture9	*pTex0 = NULL;	// テクスチャ保存用
	D3DXHANDLE			m_hmWVP;		// ワールド～射影行列
	D3DXHANDLE			m_hmWIT;		// ローカル - ワールド変換行列
	D3DXHANDLE			m_hvLightDir;	// ライトの方向
	D3DXHANDLE			m_hvCol;		// 頂点色
	D3DXHANDLE			m_hvEyePos;		// 視点の位置
	D3DXHANDLE			m_hTechnique;	// テクニック
	D3DXHANDLE			m_hTexture;		// テクスチャ
};
#endif