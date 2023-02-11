//=============================================================================
//
// �v���C���[�Ǐ]�̓G
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _ENEMY_HOMING_H_
#define _ENEMY_HOMING_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "enemy.h"

//-----------------------------------------------------------------------------
// �G�l�~�[�N���X
//-----------------------------------------------------------------------------
class CEnemyHoming : public CEnemy
{
private: // �萔
	static const D3DXVECTOR3 SCALE;
	static const D3DXVECTOR3 MOVE_POWER;
	static const int MOVE_START_TIME;
	static const int MOVE_END_TIME;

public: // �p�u���b�N�֐�
	CEnemyHoming();
	~CEnemyHoming();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void PopUpdate() override;		// �X�V
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �I���X�V
	void Draw() override;	// �X�V

	void SeeTarget();

	void SetMove(const D3DXVECTOR3& inMove) override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
};
#endif
