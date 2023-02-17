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
#include "goal_effect.h"
#include "utility.h"

//-----------------------------------------------------------------------------
// �萔
//-----------------------------------------------------------------------------
const D3DXVECTOR3 CGoalEffect::SCALE = D3DXVECTOR3(0.5f, 1.4f, 0.5f);
const D3DXVECTOR3 CGoalEffect::MOVE_POWER = D3DXVECTOR3(0.0f, 3.5f, 0.0f);

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CGoalEffect::CGoalEffect()
{
	SetType(CObject::EType::PLAYER);
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CGoalEffect::~CGoalEffect()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CGoalEffect::Init()
{
	LoadModel("BOX");

	CObjectX::Init();
	SetScale(SCALE);
	SetMaterialDiffuse(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//SetColorAlpha(0.35f);
	//AttachOutLine();
	m_life = 20;
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CGoalEffect::Uninit()
{
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CGoalEffect::NormalUpdate()
{
	m_life--;

	D3DXVECTOR3 scale = GetScale();
	scale.x -= 0.02f;
	scale.y -= 0.01f;
	scale.z -= 0.02f;
	SetScale(scale);

	// �ŏ��Ɍ������w�肵���^�C�v��object�������Ă���
	CObject* object = SearchType(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1);

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();	// �����^�C�v��object�������Ă���

		if (OBBAndOBB((CObjectX*)object))
		{
			SetUpdateStatus(EUpdateStatus::END);
		}
		object = next;
	}


	if (m_life <= 0)
	{
		SetUpdateStatus(EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CGoalEffect::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// �ړ��ʂ̐ݒ�
//-----------------------------------------------------------------------------
void CGoalEffect::SetMove(const D3DXVECTOR3 & inMove)
{
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// �s���]�֐�

	D3DXVECTOR3 move = inMove;
	D3DXVec3TransformCoord(&move, &move, &mtxRot);

	CObjectX::SetMove(move);
}

//-----------------------------------------------------------------------------
// �쐬
//-----------------------------------------------------------------------------
CGoalEffect* CGoalEffect::Create(const D3DXVECTOR3 & inPos)
{
	CGoalEffect* effect = new CGoalEffect;

	effect->Init();

	D3DXVECTOR3 pos(FloatRandom(-5.0f,5.0f), FloatRandom(-5.0f, 5.0f), FloatRandom(-5.0f, 5.0f));

	effect->SetPos(inPos + pos);
	effect->SetRot(D3DXVECTOR3(FloatRandom(-D3DX_PI,D3DX_PI), FloatRandom(-D3DX_PI, D3DX_PI), FloatRandom(-D3DX_PI, D3DX_PI)));
	effect->SetMove(CGoalEffect::MOVE_POWER);

	if (FloatRandom(-1.0f, 1.0f) <= 0.0f)
	{
		effect->SetColorAlpha(0.35f);
		effect->SetScale(MOVE_POWER * 2.0f);
	}

	return nullptr;
}
