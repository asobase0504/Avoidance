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
public:
	static const float ATTENUATION;		// �����W��
	static const float SPEED;			// �X�s�[�h
	
public:
	CPlayer();
	~CPlayer();

	HRESULT Init()override;			// ������
	void Uninit()override;			// �j��
	void NormalUpdate()override;	// �X�V
	void Draw()override;			// �`��

	static CPlayer *Create();

private:
	
	void Move();	// �ړ�
	void Jump();	// ����

	D3DXQUATERNION m_quaternion;
	float m_MoveSpeed;
private:
};
#endif
