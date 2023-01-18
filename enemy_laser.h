//=============================================================================
//
// �G�l�~�[(������ɂ�����)�ݒ�w�b�_�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _ENEMY_LASER_H_
#define _ENEMY_LASER_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "enemy.h"

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CLine;

//-----------------------------------------------------------------------------
// �G�l�~�[�N���X
//-----------------------------------------------------------------------------
class CEnemyLaser : public CEnemy
{
private: // �萔
	static const D3DXVECTOR3 SCALE;
	static const D3DXVECTOR3 MOVE_POWER;
	static const int MOVE_START_TIME;

public: // �p�u���b�N�֐�
	CEnemyLaser();
	~CEnemyLaser();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void PopUpdate() override;		// �X�V
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �I���X�V
	void Draw() override;	// �X�V

	void SetMove(const D3DXVECTOR3& inMove) override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�

	int m_startCnt;
	float m_moveScale;
	CLine* m_guideLine;
};
#endif