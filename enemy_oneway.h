//=============================================================================
//
// �G�l�~�[(������ɂ�����)�ݒ�w�b�_�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _ENEMY_ONEWAY_H_
#define _ENEMY_ONEWAY_H_

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
class CEnemyOneWay : public CEnemy
{
private: // �萔
	static const D3DXVECTOR3 SCALE;
	static const D3DXVECTOR3 MOVE_POWER;

public: // �p�u���b�N�֐�
	CEnemyOneWay();
	~CEnemyOneWay();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �I���X�V
	void Draw() override;	// �X�V

	void SetMove(const D3DXVECTOR3& inMove) override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
};
#endif
