//=============================================================================
//
// �X�e�[�W�N���A
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _CLEAR_H_		//���̃}�N������`����ĂȂ�������
#define _CLEAR_H_		//2�d�C���N���[�h�h�~�̃}�N����`

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
class CClear : public CObject
{
private:
	static const int START_TIME;
public:
	CClear();
	~CClear();

	HRESULT Init() override;
	void Uninit() override;
	void PopUpdate() override;
	void NormalUpdate() override;
	void EndUpdate() override;

	bool IsEnd() { return m_end; }

	static CClear* Create(const D3DXVECTOR3& inPos);

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