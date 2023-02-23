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
#include "sound.h"
#include "color.h"

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const unsigned int CPlayerDied::MAX_LIFE = 100;			// 生存時間
const unsigned int CPlayerDied::AGGREGATE_TIME = 60;	// 集合する時間
const float CPlayerDied::SPATTER_SPEED = 8.5f;			// 飛び散る速度
const float CPlayerDied::GRAVITY = -1.25f;				// 加算角度の上限
D3DXVECTOR3 CPlayerDied::m_posOrigin(0.0f, 0.0f, 0.0f);	// 重力

//-----------------------------------------------------------------------------
// コンストラクタ
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CPlayerDied::CPlayerDied()
{
	m_life = 0;	// 寿命
	m_hit = false;
	m_scaleOrigin = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 初期位置
}

//-----------------------------------------------------------------------------
// デストラクタ
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CPlayerDied::~CPlayerDied()
{
}

//-----------------------------------------------------------------------------
// 初期化
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CPlayerDied::Init()
{
	// 現在のモーション番号の保管
	CObjectX::Init();
	LoadModel("BOX");

	m_life = MAX_LIFE;
	m_hit = false;
	AttachOutLine();
	AttachShadow();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CPlayerDied::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// 更新
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CPlayerDied::NormalUpdate()
{
	m_life--;

	if (!m_hit)
	{
		if (m_life > AGGREGATE_TIME)
		{
			AddMove(D3DXVECTOR3(0.0f, GRAVITY, 0.0f));
		}

		m_hit = OnHitPlain();
	}

	if (m_life == AGGREGATE_TIME)
	{
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_BACK);
		SetMoveRot(D3DXVECTOR3(FloatRandom(-0.15f,0.15f), FloatRandom(-0.15f, 0.15f), FloatRandom(-0.15f, 0.15f)));
		SetMove((m_posOrigin - m_pos) / (float)AGGREGATE_TIME);
	}

	if (m_life <= 0)
	{
		SetUpdateStatus(CObject::EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// 更新
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CPlayerDied::EndUpdate()
{
	Release();
}

//-----------------------------------------------------------------------------
// 描画
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CPlayerDied::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// 生成
// Author:Yuda Kaito
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
		objectX->SetMove(D3DXVECTOR3(FloatRandom(-SPATTER_SPEED, SPATTER_SPEED), FloatRandom(0.5f, 15.5f), FloatRandom(-SPATTER_SPEED, SPATTER_SPEED)));
		objectX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2));
	}

	return objectX;
}

//-----------------------------------------------------------------------------
// 地面との当たり判定
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
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
