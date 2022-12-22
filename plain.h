//=============================================================================
//
// �v���C���[�ݒ�w�b�^�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _PLAIN_H_
#define _PLAIN_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "motion.h"
#include "objectX.h"

class CPlain : public CObjectX
{
public: // �p�u���b�N�֐�
	CPlain();
	~CPlain();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �X�V
	void Draw() override;			// �`��

	static CPlain* Create();	// ����

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	int m_endCnt;
	bool begin;
};
#endif
