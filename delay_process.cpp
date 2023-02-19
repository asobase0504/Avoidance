//=============================================================================
//
// 遅れて処理を行う
// Author : Yuda Kaito
//
//=============================================================================
#include "delay_process.h"

//-----------------------------------------------------------------------------
// コンストラクタ
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CDelayProcess::CDelayProcess(CTask* inTask) : CTask(inTask->GetPriority(), CTaskGroup::EPushMethod::PUSH_NEXT, inTask)
{
	m_task = nullptr;		// 処理を行うタスク
	m_func = nullptr;		// 処理
	m_executionTime = 0;	// 必要タイム
	m_count = 0;			// 現在タイム
	m_loopCount = 0;		// 繰り返す処理
	SetRole(ROLE_NONE);
}

//-----------------------------------------------------------------------------
// デストラクタ
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CDelayProcess::~CDelayProcess()
{
}

//-----------------------------------------------------------------------------
// 初期化
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CDelayProcess::Init()
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// 更新
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CDelayProcess::Update()
{
	if (m_task->IsDeleted())
	{
		Release();
	}

	m_count--;

	CDebugProc::Print("m_loopCount  : %d\n", m_loopCount);
	CDebugProc::Print("DelayCount  : %d\n", m_count);

	if (m_count <= 0)
	{
		m_func();	// 関数

		m_loopCount--;

		if (m_loopCount != 0)
		{
			m_count = m_executionTime;
		}
		else
		{
			Release();
		}
	}
}

//-----------------------------------------------------------------------------
// 処理の登録
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CDelayProcess::DelayProcess(const unsigned int inCount, CTask* inTask, std::function<void()> inFunc, int inLoopCount)
{
	CDelayProcess* processs = new CDelayProcess(inTask);
	processs->m_task = inTask;
	processs->m_executionTime = inCount;
	processs->m_count = inCount;
	processs->m_func = inFunc;
	processs->m_loopCount = inLoopCount;
}
