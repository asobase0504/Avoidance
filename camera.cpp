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
#include "input.h"

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
	m_posV = D3DXVECTOR3(0.0f, 50.0f, -100.0f);
	m_posR = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 distPos = m_posR - m_posV;
	m_fDistance = sqrtf(distPos.x * distPos.x + distPos.z * distPos.z);
	m_fDistance = sqrtf(distPos.y * distPos.y + (m_fDistance * m_fDistance));

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
			10000.0f);
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
// GetPos
//=============================================================================
const D3DXVECTOR3& CCamera::GetPos()
{
	return m_posV;
}

////=============================================================================
//// GetRot
////=============================================================================
//D3DXVECTOR3* CCamera::GetRot()
//{
//	return &m_rot;
//}

//=============================================================================
// �J�����̌����ɍ��킹���x�N�g���ɕϊ�����
//=============================================================================
const D3DXVECTOR3 & CCamera::VectorCombinedRot(const D3DXVECTOR3& inVector)
{
	if (D3DXVec3Length(&inVector) < 0.0f)
	{
		return inVector;
	}

	D3DXVECTOR3 vector = inVector;

	D3DXVec3Normalize(&vector, &vector);

	float c = cosf(-m_rot.y);
	float s = sinf(-m_rot.y);

	// move �̒����� 1 �ɂȂ�B
	vector.x = vector.x * c - vector.z * s;
	vector.z = vector.x * s + vector.z * c;

	return vector;
}

//=============================================================================
// MtxProje
//=============================================================================
D3DXMATRIX* CCamera::GetMtxProje()
{
	return &m_MtxProject;
}

//=============================================================================
// MtxView
//=============================================================================
D3DXMATRIX* CCamera::GetMtxView()
{
	return &m_MtxView;
}
