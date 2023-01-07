//=============================================================================
//
// �}�E�X�J�[�\���̏�ɓ\��
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _MOUSE_OBJECT_H_	// ���̃}�N����`������ĂȂ�������
#define _MOUSE_OBJECT_H_	// ��d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object2d.h"

//=============================================================================
// �}�E�X�J�[�\���̏�ɓ\��object�̃N���X
//=============================================================================
class CMouseObject : public CObject2d
{
public:
	static CMouseObject *CMouseObject::Create();

	CMouseObject(CTaskGroup::EPriority list = CTaskGroup::EPriority::LEVEL_2D_UI);
	~CMouseObject() override;

	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
private:
};

#endif
