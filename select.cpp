//=============================================================================
//
// �J�E���g�_�E��UI
// Author:Yuda Kaito
//
//=============================================================================
#include "select.h"
#include "utility.h"
#include "delay_process.h"
#include "input.h"
#include "mouse.h"
#include "application.h"
#include "sound.h"

//-----------------------------------------------------------------------------
// �萔
// Author:Yuda Kaito
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �R���X�g���N�^
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CSelect::CSelect() : CObject2d(CTaskGroup::EPriority::LEVEL_2D_UI)
{
	m_click = nullptr;
	m_selection = nullptr;
	m_noSelection = nullptr;
	m_isSelect = false;
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CSelect::~CSelect()
{
}

//-----------------------------------------------------------------------------
// ������
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CSelect::Init()
{
	CObject2d::Init();
	return S_OK;
}

//-----------------------------------------------------------------------------
// �o���X�V
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CSelect::PopUpdate()
{
	SetUpdateStatus(CObject::EUpdateStatus::NORMAL);
}

//-----------------------------------------------------------------------------
// �ʏ�X�V
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CSelect::NormalUpdate()
{
	// �ʏ�X�V
	CObject2d::NormalUpdate();

	if (CInput::GetKey()->GetAcceptJoyPadCount() == 0)
	{
		// �}�E�X�̈ʒu
		D3DXVECTOR3 mousePos = CInput::GetKey()->GetMouse()->GetMouseCursorPos();

		if (PointAndAABB(mousePos))
		{
			m_selection(this);

			// �I���̏���
			if (CInput::GetKey()->Trigger(CMouse::MOUSE_KEY_LEFT))
			{
				CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_CLICK);
				// �N���b�N������s������
				m_click(this);
			}
		}
		else
		{
			m_noSelection(this);
		}
	}
	else
	{
		if (m_isSelect)
		{
			m_selection(this);

			// �I���̏���
			if (CInput::GetKey()->Trigger(JOYPAD_A, 0))
			{
				CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_CLICK);
				// �I��������s������
				m_click(this);
			}
		}
		else
		{
			m_noSelection(this);
		}
	}
}

//-----------------------------------------------------------------------------
// �쐬
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CSelect* CSelect::Create(const D3DXVECTOR3 & inPos)
{
	CSelect* object = new CSelect;

	if (object != nullptr)
	{
		object->SetPos(inPos);
		object->Init();
	}

	return object;
}
