//=============================================================================
//
// �}�E�X�J�[�\���̏�ɓ\��
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _MOUSE_EFFECT_H_	// ���̃}�N����`������ĂȂ�������
#define _MOUSE_EFFECT_H_	// ��d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object2d.h"

//=============================================================================
// �}�E�X�J�[�\���̏�ɓ\��object�̃N���X
//=============================================================================
class CMouseEffect : public CObject2d
{
public:
	static CMouseEffect *CMouseEffect::Create();

	CMouseEffect(CTaskGroup::EPriority list = CTaskGroup::EPriority::LEVEL_2D_UI);
	~CMouseEffect() override;

	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
private:
	int m_life;
};

#endif
