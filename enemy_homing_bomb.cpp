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
#include "enemy_homing_bomb.h"
#include "utility.h"

//-----------------------------------------------------------------------------
// �萔
//-----------------------------------------------------------------------------
const D3DXVECTOR3 CEnemyHomingBomb::SCALE = D3DXVECTOR3(0.15f, 0.15f, 0.15f);
const D3DXVECTOR3 CEnemyHomingBomb::MOVE_POWER = D3DXVECTOR3(0.0f, -3.5f, 0.0f);
const int CEnemyHomingBomb::MOVE_START_TIME = 10;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CEnemyHomingBomb::CEnemyHomingBomb()
{
	m_scaleNow = SCALE.x;
	m_startCnt = 0;
	SetType(CObject::EType::PLAYER);
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CEnemyHomingBomb::~CEnemyHomingBomb()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CEnemyHomingBomb::Init()
{
	// ���݂̃��[�V�����ԍ��̕ۊ�
	CEnemy::Init();
	SetScale(SCALE);
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CEnemyHomingBomb::Uninit()
{
	CEnemy::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CEnemyHomingBomb::NormalUpdate()
{

	m_startCnt++;
	float speed;
	speed = MOVE_POWER.y + (m_scaleNow);
	if (speed > -0.001f)
	{
		speed = -0.001f;
	}
	//if (m_startCnt % MOVE_START_TIME == 0)
	{
		m_startCnt = 0;
		SeeTarget();

		SetMove(D3DXVECTOR3(0.0f, speed,0.0f));
	}
	CDebugProc::Print("EnemyInfo : %f\n", speed);

	m_scaleNow += 0.01f;
	SetScale(D3DXVECTOR3(m_scaleNow, m_scaleNow, m_scaleNow));

	if (m_scaleNow >= 5.0f)
	{
		SetUpdateStatus(EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// �I���X�V
//-----------------------------------------------------------------------------
void CEnemyHomingBomb::EndUpdate()
{
	CEnemy::EndUpdate();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CEnemyHomingBomb::Draw()
{
	CEnemy::Draw();
}

//-----------------------------------------------------------------------------
// �ړ��ʂ̐ݒ�
//-----------------------------------------------------------------------------
void CEnemyHomingBomb::SetMove(const D3DXVECTOR3 & inMove)
{
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// �s���]�֐�

	D3DXVECTOR3 move = inMove;
	D3DXVec3TransformCoord(&move, &move, &mtxRot);

	CObjectX::SetMove(move);
}

//-----------------------------------------------------------------------------
// �ړ��ʂ̐ݒ�
//-----------------------------------------------------------------------------
void CEnemyHomingBomb::SeeTarget()
{
	CObject* object = SearchType(CObject::EType::PLAYER, CTaskGroup::EPriority::LEVEL_3D_1);

	D3DXVECTOR3 dist = m_pos - object->GetPos();

	D3DXVECTOR3 rot(0.0f, 0.0f, 0.0f);
	rot.y = atan2f(dist.x, dist.z);
	rot.x = atan2f(sqrtf(dist.z * dist.z + dist.x * dist.x), dist.y);

	SetRot(rot);
}
