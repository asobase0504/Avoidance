//=============================================================================
//
// �S�[���̉��o
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _GOAL_EFFECT_H_
#define _GOAL_EFFECT_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "objectX.h"

//-----------------------------------------------------------------------------
// �G�l�~�[�N���X
//-----------------------------------------------------------------------------
class CGoalEffect : public CObjectX
{
private: // �萔
	static const D3DXVECTOR3 SCALE;
	static const D3DXVECTOR3 MOVE_POWER;

public: // �p�u���b�N�֐�
	CGoalEffect();
	~CGoalEffect();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void Draw() override;			// �`��

	void SetMove(const D3DXVECTOR3& inMove) override;

	static CGoalEffect* Create(const D3DXVECTOR3& inPos);

private: // �v���C�x�[�g�֐�
private: // �����o�[�ϐ�
	int m_life;
};
#endif
