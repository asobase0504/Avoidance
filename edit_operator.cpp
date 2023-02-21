//=============================================================================
//
// �v���C���[�ݒ�
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "edit_operator.h"

#include "input.h"
#include "camera.h"
#include "application.h"
#include "utility.h"
#include "collision.h"
#include "goal.h"
#include "color.h"
#include "delay_process.h"

//-----------------------------------------------------------------------------
// �萔
//-----------------------------------------------------------------------------
const float CEditOperator::MAX_SPEED = 5.25f;		// �ړ���
const float CEditOperator::SPEED = 5.25f;			// �ړ���
const float CEditOperator::ATTENUATION = 0.5f;		// �ړ������W��
const float CEditOperator::JUMPING_POWER = 7.5f;	// ������
const float CEditOperator::GRAVITY = 0.175f;		// �d��

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CEditOperator::CEditOperator()
{
	SetType(CObject::EType::PLAYER);
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CEditOperator::~CEditOperator()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CEditOperator::Init()
{
	LoadModel("BOX");

	CObjectX::Init();

	AttachOutLine();	// �A�E�g���C���𒅂���
	AttachShadow();		// �e�𒅂���

	m_isEditEnemy = false;
	m_isEditBlock = true;

	SetColorAlpha(0.75f);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CEditOperator::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CEditOperator::NormalUpdate()
{
#ifdef _DEBUG

#if 1	// �f�o�b�O�\��
	CDebugProc::Print("----------------------------------------------------------------\n");
	CDebugProc::Print("pos  : %.2f,%.2f,%.2f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("move : %.2f,%.2f,%.2f\n", m_move.x, m_move.y, m_move.z);
	CDebugProc::Print("rot : %.2f,%.2f,%.2f\n", m_rot.x, m_rot.y, m_rot.z);
	CDebugProc::Print("scale : %.2f,%.2f,%.2f\n", m_rot.x, m_rot.y, m_rot.z);
	CDebugProc::Print("----------------------------------------------------------------\n");
#endif

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

#endif // _DEBUG

	switch (m_editType)
	{
	case CStageEdit::NEW_ENEMY:
		break;
	case CStageEdit::NEW_BLOCK:
		break;
	case CStageEdit::RE_GOAL:
		TypeAllFunc(CObject::EType::GOAL, CTaskGroup::EPriority::LEVEL_3D_1, [this](CObject* inObject)
		{
			inObject->SetPos(GetPos());
		});
		break;
	case CStageEdit::RE_ENEMY:
		break;
	case CStageEdit::RE_BLOCK:
		break;
	case CStageEdit::FREE:
		break;
	default:
		break;
	}

	Move();			// �ړ�
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CEditOperator::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
CEditOperator* CEditOperator::Create()
{
	CEditOperator* player = new CEditOperator;

	if (player != nullptr)
	{
		player->Init();
	}

	return player;
}

//-----------------------------------------------------------------------------
// �ړ�
//-----------------------------------------------------------------------------
void CEditOperator::Move()
{
	CInput* input = CInput::GetKey();
	D3DXVECTOR3 moveInput = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	if (input->Press(DIK_SPACE))
	{
		m_pos.y += 2.0f;
	}
	if (input->Press(DIK_LSHIFT))
	{
		m_pos.y -= 2.0f;
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
		if (fabs(m_move.x) < fabs(moveInput.x * MAX_SPEED))
		{
			m_move.x += moveInput.x * SPEED;

			if (m_move.x > MAX_SPEED)
			{
				m_move.x = MAX_SPEED;
			}
			else if (m_move.x < -MAX_SPEED)
			{
				m_move.x = -MAX_SPEED;
			}
		}

		if (fabs(m_move.z) < fabs(moveInput.z * MAX_SPEED))
		{
			m_move.z += moveInput.z * SPEED;

			if (m_move.z > MAX_SPEED)
			{
				m_move.z = MAX_SPEED;
			}
			else if (m_move.z < -MAX_SPEED)
			{
				m_move.z = -MAX_SPEED;
			}
		}
	}

	//�i�ړI�̒l - ���݂̒l�j �� �����W��
	m_move.x += (0.0f - m_move.x) * ATTENUATION;
	m_move.z += (0.0f - m_move.z) * ATTENUATION;
}
