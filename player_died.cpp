//=============================================================================
//
// �v���C���[�̎��S�G�t�F�N�g
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "player_died.h"
#include "utility.h"
#include "application.h"
#include "sound.h"
#include "color.h"

//-----------------------------------------------------------------------------
// �萔
//-----------------------------------------------------------------------------
const unsigned int CPlayerDied::MAX_LIFE = 100;			// ��������
const unsigned int CPlayerDied::AGGREGATE_TIME = 60;	// �W�����鎞��
const float CPlayerDied::SPATTER_SPEED = 8.5f;			// ��юU�鑬�x
const float CPlayerDied::GRAVITY = -1.25f;				// ���Z�p�x�̏��
D3DXVECTOR3 CPlayerDied::m_posOrigin(0.0f, 0.0f, 0.0f);	// �d��

//-----------------------------------------------------------------------------
// �R���X�g���N�^
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CPlayerDied::CPlayerDied()
{
	m_life = 0;	// ����
	m_hit = false;
	m_scaleOrigin = D3DXVECTOR3(0.0f,0.0f,0.0f);	// �����ʒu
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CPlayerDied::~CPlayerDied()
{
}

//-----------------------------------------------------------------------------
// ������
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CPlayerDied::Init()
{
	// ���݂̃��[�V�����ԍ��̕ۊ�
	CObjectX::Init();
	LoadModel("BOX");

	m_life = MAX_LIFE;
	m_hit = false;
	AttachOutLine();
	AttachShadow();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CPlayerDied::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CPlayerDied::NormalUpdate()
{
	m_life--;

	if (!m_hit)
	{
		if (m_life > AGGREGATE_TIME)
		{
			AddMove(D3DXVECTOR3(0.0f, GRAVITY, 0.0f));
		}

		m_hit = OnHitPlain();
	}

	if (m_life == AGGREGATE_TIME)
	{
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_BACK);
		SetMoveRot(D3DXVECTOR3(FloatRandom(-0.15f,0.15f), FloatRandom(-0.15f, 0.15f), FloatRandom(-0.15f, 0.15f)));
		SetMove((m_posOrigin - m_pos) / (float)AGGREGATE_TIME);
	}

	if (m_life <= 0)
	{
		SetUpdateStatus(CObject::EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// �X�V
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CPlayerDied::EndUpdate()
{
	Release();
}

//-----------------------------------------------------------------------------
// �`��
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CPlayerDied::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// ����
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CPlayerDied* CPlayerDied::Create(const D3DXVECTOR3& inPos)
{
	CPlayerDied* objectX = new CPlayerDied;

	if (objectX != nullptr)
	{
		objectX->Init();
		objectX->SetPos(inPos);
		float scale = FloatRandom(0.45f, 0.85f);
		objectX->SetScale(D3DXVECTOR3(scale, scale, scale));
		objectX->m_scaleOrigin = objectX->GetScale();;
		objectX->SetRot(D3DXVECTOR3(FloatRandom(-D3DX_PI, D3DX_PI), FloatRandom(-D3DX_PI, D3DX_PI), FloatRandom(-D3DX_PI, D3DX_PI)));
		objectX->SetMove(D3DXVECTOR3(FloatRandom(-SPATTER_SPEED, SPATTER_SPEED), FloatRandom(0.5f, 15.5f), FloatRandom(-SPATTER_SPEED, SPATTER_SPEED)));
		objectX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2));
	}

	return objectX;
}

//-----------------------------------------------------------------------------
// �n�ʂƂ̓����蔻��
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
bool CPlayerDied::OnHitPlain()
{
	// �ŏ��Ɍ������w�肵���^�C�v��object�������Ă���
	CObject* object = SearchType(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1);

	bool hit = false;

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();	// �����^�C�v��object�������Ă���

		if (SphereAndAABB((CObjectX*)object))
		{
			SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			return true;
		}

		object = next;
	}
	return hit;
}
