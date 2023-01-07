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
	m_pLight = new CLight;
	m_pLight->Init();

	// �J����
	m_pCamera = new CCameraTitle;
	m_pCamera->Init();

	// �p�[�e�B�N���}�l�[�W���[
	m_pPaticleManager = new CParticleManager;
	if (FAILED(m_pPaticleManager->Init()))
	{
		return E_FAIL;
	}

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
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(40.0f, 0.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(20.0f, 0.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(-20.0f, 0.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3));
		testX->CalculationVtx();
	}

	{
		m_mouseCursor = CMouseObject::Create();
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
	if (CInput::GetKey()->Trigger(KEY_UP))
	{
		m_pPaticleManager->Create(CParticleEmitter::EObjectType::POLIGON2D, CApplication::CENTER_POS, 0);
	}
	if (CInput::GetKey()->Trigger(KEY_DOWN))
	{
		CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_GAME);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}