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
/* 3D�n�� */
#include "camera.h"
#include "light.h"
/* Object�n��*/
#include "object_polygon3d.h"
#include "player.h"
#include "pause.h"

//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ��̐錾
//-----------------------------------------------------------------------------

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
	CLight* light = new CLight;
	light->Init();

	CCamera* camera = new CCamera;
	camera->Init();

	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	{
		CPlayer* player = CPlayer::Create();
		player->LoadModel("BOX");
		player->SetPos(D3DXVECTOR3(0.0f, 15.0f, 0.0f));
		player->SetMaterialDiffuse(0, D3DCOLOR(0xff46bdc6));
		player->CalculationVtx();
	}

	{
		CObjectPolygon3D* floor = CObjectPolygon3D::Create();
		floor->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		floor->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.25f, 0.0f));
		floor->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		floor->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		floor->SetColor(D3DCOLOR(0xff4285f4));
	}
	{
		CObjectPolygon3D* wall = CObjectPolygon3D::Create();
		wall->SetPos(D3DXVECTOR3(75.0f, 100.0f, 75.0f));
		wall->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.25f, 0.0f));
		wall->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		wall->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		wall->SetColor(D3DCOLOR(0xff4285f4));
	}
	{
		CObjectPolygon3D* wall = CObjectPolygon3D::Create();
		wall->SetPos(D3DXVECTOR3(-75.0f, 100.0f, 75.0f));
		wall->SetRot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.25f, 0.0f));
		wall->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		wall->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		wall->SetColor(D3DCOLOR(0xff4285f4));
	}

	return S_OK;
}

//=============================================================================
// �Q�[���̏I������
//=============================================================================
void CGame::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();
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
