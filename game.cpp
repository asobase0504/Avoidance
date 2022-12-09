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
/* 3D�n�� */
#include "camera.h"
#include "light.h"
/* Object�n�� */
#include "object_polygon3d.h"
#include "player.h"
#include "goal.h"
#include "pause.h"
/* Enemy�n�� */
#include "enemy_oneway.h"

//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ��̐錾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CGame::CGame()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CGame::~CGame()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CGame::Init(void)
{
	// ���C�g
	CLight* light = new CLight;
	light->Init();

	// �J����
	CCamera* camera = new CCamera;
	camera->Init();

	// ��
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	{// �v���C���[
		CPlayer* player = CPlayer::Create();
		player->LoadModel("BOX");
		player->SetPos(D3DXVECTOR3(0.0f, 15.0f, 0.0f));
		player->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0));
		player->CalculationVtx();
	}

	{// �S�[��
		CGoal* goal = CGoal::Create();
		goal->LoadModel("BOX");
		goal->SetPos(D3DXVECTOR3(60.0f, 15.0f, 0.0f));
		goal->SetRot(D3DXVECTOR3(D3DX_PI * 0.25f, D3DX_PI * 0.25f, D3DX_PI * 0.25f));
		goal->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2));
		goal->CalculationVtx();
	}

	{// ��
		CObjectPolygon3D* floor = CObjectPolygon3D::Create();
		floor->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		floor->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.25f, 0.0f));
		floor->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		floor->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		floor->SetColor(CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1));
	}

	{// ��
		CObjectPolygon3D* wall = CObjectPolygon3D::Create();
		wall->SetPos(D3DXVECTOR3(75.0f, 100.0f, 75.0f));
		wall->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.25f, 0.0f));
		wall->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		wall->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		wall->SetColor(CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1));
	}

	{// ��
		CObjectPolygon3D* wall = CObjectPolygon3D::Create();
		wall->SetPos(D3DXVECTOR3(-75.0f, 100.0f, 75.0f));
		wall->SetRot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.25f, 0.0f));
		wall->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		wall->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		wall->SetColor(CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1));
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			{// �G
				CEnemyOneWay* enemy = new CEnemyOneWay;
				enemy->Init();
				enemy->SetPos(D3DXVECTOR3(-30.0f + j * 20.0f, 210.0f + i * 40.0f + j * 20.0f, -70.0f + 40.0f * i));
				enemy->SetMove(D3DXVECTOR3(0.0f, -0.75f, 0.0f));
			}
		}
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CGame::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();
	Release();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CGame::Update(void)
{
	CPlayer* player = (CPlayer*)CObject::SearchType(CObject::PLAYER,CTaskGroup::EPriority::LEVEL_3D_1);

	if (player != nullptr)
	{
		if (player->GetIsGoal())
		{
			CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_RESULT);
		}
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CGame::Draw(void)
{

}
