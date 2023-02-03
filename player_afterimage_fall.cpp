//=============================================================================
//
// �v���C���[�̎c���ݒ�
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "player_afterimage_fall.h"
#include "utility.h"

const int CPlayerAfterimageFall::MAX_LIFE = 10;
const float CPlayerAfterimageFall::ALPHA_COLOR = 0.45f;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CPlayerAfterimageFall::CPlayerAfterimageFall()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CPlayerAfterimageFall::~CPlayerAfterimageFall()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CPlayerAfterimageFall::Init()
{
	// ���݂̃��[�V�����ԍ��̕ۊ�
	CObjectX::Init();
	LoadModel("BOX");

	m_life = MAX_LIFE;
	SetColorAlpha(ALPHA_COLOR);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CPlayerAfterimageFall::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CPlayerAfterimageFall::NormalUpdate()
{
	m_life--;

	float scale = ease::OutSine((float)m_life / (float)MAX_LIFE) * 0.75f;

	SetScale(D3DXVECTOR3(scale, scale, scale));

	if (m_life <= 0)
	{
		SetUpdateStatus(CObject::EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CPlayerAfterimageFall::EndUpdate()
{
	Release();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CPlayerAfterimageFall::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
CPlayerAfterimageFall* CPlayerAfterimageFall::Create(const D3DXVECTOR3& inPos)
{
	CPlayerAfterimageFall* objectX = new CPlayerAfterimageFall;

	if (objectX != nullptr)
	{
		objectX->Init();
		objectX->SetPos(inPos);
		objectX->SetMove(D3DXVECTOR3(0.0f,FloatRandam(25.0f,35.5f),0.0f));
	}

	return objectX;
}
