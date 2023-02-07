//=============================================================================
//
// �x��ď������s��
// Author : Yuda Kaito
//
//=============================================================================
#include "delay_process.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CDelayProcess::CDelayProcess(CTask* inTask) : CTask(inTask->GetPriority(), CTaskGroup::EPushMethod::PUSH_NEXT, inTask)
{
	m_count = 0;
	m_func = nullptr;
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CDelayProcess::~CDelayProcess()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CDelayProcess::Init()
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CDelayProcess::Update()
{
	if (m_task->IsDeleted())
	{
		Release();
	}

	m_count--;

	if (m_count <= 0)
	{
		m_func();	// �֐�

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
// �����̓o�^
//-----------------------------------------------------------------------------
void CDelayProcess::DelayProcess(int inCount, CTask* inTask, std::function<void()> inFunc, int inLoopCount)
{
	CDelayProcess* processs = new CDelayProcess(inTask);
	processs->m_task = inTask;
	processs->m_executionTime = inCount;
	processs->m_count = inCount;
	processs->m_func = inFunc;
	processs->m_loopCount = inLoopCount;
}
