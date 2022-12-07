//=============================================================================
//
// �v���C���[�ݒ�w�b�^�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _GOAL_H_
#define _GOAL_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "motion.h"
#include "objectX.h"

class CGoal : public CObjectX
{
private: // �萔
	static const float SPEED;			// �X�s�[�h
	static const float ATTENUATION;		// �����W��
	static const float JUMPING_POWER;	// ������
	static const float GRAVITY;			// �d��

public: // �p�u���b�N�֐�
	CGoal();
	~CGoal();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void Draw() override;			// �`��

	static CGoal* Create();	// ����

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
};
#endif
