//=============================================================================
//
// Object
// Author : 浜田琉雅
//
//=============================================================================

#include "object.h"
#include "object2d.h"
#include "application.h"
#include "input.h"


const D3DXVECTOR3 CObject2d::m_Vtx[4] =
{
	D3DXVECTOR3(-1.0f, -1.0f, 0.0f),
	D3DXVECTOR3(+1.0f, -1.0f, 0.0f),
	D3DXVECTOR3(-1.0f, +1.0f, 0.0f),
	D3DXVECTOR3(+1.0f, +1.0f, 0.0f),
};

//=============================================================================
// コンストラクト関数
//=============================================================================
CObject2d::CObject2d(CTaskGroup::EPriority list) :
	CObject(list,CTaskGroup::EPushMethod::PUSH_CURRENT)
{
}

//=============================================================================
// デストラクト関数
//=============================================================================
CObject2d::~CObject2d()
{
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CObject2d::Init()
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nScale = 10.0f;

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();	//デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------ 
	// 頂点情報の設定
	//------------------------
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	 m_CounterAnim = 0;
	 m_PatternAnimX = 1;
	 m_PatternAnimY = 1;

	 m_DivisionX = 1;
	 m_DivisionY = 1;
	 m_DivisionMAX = m_DivisionX*m_DivisionY;

	 m_AnimationSpeed = 0;
	 m_AnimationSpeedCount = 0;

	 m_Timar = 0;
	 m_TimaCount = 0;
	 m_OnAnimation = false;
	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CObject2d::Uninit()
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
void CObject2d::NormalUpdate()
{
	VERTEX_2D *pVtx; //頂点へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 addPos[4];
	D3DXMATRIX mtx;	// 計算用マトリックス

	//マトリックス作成
	D3DXMatrixIdentity(&mtx);
	D3DXMatrixRotationYawPitchRoll(&mtx, 0.0f, 0.0f, m_rot.z);

	//頂点座標
	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&addPos[i], &m_Vtx[i], &mtx);

		pVtx[i].pos.x = m_pos.x + (addPos[i].x * m_size.x);	// サイズ変更
		pVtx[i].pos.y = m_pos.y + (addPos[i].y * m_size.y);	// サイズ変更
		pVtx[i].pos.z = 0.0f;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	Animation();	// アニメーションの更新
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CObject2d::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの取得
	CTexture* pTexture = CApplication::GetInstance()->GetTexture();

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(GetTexture()));
	
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//プリミティブ(ポリゴン)数
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// create関数
//=============================================================================
CObject2d *CObject2d::Create(CTaskGroup::EPriority list)
{
	CObject2d * pObject = nullptr;
	pObject = new CObject2d(list);

	if (pObject != nullptr)
	{
		pObject->Init();
	}

	return pObject;
}

//-----------------------------------------------------------------------------
// 位置がobject内に入っているか否か
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
bool CObject2d::PointAndAABB(const D3DXVECTOR3 & inPos)
{
	bool hitX = m_pos.x + m_size.x > inPos.x && m_pos.x - m_size.x < inPos.x;
	bool hitY = m_pos.y + m_size.y > inPos.y && m_pos.y - m_size.y < inPos.y;

	if (hitX && hitY)
	{
		return true;
	}

	return false;
}

//--------------------------------------------------
// アニメーションの動き
//--------------------------------------------------
void CObject2d::Animation()
{
	if (!m_OnAnimation)
	{
		return;
	}

	m_TimaCount++;

	if (m_TimaCount >= m_Timar)
	{
		m_AnimationSpeedCount++;
		if (m_AnimationSpeedCount >= m_AnimationSpeed)
		{
			m_AnimationSpeedCount = 0;
			m_PatternAnimX++;

			if (m_PatternAnimX > m_DivisionX)
			{//アニメーション
				m_PatternAnimX = 0;
				m_PatternAnimY++;
				if (m_PatternAnimY >= m_DivisionY)
				{
					m_PatternAnimY = 0;
					if (!m_Loop)
					{
						Uninit();
					}
					return;
				}
			}

			float U = 1.0f / (m_DivisionX);
			float V = 1.0f / (m_DivisionY);
			float AnimU = U * (m_PatternAnimX);
			float AnimV = V * (m_PatternAnimY);
			SetTex(PositionVec4(AnimU, AnimU + U, AnimV, AnimV + V));
		}
	}
}

//---------------------------------------
//セットテクスチャ(2d)
//---------------------------------------
void CObject2d::SetTex(PositionVec4 Tex)
{
	VERTEX_2D *pVtx; //頂点へのポインタ

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
// 色の設定
//---------------------------------------
void CObject2d::SetColor(const D3DXCOLOR& inColor)
{
	m_color = inColor;	// 色の代入

	VERTEX_2D *pVtx;	// 頂点へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = m_color;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// Animationの枚数設定関数
//=============================================================================
void CObject2d::SetAnimation(const int U, const int V,const int Speed,const int Drawtimer,const bool loop)
{
	m_DivisionX = U;
	m_DivisionY = V;
	m_DivisionMAX = m_DivisionY*m_DivisionX;

	m_PatternAnimX = 0;
	m_PatternAnimY = 0;
	m_AnimationSpeed = Speed;
	m_Timar = Drawtimer;
	m_OnAnimation = true;
	m_Loop = loop;

	//表示座標を更新
	SetTex(PositionVec4(
		1.0f / m_DivisionX * (m_PatternAnimX / (m_DivisionX))
		, 1.0f / m_DivisionX *(m_PatternAnimX / (m_DivisionX)) + 1.0f / m_DivisionX
		, 1.0f / m_DivisionY * (m_PatternAnimY % (m_DivisionY))
		, 1.0f / m_DivisionY * (m_PatternAnimY % (m_DivisionY)+1.0f / m_DivisionY* m_DivisionY)));
}
