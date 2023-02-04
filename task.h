//=============================================================================
//
// 根幹クラス
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _TASK_H_			// このマクロ定義がされてなかったら
#define _TASK_H_			// 二重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <Windows.h>
#include "task_group.h"

//=============================================================================
// タスククラス
//=============================================================================
class CTask
{
public:
	// タイプ
	enum ERole
	{
		ROLE_OBJECT = 0,
		ROLE_CAMERA,
		ROLE_LIGHT,
		ROLE_STAGE,
		ROLE_MAX
	};

public:
	CTask(CTaskGroup::EPriority inPriority = CTaskGroup::EPriority::LEVEL_3D_1, CTaskGroup::EPushMethod inMethod = CTaskGroup::EPushMethod::PUSH_CURRENT);
	virtual ~CTask();

	virtual HRESULT Init() = 0;
	virtual void Uninit();
	virtual void Update() {}
	virtual void Draw() {}

	void Release() { m_isDeleted = true; }

	/* リスト構造 */
	void SetPrev(CTask* inTask) { m_prev = inTask; }
	CTask* GetPrev() { return m_prev; }
	void SetNext(CTask* inTask) { m_next = inTask; }
	CTask* GetNext() { return m_next; }

	/* 破棄予定 */
	bool IsDeleted() { return m_isDeleted; }

	/* 破棄を保護する */
	void AttachProtect() { m_isProtect = true; }
	bool IsProtect() { return m_isProtect; }

	/* 役割 */
	void SetRole(const ERole& inJobRole) { m_role = inJobRole; }
	const ERole& GetRole() const { return m_role; }

	/* 優先順位 */
	int GetPriority() { return m_priority; }

	/* 更新する必要があるか */
	bool NeedUpdate() { return m_isUpdate; }

	/* ポーズ */
	void SetPouseUpdate(bool isUpdate) { m_isPouseUpdate = isUpdate; }	// ポーズ時に更新を行うか決める
	void PauseOn() { m_isUpdate = m_isPouseUpdate; }						// 更新を行うか行わないかポーズ状態で判断
	void PauseOff() { if (!m_isPouseUpdate) { m_isUpdate = true; } }	// ポーズ状態で止まっていた更新を稼働させる

private:
	int m_priority;

	CTask* m_prev;		// 前のタスク
	CTask* m_next;		// 後のタスク

	// 親子関係
	CTask* m_pearent;	// 親
	std::list<CTask*> m_childrens;	// 子

	bool m_isDeleted;	// 破棄予定
	bool m_isProtect;	// 全体破棄から保護する

	ERole m_role;		// 役割

	bool m_isUpdate;		// 更新を行うか
	bool m_isPouseUpdate;	// ポーズ中に更新処理を行うか
};
#endif
