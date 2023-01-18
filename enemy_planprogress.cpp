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

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const D3DXVECTOR3 CEnemyPlanProgress::SCALE = D3DXVECTOR3(0.55f, 1.25f, 0.55f);
const D3DXVECTOR3 CEnemyPlanProgress::MOVE_POWER = D3DXVECTOR3(0.0f, -7.5f, 0.0f);
const int CEnemyPlanProgress::MOVE_START_TIME = 80;

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CEnemyPlanProgress::CEnemyPlanProgress()
{
	m_startCnt = 0;
	SetType(CObject::EType::PLAYER);
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CEnemyPlanProgress::~CEnemyPlanProgress()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CEnemyPlanProgress::Init()
{
	// 現在のモーション番号の保管
	CEnemy::Init();
	LoadModel("BOX");
	SetScale(SCALE);
	m_guideLine = CLine::Create();
	SetMove(D3DXVECTOR3(0.0f,0.0f,0.0f));
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::Uninit()
{
	CEnemy::Uninit();
}

//-----------------------------------------------------------------------------
// 出現中更新
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::PopUpdate()
{
	SeeTarget();

	m_startCnt++;
	if (m_startCnt % MOVE_START_TIME == 0)
	{
		SetMove(MOVE_POWER);
		SetUpdateStatus(EUpdateStatus::NORMAL);
	}
}

//-----------------------------------------------------------------------------
// 更新
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
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::EndUpdate()
{
	CEnemy::EndUpdate();
	m_guideLine->Release();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CEnemyPlanProgress::Draw()
{
	CEnemy::Draw();
	m_guideLine->SetLine(m_pos, m_rot, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -700.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
}

//-----------------------------------------------------------------------------
// 移動量の設定
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
