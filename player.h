//============================
//
// �v���C���[�ݒ�w�b�^�[
// Author:hamada ryuuga
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

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CMotion;

class CPlayer : public CObjectX
{
public:
	static const int MAXLIFE = 300;
	static const float ATTENUATION;		// �����W��
	static const float SPEED;			// �X�s�[�h
	static const float WIDTH;			// ���f���̔��a
	static const int MAX_PRAYER;		// �ő吔
	static const int MAX_MOVE;			// �A�j���[�V�����̍ő吔
	static const int INVINCIBLE;		// ���G����
	static const int MAX_MODELPARTS = 9;
	
public:
	CPlayer();
	~CPlayer();

	HRESULT Init()override;			// ������
	void Uninit()override;			// �j��
	void NormalUpdate()override;	// �X�V
	void Draw()override;			// �`��

	static CPlayer *Create();

private:
	
	void Move();		// �ړ�

	D3DXQUATERNION m_quaternion;
	float m_MoveSpeed;
private:
};
#endif
