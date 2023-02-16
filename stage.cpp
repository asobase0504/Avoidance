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
	m_goal->Release();	// ゴール
	m_floor->Release();	// 床

	for (int i = 0; i < 5; i++)
	{
		m_wall[i]->Release();	// 壁
	}

	for (int i = 0; i < (int)m_midairFloor.size(); i++)
	{
		m_midairFloor.at(i)->Release();	// 空中床
	}

	CTask::Uninit();
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CStage::Update()
{
	// スタートするまで更新が通らない
	if (!m_isStart)
	{
		return;
	}
	
	// 当たり判定を着ける(1度しか通らない)
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

	// エネミーを召喚する
	for (int i = 0; i < (int)m_enemy.size(); i++)
	{
		if (m_startCnt == m_enemy.at(i).popFream)
		{
			PopEnemy(m_enemy.at(i).type, m_enemy.at(i).pos, m_enemy.at(i).rot);
		}
	}

	// ゴールしたらステージを終了更新に変更する
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

	// スタートしてからどれ程経ったか記録する
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
// 床の設置
//-----------------------------------------------------------------------------
void CStage::SetFloor(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, const D3DXVECTOR3 & scale)
{
	CPlain* objectX = CPlain::Create();
	objectX->LoadModel("BOX");
	objectX->SetPos(pos);
	objectX->SetRot(rot);
	objectX->SetScale(scale);
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
	objectX->AttachOutLine(false);
	objectX->SetCollisionFlag(false);
	//objectX->CalculationVtx();

	m_wall[index] = objectX;
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
// Goalをリセットする
//-----------------------------------------------------------------------------
void CStage::ResetGoal()
{
	m_goal->Release();

	SetGoal(m_goal->GetPos(), m_goal->GetTime());
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
void CStage::SetGoal(const D3DXVECTOR3 & pos,const int inTime)
{
	m_goal = CGoal::Create();
	m_goal->SetPos(pos);
	m_goal->SetTime(inTime);
	m_goal->SetScale(D3DXVECTOR3(1.5f,1.5f,1.5f));
	m_goal->SetRot(D3DXVECTOR3(D3DX_PI * 0.25f, D3DX_PI * 0.25f, D3DX_PI * 0.25f));
	m_goal->SetMoveRot(D3DXVECTOR3(0.01f, 0.01f, 0.0f));
	m_goal->SetMaterialDiffuse(0, D3DXCOLOR(0.5f,0.5f,0.5f,0.4f));
	m_goal->AttachOutLine();
	m_goal->CalculationVtx();
}
