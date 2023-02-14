//=============================================================================
//
// �Q�[�����
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "game.h"
/* Sestem�n�� */
#include "application.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "color.h"
#include "utility.h"

/* 3D�n�� */
#include "camera_game.h"
#include "light.h"
#include "stage.h"
#include "load_stage.h"

/* Object�n�� */
#include "object_polygon3d.h"
#include "player.h"
#include "player_died.h"
#include "goal.h"
#include "pause.h"
#include "bg_box.h"
#include "countdown.h"

//-----------------------------------------------------------------------------
// �萔
//-----------------------------------------------------------------------------
const int CGame::FALL_TIME = 145;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CGame::CGame():
	m_stage(nullptr),
	m_stageNext(nullptr),
	m_section(0),
	m_fallCount(0),
	m_stageSection(0),
	m_nextText(nullptr),
	m_retryText(nullptr),
	m_backText(nullptr)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CGame::~CGame()
{
}

//-----------------------------------------------------------------------------
// ������
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CGame::Init(void)
{
	m_section = 0;
	CApplication::GetInstance()->GetColor()->SetTheme(3);

	// ���C�g
	CLight* light = new CLight;
	light->Init();

	// �J����
	CCamera* camera = new CCameraGame;
	camera->Init();

	// ��
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	// �w�i
	{
		CObject2d* bg = CObject2d::Create();
		bg->SetPos(CApplication::CENTER_POS);
		bg->SetSize(CApplication::CENTER_POS);
		D3DXCOLOR color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1);
		bg->SetColor(color);
	}

	{// �v���C���[
		m_player = CPlayer::Create();
		m_player->LoadModel("BOX");
		m_player->SetPos(D3DXVECTOR3(0.0f, 60.0f, 0.0f));
		D3DXCOLOR color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2);
		m_player->SetMaterialDiffuse(0, color);
		m_player->CalculationVtx();
	}

	m_stagePath = LoadPath("data/FILE/stageGroup.json");

	m_stage = LoadAll(m_stagePath[m_stageSection]);
	m_stage->SetStart(false);

	// �}�E�X�̈ʒu���b�N
	CInput::GetKey()->GetMouse()->UseSetPosLock(true);

	m_countdown = CCountdown::Create(CApplication::CENTER_POS);

	m_isDeathStop = false;
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::Uninit(void)
{
	CInput::GetKey()->GetMouse()->UseSetPosLock(false);	// ���b�N�̉���
	CApplication::GetInstance()->GetSound()->Stop();	// ���y�̒�~
	Release();
}

//-----------------------------------------------------------------------------
// �X�V
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::Update(void)
{
	// �X�e�[�W���n�܂�܂ł̊Ԋu
	if (m_countdown != nullptr)
	{
		if (m_countdown->IsEnd())
		{
			m_countdown->SetUpdateStatus(CObject::EUpdateStatus::END);
			m_countdown = nullptr;
			m_stage->SetStart(true);	// �X�e�[�W���X�^�[�g����
			m_player->SetIsMove(true);	// �v���C���[�̈ړ���������
		}
	}

	// �|�[�Y
	if (CInput::GetKey()->Trigger(DIK_P))
	{
		CPause* pause = new CPause;
		pause->Init();
	}

	StageClear();
	PlayerDeath();
}

