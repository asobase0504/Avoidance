//=============================================================================
//
// �J�E���g�_�E��UI
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _COUNTDOWN_H_		//���̃}�N������`����ĂȂ�������
#define _COUNTDOWN_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object.h"

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CObject2d;
class CNumber;

//=============================================================================
// �J�E���g�_�E��UI
//=============================================================================
class CCountdown : public CObject
{
private:
	static const int START_TIME;
public:
	CCountdown();
	~CCountdown();

	HRESULT Init() override;
	void Uninit() override;
	void PopUpdate() override;
	void NormalUpdate() override;

	bool IsEnd() { return m_end; }

	static CCountdown* Create(const D3DXVECTOR3& inPos);
private:
	CObject2d* m_side[4];
	CObject2d* m_sideTrace[4];
	CObject2d* m_stopLogo;
	int m_count;
	bool m_end;

	std::function<void()> m_pop;
	std::function<void()> m_countdown;
};
#endif