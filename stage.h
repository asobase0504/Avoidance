//=============================================================================
//
// ステージ全体
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
// ステージ
//-----------------------------------------------------------------------------
class CObjectX;
class CEnemy;
class CGoal;
class CPlain;

class CStage : public CTask
{
private: // 定数

	struct SEnemyPop
	{
		int type;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		int popFream;
	};

public: // パブリック関数
	CStage();
	~CStage();

	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 破棄
	void Update() override;		// 更新
	void Draw() override;		// 描画

	void SetScale(const float inScale);			// 大きさの設定
	const float GetScale() { return m_scale; }	// 大きさの取得

	static CStage* Create();	// 生成

	void SetPos(const D3DXVECTOR3& inPos) { m_pos = inPos; }
	const D3DXVECTOR3& GetPos() { return m_pos; }

	void SetGoal(const D3DXVECTOR3& pos);

	void SetFloor(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);

	void SetWall(int index, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);

	void AddFloor(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);

	void AddEnemy(const int type, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inRot, const int inFream);
	void PopEnemy(const int type, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inRot);
	std::unordered_map<int, SEnemyPop> GetEnemy() { return m_enemy; }

	void SetStart(bool start) { m_isStart = start; }
	bool IsEnd() { return m_isEnd; }

private: // プライベート関数
private: // メンバー変数
	CGoal* m_goal;
	CPlain* m_floor;
	CPlain* m_wall[5];
	float m_scale;

	D3DXVECTOR3 m_pos;

	std::unordered_map<int, CPlain*> m_midairFloor;
	std::unordered_map<int, SEnemyPop> m_enemy;
	int m_startCnt;

	bool m_isStart;
	bool m_isEnd;
};
#endif
