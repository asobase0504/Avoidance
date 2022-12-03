//=============================================================================
//
// object_polygon3d
// Author : Hamada Ryuuga
// AUthor : Yuda Kaito
//
//=============================================================================
#ifndef _3DPOLYGON_H_	// ���̃}�N����`������ĂȂ�������
#define _3DPOLYGON_H_	// ��d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object2d.h"

//-----------------------------------------------------------------------------
// �}�N���錾
//-----------------------------------------------------------------------------
#define TIMER (m_time + 90)

//=============================================================================
// 3D�|���S���N���X
//=============================================================================
class  CObjectPolygon3D : public CObject
{
public:
	static const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	//���_���u3D�v�̍\���̂��`
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;//���_���W
		D3DXVECTOR3 nor;//�x�N�g��
		D3DCOLOR col;//�J���[
		D3DXVECTOR2 tex;//�e�N�X�`��
	};

protected:
	//polygon�̊�T�C�Y
	static const D3DXVECTOR3 m_Vtx[4];

public:

	static CObjectPolygon3D *Create(CTaskGroup::EPriority list = CTaskGroup::LEVEL_3D_1);

	CObjectPolygon3D(CTaskGroup::EPriority list = CTaskGroup::LEVEL_3D_1);
	~CObjectPolygon3D() override;
	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
	void Draw() override;

	// Setter
	void SetTex(PositionVec4 inTex);
	void SetPos(const D3DXVECTOR3& inPos) override;
	void SetRot(const D3DXVECTOR3& inRot) override;
	void SetSize(const D3DXVECTOR3& inSize) override;
	void SetColor(const D3DXCOLOR& inColor) override;

	LPDIRECT3DVERTEXBUFFER9 GetVtx() { return m_pVtxBuff; }	// ���_�o�b�t�@�̎擾

protected:
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// ���_�o�b�t�@
	D3DXMATRIX m_mtxWorld;				// ���[���h�}�g���b�N�X
};
#endif