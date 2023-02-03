//=============================================================================
//
// マネージャー
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "task_group.h"
#include "task.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
// Author : Yuda Kaito
//=============================================================================
CTaskGroup::CTaskGroup() :
	m_priorityNumber(0)
{
	m_list.clear();
}

//=============================================================================
// デストラクタ
// Author : Yuda Kaito
//=============================================================================
CTaskGroup::~CTaskGroup()
{
}

//=============================================================================
// 初期化
// Author : Yuda Kaito
//=============================================================================
HRESULT CTaskGroup::Init()
{
	return S_OK;
}

//=============================================================================
// 全てのタスクに処理を行う
// Author : Yuda Kaito
//=============================================================================
template<typename Func>
void CTaskGroup::AllProcess(Func func)
{
	for (int i = 0; i <= m_priorityNumber; i++)
	{
		PriorityProcess(i, func);
	}
}

//=============================================================================
// 指定されたpriorityにあるタスク全ての処理を行う
// Author : Yuda Kaito
//=============================================================================
template<typename Func>
void CTaskGroup::PriorityProcess(int cnt, Func func)
{
	if (m_list.count(cnt) == 0)
	{
		return;
	}

	CTask* now = m_list.at(cnt).top;

	while (now != nullptr)
	{
		CTask* next = now->GetNext();

		func(now);

		now = next;
	}
}

//=============================================================================
// 終了
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::Uninit()
{
	AllProcess([](CTask* task)
	{
		task->Uninit();
		task->Release();
	});

	DeleteTask();	// タスクリストの削除
}

//=============================================================================
// 更新
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::Update()
{
	AllProcess([](CTask* task)
	{
		if (!task->IsDeleted())
		{
			if (task->NeedUpdate())
			{
				task->Update();
			}
		}
	});

	DeleteTask();	// タスクリストの削除
}

//=============================================================================
// 描画
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::Draw()
{
	for (int i = 0; i <= m_priorityNumber; i++)
	{
		if (m_list.count(i) == 0)
		{
			continue;
		}

		// 画面クリア(バックバッファ＆Zバッファのクリア)
		CRenderer::GetInstance()->GetDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		{
			CTask* now = m_list.at(i).top;

			while (now != nullptr)
			{
				CTask* next = now->GetNext();
				if (!now->IsDeleted())
				{
					now->Draw();
				}
				now = next;
			}
		}
	}
}

//=============================================================================
// 所持しているタスクの破棄
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::AllRelease()
{
	AllProcess([](CTask* task)
	{
		if (!task->IsProtect())
		{
			task->Release();
		}
	});

	DeleteTask();	// タスクリストの削除
}

//=============================================================================
// 絶対にタスクを破棄
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::AbsolutelyRelease()
{
	AllProcess([](CTask* task) {task->Release(); });
}

//=============================================================================
// 指定したpriorityのタスクを破棄
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::PriorityRelease(const EPriority inPriotity)
{
	if (m_list.count(inPriotity) == 0)
	{
		return;
	}

	CTask* now = m_list.at(inPriotity).top;

	while (now != nullptr)
	{
		if (!now->IsProtect())
		{
			now->Release();
		}
		now = now->GetNext();
	}
}

//=============================================================================
// リストの最後にタスクを入れる
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::SetPushCurrent(CTask * inTask, int inPriority)
{
	if (m_list.count(inPriority) == 0)
	{
		SList inList;
		inList.top = nullptr;
		inList.current = nullptr;
		m_list.insert(std::make_pair(inPriority, inList));

		if (m_priorityNumber < inPriority)
		{
			m_priorityNumber = inPriority;
		}
	}

	SList* list = &m_list.at(inPriority);

	if (list->current == nullptr && list->top == nullptr)
	{
		list->top = inTask;
		list->current = inTask;
	}
	else
	{
		CTask* current = list->current;		// 前回末尾
		list->current = inTask;				// 新規末尾
		list->current->SetPrev(current);	// 新規末尾の前に前回末尾を設定
		current->SetNext(list->current);	// 前回末尾の後に新規末尾を設定
	}
}

//=============================================================================
// リストの最初にタスクを入れる
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::SetPushTop(CTask * inTask, int inPriority)
{
	if (m_list.count(inPriority) == 0)
	{
		SList inList;
		m_list.insert(std::make_pair(inPriority, inList));
		m_list.at(inPriority).top = nullptr;
		m_list.at(inPriority).current = nullptr;

		if (m_priorityNumber < inPriority)
		{
			m_priorityNumber = inPriority;
		}
	}

	if (m_list.at(inPriority).current == nullptr && m_list.at(inPriority).top == nullptr)
	{
		m_list.at(inPriority).top = inTask;
		m_list.at(inPriority).current = inTask;
	}
	else
	{
		CTask* top = m_list.at(inPriority).top;		// 前回先頭
		m_list.at(inPriority).top = inTask;			// 新規先頭
		m_list.at(inPriority).top->SetNext(top);	// 新規先頭の次に前回先頭を設定
		top->SetPrev(m_list.at(inPriority).top);	// 新規先頭の後に新規先頭を設定
	}
}

