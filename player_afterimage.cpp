//=============================================================================
//
// �v���C���[�̎c���ݒ�
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "player_afterimage.h"
#include "utility.h"

const int CPlayerAfterimage::MAX_LIFE = 120;
const float CPlayerAfterimage::ALPHA_COLOR = 0.45f;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CPlayerAfterimage::CPlayerAfterimage()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CPlayerAfterimage::~CPlayerAfterimage()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CPlayerAfterimage::Init()
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
void CPlayerAfterimage::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CPlayerAfterimage::NormalUpdate()
{
	m_life--;

	float scale = ease::InSine((float)m_life / (float)MAX_LIFE);

	SetScale(D3DXVECTOR3(scale, scale, scale));
	if (m_life <= 0)
	{
		SetUpdateStatus(CObject::EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CPlayerAfterimage::EndUpdate()
{
	Release();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CPlayerAfterimage::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
CPlayerAfterimage* CPlayerAfterimage::Create(const D3DXVECTOR3& inPos)
{
	CPlayerAfterimage* objectX = new CPlayerAfterimage;

	if (objectX != nullptr)
	{
		objectX->Init();
		objectX->SetPos(inPos);
	}

	return objectX;
}
