//=============================================================================
//
// �J����
// Author : Hamada Ryuuga
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "application.h"

#include "camera.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera(CTaskGroup::EPriority inPriority) :
	CTask(inPriority,CTaskGroup::EPushMethod::PUSH_TOP)
{
	SetRole(ROLE_CAMERA);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// ������
//=============================================================================
HRESULT CCamera::Init(void)
{
	// ���_�@�����_�@������@�ݒ�
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CCamera::Update(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CCamera::Draw()
{
	Set(0);
}

//=============================================================================
// �`��
//=============================================================================
void CCamera::Set(int Type)
{
	m_Type = Type;
	LPDIRECT3DDEVICE9  pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �r���[�}�g���b�N�X��������
	D3DXMatrixIdentity(&m_MtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_MtxView, &m_posV, &m_posR, &m_vecU);

	// �K�p
	pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);

	// �v���W�F�N�V�����}�g���b�N�X��������
	D3DXMatrixIdentity(&m_MtxProject);

	if (Type == 0)
	{
		//�v���W�F�N�V�����}�g���b�N�X�쐬
		D3DXMatrixPerspectiveFovLH(&m_MtxProject,
			D3DXToRadian(90.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f,
			20000.0f);
	}
	else
	{
		// �v���W�F�N�V�����}�g���b�N�X�̍쐬(���s���e)
		D3DXMatrixOrthoLH(&m_MtxProject,					// �v���W�F�N�V�����}�g���b�N�X
			(float)SCREEN_WIDTH,							// ��
			(float)SCREEN_HEIGHT,							// ����
			-100.0f,										// �j�A
			2000.0f);										// �t�@�[
	}

	// �K�p
	pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProject);
}

//=============================================================================
// �J�����̌����ɍ��킹���x�N�g���ɕϊ�����
//=============================================================================
const D3DXVECTOR3 & CCamera::VectorCombinedRot(const D3DXVECTOR3& inVector)
{
	if (D3DXVec3Length(&inVector) <= 0.0f)
	{
		return inVector;
	}

	D3DXVECTOR3 vector = inVector;

	D3DXVec3Normalize(&vector, &vector);

	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXVec3TransformCoord(&vector, &vector, &mtxRot);

	vector.y = 0.0f;

	D3DXVec3Normalize(&vector, &vector);

	return vector;
}
