//=============================================================================
//
// �X�e�[�W�S��
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "task.h"
#include <unordered_map>
#include <iostream>

//-----------------------------------------------------------------------------
// �X�e�[�W
//-----------------------------------------------------------------------------
class CObjectX;
class CEnemy;
class CGoal;
class CPlain;

class CStage : public CTask
{
private: // �萔

	struct SEnemyPop
	{
		int type;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		int popFream;
	};

public: // �p�u���b�N�֐�
	CStage();
	~CStage();

	HRESULT Init() override;	// ������
	void Uninit() override;		// �j��
	void Update() override;		// �X�V
	void Draw() override;		// �`��

	static CStage* Create();	// ����

	void SetPos(const D3DXVECTOR3& inPos) { m_pos = inPos; }
	const D3DXVECTOR3& GetPos() { return m_pos; }

	void SetGoal(const D3DXVECTOR3& pos, const int inTime);

	void SetFloor(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);

	void SetWall(int index, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);

	void AddFloor(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);

	void AddEnemy(const int type, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inRot, const int inFream);
	void PopEnemy(const int type, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inRot);
	std::unordered_map<int, SEnemyPop> GetEnemy() { return m_enemy; }

	void SetStart(bool start) { m_isStart = start; }
	bool IsEnd() { return m_isEnd; }

	void EnemyDeath();
	void PopReset() { m_startCnt = -60; }

	void ResetGoal();

private: // �v���C�x�[�g�֐�

private: // �����o�[�ϐ�

	D3DXVECTOR3 m_pos;

	/* �}�b�v */
	CGoal* m_goal;		// �S�[��
	CPlain* m_floor;	// ��
	CPlain* m_wall[5];	// ��
	std::unordered_map<int, CPlain*> m_midairFloor;	// �󒆏�
	std::unordered_map<int, SEnemyPop> m_enemy;		// �o���\��̃G�l�~�[

	int m_startCnt;

	bool m_isFrag;
	bool m_isStart;
	bool m_isEnd;
};
#endif
