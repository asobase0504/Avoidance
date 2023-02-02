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

class CPlayerDied : public CObjectX
{
public: // �萔
	static const int MAX_LIFE;
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
	bool OnHitPlain();	// Polygon�Ƃ̓����蔻��

private: // �����o�[�ϐ�
	int m_life;	// ����
	bool m_hit;
	static D3DXVECTOR3 m_posOrigin;	// �����ʒu
};
#endif
#pragma once
