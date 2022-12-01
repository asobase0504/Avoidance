//============================
//
// 背景設定
// Author:hamada ryuuga
//
//============================
#include "3dpolygontemplate.h"
#include "utility.h"
#include "application.h"

//------------------------------------
// コンストラクタ
//------------------------------------
CTest3d::CTest3d(CTaskGroup::EPriority list) :CObjectPolygon3D(list)
{
}

//------------------------------------
// デストラクタ
//------------------------------------
CTest3d::~CTest3d()
{
}

//------------------------------------
// 初期化
//------------------------------------
HRESULT CTest3d::Init()
{

	CObjectPolygon3D::Init();
	return E_NOTIMPL;
}

//------------------------------------
// 終了
//------------------------------------
void CTest3d::Uninit()
{
	CObjectPolygon3D::Uninit();
}

//------------------------------------
// 更新
//------------------------------------
void CTest3d::NormalUpdate()
{
	//動き
	CTest3d::move();

	CObjectPolygon3D::NormalUpdate();
}

//------------------------------------
// 描画
//------------------------------------
void CTest3d::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();
	//アルファブレンディングを加算合成に設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Ｚ軸で回転しますちなみにm_rotつかうとグルグル回ります
	m_mtxWorld = *GiftMtx(&m_mtxWorld, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//m_mtxWorld = *hmd::GiftMtx(&m_mtxWorld, m_pos, m_rot);
	CObjectPolygon3D::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//------------------------------------
// create
//------------------------------------
CTest3d *CTest3d::Create()
{
	CTest3d * pObject = nullptr;
	pObject = new CTest3d(CTaskGroup::LEVEL_3D_1);

	if (pObject != nullptr)
	{
		pObject->Init();
		pObject->SetTexture("EXPLOSION");		// テクスチャ選択
		pObject->SetSize(D3DXVECTOR3(640.0f, 360.0f, 0.0f));	// サイズ設定
		pObject->SetPos(D3DXVECTOR3(0.0f, 0.0f, 10200.0f));		// 座標設定
		pObject->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));	// 色設定
		pObject->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// moveの設定
	}

	return pObject;
}

//------------------------------------
// 動き系統
//------------------------------------
void CTest3d::move()
{
	//動き入れたいときはここに	SetMove()で変えれるよ

	m_pos += m_Move;
}

