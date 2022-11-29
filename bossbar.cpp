//============================
//
// BOSSばー
// Author:hamada ryuuga
//
//============================

#include "bossbar.h"
#include "hamada.h"
#include "manager.h"
#include "player.h"
#include "game.h"


bool CBossbar::m_MaxHp;
//------------------------------------
// コンストラクタ
//------------------------------------
CBossbar::CBossbar(int list) :CObject2d(list)
{
	m_object2d[0] = nullptr;
}

//------------------------------------
// デストラクタ
//------------------------------------
CBossbar::~CBossbar()
{
}

//------------------------------------
// 初期化
//------------------------------------
HRESULT CBossbar::Init()
{
	CObject2d::Init();
	m_MaxLife = 0;
	m_object2d[0] = CObject2d::Create(1);
	m_object2d[0]->SetTexture(CTexture::TEXTURE_BOSSHP);

	m_object2d[0]->SetSize(D3DXVECTOR3((m_Life * 0.1f) + 7, 50.0f, 0.0f));
	m_object2d[0]->SetPos(D3DXVECTOR3(m_SetUpPos.x + 6, m_pos.y - 20.0f, m_pos.z));

	m_Life = 0;
	return E_NOTIMPL;
}

//------------------------------------
// 終了
//------------------------------------
void CBossbar::Uninit()
{
	if (m_object2d[0] != nullptr)
	{
		m_object2d[0]->Uninit();
		m_object2d[0] = nullptr;
	}
	
	CObject2d::Uninit();

}

//------------------------------------
// 更新
//------------------------------------
void CBossbar::Update()
{

	CObject2d::Update();

	if (m_MaxLife <= BOSSHP)
	{
		m_MaxLife += 10;
		SetDamage(-10);
	}
	else
	{
		SetCollar(PositionVec4(0.0f, 1.0f, 0.0f, 1.0f));
		m_MaxHp = true;
	}
	if (m_Life >= BOSSHP*0.5)
	{
		SetCollar(PositionVec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	if (m_Life <= BOSSHP / 2.0f)
	{
		SetCollar(PositionVec4(1.0f, 1.0f, 0.0f, 1.0f));
	}
	if (m_Life <= BOSSHP/10.0f)
	{
		SetCollar(PositionVec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	m_move.x += (0.0f - m_move.x)* 0.5f;//（目的の値-現在の値）＊減衰係数
	
	m_Size.x += m_move.x;
	m_pos.x -= m_move.x;
}

//------------------------------------
// 描画
//------------------------------------
void CBossbar::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	//アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2d::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//------------------------------------
// create
//------------------------------------
CBossbar *CBossbar::Create(const D3DXVECTOR3 & pos, float Life)
{
	CBossbar * pObject = nullptr;
	pObject = new CBossbar(1);

	if (pObject != nullptr)
	{

		pObject->m_SetUpPos = pos;
		pObject->SetPos(D3DXVECTOR3(1272.0f, pos.y, pos.z));
		pObject->m_Life = Life;
		pObject->Init();
		pObject->SetSize(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
		pObject->SetCollar(PositionVec4(0.0f, 1.0f, 0.0f, 1.0f));
		pObject->SetTexture(CTexture::TEXTURE_NONE);
		pObject->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	}

	return pObject;
}

//------------------------------------
// Get＆Set 
//------------------------------------
const D3DXVECTOR3 * CBossbar::GetPos() const
{
	return &m_pos;
}

void CBossbar::SetPos(const D3DXVECTOR3 & pos)
{
	m_pos = pos;
}

void CBossbar::SetMove(const D3DXVECTOR3 & move)
{
	m_move = move;
}

void CBossbar::SetDamage(const int Damage)
{
	m_move.x -= Damage*0.1f;
	m_Life -= Damage;
}
