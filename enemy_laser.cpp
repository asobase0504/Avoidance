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
	LoadModel("BOX");
	SetScale(SCALE);
	m_guideLine = CLine::Create();
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_moveScale = 0.2f;
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
// �o�����X�V
//-----------------------------------------------------------------------------
void CEnemyLaser::PopUpdate()
{
	m_startCnt++;
	if (m_startCnt % MOVE_START_TIME == 0)
	{
		//SetMove(MOVE_POWER);
		SetUpdateStatus(EUpdateStatus::NORMAL);
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CEnemyLaser::NormalUpdate()
{
	SetMove(MOVE_POWER);
	D3DXVECTOR3 scale = GetScale();
	SetScale(D3DXVECTOR3(scale.x, scale.y + m_moveScale, scale.z));
	CDebugProc::Print("%f\n", GetScale().y);
	CEnemy::NormalUpdate();

	if (OnHitPlain())
	{
		SetUpdateStatus(EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// �I���X�V
//-----------------------------------------------------------------------------
void CEnemyLaser::EndUpdate()
{
	CEnemy::EndUpdate();
	m_guideLine->Release();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CEnemyLaser::Draw()
{
	CEnemy::Draw();
	m_guideLine->SetLine(m_pos, m_rot, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -700.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
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
