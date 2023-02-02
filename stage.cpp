//=============================================================================
//
// ステージの設定
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "stage.h"
#include "objectX.h"
#include "goal.h"
#include "application.h"
#include "color.h"
#include "factory_enemy.h"
#include "player.h"
#include "plain.h"
#include "input.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CStage::CStage() :
	CTask(CTaskGroup::EPriority::LEVEL_SYSTEM),
	m_isStart(false),
	m_isEnd(false),
	m_isFrag(false)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CStage::~CStage()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CStage::Init()
{
	SetRole(ROLE_STAGE);
	m_startCnt = 0;

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CStage::Uninit()
{
	CTask::Uninit();
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CStage::Update()
{
	if (!m_isStart)
	{
		return;
	}
	
	if (!m_isFrag)
	{
		m_isFrag = true;
		for (int i = 0; i < 5; i++)
		{
			m_wall[i]->SetCollisionFlag(true);
		}

		for (int i = 0; i < (int)m_midairFloor.size(); i++)
		{
			m_midairFloor[i]->SetCollisionFlag(true);
		}
	}

	CTask::Update();

	for (int i = 0; i < (int)m_enemy.size(); i++)
	{
		if (m_startCnt == m_enemy[i].popFream)
		{
			PopEnemy(m_enemy[i].type, m_enemy[i].pos, m_enemy[i].rot);
		}
	}

	if (m_goal->IsGoal())
	{
		m_goal->SetUpdateStatus(CObject::EUpdateStatus::END);
		m_floor->SetUpdateStatus(CObject::EUpdateStatus::END);

		for (int i = 0; i < 5; i++)
		{
			m_wall[i]->SetUpdateStatus(CObject::EUpdateStatus::END);
		}

		for (int i = 0; i < (int)m_midairFloor.size(); i++)
		{
			m_midairFloor[i]->SetUpdateStatus(CObject::EUpdateStatus::END);
		}
		m_isEnd = true;
	}

	m_startCnt++;
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CStage::Draw()
{
	CTask::Draw();
}

//-----------------------------------------------------------------------------
// 大きさの設定
//-----------------------------------------------------------------------------
void CStage::SetScale(const float inScale)
{
	m_scale = inScale;
	m_floor->SetPos(D3DXVECTOR3(0.0f, -15.0f, 0.0f));
	m_floor->SetScale(D3DXVECTOR3(m_scale, 1.5f, m_scale));
	m_wall[0]->SetPos(D3DXVECTOR3(0.0f, 10.0f * m_scale, 10.0f * m_scale));
	m_wall[0]->SetScale(D3DXVECTOR3(m_scale, m_scale, 0.5f));
	m_wall[1]->SetPos(D3DXVECTOR3(0.0f, 10.0f * m_scale, -10.0f * m_scale));
	m_wall[1]->SetScale(D3DXVECTOR3(m_scale, m_scale, 0.5f));
	m_wall[2]->SetPos(D3DXVECTOR3(10.0f * m_scale, 10.0f * m_scale, 0.0f));
	m_wall[2]->SetScale(D3DXVECTOR3(0.5f, m_scale, m_scale));
	m_wall[3]->SetPos(D3DXVECTOR3(-10.0f * m_scale, 10.0f * m_scale, 0.0f));
	m_wall[3]->SetScale(D3DXVECTOR3(0.5f, m_scale, m_scale));
}

//-----------------------------------------------------------------------------
// 床の設置
//-----------------------------------------------------------------------------
void CStage::SetFloor(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, const D3DXVECTOR3 & scale)
{
	CPlain* objectX = CPlain::Create();
	objectX->LoadModel("BOX");
	objectX->SetPos(pos);
	objectX->SetRot(rot);
	objectX->SetScale(scale);
	m_scale = scale.x;
	objectX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1));
	objectX->SetDisplayOperation();
	//objectX->CalculationVtx();

	m_floor = objectX;
}

//-----------------------------------------------------------------------------
// 壁の設置
//-----------------------------------------------------------------------------
void CStage::SetWall(int index, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, const D3DXVECTOR3 & scale)
{
	CPlain* objectX = CPlain::Create();
	objectX->LoadModel("BOX");
	objectX->SetPos(pos);
	objectX->SetRot(rot);
	objectX->SetScale(scale);
	objectX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1));
	objectX->SetDisplayOperation();
	objectX->SetCollisionFlag(false);
	//objectX->CalculationVtx();

	m_wall[index] = objectX;
	m_wall[index]->SetDisplayOperation();
}

//-----------------------------------------------------------------------------
// 空中床の追加
//-----------------------------------------------------------------------------
void CStage::AddFloor(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale)
{
	CPlain* objectX = CPlain::Create();
	objectX->LoadModel("BOX");
	objectX->SetPos(pos);
	objectX->SetRot(rot);
	objectX->SetScale(scale);
	objectX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0));
	objectX->SetCollisionFlag(false);
	//objectX->CalculationVtx();

	m_midairFloor.emplace(m_midairFloor.size(), objectX);
}

//-----------------------------------------------------------------------------
// 敵の追加
//-----------------------------------------------------------------------------
void CStage::AddEnemy(const int type, const D3DXVECTOR3 & inPos, const D3DXVECTOR3 & inRot,const int inFream)
{
	SEnemyPop popData;
	popData.type = type;
	popData.pos = inPos;
	popData.rot = inRot;
	popData.popFream = inFream;

	m_enemy.emplace(m_enemy.size(), popData);
}

//-----------------------------------------------------------------------------
// 敵の出現
//-----------------------------------------------------------------------------
void CStage::PopEnemy(const int type, const D3DXVECTOR3 & inPos, const D3DXVECTOR3 & inRot)
{
	CEnemy* enemy = nullptr;

	enemy = CreateEnemy((EEnemyType)type);

	if (enemy == nullptr)
	{
		assert(false);
	}

	enemy->Init();
	enemy->SetPos(inPos);
	enemy->SetRot(inRot);
}

//-----------------------------------------------------------------------------
// エネミーを殺す
//-----------------------------------------------------------------------------
void CStage::EnemyDeath()
{
}

//-----------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------
CStage* CStage::Create()
{
	CStage* stage = new CStage;

	if (stage != nullptr)
	{
		stage->Init();
	}

	return stage;
}

//-----------------------------------------------------------------------------
// Goalの設定
//-----------------------------------------------------------------------------
void CStage::SetGoal(const D3DXVECTOR3 & pos)
{
	m_goal = CGoal::Create();
	m_goal->LoadModel("BOX");
	m_goal->SetPos(pos);
	m_goal->SetRot(D3DXVECTOR3(D3DX_PI * 0.25f, D3DX_PI * 0.25f, D3DX_PI * 0.25f));
	m_goal->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2));
	m_goal->CalculationVtx();
}
