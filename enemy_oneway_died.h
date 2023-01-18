//=============================================================================
//
// �v���C���[�̎c���w�b�^�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _ENEMY_ONEWAY_DIED_H_
#define _ENEMY_ONEWAY_DIED_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "objectX.h"

class CEnemyOneWayDied : public CObjectX
{
private: // �萔
	static const int MAX_LIFE;
	static const float ALPHA_COLOR;
public: // �p�u���b�N�֐�
	CEnemyOneWayDied();
	~CEnemyOneWayDied();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �X�V
	void Draw() override;			// �`��

	static CEnemyOneWayDied* Create(const D3DXVECTOR3& inPos);	// ����

private: // �����o�[�ϐ�
	int m_life;	// ����
};
#endif
#pragma once
