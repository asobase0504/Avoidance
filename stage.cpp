//=============================================================================
//
// �X�e�[�W�̐ݒ�
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "stage.h"
#include "objectX.h"
#include "goal.h"
#include "application.h"
#include "color.h"
#include "enemy.h"
#include "enemy_oneway.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CStage::CStage() : CTask(CTaskGroup::EPriority::LEVEL_SYSTEM)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CStage::~CStage()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CStage::Init()
{
	SetRole(ROLE_STAGE);
	m_startCnt = 0;

	{// �S�[��
		CGoal* goal = CGoal::Create();
		goal->LoadModel("BOX");
		goal->SetPos(D3DXVECTOR3(60.0f, 15.0f, 0.0f));
		goal->SetRot(D3DXVECTOR3(D3DX_PI * 0.25f, D3DX_PI * 0.25f, D3DX_PI * 0.25f));
		goal->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2));
		goal->CalculationVtx();
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CStage::Uninit()
{
	CTask::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CStage::Update()
{
	CTask::Update();

	for (int i = 0; i < m_enemy.size(); i++)
	{
		if (m_startCnt == m_enemy[i].popFream)
		{
			PopEnemy(m_enemy[i].type, m_enemy[i].pos, m_enemy[i].rot);
		}
	}
	m_startCnt++;
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CStage::Draw()
{
	CTask::Draw();
}

//-----------------------------------------------------------------------------
// �傫���̐ݒ�
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
// ���̐ݒu
//-----------------------------------------------------------------------------
void CStage::SetFloor(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, const D3DXVECTOR3 & scale)
{
	CObjectX* objectX = CObjectX::Create(pos);
	objectX->LoadModel("BOX");
	objectX->SetRot(rot);
	objectX->SetScale(scale);
	m_scale = scale.x;
	objectX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1));
	//objectX->CalculationVtx();

	m_floor = objectX;
}

//-----------------------------------------------------------------------------
// �ǂ̐ݒu
//-----------------------------------------------------------------------------
void CStage::SetWall(int index, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, const D3DXVECTOR3 & scale)
{
	CObjectX* objectX = CObjectX::Create(pos);
	objectX->LoadModel("BOX");
	objectX->SetRot(rot);
	objectX->SetScale(scale);
	objectX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1));
	//objectX->CalculationVtx();

	m_wall[index] = objectX;
}

//-----------------------------------------------------------------------------
// �󒆏��̒ǉ�
//-----------------------------------------------------------------------------
void CStage::AddFloor(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale)
{
	CObjectX* objectX = CObjectX::Create(pos);
	objectX->LoadModel("BOX");
	objectX->SetRot(rot);
	objectX->SetScale(scale);
	objectX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0));
	//objectX->CalculationVtx();

	m_midairFloor.emplace(m_midairFloor.size(), objectX);
}

//-----------------------------------------------------------------------------
// �G�̒ǉ�
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
// �G�̏o��
//-----------------------------------------------------------------------------
void CStage::PopEnemy(const int type, const D3DXVECTOR3 & inPos, const D3DXVECTOR3 & inRot)
{
	CEnemy* enemy = nullptr;

	switch (type)
	{
	case 0:
		enemy = new CEnemyOneWay;
		break;
	default:
		break;
	}
	enemy->Init();
	enemy->SetPos(inPos);
	enemy->SetRot(inRot);
}

//-----------------------------------------------------------------------------
// ����
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
