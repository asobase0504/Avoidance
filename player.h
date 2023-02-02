//=============================================================================
//
// �v���C���[�ݒ�w�b�^�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "motion.h"
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

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void Draw() override;			// �`��

	static CPlayer* Create();	// ����

	bool GetIsGoal() { return m_isGoal; }
	void SetIsMove(bool isMove) { m_isMove = isMove; }
	bool IsMove() { return m_isMove; }

private: // �v���C�x�[�g�֐�
	void Move();	// �ړ�
	void Jump();	// ����
	void boost();	// �ːi
	void Landing();	// ����
	void OnHitGoal();	// Goal�Ƃ̓����蔻��
	void OnHitEnemy();	// Enemy�Ƃ̓����蔻��
	bool OnHitPlain();	// Polygon�Ƃ̓����蔻��

private: // �����o�[�ϐ�

	D3DXQUATERNION m_quaternion;	// �N�I�[�^�j�I��
	D3DXQUATERNION m_quaternionOld;	// �O��N�I�[�^�j�I��
	int m_jumpCount;				// �W�����v��
	bool m_isGoal;					// Goal������
	bool m_isMove;					// �����Ă�����
	bool m_isJump;
	int m_jumpTime;

	D3DXVECTOR3 jumpDirection;				// �W�����v����
};
#endif
