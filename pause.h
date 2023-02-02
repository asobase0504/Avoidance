//=============================================================================
// 
// �|�[�Y��ʂ̍쐬
// Author : Yuda Kaito
// 
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object.h"

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CMouseObject;
class CObject2d;

//=============================================================================
// �|�[�Y�N���X
//=============================================================================
class CPause :public CObject
{
public:
	//�I���ł�����
	enum ESelectType
	{
		MODE_NONE = 0,	// �I�����Ă��Ȃ�
		MODE_BACK,		// �Q�[����ʂɖ߂�
		MODE_RETURN,	// �Q�[������蒼��
		MODE_END,		// �^�C�g���ɖ߂�
		MODE_MAX
	};

	CPause();
	~CPause();

	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;

	// Setter
	void Set() { m_onPause = !m_onPause; };

	// Getter
	bool Get() { return m_onPause; };

private:
	bool m_onPause;
	ESelectType m_nextMode;
	CObject2d* m_bg;
	CObject2d* m_selectText[4];
	CMouseObject* m_mouseCursor;
};

#endif 
