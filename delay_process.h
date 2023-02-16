//=============================================================================
//
// �x��ď������s��
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _DELAY_PROCESS_H_		//���̃}�N������`����ĂȂ�������
#define _DELAY_PROCESS_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "task.h"

//-----------------------------------------------------------------------------
// �x������
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
class CDelayProcess : public CTask
{
public:
	CDelayProcess(CTask* inTask);
	~CDelayProcess() override;

	HRESULT Init() override;
	void Update() override;

	static void DelayProcess(const unsigned int inDelaytime, CTask* inTask, std::function<void()> inFunc, int inLoopCount = 1);

private:
	CTask* m_task;					// �������s���^�X�N
	std::function<void()> m_func;	// ����
	int m_executionTime;			// �K�v�^�C��
	int m_count;					// ���݃^�C��
	int m_loopCount;				// �J��Ԃ�����
};
#endif
