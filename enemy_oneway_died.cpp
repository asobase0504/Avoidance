//=============================================================================
//
// ���i����G�̎��S�G�t�F�N�g
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "enemy_oneway_died.h"
#include "utility.h"
#include "application.h"
#include "color.h"

//-----------------------------------------------------------------------------
// �萔
//-----------------------------------------------------------------------------
const int CEnemyOneWayDied::MAX_LIFE = 20;
const float CEnemyOneWayDied::ALPHA_COLOR = 0.25f;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CEnemyOneWayDied::CEnemyOneWayDied()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CEnemyOneWayDied::~CEnemyOneWayDied()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CEnemyOneWayDied::Init()
{
	LoadModel("BOX");
	CObjectX::Init();

	m_life = MAX_LIFE;
	SetColorAlpha(ALPHA_COLOR);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CEnemyOneWayDied::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CEnemyOneWayDied::NormalUpdate()
{
	m_life--;

	float scale = ease::OutQuad((float)m_life / (float)MAX_LIFE) * 0.5f;

	SetScale(D3DXVECTOR3(scale, scale, scale));

	AddMove(D3DXVECTOR3(0.0f, -0.35f, 0.0f));

	if (m_life <= 0)
	{
		SetUpdateStatus(CObject::EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CEnemyOneWayDied::EndUpdate()
{
	Release();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CEnemyOneWayDied::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
CEnemyOneWayDied* CEnemyOneWayDied::Create(const D3DXVECTOR3& inPos)
{
	CEnemyOneWayDied* objectX = new CEnemyOneWayDied;

	if (objectX != nullptr)
	{
		objectX->Init();
		objectX->SetPos(inPos);
		objectX->SetRot(D3DXVECTOR3(FloatRandam(-D3DX_PI, D3DX_PI), FloatRandam(-D3DX_PI, D3DX_PI), FloatRandam(-D3DX_PI, D3DX_PI)));
		objectX->SetMove(D3DXVECTOR3(FloatRandam(-5.5f,5.5f),7.5f, FloatRandam(-5.5f, 5.5f)));
		objectX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3));
	}

	return objectX;
}
