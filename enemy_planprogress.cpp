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
#include "enemy_planprogress.h"
#include "line.h"
#include "delay_process.h"

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const D3DXVECTOR3 CEnemyPlanProgress::SCALE = D3DXVECTOR3(0.55f, 1.25f, 0.55f);
const D3DXVECTOR3 CEnemyPlanProgress::MOVE_POWER = D3DXVECTOR3(0.0f, -7.5f, 0.0f);
const int CEnemyPlanProgress::MOVE_START_TIME = 80;

//-----------------------------------------------------------------------------
// コンストラクタ
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CEnemyPlanProgress::CEnemyPlanProgress()
{
	SetType(CObject::EType::PLAYER);
}

//-----------------------------------------------------------------------------
// デストラクタ
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CEnemyPlanProgress::~CEnemyPlanProgress()
{
}

//-----------------------------------------------------------------------------
// 初期化
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CEnemyPlanProgress::Init()
{
	CEnemy::Init();
	SetScale(SCALE);
	m_guideLine = CLine::Create();
	SetMove(D3DXVECTOR3(0.0f,0.0f,0.0f));

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::Uninit()
{
	m_guideLine->Release();
	CEnemy::Uninit();
}

//-----------------------------------------------------------------------------
// 出現中更新
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::PopUpdate()
{
	CEnemy::PopUpdate();
	SeeTarget();
}

//-----------------------------------------------------------------------------
// 更新
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::NormalUpdate()
{
	SetMove(MOVE_POWER);

	if (OnHitPlain())
	{
		SetUpdateStatus(EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// 終了更新
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::EndUpdate()
{
	CEnemy::EndUpdate();
	m_guideLine->Release();
}

//-----------------------------------------------------------------------------
// 描画
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::Draw()
{
	CEnemy::Draw();
	m_guideLine->SetLine(m_pos, m_rot, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -800.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
}

//-----------------------------------------------------------------------------
// 移動量の設定
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::SetMove(const D3DXVECTOR3 & inMove)
{
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// 行列回転関数

	D3DXVECTOR3 move = inMove;
	D3DXVec3TransformCoord(&move, &move, &mtxRot);

	CObjectX::SetMove(move);
}

//-----------------------------------------------------------------------------
// 移動量の設定
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::SeeTarget()
{
	CObject* object = SearchType(CObject::EType::PLAYER, CTaskGroup::EPriority::LEVEL_3D_1);

	D3DXVECTOR3 dist = m_pos - object->GetPos();

	D3DXVECTOR3 rot(0.0f,0.0f,0.0f);
	rot.y = atan2f(dist.x, dist.z);
	rot.x = atan2f(sqrtf(dist.z * dist.z + dist.x * dist.x), dist.y);

	SetRot(rot);
}
