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
#include "factory_enemy.h"
#include "player.h"
#include "plain.h"
#include "input.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CStage::CStage() :
	CTask(CTaskGroup::EPriority::LEVEL_SYSTEM),
	m_isStart(false),
	m_isEnd(false),
	m_isFrag(false)
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

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CStage::Uninit()
{
	m_goal->Release();	// �S�[��
	m_floor->Release();	// ��

	for (int i = 0; i < 5; i++)
	{
		m_wall[i]->Release();	// ��
	}

	for (int i = 0; i < (int)m_midairFloor.size(); i++)
	{
		m_midairFloor.at(i)->Release();	// �󒆏�
	}

	CTask::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CStage::Update()
{
	// �X�^�[�g����܂ōX�V���ʂ�Ȃ�
	if (!m_isStart)
	{
		return;
	}
	
	// �����蔻��𒅂���(1�x�����ʂ�Ȃ�)
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

	// �G�l�~�[����������
	for (int i = 0; i < (int)m_enemy.size(); i++)
	{
		if (m_startCnt == m_enemy.at(i).popFream)
		{
			PopEnemy(m_enemy.at(i).type, m_enemy.at(i).pos, m_enemy.at(i).rot);
		}
	}

	// �S�[��������X�e�[�W���I���X�V�ɕύX����
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

	// �X�^�[�g���Ă���ǂ���o�������L�^����
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
// ���̐ݒu
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
// �ǂ̐ݒu
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
// �󒆏��̒ǉ�
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
// �G�l�~�[���E��
//-----------------------------------------------------------------------------
void CStage::EnemyDeath()
{
}

//-----------------------------------------------------------------------------
// Goal�����Z�b�g����
//-----------------------------------------------------------------------------
void CStage::ResetGoal()
{
	m_goal->Release();

	SetGoal(m_goal->GetPos(), m_goal->GetTime());
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

//-----------------------------------------------------------------------------
// Goal�̐ݒ�
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
