//=============================================================================
//
// Object
// Author : Hamada Ryuuga
// AUthor : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object_polygon3d.h"
#include "application.h"
#include "input.h"
#include "utility.h"
#include "camera.h"
#include "renderer.h"
#include "texture.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
const D3DXVECTOR3 CObjectPolygon3D::m_Vtx[4] =
{
	D3DXVECTOR3(-1.0f, +1.0f, 0.0f),
	D3DXVECTOR3(+1.0f, +1.0f, 0.0f),
	D3DXVECTOR3(-1.0f, -1.0f, 0.0f),
	D3DXVECTOR3(+1.0f, -1.0f, 0.0f),
};

//=============================================================================
// コンストラクタ
//=============================================================================
CObjectPolygon3D::CObjectPolygon3D(CTaskGroup::EPriority list) :
	CObject(list,CTaskGroup::EPushMethod::PUSH_CURRENT),
	m_pVtxBuff(nullptr)
{
	
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectPolygon3D::~CObjectPolygon3D()
{
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CObjectPolygon3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();	// デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx;	// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//rhwの設定
	pVtx[0].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CObjectPolygon3D::Uninit()
{
	// 破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	Release();
}

//=============================================================================
// ポリゴンの更新
//=============================================================================
void CObjectPolygon3D::NormalUpdate()
{
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CObjectPolygon3D::Draw()
{
	// デバイスへのポインタ
	// デバイスの取得
 	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// ワールド座標行列の設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, CApplication::GetInstance()->GetTexture()->GetTexture(GetTexture()));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// create関数
//=============================================================================
CObjectPolygon3D *CObjectPolygon3D::Create(CTaskGroup::EPriority list)
{
	CObjectPolygon3D * pObject = nullptr;
	pObject = new CObjectPolygon3D(list);

	if (pObject != nullptr)
	{
		pObject->Init();
	}

	return pObject;
}

//---------------------------------------
//セットテクスチャ(2d)
//---------------------------------------
void CObjectPolygon3D::SetTex(PositionVec4 Tex)
{
	VERTEX_3D *pVtx; //頂点へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(Tex.P0, Tex.P2);
	pVtx[1].tex = D3DXVECTOR2(Tex.P1, Tex.P2);
	pVtx[2].tex = D3DXVECTOR2(Tex.P0, Tex.P3);
	pVtx[3].tex = D3DXVECTOR2(Tex.P1, Tex.P3);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//---------------------------------------
// 位置の設定
//---------------------------------------
void CObjectPolygon3D::SetPos(const D3DXVECTOR3& inPos)
{
	CObject::SetPos(inPos);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);	// (※行列移動関数(第1引数にx,y,z方向の移動行列を作成))
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//---------------------------------------
// 向きの設定
//---------------------------------------
void CObjectPolygon3D::SetRot(const D3DXVECTOR3 & inRot)
{
	CObject::SetRot(inRot);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);	// (※行列移動関数(第1引数にx,y,z方向の移動行列を作成))
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//---------------------------------------
//セットサイズ
//---------------------------------------
void CObjectPolygon3D::SetSize(const D3DXVECTOR3 &size)
{
	CObject::SetSize(size);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------
	// 頂点情報の設定
	//------------------------
	for (int i = 0; i < 4; ++i)
	{
		pVtx[i].pos.x = m_Vtx[i].x * GetSize().x;
		pVtx[i].pos.y = m_Vtx[i].y * GetSize().y;
		pVtx[i].pos.z = m_Vtx[i].z * GetSize().z;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//---------------------------------------
//頂点Collarの設定
//---------------------------------------
void CObjectPolygon3D::SetColor(const D3DXCOLOR& Collar)
{
	CObject::SetColor(Collar);

	VERTEX_3D *pVtx; //頂点へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	//頂点カラーの設定
	pVtx[0].col = GetColor();
	pVtx[1].col = GetColor();
	pVtx[2].col = GetColor();
	pVtx[3].col = GetColor();

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