//=============================================================================
// 参照したタスクの次にタスクを入れる
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::SetNextTask(CTask * inReference, CTask * inTask)
{
	if (inReference == nullptr)
	{
		SetPushCurrent(inTask, 0);
		return;
	}
}

//=============================================================================
// 参照したタスクの前にタスクを入れる
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::SetPrevTask(CTask * inReference, CTask * inTask)
{
	if (inReference == nullptr)
	{
		SetPushCurrent(inTask);
		return;
	}
}

//=============================================================================
// タスクの役割ごとの検索(Top側から検索して見つかった先頭を返す)
// Author : Yuda Kaito
//=============================================================================
CTask * CTaskGroup::SearchRoleTop(int inRole, int inPriority)
{
	if (m_list.count(inPriority) == 0)
	{
		// 検索先のリストがなかった場合
		assert(false);
		return nullptr;
	}

	CTask* now = m_list.at(inPriority).top;

	while (now != nullptr)
	{
		CTask* next = now->GetNext();
		if (now->GetRole() == inRole)
		{
			return now;
		}
		now = next;
	}
	return nullptr;
}

//=============================================================================
// タスクの役割ごとの検索(Current側から検索して見つかった先頭を返す)
// Author : Yuda Kaito
//=============================================================================
CTask * CTaskGroup::SearchRoleCurrent(int inRole, int inPriority)
{
	if (m_list.count(inPriority) == 0)
	{
		// 検索先のリストがなかった場合
		assert(false);
		return nullptr;
	}

	CTask* now = m_list.at(inPriority).current;

	while (now != nullptr)
	{
		CTask* prev = now->GetPrev();
		if (now->GetRole() == inRole)
		{
			return now;
		}
		now = prev;
	}
	return nullptr;
}

//=============================================================================
// 受けとったタスクと同じ役割のタスクを検索(Next側から検索)
// Author : Yuda Kaito
//=============================================================================
CTask * CTaskGroup::SearchSameRoleNext(CTask* inTask)
{
	CTask* now = inTask->GetNext();
	CTask::ERole role = inTask->GetRole();

	while (now != nullptr)
	{
		CTask* next = now->GetNext();
		if (now->GetRole() == role)
		{
			return now;
		}
		now = next;
	}
	return nullptr;
}

//=============================================================================
// 受けとったタスクと同じ役割のタスクを検索(Prev側から検索)
// Author : Yuda Kaito
//=============================================================================
CTask * CTaskGroup::SearchSameRolePrev(CTask * inTask)
{
	CTask* now = inTask;
	CTask::ERole role = inTask->GetRole();

	while (now != nullptr)
	{
		CTask* prev = now->GetPrev();
		if (now->GetRole() == role)
		{
			return now;
		}
		now = prev;
	}
	return nullptr;
}

//=============================================================================
// ポーズ中に更新を止めるobjectの更新を止める
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::Pause(bool isPause)
{
	if (isPause)
	{
		AllProcess([](CTask* task) {task->Pouse(); });
	}
	else
	{
		AllProcess([](CTask* task) {task->PouseOff(); });
	}
}

//=============================================================================
// タスクの削除
// Author : Yuda Kaito
//=============================================================================
void CTaskGroup::DeleteTask()
{
	for (int i = 0; i <= m_priorityNumber; i++)
	{
		// 削除
		auto Delete = [i, this](CTask* tack)
		{
			CTask* next = tack->GetNext();
			CTask* prev = tack->GetPrev();

			if (!tack->IsDeleted())
			{
				return;
			}

			// 前後の状態を取得
			bool isNextNullptr = (next == nullptr);
			bool isPrevNullptr = (prev == nullptr);

			if (!isNextNullptr && !isPrevNullptr)
			{ // 前後にタスクが入っている場合
				next->SetPrev(prev);
				prev->SetNext(next);
			}
			else if (!isNextNullptr && isPrevNullptr)
			{ // 後のみタスクが入っている場合
				m_list.at(i).top = next;
				next->SetPrev(nullptr);
			}
			else if (isNextNullptr && !isPrevNullptr)
			{ // 前のみタスクが入っている場合
				m_list.at(i).current = prev;
				prev->SetNext(nullptr);
			}
			else
			{ // 前後にタスクが入っていない場合
				m_list.at(i).current = nullptr;
				m_list.at(i).top = nullptr;
				m_list.erase(i);
			}

			tack->Uninit();	// 終了
			delete tack;	// 削除
		};

		// priorityの全てのTackに引数の関数を行う
		PriorityProcess(i, Delete);
	}
}
