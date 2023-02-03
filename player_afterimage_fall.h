//=============================================================================
//
// �v���C���[�̎c���w�b�^�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _PLAYER_AFTERIMAGE_FALL_H_
#define _PLAYER_AFTERIMAGE_FALL_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "objectX.h"

class CPlayerAfterimageFall : public CObjectX
{
private: // �萔
	static const int MAX_LIFE;
public: // �p�u���b�N�֐�
	CPlayerAfterimageFall();
	~CPlayerAfterimageFall();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �X�V
	void Draw() override;			// �`��

	static CPlayerAfterimageFall* Create(const D3DXVECTOR3& inPos);	// ����

private: // �����o�[�ϐ�
	int m_life;	// ����
};
#endif
#pragma once
