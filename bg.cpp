//============================
//
// 背景設定
// Author:hamada ryuuga
//
//============================

#include "bg.h"
#include "utility.h"
#include "application.h"


//------------------------------------
// コンストラクタ
//------------------------------------
CBg::CBg(CTaskGroup::EPriority list):CObjectPolygon3D(list)
{
}

//------------------------------------
// デストラクタ
//------------------------------------
CBg::~CBg()
{
}

//------------------------------------
// 初期化
//------------------------------------
HRESULT CBg::Init()
{
	m_Speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AddSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CObjectPolygon3D::Init();
	return E_NOTIMPL;
}

//------------------------------------
// 終了
//------------------------------------
void CBg::Uninit()
{
	CObjectPolygon3D::Uninit();
}

//------------------------------------
// 更新
//------------------------------------
void CBg::NormalUpdate()
{
	//加算の値を関数化
		m_Speed += (m_MoveSpeed);
	
	CObjectPolygon3D::SetTex(PositionVec4(0.0f+ m_Speed.x, 1.0f+ m_Speed.x,0.0f + m_Speed.y,1.0f + m_Speed.y));
	CObjectPolygon3D::NormalUpdate();
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 1000.0f));
}

//------------------------------------
// 描画
//------------------------------------
void CBg::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();
	//アルファブレンディングを加算合成に設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_mtxWorld = *GiftMtx(&m_mtxWorld, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	CObjectPolygon3D::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//------------------------------------
// create
//------------------------------------
CBg *CBg::Create()
{
	CBg * pObject = nullptr;
	pObject = new CBg(CTaskGroup::LEVEL_2D_BG);

	if (pObject != nullptr)
	{
		pObject->Init();
		pObject->SetSize(D3DXVECTOR3(640.0f, 360.0f,0.0f));
		pObject->SetPos(D3DXVECTOR3(0.0f, 0.0f,10200.0f));
		pObject->SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	}

	return pObject;
}