//-----------------------------------------------------------------------------
// 1�X�e�[�W�N���A��
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::StageClear()
{
	// �X�e�[�W���I����
	if (!m_stage->IsEnd())
	{
		return;
	}

	if (m_fallCount == 0)
	{
		// Enemy�ɑS�ă����[�X������������
		CObject::TypeAllFunc(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1, [](CObject* inObject)
		{
			inObject->Release();
		});

		m_nextText = CObject2d::Create(CTaskGroup::EPriority::LEVEL_2D_UI);
		D3DXVECTOR3 pos = CApplication::CENTER_POS;
		pos.x += 375.0f;
		pos.y += 180.0f;
		pos.y -= 70.0f;
		m_nextText->SetPos(pos);
		m_nextText->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f));
		m_nextText->SetTexture("TEXT_NEXT");

		m_retryText = CObject2d::Create(CTaskGroup::EPriority::LEVEL_2D_UI);
		pos.y += 70.0f;
		m_retryText->SetPos(pos);
		m_retryText->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f));
		m_retryText->SetTexture("TEXT_RETRY");

		m_backText = CObject2d::Create(CTaskGroup::EPriority::LEVEL_2D_UI);
		pos.y += 70.0f;
		m_backText->SetPos(pos);
		m_backText->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f));
		m_backText->SetTexture("TEXT_TITLE");
	}

	m_fallCount++;

	if (m_fallCount >= FALL_TIME)
	{
		m_fallCount = FALL_TIME;

		m_player->SetMove(D3DXVECTOR3(0.0f,0.0f,0.0f));

		// ���̃X�e�[�W�Ɉڍs
		if (CInput::GetKey()->Trigger(DIK_RETURN))
		{
			m_player->SetPos(D3DXVECTOR3(0.0f, 1900.0f, 0.0f));
			m_fallCount = 0;
			NextStage();

			m_nextText->Release();
			m_retryText->Release();
			m_backText->Release();
		}

		// �^�C�g���Ɉڍs
		if (CInput::GetKey()->Trigger(DIK_BACK))
		{
			m_fallCount = 0;
			CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
		}

		// ���g���C
		if (CInput::GetKey()->Trigger(DIK_R))
		{
			m_player->SetPos(D3DXVECTOR3(0.0f, 1900.0f, 0.0f));
			m_fallCount = 0;
			RetryStage();

			m_nextText->Release();
			m_retryText->Release();
			m_backText->Release();
		}
	}
}

//-----------------------------------------------------------------------------
// ���̃X�e�[�W�ֈڍs
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::NextStage()
{
	m_countdown = CCountdown::Create(CApplication::CENTER_POS);

	m_stage->Release();	// �I������

	m_stageSection++;

	m_stageNext = LoadAll(m_stagePath[m_stageSection]);

	m_stage = m_stageNext;		// ���̃X�e�[�W�����݂̃X�e�[�W�ɂ���
	m_stage->SetStart(false);	// �X�^�[�g���ĂȂ���Ԃɂ���
	m_player->SetIsGoal(false);	// �v���C���[���S�[�����Ă��Ȃ���Ԃɂ���
}

//-----------------------------------------------------------------------------
// �����X�e�[�W���J��Ԃ�
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::RetryStage()
{
	m_countdown = CCountdown::Create(CApplication::CENTER_POS);

	m_stage->Release();	// �I������

	m_stageNext = LoadAll(m_stagePath[m_stageSection]);

	m_stage = m_stageNext;		// ���̃X�e�[�W�����݂̃X�e�[�W�ɂ���
	m_stage->SetStart(false);	// �X�^�[�g���ĂȂ���Ԃɂ���
	m_player->SetIsGoal(false);	// �v���C���[���S�[�����Ă��Ȃ���Ԃɂ���
}

//-----------------------------------------------------------------------------
// �v���C���[���S��
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::PlayerDeath()
{
	if (m_player->IsDied() && !m_isDeathStop)
	{
		D3DXVECTOR3 pos = m_stage->GetPos();
		pos.y += 60.0f;

		CPlayerDied::SetOriginPos(pos);
		pos -= m_player->GetPos();
		pos /= 210.0f;
		m_player->AddMove(pos);
		m_isDeathStop = true;

		// Enemy�ɑS�ă����[�X������������
		CObject::TypeAllFunc(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1, [](CObject* inObject)
		{
			inObject->Release();
		});
	}

	if (m_isDeathStop)
	{
		m_stage->PopReset();
		m_stage->ResetGoal();

		if (!m_player->IsDied())
		{
			m_isDeathStop = false;
		}
	}
}
