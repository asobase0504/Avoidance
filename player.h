//============================
//
// �v���C���[�ݒ�w�b�^�[
// Author:Yuda Kaito
//
//============================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "main.h"
#include "motion.h"
#include "renderer.h"
#include "objectX.h"

class CPlayer : public CObjectX
{
private: // �萔
	static const float SPEED;			// �X�s�[�h
	static const float ATTENUATION;		// �����W��
	static const float JUMPING_POWER;	// ������
	static const float GRAVITY;			// �d��

public: // �p�u���b�N�֐�
	CPlayer();
	~CPlayer();

	HRESULT Init()override;			// ������
	void Uninit()override;			// �j��
	void NormalUpdate()override;	// �X�V
	void Draw()override;			// �`��

	static CPlayer *Create();

private: // �v���C�x�[�g�֐�
	void Move();	// �ړ�
	void Jump();	// ����

private: // �����o�[�ϐ�

	D3DXQUATERNION m_quaternion;	// �N�I�[�^�j�I��
	int m_jumpCount;				// �W�����v��
};
#endif
