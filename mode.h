//=============================================================================
//
// ���[�h�N���X�̃w�b�_�[
// Author:Hamada Ryuuga
//
//=============================================================================
#ifndef _MODE_H_		//���̃}�N������`����ĂȂ�������
#define _MODE_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "task.h"

//=============================================================================
// �Q�[���N���X
//=============================================================================
class CMode : public CTask
{
public:
	CMode() : CTask(CTaskGroup::LEVEL_SYSTEM, CTaskGroup::EPushMethod::PUSH_CURRENT) {}
	~CMode() override {}

	virtual HRESULT Init() override = 0;
	virtual void Uninit() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;

private:
};
#endif