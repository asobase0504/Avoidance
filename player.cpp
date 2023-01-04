//=============================================================================
//
// �v���C���[�ݒ�
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "player.h"
#include "input.h"
#include "camera.h"
#include "application.h"
#include "particle_manager.h"
#include "utility.h"
#include "collision.h"
#include "object_polygon3d.h"
#include "player_afterimage.h"
#include "goal.h"

#include "color.h"
//-----------------------------------------------------------------------------
// �萔
//-----------------------------------------------------------------------------
const float CPlayer::SPEED = 6.5f;			// �ړ���
const float CPlayer::ATTENUATION = 0.25f;	// �ړ������W��
const float CPlayer::JUMPING_POWER = 1.5f;	// ������
const float CPlayer::GRAVITY = 0.75f;		// �d��

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CPlayer::CPlayer() :
	m_quaternion(D3DXQUATERNION(0.0f,0.0,0.0f,1.0f)),
	m_quaternionOld(D3DXQUATERNION(0.0f, 0.0, 0.0f, 1.0f)),
	m_jumpCount(0),
	m_isGoal(false)
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
	// ���݂̃��[�V�����ԍ��̕ۊ�
	CObjectX::Init();
	LoadModel("BOX");
	SetScale(D3DXVECTOR3(1.0f,1.0f,1.0f));
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
	m_quaternionOld = m_quaternion;
	Move();		// �ړ�
	boost();	// �ːi
	Jump();		// �W�����v
	//Landing();	// ����
	OnHitGoal();	// Goal�Ƃ̓����蔻��
	OnHitEnemy();

	CInput* input = CInput::GetKey();

	if (input->Press(DIK_0))
	{
		SetPos(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	if (input->Press(DIK_9))
	{
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (m_pos.y < -5000.0f)
	{
		SetPos(D3DXVECTOR3(m_pos.x, 1200.0f, m_pos.z));
	}

	CDebugProc::Print("quaternion  : %.2f,%.2f,%.2f\n", m_quaternion.x, m_quaternion.y, m_quaternion.z);
	CDebugProc::Print("quaternionOld  : %.2f,%.2f,%.2f\n", m_quaternionOld.x, m_quaternionOld.y, m_quaternionOld.z);
	CDebugProc::Print("pos  : %.2f,%.2f,%.2f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("move : %.2f,%.2f,%.2f\n", m_move.x, m_move.y, m_move.z);

	static int time = 0;
	time++;
	if (time % 1 == 0)
	{
		CPlayerAfterimage* afterimage = CPlayerAfterimage::Create(m_pos);
		afterimage->SetMtxQuaternion(m_quaternion);
		afterimage->SetMaterialDiffuse(0,GetMaterialDiffuse(0));
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CPlayer::Draw()
{
	CObjectX::Draw();
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
	CInput* input = CInput::GetKey();
	D3DXVECTOR3 moveInput = D3DXVECTOR3(0.0f,0.0f,0.0f);

	if (input->Press(KEY_RIGHT))
	{
		moveInput.x += 1.0f;
	}
	if (input->Press(KEY_LEFT))
	{
		moveInput.x -= 1.0f;
	}
	if (input->Press(KEY_DOWN))
	{	
		moveInput.z -= 1.0f;
	}
	if (input->Press(KEY_UP))
	{	
		moveInput.z += 1.0f;
	}

	CCamera* camera = (CCamera*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(ROLE_CAMERA, CTaskGroup::LEVEL_3D_1);

	if (camera != nullptr)
	{
		moveInput = camera->VectorCombinedRot(moveInput);
	}

	if (D3DXVec3Length(&moveInput) != 0.0f)
	{
		m_move.x = moveInput.x * SPEED;
		m_move.z = moveInput.z * SPEED;
	}

	//�i�ړI�̒l - ���݂̒l�j �� �����W��
	m_move.x += (0.0f - m_move.x) * ATTENUATION;
	m_move.z += (0.0f - m_move.z) * ATTENUATION;

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

	SetMtxQuaternion(m_quaternion);
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
void CPlayer::Jump()
{
	// ����
	if (CInput::GetKey()->Trigger(DIK_SPACE) && (m_jumpCount == 0))
	{
		m_jumpCount++;
		m_move.y = 15.0f;
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

		m_move.y *= 2.0f;

		m_move.x *= 7.0f;
		m_move.z *= 7.0f;
	}
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
void CPlayer::Landing()
{
	if (OnHitPolygon())
	{
		//m_pos.y = m_posOld.y;
	}
	else
	{
		m_move.y -= GRAVITY;	// �d��
	}
}

//-----------------------------------------------------------------------------
// Goal�Ƃ̓����蔻��
//-----------------------------------------------------------------------------
void CPlayer::OnHitGoal()
{
	CObject* object = SearchType(CObject::EType::GOAL, CTaskGroup::EPriority::LEVEL_3D_1);

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();
		
		if (OBBAndOBB((CObjectX*)object))
		{
			CGoal* goal = (CGoal*)object;	// Goal
			goal->Goal(true);
		}

		object = next;
	}
}

//-----------------------------------------------------------------------------
// Enemy�Ƃ̓����蔻��
//-----------------------------------------------------------------------------
void CPlayer::OnHitEnemy()
{
	CObject* object = SearchType(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1);

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();

		if (OBBAndOBB((CObjectX*)object))
		{
			//m_isGoal = true;	// Goal
		}

		object = next;
	}
}

//-----------------------------------------------------------------------------
// Polygon�Ƃ̓����蔻��
//-----------------------------------------------------------------------------
bool CPlayer::OnHitPolygon()
{
	// �ŏ��Ɍ������w�肵���^�C�v��object�������Ă���
	CObject* object = SearchType(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1);

	bool hit = false;

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();	// �����^�C�v��object�������Ă���

		CObjectX* objectX = (CObjectX*)object;	// �ϊ�

		if (OBBAndOBB(objectX))
		{
			D3DXVECTOR3 move = m_posOld - m_pos;
			CDebugProc::Print("dist : %.2f,%.2f,%.2f\n", move.x, move.y, move.z);

			if (D3DXVec3Length(&(m_posOld - m_pos)) == 0.0f)
			{
				AddMove(m_move);
			}
			else
			{
				AddMove(m_posOld - m_pos);
			}

			AddPos(GetMove());
			SetMtxQuaternion(m_quaternionOld);

			if (m_move.y < -20.0f)
			{
				m_move.y = 0.0f;
			}
			hit = true;
		}
		object = next;
	}
	return hit;
}
