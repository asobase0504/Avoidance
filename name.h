//=============================================================================
//
// 名前の決定
// Author : Hamada Ryuuga
//
//=============================================================================
#ifndef _NAME_H_
#define _NAME_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object2d.h"

//=============================================================================
// 名前の決定クラス
//=============================================================================
class CName : public CObject2d
{
public:

	enum EAlphabet
	{
		A = 0,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		MAX
	};

	CName(CTaskGroup::EPriority list);
	~CName();

	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
	void Draw() override;

	static CName* Create();

	// Setter
	void SetAlphabet(const EAlphabet Alphabet);
	void SetMoveSwitch(bool move);

	// Getter
	EAlphabet GetAlphabet();

private:
	PositionVec4 color;
	int m_desTimarMax;
	int m_desTimar;

	int m_divisionX;
	int m_divisionY;
	int m_divisionMAX;

	EAlphabet m_type;

	bool m_isMove;
};


#endif