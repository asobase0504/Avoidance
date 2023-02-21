//=============================================================================
//
// �G�l�~�[�ݒ�w�b�_�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "motion.h"
#include "objectX.h"

//-----------------------------------------------------------------------------
// �G�l�~�[�N���X
//-----------------------------------------------------------------------------
class CEnemy : public CObjectX
{
public: // �p�u���b�N�֐�
	CEnemy();
	~CEnemy();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void PopUpdate() override;		// �X�V
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �X�V

	static CEnemy* Create();	// ����
	bool OnHitPlain();	// Plain�Ƃ̓����蔻��

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
};
#endif
