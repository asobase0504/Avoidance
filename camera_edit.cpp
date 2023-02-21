//=============================================================================
//
// Edit�œ������J����
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "camera_edit.h"
#include "input.h"
#include "object.h"
#include "objectX.h"
#include "debug_proc.h"
#include "utility.h"
#include "light.h"
#include "application.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CCameraEdit::CCameraEdit(CTaskGroup::EPriority inPriority) : CCamera(inPriority)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CCameraEdit::~CCameraEdit()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CCameraEdit::Init()
{
	CCamera::Init();
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -200.0f);

	D3DXVECTOR3 distPos = m_posR - m_posV;
	m_fDistance = sqrtf(distPos.x * distPos.x + distPos.z * distPos.z);
	m_fDistance = sqrtf(distPos.y * distPos.y + (m_fDistance * m_fDistance));

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CCameraEdit::Uninit()
{
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CCameraEdit::Update()
{
	Rotate();

	m_posR = D3DXVECTOR3(0.0f, 15.0f, 30.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -250.0f);

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot, mtxTrans;

	D3DXMatrixIdentity(&mtxWorld);	// �s�񏉊����֐�

									// �����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// �s���]�֐�
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);						// �s��|���Z�֐� 

	// �ʒu�𔽉f
	D3DXVECTOR3 pos = CObject::SearchType(CObject::EType::PLAYER, CTaskGroup::EPriority::LEVEL_3D_1)->GetPos();	// �v���C���[������
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);	// �s��ړ��֐�
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);	// �s��|���Z�֐�as

	// D3DXVECTOR3�ɔ��f
	D3DXVec3TransformCoord(&m_posV, &m_posV, &mtxWorld);
	D3DXVec3TransformCoord(&m_posR, &m_posR, &mtxWorld);

	D3DXVECTOR3 dist = m_posR - m_posV;
	D3DXVec3Normalize(&dist, &dist);

	D3DXVECTOR3 outPos;

	// �J�����ƒn�ʂ̓����蔻��
	CObject::TypeAllFunc(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1, [&dist, this, &pos, &outPos](CObject* inObject)
	{
		CObjectX* objectX = (CObjectX*)inObject;
		if (objectX->SegmentAndAABB(m_posV, pos, &outPos))
		{
			CDebugProc::Print("outPos : %f,%f,%f\n", outPos.x, outPos.y, outPos.z);
			m_posV = outPos + dist * 20.0f;
			objectX->AttachOutLine(false);
		}
		else
		{
			objectX->AttachOutLine();
		}
	});

	CDebugProc::Print("PosR : %f,%f,%f\n", m_posR.x, m_posR.y, m_posR.z);
	CDebugProc::Print("PosV : %f,%f,%f\n", m_posV.x, m_posV.y, m_posV.z);
}

//-----------------------------------------------------------------------------
// ��]
//-----------------------------------------------------------------------------
void CCameraEdit::Rotate()
{
	// ���͏��̎擾
	const float MIN_MOUSE_MOVED = 0.01f;	// �f�b�h�]�[��

	D3DXVECTOR3 rollDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 cursorMove = CInput::GetKey()->GetMouse()->GetMouseCursorMove();
	D3DXVECTOR3 joyPadMove = CInput::GetKey()->VectorMoveJoyStick(true);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CDebugProc::Print("CursorMove : %.2f,%.2f,%.2f\n", cursorMove.x, cursorMove.y, cursorMove.z);
	CDebugProc::Print("JoyPadMove : %.2f,%.2f,%.2f\n", joyPadMove.x, joyPadMove.y, joyPadMove.z);

	if (D3DXVec3Length(&joyPadMove) == 0.0f)
	{ // JoyPad�̓��͂��Ȃ������ꍇ�J�[�\���̈ړ��ʂ���
		move = D3DXVECTOR3(cursorMove.y, cursorMove.x, cursorMove.z) * 0.1f;
	}
	else
	{ // JoyPad�̓��͂��������ꍇ
		move = D3DXVECTOR3(joyPadMove.y, joyPadMove.x, joyPadMove.z) * 1.5f;
	}

	if (D3DXVec3Length(&move) > MIN_MOUSE_MOVED || D3DXVec3Length(&move) < -MIN_MOUSE_MOVED)
	{
		// �}�E�X�̈ړ������̃m�[�}���C�Y
		//D3DXVec3Normalize(&move, &move);

		// �ړ������̎Z�o
		rollDir += move * (D3DX_PI / 180.0f);
	}

	// ��]
	m_rot += rollDir * 2.0f;
	m_rot.z = 0.0f;

	CDebugProc::Print("m_rot : %.2f,%.2f,%.2f\n", m_rot.x, m_rot.y, m_rot.z);

	// ���K��
	NormalizeAngle(m_rot.y);

	if (m_rot.x < -D3DX_PI * 0.5f + 0.75f)
	{// ������-D3DX_PI�����̎�
		m_rot.x = -D3DX_PI * 0.5f + 0.75f;
	}
	if (m_rot.x > D3DX_PI * 0.5f - 0.75f)
	{// ������D3DX_PI�ȏ�̎�
		m_rot.x = D3DX_PI * 0.5f - 0.75f;
	}
}
