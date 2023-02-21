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
#include "enemy_laser.h"
#include "line.h"
#include "debug_proc.h"
#include "delay_process.h"

//-----------------------------------------------------------------------------
// �萔
//-----------------------------------------------------------------------------
const D3DXVECTOR3 CEnemyLaser::SCALE = D3DXVECTOR3(0.55f, 1.25f, 0.55f);
const D3DXVECTOR3 CEnemyLaser::MOVE_POWER = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
const int CEnemyLaser::MOVE_START_TIME = 80;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CEnemyLaser::CEnemyLaser()
{
	m_startCnt = 0;
	SetType(CObject::EType::PLAYER);
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CEnemyLaser::~CEnemyLaser()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CEnemyLaser::Init()
{
	// ���݂̃��[�V�����ԍ��̕ۊ�
	CEnemy::Init();
	SetScale(SCALE);
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_moveScale = 0.2f;
	m_hit = false;
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CEnemyLaser::Uninit()
{
	CEnemy::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CEnemyLaser::NormalUpdate()
{
	if (!m_hit)
	{
		SetMove(MOVE_POWER);
		D3DXVECTOR3 scale = GetScale();
		SetScale(D3DXVECTOR3(scale.x, scale.y + m_moveScale, scale.z));
	}

	CEnemy::NormalUpdate();

	if (OnHitPlain() && !m_hit)
	{
		m_hit = true;
		SetMove(D3DXVECTOR3(0.0f,0.0f,0.0f));
		CDelayProcess::DelayProcess(60, this, [this]() { SetUpdateStatus(EUpdateStatus::END); });
	}
}

//-----------------------------------------------------------------------------
// �I���X�V
//-----------------------------------------------------------------------------
void CEnemyLaser::EndUpdate()
{
	CEnemy::EndUpdate();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CEnemyLaser::Draw()
{
	CEnemy::Draw();
}

//-----------------------------------------------------------------------------
// �ړ��ʂ̐ݒ�
//-----------------------------------------------------------------------------
void CEnemyLaser::SetMove(const D3DXVECTOR3 & inMove)
{
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// �s���]�֐�

	D3DXVECTOR3 move = inMove;
	D3DXVec3TransformCoord(&move, &move, &mtxRot);

	CObjectX::SetMove(move);
}
