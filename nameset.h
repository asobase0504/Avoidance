//=============================================================================
//
// ���� ( ���O )
// Author : Hamada Ryuuga
//
//=============================================================================
#ifndef _NAMESET_H_
#define _NAMESET_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "main.h"
#include "object2d.h"
#include "name.h"
#include "mode.h"

//=============================================================================
// ���O�̍쐬�N���X
//=============================================================================
class CNameSet : public CMode
{
public:
	CNameSet();
	~CNameSet();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	void RankingNeme();

private:
	CObject2d* m_object2d[2];
	static std::string m_PlayName;
	CName* m_ListName[3];
	CName* m_PlayNameSet[10];
	D3DXVECTOR3 m_NemePos;

	int m_NowPlay;
};

#endif