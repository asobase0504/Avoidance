//=============================================================================
//
// �Q�[�����
// Author : Hamada Ryuuga
//
//=============================================================================

//-----------------------------------------------------------------------------
// �C���N���[�h
//-----------------------------------------------------------------------------
#include "game.h"
#include "sound.h"
#include "input.h"
#include "application.h"
#include "object.h"
#include "light.h"
#include "player.h"
#include "file.h"
#include "fade.h"
#include "multiply.h"
#include "particle_manager.h"
#include "ranking.h"
#include "score.h"
#include "bg.h"
#include "pause.h"
#include "camera.h"
#include "task_group.h"
#include "text.h"
#include "object_mesh.h"

//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ��̐錾
//-----------------------------------------------------------------------------
CParticleManager*CGame::m_pPaticleManager = nullptr;
CPlayer*CGame::m_pPlayer = nullptr;
CPause *CGame::m_pPause = nullptr;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// �Q�[���̏���������
//=============================================================================
HRESULT CGame::Init(void)
{
	{
		CPlayer* player = CPlayer::Create();
		player->LoadModel("BOX");
		player->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		player->CalculationVtx();
	}

	//{
	//	CMesh* test = CMesh::Create();
	//	test->SetNumber(0);
	//	test->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//	test->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//	test->SetMesh(10);
	//	test->SetType(0);
	//}

	{
		CObjectPolygon3D* test = CObjectPolygon3D::Create();
		test->SetPos(D3DXVECTOR3(50.0f, 0.0f, 0.0f));
		test->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
		test->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		test->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
		test->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	return S_OK;
}

//=============================================================================
// �Q�[���̏I������
//=============================================================================
void CGame::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();

	if (m_pPaticleManager != nullptr)
	{
		m_pPaticleManager->Uninit();
		delete m_pPaticleManager;
		m_pPaticleManager = nullptr;
	}

	Release();
}

//=============================================================================
// �Q�[���̍X�V����
//=============================================================================
void CGame::Update(void)
{
	if (CInput::GetKey()->Trigger(DIK_RETURN))
	{
		CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_RESULT);
	}
}

//=============================================================================
// �Q�[���̕`�揈��
//=============================================================================
void CGame::Draw(void)
{

}
