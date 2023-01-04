//=============================================================================
//
// �v���C���[�̎c���w�b�^�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _PLAYER_AFTERIMAGE_H_
#define _PLAYER_AFTERIMAGE_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "objectX.h"

class CPlayerAfterimage : public CObjectX
{
private: // �萔
	static const int MAX_LIFE;
public: // �p�u���b�N�֐�
	CPlayerAfterimage();
	~CPlayerAfterimage();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �X�V
	void Draw() override;			// �`��

	static CPlayerAfterimage* Create(const D3DXVECTOR3& inPos);	// ����

private: // �����o�[�ϐ�
	int m_life;	// ����
};
#endif
#pragma once
