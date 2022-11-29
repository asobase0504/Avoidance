//=============================================================================
//
// 根幹クラス
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "task.h"
#include <assert.h>
#include "application.h"
#include "task_group.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CTask::CTask(CTaskGroup::EPriority inPriority, CTaskGroup::EPushMethod inMethod) :
	m_prev(nullptr),
	m_next(nullptr),
	m_isDeleted(false),
	m_isProtect(false)
{

	// タスクグループに自身を登録する
	switch (inMethod)
	{
	case CTaskGroup::PUSH_CURRENT:
		CApplication::GetInstance()->GetTaskGroup()->SetPushCurrent(this, inPriority);
		break;
	case CTaskGroup::PUSH_TOP:
		CApplication::GetInstance()->GetTaskGroup()->SetPushTop(this, inPriority);
		break;
	default:
		assert(false);
		break;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CTask::~CTask()
{
}

//=============================================================================
// 終了
//=============================================================================
void CTask::Uninit()
{
	Release();
}
