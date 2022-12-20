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

	void SetFloor(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);
	CObjectX* GetFloor() { return m_floor; }

	void SetWall(int index, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);
	std::vector<CObjectX*> GetWall() { return std::vector<CObjectX*>(std::begin(m_wall), std::end(m_wall)); }

	void AddFloor(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);
	std::unordered_map<int, CObjectX*> GetMidairFloor() { return m_midairFloor; }

	void AddEnemy(const int type, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inRot, const int inFream);
	void PopEnemy(const int type, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inRot);
	std::unordered_map<int, SEnemyPop> GetEnemy() { return m_enemy; }

private: // プライベート関数
private: // メンバー変数
	CObjectX* m_floor;
	CObjectX* m_wall[4];
	float m_scale;
	std::unordered_map<int, CObjectX*> m_midairFloor;
	std::unordered_map<int, SEnemyPop> m_enemy;
	int m_startCnt;
};
#endif
