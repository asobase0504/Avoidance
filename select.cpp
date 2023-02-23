//=============================================================================
//
// カウントダウンUI
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
// 定数
// Author:Yuda Kaito
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// コンストラクタ
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
// デストラクタ
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CSelect::~CSelect()
{
}

//-----------------------------------------------------------------------------
// 初期化
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CSelect::Init()
{
	CObject2d::Init();
	return S_OK;
}

//-----------------------------------------------------------------------------
// 出現更新
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CSelect::PopUpdate()
{
	SetUpdateStatus(CObject::EUpdateStatus::NORMAL);
}

//-----------------------------------------------------------------------------
// 通常更新
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CSelect::NormalUpdate()
{
	// 通常更新
	CObject2d::NormalUpdate();

	if (CInput::GetKey()->GetAcceptJoyPadCount() == 0)
	{
		// マウスの位置
		D3DXVECTOR3 mousePos = CInput::GetKey()->GetMouse()->GetMouseCursorPos();

		if (PointAndAABB(mousePos))
		{
			m_selection(this);

			// 選択の条件
			if (CInput::GetKey()->Trigger(CMouse::MOUSE_KEY_LEFT))
			{
				CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_CLICK);
				// クリックしたら行う処理
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

			// 選択の条件
			if (CInput::GetKey()->Trigger(JOYPAD_A, 0))
			{
				CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_CLICK);
				// 選択したら行う処理
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
// 作成
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
