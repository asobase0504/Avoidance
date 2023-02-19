//=============================================================================
//
// �x��ď������s��
// Author : Yuda Kaito
//
//=============================================================================
#include "delay_process.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CDelayProcess::CDelayProcess(CTask* inTask) : CTask(inTask->GetPriority(), CTaskGroup::EPushMethod::PUSH_NEXT, inTask)
{
	m_task = nullptr;		// �������s���^�X�N
	m_func = nullptr;		// ����
	m_executionTime = 0;	// �K�v�^�C��
	m_count = 0;			// ���݃^�C��
	m_loopCount = 0;		// �J��Ԃ�����
	SetRole(ROLE_NONE);
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CDelayProcess::~CDelayProcess()
{
}

//-----------------------------------------------------------------------------
// ������
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CDelayProcess::Init()
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// �X�V
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
