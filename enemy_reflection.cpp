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
#include "enemy_reflection.h"
#include "line.h"
#include "utility.h"

//-----------------------------------------------------------------------------
// �萔
//-----------------------------------------------------------------------------
const D3DXVECTOR3 CEnemyReflection::SCALE(1.5f, 1.5f, 1.5f);
const D3DXVECTOR3 CEnemyReflection::MOVE_POWER(0.0f, -7.5f, 0.0f);
const int CEnemyReflection::REFLECT_COUNT = 5;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CEnemyReflection::CEnemyReflection()
{
	m_reflectCnt = 0;
	SetType(CObject::EType::PLAYER);
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CEnemyReflection::~CEnemyReflection()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CEnemyReflection::Init()
{
	// ���݂̃��[�V�����ԍ��̕ۊ�
	CEnemy::Init();
	LoadModel("BOX");
	SetScale(SCALE);
	SetMove(MOVE_POWER);
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CEnemyReflection::Uninit()
{
	CEnemy::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CEnemyReflection::NormalUpdate()
{
	if (OnHitPlain())
	{
		MulMove(-1.0f);
		m_reflectCnt++;
	}

	if (m_reflectCnt >= REFLECT_COUNT)
	{
		SetUpdateStatus(EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// �I���X�V
//-----------------------------------------------------------------------------
void CEnemyReflection::EndUpdate()
{
	for (int i = 0; i < 10; i++)
	{
		D3DXVECTOR3 pos = m_pos;
		pos.x += FloatRandam(-20.0f, 20.0f);
		pos.y += FloatRandam(-20.0f, 20.0f);
		pos.z += FloatRandam(-20.0f, 20.0f);
	}

	CEnemy::EndUpdate();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CEnemyReflection::Draw()
{
	CEnemy::Draw();
}

//-----------------------------------------------------------------------------
// �ړ��ʂ̐ݒ�
//-----------------------------------------------------------------------------
void CEnemyReflection::SetMove(const D3DXVECTOR3 & inMove)
{
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// �s���]�֐�

	D3DXVECTOR3 move = inMove;
	D3DXVec3TransformCoord(&move, &move, &mtxRot);

	CObjectX::SetMove(move);
}