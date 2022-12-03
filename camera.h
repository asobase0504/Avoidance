//=============================================================================
//
// �J����
// Author : Hamada Ryuuga
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "renderer.h"
#include "task.h"

//=============================================================================
// �t�F�[�h�N���X
//=============================================================================
class CCamera : public CTask
{

public:
	CCamera(CTaskGroup::EPriority inPriority = CTaskGroup::LEVEL_3D_1);
	~CCamera() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Set(int Type);

	// Getter
	D3DXMATRIX* GetMtxProje();
	D3DXMATRIX* GetMtxView();
	const D3DXVECTOR3& GetPos();
	//D3DXVECTOR3* GetRot();

	const D3DXVECTOR3& VectorCombinedRot(const D3DXVECTOR3& inVector);

private:
	D3DXVECTOR3 m_posV;			// �ʒu
	D3DXVECTOR3 m_posVDest;		// �ړI�n����̍�
	D3DXVECTOR3 m_posR;			// �����_
	D3DXVECTOR3 m_posRDest;		// �ړI�n����̍�
	D3DXVECTOR3 m_vecU;			// �x�N�g��
	D3DXVECTOR3 m_rot;			// �ʒu��]
	D3DVIEWPORT9 m_viewport;	//
	D3DXMATRIX m_MtxProject;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_MtxView;		// �r���[�}�g���b�N�X
	float m_fDistance;			// ����
	int m_Type;
};

#endif		// _CAMERA_H_