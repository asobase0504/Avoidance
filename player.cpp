//=============================================================================
//
// �v���C���[�ݒ�
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "player.h"
#include "player_afterimage.h"
#include "player_afterimage_fall.h"
#include "player_died.h"

#include "input.h"
#include "camera.h"
#include "application.h"
#include "utility.h"
#include "collision.h"
#include "goal.h"
#include "color.h"

//-----------------------------------------------------------------------------
// �萔
//-----------------------------------------------------------------------------
const float CPlayer::SPEED = 6.5f;			// �ړ���
const float CPlayer::ATTENUATION = 0.45f;	// �ړ������W��
const float CPlayer::JUMPING_POWER = 8.5f;	// ������
const float CPlayer::GRAVITY = 0.15f;		// �d��

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CPlayer::CPlayer() :
	m_quaternion(0.0f,0.0,0.0f,1.0f),
	m_quaternionOld(0.0f, 0.0, 0.0f, 1.0f),
	m_isGoal(false),
	m_isMove(false),
	jumpDirection(0.0f, 0.0, 0.0f),
	m_isJump(false),
	m_jumpCount(0),
	m_jumpTime(0),
	m_isDied(false)
{
	SetType(CObject::EType::PLAYER);
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CPlayer::Init()
{
	LoadModel("BOX");

	CObjectX::Init();

	AttachOutLine();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CPlayer::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CPlayer::NormalUpdate()
{
	// �f�o�b�O
	CDebugProc::Print("jumpDirection : %.2f,%.2f,%.2f\n", jumpDirection.x, jumpDirection.y, jumpDirection.z);
	CDebugProc::Print("quaternion  : %.2f,%.2f,%.2f\n", m_quaternion.x, m_quaternion.y, m_quaternion.z);
	CDebugProc::Print("quaternionOld  : %.2f,%.2f,%.2f\n", m_quaternionOld.x, m_quaternionOld.y, m_quaternionOld.z);
	CDebugProc::Print("pos  : %.2f,%.2f,%.2f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("move : %.2f,%.2f,%.2f\n", m_move.x, m_move.y, m_move.z);

	if (!m_isDied)
	{
		m_quaternionOld = m_quaternion;
		Move();			// �ړ�
		boost();		// �ːi
		Jump();			// �W�����v
		Landing();		// ����
		OnHitGoal();	// Goal�Ƃ̓����蔻��
		OnHitEnemy();	// Enemy�Ƃ̓����蔻��

		NextStageWait();

		CInput* input = CInput::GetKey();

#ifdef _DEBUG
		if (input->Press(DIK_0))
		{
			SetPos(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
			SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		if (input->Press(DIK_9))
		{
			SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
#endif // _DEBUG

		// �����̔��q�ŉ��ɍs�����ꍇ�̑Ώ��@
		if (m_pos.y < -5000.0f)
		{
			SetPos(D3DXVECTOR3(m_pos.x, 1200.0f, m_pos.z));
		}

		// �O�Ճp�[�e�B�N��
		static int time = 0;
		time++;
		if (m_isGoal)
		{
			// �����O��
			//if (time % 2 == 0)
			{
				time = 0;
				CPlayerAfterimageFall* afterimage = CPlayerAfterimageFall::Create(m_pos);
				afterimage->SetMtxRot(GetMtxRot());
				afterimage->SetMaterialDiffuse(0, GetMaterialDiffuse(0));
			}
		}
		else
		{
			// �ړ��O��
			if (time % 2 == 0)
			{
				time = 0;
				CPlayerAfterimage* afterimage = CPlayerAfterimage::Create(m_pos);
				afterimage->SetMtxRot(GetMtxRot());
				afterimage->SetMaterialDiffuse(0, GetMaterialDiffuse(0));
			}
		}
	}
	else
	{
		static int cnt = 0;

		cnt++;
		if (cnt >= CPlayerDied::MAX_LIFE)
		{
			cnt = 0;
			m_isDied = false;
		}
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CPlayer::Draw()
{
	if (!m_isDied)
	{
		CObjectX::Draw();
	}
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
CPlayer* CPlayer::Create()
{
	CPlayer* player = new CPlayer;

	if (player != nullptr)
	{
		player->Init();
	}

	return player;
}

//-----------------------------------------------------------------------------
// �ړ�
//-----------------------------------------------------------------------------
void CPlayer::Move()
{
	if (!m_isMove)
	{
		return;
	}

	CInput* input = CInput::GetKey();
	D3DXVECTOR3 moveInput = D3DXVECTOR3(0.0f,0.0f,0.0f);

	// �L�[�{�[�h����
	if (input->Press(DIK_D))
	{
		moveInput.x += 1.0f;
	}
	if (input->Press(DIK_A))
	{
		moveInput.x -= 1.0f;
	}
	if (input->Press(DIK_S))
	{	
		moveInput.z -= 1.0f;
	}
	if (input->Press(DIK_W))
	{	
		moveInput.z += 1.0f;
	}

	// JoyPad�̓��͂��������ꍇ��JoyPad�ɕύX����
	D3DXVECTOR3 joypadInput = input->VectorMoveJoyStick(false);
	if (D3DXVec3LengthSq(&joypadInput) != 0.0f)
	{
		moveInput.x = joypadInput.x;
		moveInput.z = joypadInput.y * -1.0f;
	}

	// �J�����̌�����Input�ɔ��f
	CCamera* camera = (CCamera*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(ROLE_CAMERA, CTaskGroup::LEVEL_3D_1);
	if (camera != nullptr)
	{
		moveInput = camera->VectorCombinedRot(moveInput);
	}

	// ���͂��������ꍇ�ړ��ʂɔ��f
	if (D3DXVec3Length(&moveInput) != 0.0f)
	{
		if (fabs(m_move.x) < fabs(moveInput.x * SPEED))
		{
			m_move.x = moveInput.x * SPEED;
		}

		if (fabs(m_move.z) < fabs(moveInput.z * SPEED))
		{
			m_move.z = moveInput.z * SPEED;
		}
	}

	//�i�ړI�̒l - ���݂̒l�j �� �����W��
	m_move.x += (0.0f - m_move.x) * ATTENUATION;
	m_move.z += (0.0f - m_move.z) * ATTENUATION;

	// �ړ��ʂɍ��킹�ĉ�]
	D3DXVECTOR2 horizontalAxis(m_move.x, m_move.z);

	D3DXVECTOR3 axis;	// ��]��
	D3DXVECTOR3 inverseVec = -m_move;					// move�l�𔽑΂ɂ���
	D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&axis, &inverseVec, &vecY);			// �O�ςŉ�]�����Z�o�B

	// �N�I�[�^�j�I���̌v�Z
	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationAxis(&quaternion, &axis, D3DXVec2Length(&horizontalAxis) * 0.035f);	// ��]���Ɖ�]�p�x���w��

	// �N�I�[�^�j�I����K�p
	m_quaternion *= quaternion;

	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	// ��]�s��ɔ��f
	SetMtxQuaternion(m_quaternion);
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
void CPlayer::Jump()
{
	if (!m_isMove)
	{
		return;
	}

	// ����
	if (CInput::GetKey()->Trigger(DIK_SPACE) && (m_jumpCount == 0))
	{
		m_jumpCount++;
		m_isJump = true;

		//jumpDirection.x *= 6.0f;
		//jumpDirection.z *= 6.0f;
		m_pos += 1.0f * jumpDirection;
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_move += JUMPING_POWER * jumpDirection;
	}
}

//-----------------------------------------------------------------------------
// �ːi
//-----------------------------------------------------------------------------
void CPlayer::boost()
{
	// �ːi
	if (CInput::GetKey()->Trigger(DIK_SPACE) && (m_jumpCount == 1))
	{
		m_jumpCount++;
		m_move.x *= 7.0f;
		m_move.z *= 7.0f;
	}
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
void CPlayer::Landing()
{
	m_move.y -= GRAVITY;	// �d��

	if (!OnHitPlain())
	{
		jumpDirection = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
}

//-----------------------------------------------------------------------------
// ���̃X�e�[�W�܂őҋ@������
//-----------------------------------------------------------------------------
void CPlayer::NextStageWait()
{
	if (!m_isGoal)
	{
		return;
	}
}

//-----------------------------------------------------------------------------
// Goal�Ƃ̓����蔻��
//-----------------------------------------------------------------------------
void CPlayer::OnHitGoal()
{
	// �w�肵���^�C�v��S�Č������Ďw�肵���֐����Ă�
	TypeAllFunc(CObject::EType::GOAL, CTaskGroup::EPriority::LEVEL_3D_1, [this](CObject* inObject)
	{
		if (OBBAndOBB((CObjectX*)inObject))
		{
			CGoal* goal = (CGoal*)inObject;	// Goal
			goal->Goal(true);
			m_isGoal = true;
		}
	});
}

//-----------------------------------------------------------------------------
// Enemy�Ƃ̓����蔻��
//-----------------------------------------------------------------------------
void CPlayer::OnHitEnemy()
{
	// �w�肵���^�C�v��S�Č������Ďw�肵���֐����Ă�
	TypeAllFunc(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1, [this](CObject* inObject)
	{
		if (m_isDied)
		{
			return;
		}

		if (OBBAndOBB((CObjectX*)inObject))
		{
			// ���S���o
			for (int i = 0; i < 50; i++)
			{
				D3DXVECTOR3 pos = m_pos;
				pos.x += FloatRandom(-20.0f, 20.0f);
				pos.y += FloatRandom(-20.0f, 20.0f);
				pos.z += FloatRandom(-20.0f, 20.0f);
				CPlayerDied::Create(pos);
			}

			SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//CApplication::GetInstance()->Delay(60, 2);
			m_isDied = true;	// Goal
		}
	});
}

//-----------------------------------------------------------------------------
// Plain�Ƃ̓����蔻��
//-----------------------------------------------------------------------------
bool CPlayer::OnHitPlain()
{
	bool hit = false;
	D3DXVECTOR3 dist(0.0f, 0.0f, 0.0f);

	// �w�肵���^�C�v��S�Č������Ďw�肵���֐����Ă�
	TypeAllFunc(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1, [this, &dist, &hit](CObject* inObject)
	{
		dist = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (SphereAndAABB((CObjectX*)inObject, &dist))
		{
			jumpDirection = -dist;
			jumpDirection.y += 2.0f;
			D3DXVec3Normalize(&jumpDirection, &jumpDirection);
			m_jumpCount = 0;
			//m_pos = m_posOld;

			if (dist.y < 0.0f)
			{
				m_move.y = 0.0f;
			}

			hit = true;
		}
	});

	return hit;
}
