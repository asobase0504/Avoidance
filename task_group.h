//=============================================================================
//
// タスクの集まり
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _TASK_GROUP_H_			// このマクロ定義がされてなかったら
#define _TASK_GROUP_H_			// 二重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <unordered_map>

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CTask;

//=============================================================================
// タスククラス
//=============================================================================
class CTaskGroup
{
public:
	// リスト構造体
	struct SList
	{
		CTask* top;
		CTask* current;
	};

	enum EPriority
	{
		LEVEL_SYSTEM = 0,
		LEVEL_2D_BG,
		LEVEL_3D_1,
		LEVEL_3D_2,
		LEVEL_2D_1,
		LEVEL_2D_2,
		LEVEL_2D_UI,
		LEVEL_3D_UI,
		LEVEL_FADE,
		LEVEL_MAX
	};

	// 追加方法
	enum EPushMethod
	{
		PUSH_CURRENT = 0,
		PUSH_TOP
	};

public:
	CTaskGroup();
	~CTaskGroup();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// Release
	void AllRelease();
	void AbsolutelyRelease();
	void PriorityRelease(const EPriority inPriotity);

	template<typename Func>
	void AllProcess(Func func);
	template<typename Func>
	void PriorityProcess(int cnt, Func func);

	// Setter
	void SetPushCurrent(CTask* inTask, int inPriority = 0);
	void SetPushTop(CTask* inTask, int inPriority = 0);
	void SetNextTask(CTask* inReference, CTask* inTask);
	void SetPrevTask(CTask* inReference, CTask* inTask);

	// Getter
	CTask* GetTop(int priority = 0) { return m_list.at(priority).top; }
	CTask* GetCurrent(int priority = 0) { return m_list.at(priority).current; }

	// 検索
	CTask* SearchRoleTop(int inRole, int inPriority);
	CTask* SearchRoleCurrent(int inRole, int inPriority);
	CTask* SearchSameRoleNext(CTask* inTask);
	CTask* SearchSameRolePrev(CTask* inTask);

	void Pause(bool isPause);

private:
	void DeleteTask();

private:
	std::unordered_map<int,SList> m_list;	// リスト一覧
	unsigned int m_createNumber;			// 生成数
	int m_priorityNumber;					// 描画順数
};

#endif
