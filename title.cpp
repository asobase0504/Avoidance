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
// 3D�n��
#include "camera.h"
#include "light.h"
// Object�n��
#include "object2d.h"
#include "objectX.h"
// Particle�n��
#include "particle_manager.h"
#include "particle_emitter.h"

#include "title_logo.h"

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

	// ���C�g
	m_pLight = new CLight;
	m_pLight->Init();

	// �J����
	m_pCamera = new CCamera;
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
		titleBg->SetColor(D3DCOLOR(0xfffbbc04));
	}

	// �^�C�g�����S
	CTitleLogo* titleLogo = new CTitleLogo(CTaskGroup::LEVEL_2D_1);
	titleLogo->Init();

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, D3DCOLOR(0xff46bdc6));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, D3DCOLOR(0xffea4335));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(15.0f, 0.0f, -15.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, D3DCOLOR(0xff4285f4));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(-15.0f, 0.0f, -15.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, D3DCOLOR(0xfffbbc04));
		testX->CalculationVtx();
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
	static bool a = false;
	if (CInput::GetKey()->Trigger(KEY_UP) && !a)
	{
		a = true;
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