//=============================================================================
//
// �G�l�~�[(������ɂ�����)�ݒ�w�b�_�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _ENEMY_ONEWAY_REFLECTION_H_
#define _ENEMY_ONEWAY_REFLECTION_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "enemy.h"

//-----------------------------------------------------------------------------
// �G�l�~�[�N���X
//-----------------------------------------------------------------------------
class CEnemyReflection : public CEnemy
{
private: // �萔
	static const D3DXVECTOR3 SCALE;
	static const D3DXVECTOR3 MOVE_POWER;
	static const int REFLECT_COUNT;
	static const int MAX_LIFE;

public: // �p�u���b�N�֐�
	CEnemyReflection();
	~CEnemyReflection();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void EndUpdate() override;		// �I���X�V
	void Draw() override;	// �X�V

	void SetMove(const D3DXVECTOR3& inMove) override;

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�
	int m_reflectCnt;
	int m_life;
};
#endif
