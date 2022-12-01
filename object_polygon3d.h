//=============================================================================
//
// object_polygon3d
// Author : Hamada Ryuuga
// AUthor : Yuda Kaito
//
//=============================================================================
#ifndef _3DPOLYGON_H_	// このマクロ定義がされてなかったら
#define _3DPOLYGON_H_	// 二重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object2d.h"

//-----------------------------------------------------------------------------
// マクロ宣言
//-----------------------------------------------------------------------------
#define TIMER (m_time + 90)

//=============================================================================
// 3Dポリゴンクラス
//=============================================================================
class  CObjectPolygon3D : public CObject
{
public:
	const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	//頂点情報「3D」の構造体を定義
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;//頂点座標
		D3DXVECTOR3 nor;//ベクトル
		D3DCOLOR col;//カラー
		D3DXVECTOR2 tex;//テクスチャ
	};

protected:
	//polygonの基準サイズ
	static const D3DXVECTOR3 m_Vtx[4];

public:

	static CObjectPolygon3D *Create(CTaskGroup::EPriority list = CTaskGroup::LEVEL_3D_1);

	CObjectPolygon3D(CTaskGroup::EPriority list = CTaskGroup::LEVEL_3D_1);
	~CObjectPolygon3D() override;
	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
	void Draw() override;

	// Setter
	void SetTex(PositionVec4 inTex);
	void SetPos(const D3DXVECTOR3& inPos) override;
	void SetRot(const D3DXVECTOR3& inRot) override;
	void SetSize(const D3DXVECTOR3& inSize) override;
	void SetColor(const D3DXCOLOR& inColor) override;

	LPDIRECT3DVERTEXBUFFER9 GetVtx() { return m_pVtxBuff; }	// 頂点バッファの取得

protected:
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// 頂点バッファ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
};
#endif
