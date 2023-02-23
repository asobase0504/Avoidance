//=============================================================================
//
// ���� ( �^�C�g�� )
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "title.h"
#include "application.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "color.h"
// 3D�n��
#include "camera_title.h"
#include "light.h"
// Object�n��
#include "object2d.h"
#include "objectX.h"
// Particle�n��
#include "particle_manager.h"
#include "particle_emitter.h"

#include "title_logo.h"
#include "mouse_object.h"
#include "select.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TITLE);
	CApplication::GetInstance()->GetColor()->SetTheme(3);

	// ���C�g
	CLight* light = new CLight;
	light->Init();

	// �J����
	CCamera* camera = new CCameraTitle;
	camera->Init();

	// �w�i
	{
		CObject2d* titleBg = CObject2d::Create();
		titleBg->SetPos(CApplication::CENTER_POS);
		titleBg->SetRot(D3DXVECTOR3(0.0f,0.0f,0.25f));
		titleBg->SetSize(CApplication::CENTER_POS * 0.9f);
		titleBg->SetColor(CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0));
	}

	// �^�C�g�����S
	CTitleLogo* titleLogo = new CTitleLogo(CTaskGroup::LEVEL_2D_1);
	titleLogo->Init();

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(15.0f, -15.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(-15.0f, -15.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(15.0f, 15.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(-15.0f, 15.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3));
		testX->CalculationVtx();
	}

	// �}�E�X�̌����ڐ���
	CMouseObject::Create();

	{
		D3DXVECTOR3 pos = CApplication::CENTER_POS;
		pos.y += 120.0f;
		m_start = CSelect::Create(pos);

		m_start->SetSize(D3DXVECTOR3(150.0f, 25.0f, 0.0f));
		m_start->SetTexture("TEXT_START");

		m_startShadow = CObject2d::Create(CTaskGroup::EPriority::LEVEL_2D_1);
		m_startShadow->SetSize(D3DXVECTOR3(150.0f, 25.0f, 0.0f));
		m_startShadow->SetPos(pos * 1.005f);
		m_startShadow->SetTexture("TEXT_START");
		m_startShadow->SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));

		m_start->SetFunctionClick([this](CSelect*)
		{
			CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_GAME);
		});
		m_start->SetFunctionSelection([this](CSelect*)
		{
			m_start->SetSize(D3DXVECTOR3(150.0f * 1.15f, 25.0f * 1.15f, 0.0f));
			m_startShadow->SetSize(D3DXVECTOR3(150.0f, 25.0f, 0.0f)* 1.15f);
		});
		m_start->SetFunctionNoSelection([this](CSelect*)
		{
			m_start->SetSize(D3DXVECTOR3(150.0f, 25.0f, 0.0f));
			m_startShadow->SetSize(D3DXVECTOR3(150.0f, 25.0f, 0.0f));
		});

		if (CInput::GetKey()->GetAcceptJoyPadCount() != 0)
		{
			m_start->SetSelect(true);
		}
	}

	return S_OK;
}

//=============================================================================
// �j��
//=============================================================================
void CTitle::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	if (CInput::GetKey()->Trigger(DIK_F7))
	{
		CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_EDIT);
	}
}
