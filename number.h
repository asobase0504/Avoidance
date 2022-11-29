//============================
//
// NUMBER
// Author:hamada ryuuga
//
//============================

#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "object2d.h"

class CNumber : public CObject2d
{
public:
	CNumber(CTaskGroup::EPriority list = CTaskGroup::LEVEL_2D_UI);
	~CNumber();
	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
	void Draw() override;
	static CNumber* Create();
	
	void SetSpeed(float Speed) { m_Speed = Speed; }
	void SetNumber(int Number) { m_MyNumber = Number; }
	void SetMove(bool Move) { m_Move = Move; }
	
	void ReleaseTimer(int nTimar);

private:
	D3DXCOLOR m_col;
	float m_Speed;
	int m_DesTimarMax;
	int m_DesTimar;
	int m_MyNumber;
	bool m_isRelease;
	bool m_Move;
	
};
#endif