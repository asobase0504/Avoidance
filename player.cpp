//============================
//
// �v���C���[�ݒ�
// Author:Yuda Kaito
//
//============================
#include <assert.h>
#include "player.h"
#include "input.h"
#include "camera.h"
#include "application.h"
#include "particle_manager.h"
#include "utility.h"

//------------------------------------
// static�ϐ�
//------------------------------------
const float CPlayer::SPEED = 7.0f;			// �ړ���
const float CPlayer::ATTENUATION = 0.5f;	// �ړ������W��
const float CPlayer::JUMPING_POWER = 10.0f;	// ������
const float CPlayer::GRAVITY = 0.75f;		// �d��

//------------------------------------
// �R���X�g���N�^
//------------------------------------
CPlayer::CPlayer() :
	m_quaternion(D3DXQUATERNION(0.0f,0.0,0.0f,1.0f)),
	m_jumpCount(0)
{
}

//------------------------------------
// �f�X�g���N�^
//------------------------------------
CPlayer::~CPlayer()
{
}

//------------------------------------
// ������
//------------------------------------
HRESULT CPlayer::Init()
{
	// ���݂̃��[�V�����ԍ��̕ۊ�
	CObjectX::Init();
	LoadModel("BOX");
	return S_OK;
}

//------------------------------------
// �I��
//------------------------------------
void CPlayer::Uninit()
{
	CObjectX::Uninit();
}

//------------------------------------
// �X�V
//------------------------------------
void CPlayer::NormalUpdate()
{
	Move();	// �ړ�
	Jump();	// �W�����v

	CInput* input = CInput::GetKey();

	static D3DXCOLOR color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	if (input->Trigger(DIK_0))
	{
		color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		SetMaterialDiffuse(0, color);
	}
	if (input->Trigger(DIK_1))
	{
		color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		SetMaterialDiffuse(0, color);
	}
	if (input->Trigger(DIK_2))
	{
		color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		SetMaterialDiffuse(0, color);
	}
	if (input->Trigger(DIK_3))
	{
		color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		SetMaterialDiffuse(0, color);
	}
	if (input->Press(DIK_4))
	{
		color.r += 0.01f;
		SetMaterialDiffuse(0, color);
	}
	if (input->Press(DIK_5))
	{
		color.r -= 0.01f;
		SetMaterialDiffuse(0, color);
	}
}

//------------------------------------
// �`��
//------------------------------------
void CPlayer::Draw()
{
	CObjectX::Draw(m_quaternion);
}

//------------------------------------
// create
//------------------------------------
CPlayer *CPlayer::Create()
{
	CPlayer * player = new CPlayer;

	if (player != nullptr)
	{
		player->Init();
	}

	return player;
}

//------------------------------------
// �����Z�b�g
//------------------------------------
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

	CCamera* camera = (CCamera*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(0, CTaskGroup::LEVEL_3D_1);
	if (camera != nullptr)
	{
		moveInput = camera->VectorCombinedRot(moveInput);
	}

	m_move.x = moveInput.x * SPEED;
	m_move.z = moveInput.z * SPEED;

	//�i�ړI�̒l-���݂̒l�j�������W��
	m_move.x += (0.0f - m_move.x) * ATTENUATION;
	m_move.z += (0.0f - m_move.z) * ATTENUATION;

	D3DXVECTOR3 axis;	// ��]��
	D3DXVECTOR3 inverseVec = -m_move;					// move�l�𔽑΂ɂ���
	D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&axis, &inverseVec, &vecY);			// �O�ςŉ�]�����Z�o�B

	// �N�I�[�^�j�I���̌v�Z
	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationAxis(&quaternion, &axis, 0.15f);	// ��]���Ɖ�]�p�x���w��

	// �N�I�[�^�j�I����K�p
	m_quaternion *= quaternion;

	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
}

void CPlayer::Jump()
{
	// ����
	if (CInput::GetKey()->Trigger(DIK_SPACE) && (m_jumpCount < 2))
	{
		m_jumpCount++;
		m_move.y = 10.0f;
	}

	// �d��
	m_move.y -= 0.75f;

	// �^���I�ȏ��\��
	if (m_pos.y + m_move.y <= 15.0f)
	{
		m_jumpCount = 0;
 		m_move.y = 0.0f;
	}
}
