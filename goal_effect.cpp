//=============================================================================
//
// プレイヤー設定
// Author:Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <assert.h>
#include "goal_effect.h"
#include "utility.h"

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const D3DXVECTOR3 CGoalEffect::SCALE = D3DXVECTOR3(0.5f, 1.4f, 0.5f);
const D3DXVECTOR3 CGoalEffect::MOVE_POWER = D3DXVECTOR3(0.0f, 3.5f, 0.0f);

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CGoalEffect::CGoalEffect()
{
	SetType(CObject::EType::PLAYER);
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CGoalEffect::~CGoalEffect()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CGoalEffect::Init()
{
	LoadModel("BOX");

	CObjectX::Init();
	SetScale(SCALE);
	SetMaterialDiffuse(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//SetColorAlpha(0.35f);
	//AttachOutLine();
	m_life = 20;
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CGoalEffect::Uninit()
{
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CGoalEffect::NormalUpdate()
{
	m_life--;

	D3DXVECTOR3 scale = GetScale();
	scale.x -= 0.02f;
	scale.y -= 0.01f;
	scale.z -= 0.02f;
	SetScale(scale);

	// 最初に見つけた指定したタイプのobjectを持ってくる
	CObject* object = SearchType(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1);

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();	// 同じタイプのobjectを持ってくる

		if (OBBAndOBB((CObjectX*)object))
		{
			SetUpdateStatus(EUpdateStatus::END);
		}
		object = next;
	}


	if (m_life <= 0)
	{
		SetUpdateStatus(EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CGoalEffect::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// 移動量の設定
//-----------------------------------------------------------------------------
void CGoalEffect::SetMove(const D3DXVECTOR3 & inMove)
{
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// 行列回転関数

	D3DXVECTOR3 move = inMove;
	D3DXVec3TransformCoord(&move, &move, &mtxRot);

	CObjectX::SetMove(move);
}

//-----------------------------------------------------------------------------
// 作成
//-----------------------------------------------------------------------------
CGoalEffect* CGoalEffect::Create(const D3DXVECTOR3 & inPos)
{
	CGoalEffect* effect = new CGoalEffect;

	effect->Init();

	D3DXVECTOR3 pos(FloatRandom(-5.0f,5.0f), FloatRandom(-5.0f, 5.0f), FloatRandom(-5.0f, 5.0f));

	effect->SetPos(inPos + pos);
	effect->SetRot(D3DXVECTOR3(FloatRandom(-D3DX_PI,D3DX_PI), FloatRandom(-D3DX_PI, D3DX_PI), FloatRandom(-D3DX_PI, D3DX_PI)));
	effect->SetMove(CGoalEffect::MOVE_POWER);

	if (FloatRandom(-1.0f, 1.0f) <= 0.0f)
	{
		effect->SetColorAlpha(0.35f);
		effect->SetScale(MOVE_POWER * 2.0f);
	}

	return nullptr;
}
