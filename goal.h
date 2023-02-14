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

public: // �p�u���b�N�֐�
	CGoal();
	~CGoal();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �X�V
	void Draw() override;			// �`��

	static CGoal* Create();	// ����

	void Goal(bool goal) { m_isGoal = goal; }
	bool IsGoal() { return m_isGoal; }

	void SetTime(int inTime);
	int GetTime() { return m_goalTime; }

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	int m_endCnt;
	bool m_isGoal;
	int m_goalTime;
};
#endif
