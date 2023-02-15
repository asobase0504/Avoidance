//=============================================================================
//
// プレイヤーの死亡エフェクト
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "player_died.h"
#include "utility.h"
#include "application.h"
#include "color.h"

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const int CPlayerDied::MAX_LIFE = 210;
D3DXVECTOR3 CPlayerDied::m_posOrigin(0.0f, 0.0f, 0.0f);

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayerDied::CPlayerDied()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlayerDied::~CPlayerDied()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CPlayerDied::Init()
{
	// 現在のモーション番号の保管
	CObjectX::Init();
	LoadModel("BOX");

	m_life = MAX_LIFE;
	m_hit = false;
	AttachOutLine();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CPlayerDied::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayerDied::NormalUpdate()
{
	m_life--;

	//float scale = 1.5f - (ease::InBounce((float)m_life / (float)MAX_LIFE));

	//SetScale(m_scaleOrigin * scale);

	if (!m_hit)
	{
		if (m_life > 40)
		{
			AddMove(D3DXVECTOR3(0.0f, -0.75f, 0.0f));
		}

		m_hit = OnHitPlain();
	}

	if (m_life == 40)
	{
		SetMove((m_posOrigin - m_pos) / 40.0f);
	}

	if (m_life <= 0)
	{
		SetUpdateStatus(CObject::EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayerDied::EndUpdate()
{
	Release();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CPlayerDied::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------
CPlayerDied* CPlayerDied::Create(const D3DXVECTOR3& inPos)
{
	CPlayerDied* objectX = new CPlayerDied;

	if (objectX != nullptr)
	{
		objectX->Init();
		objectX->SetPos(inPos);
		float scale = FloatRandom(0.45f, 0.85f);
		objectX->SetScale(D3DXVECTOR3(scale, scale, scale));
		objectX->m_scaleOrigin = objectX->GetScale();;
		objectX->SetRot(D3DXVECTOR3(FloatRandom(-D3DX_PI, D3DX_PI), FloatRandom(-D3DX_PI, D3DX_PI), FloatRandom(-D3DX_PI, D3DX_PI)));
		objectX->SetMove(D3DXVECTOR3(FloatRandom(-7.5f, 7.5f), FloatRandom(0.5f, 15.5f), FloatRandom(-7.5f, 7.5f)));
		objectX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2));
	}

	return objectX;
}

bool CPlayerDied::OnHitPlain()
{
	// 最初に見つけた指定したタイプのobjectを持ってくる
	CObject* object = SearchType(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1);

	bool hit = false;

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();	// 同じタイプのobjectを持ってくる

		if (SphereAndAABB((CObjectX*)object))
		{
			SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			return true;
		}

		object = next;
	}
	return hit;
}
