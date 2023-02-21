//=============================================================================
//
// �v���C���[�ݒ�
// Author:Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <assert.h>
#include "enemy.h"
#include "application.h"
#include "color.h"
#include "sound.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CEnemy::CEnemy()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CEnemy::~CEnemy()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CEnemy::Init()
{
	LoadModel("BOX");

	// ���݂̃��[�V�����ԍ��̕ۊ�
	CObjectX::Init();
	SetType(CObject::ENEMY);
	SetMaterialDiffuse(0, m_color);

	AttachOutLine();
	AttachShadow();

	SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	SetColorAlpha(0.0f);

	SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3));
	SetCollisionFlag(false);
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CEnemy::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// �o���X�V
//-----------------------------------------------------------------------------
void CEnemy::PopUpdate()
{
	SetColorAlpha(GetColorAlpha() + 0.01f);
	SetColor(D3DXCOLOR(GetColor().r, GetColor().g - 0.01f, GetColor().b - 0.01f, GetColor().a));
	SetMaterialDiffuse(0,GetColor());

	if (GetColorAlpha() >= 1.0f)
	{
		SetColorAlpha(1.0f);
		SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3));
		SetUpdateStatus(CObject::EUpdateStatus::NORMAL);
		SetCollisionFlag(true);
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CEnemy::NormalUpdate()
{
}

//-----------------------------------------------------------------------------
// �I���X�V
//-----------------------------------------------------------------------------
void CEnemy::EndUpdate()
{
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_ENEMY_HIT);
	Release();
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
CEnemy* CEnemy::Create()
{
	CEnemy* enemy = new CEnemy;

	if (enemy != nullptr)
	{
		enemy->Init();
	}

	return enemy;
}

//-----------------------------------------------------------------------------
// ���Ƃ̓����蔻��
//-----------------------------------------------------------------------------
bool CEnemy::OnHitPlain()
{
	// �ŏ��Ɍ������w�肵���^�C�v��object�������Ă���
	CObject* object = SearchType(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1);

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();	// �����^�C�v��object�������Ă���

		if (OBBAndOBB((CObjectX*)object))
		{
			return true;
		}

		object = next;
	}
	return false;
}
