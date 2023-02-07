//=============================================================================
//
// �Q�[���œ������J����
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "camera_game.h"
#include "input.h"
#include "object.h"
#include "debug_proc.h"
#include "utility.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CCameraGame::CCameraGame(CTaskGroup::EPriority inPriority) : CCamera(inPriority)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CCameraGame::~CCameraGame()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CCameraGame::Init()
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
void CCameraGame::Uninit()
{
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CCameraGame::Update()
{
	D3DXVECTOR3 move(0.0f,0.0f,0.0f);

	Rotate();

	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -300.0f);

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
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);	// �s��|���Z�֐�

	// D3DXVECTOR3�ɔ��f
	D3DXVec3TransformCoord(&m_posV, &m_posV, &mtxWorld);
	D3DXVec3TransformCoord(&m_posR, &m_posR, &mtxWorld);

	if (m_posV.x <= -380.0f)
	{
		m_posV.x = -380.0f;
	}
	if (m_posV.x >= 380.0f)
	{
		m_posV.x = 380.0f;
	}
	if (m_posV.z <= -380.0f)
	{
		m_posV.z = -380.0f;
	}
	if (m_posV.z >= 380.0f)
	{
		m_posV.z = 380.0f;
	}
	if (m_posV.y <= 5.0f)
	{
		m_posV.y = 5.0f;
	}
	if (m_posV.y >= 780.0f)
	{
		m_posV.y = 780.0f;
	}
}

//-----------------------------------------------------------------------------
// ��]
//-----------------------------------------------------------------------------
void CCameraGame::Rotate()
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

	if (m_rot.x < -D3DX_PI * 0.5f + 0.1f)
	{// ������-D3DX_PI�����̎�
		m_rot.x = -D3DX_PI * 0.5f + 0.1f;
	}
	if (m_rot.x > D3DX_PI * 0.5f - 0.1f)
	{// ������D3DX_PI�ȏ�̎�
		m_rot.x = D3DX_PI * 0.5f - 0.1f;
	}
}
