//=============================================================================
//
// �v���C���[�̎��S�G�t�F�N�g
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _PLAYER_DIED_H_
#define _PLAYER_DIED_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "objectX.h"

//-----------------------------------------------------------------------------
// �v���C���[�̎��S�G�t�F�N�g
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
class CPlayerDied : public CObjectX
{
public: // �萔
	static const unsigned int MAX_LIFE;			// ��������
	static const unsigned int AGGREGATE_TIME;	// �W�����鎞��
	static const float SPATTER_SPEED;			// ��юU�鑬�x
	static const float ROT_RANDOM_LIMIT;		// ���Z�p�x�̏��
	static const float GRAVITY;					// �d��

public: // �p�u���b�N�֐�
	CPlayerDied();
	~CPlayerDied();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �X�V
	void Draw() override;			// �`��

	static CPlayerDied* Create(const D3DXVECTOR3& inPos);	// ����
	static void SetOriginPos(const D3DXVECTOR3& inPos) { m_posOrigin = inPos; }

private: // �����o�[�֐�
	bool OnHitPlain();	// Polygon�Ƃ̓����蔻��

private: // �����o�[�ϐ�
	int m_life;	// ����
	bool m_hit;
	static D3DXVECTOR3 m_posOrigin;	// �����ʒu
	D3DXVECTOR3 m_scaleOrigin;	// �����ʒu
};
#endif
#pragma once
