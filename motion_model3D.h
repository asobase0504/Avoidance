//=============================================================================
//
// 3D���[�V�����L�����N�^�[�N���X(motion_model3D.h)
// Author : �������l
// Author : Yuda Kaito
// �T�v : 3D�v���C���[�������s��
//
//=============================================================================
#ifndef _MOTION_MODEL3D_H_			// ���̃}�N����`������ĂȂ�������
#define _MOTION_MODEL3D_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "objectX.h"
#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CMotion;

//=============================================================================
// 3D�v���C���[�N���X
// Author : �������l
// �T�v : 3D�v���C���[�������s���N���X
//=============================================================================
class CMotionModel3D : public CObjectX
{
public:
	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CMotionModel3D *Create();			// 3D�v���C���[�̐���

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CMotionModel3D(CTaskGroup::EPriority nPriority = CTaskGroup::LEVEL_3D_1);
	~CMotionModel3D();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void NormalUpdate() override;		// �X�V
	void Draw() override;		// �`��

	// Setter
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }			// �ߋ��ʒu
	void SetMotion(const char *pName);									// ���[�V�������̐ݒ�
	void SetUpdateStop(bool isStop) { m_isUpdateStop = isStop; }
	void SetMaxMinVtx();

	// Getter
	D3DXVECTOR3 GetPosOld() { return m_posOld; }		// �ߋ��ʒu
	CMotion *GetMotion() { return m_pMotion; }			// ���[�V�������̎擾

	bool SegmentCollision(CObjectX* inObjectX);
	float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0);

private:
	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	CMotion			*m_pMotion;			// ���[�V�����C���X�^���X
	D3DXVECTOR3		m_posOld;			// �ߋ��ʒu
	bool			m_isUpdateStop;		// �X�V���~������B
};

#endif