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

	void SetScale(const float inScale);			// �傫���̐ݒ�
	const float GetScale() { return m_scale; }	// �傫���̎擾

	static CStage* Create();	// ����

	void SetFloor(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);
	CObjectX* GetFloor() { return m_floor; }

	void SetWall(int index, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);
	std::vector<CObjectX*> GetWall() { return std::vector<CObjectX*>(std::begin(m_wall), std::end(m_wall)); }

	void AddFloor(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);
	std::unordered_map<int, CObjectX*> GetMidairFloor() { return m_midairFloor; }

	void AddEnemy(const int type, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inRot, const int inFream);
	void PopEnemy(const int type, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inRot);
	std::unordered_map<int, SEnemyPop> GetEnemy() { return m_enemy; }

private: // �v���C�x�[�g�֐�
private: // �����o�[�ϐ�
	CObjectX* m_floor;
	CObjectX* m_wall[4];
	float m_scale;
	std::unordered_map<int, CObjectX*> m_midairFloor;
	std::unordered_map<int, SEnemyPop> m_enemy;
	int m_startCnt;
};
#endif
