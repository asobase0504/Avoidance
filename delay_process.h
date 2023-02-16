//=============================================================================
//
// 遅れて処理を行う
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _DELAY_PROCESS_H_		//このマクロが定義されてなかったら
#define _DELAY_PROCESS_H_		//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "task.h"

//-----------------------------------------------------------------------------
// 遅延処理
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
	CTask* m_task;					// 処理を行うタスク
	std::function<void()> m_func;	// 処理
	int m_executionTime;			// 必要タイム
	int m_count;					// 現在タイム
	int m_loopCount;				// 繰り返す処理
};
#endif
