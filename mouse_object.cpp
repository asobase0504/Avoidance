//=============================================================================
//
// マウスカーソルの上に貼る2DObject
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "mouse_object.h"
#include "input.h"
#include "mouse.h"
#include "mouse_effect.h"
#include "utility.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CMouseObject::CMouseObject(CTaskGroup::EPriority list) :
	CObject2d(list)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CMouseObject::~CMouseObject()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CMouseObject::Init()
{
	CObject2d::Init();
	D3DXVECTOR3 pos = CInput::GetKey()->GetMouse()->GetMouseCursorPos();
	SetPos(pos);
	SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
	SetColor(D3DXCOLOR(1.0f, 0.0f, 0.5f, 1.0f));

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CMouseObject::Uninit()
{
}

//-----------------------------------------------------------------------------
// 通常中更新
//-----------------------------------------------------------------------------
void CMouseObject::NormalUpdate()
{
	SetPos(CInput::GetKey()->GetMouse()->GetMouseCursorPos());
	AddRot(D3DXVECTOR3(0.0f, 0.0f, 0.005f));

	CObject2d::NormalUpdate();

	float size = GetSize().x;

	if (CInput::GetKey()->Trigger(CMouse::MOUSE_KEY::MOUSE_KEY_LEFT))
	{
		size = 15.0f;
	}

	if (size > 10.0f)
	{
		size -= 0.15f;
	}

	SetSize(D3DXVECTOR3(size, size, 0.0f));

	static int cnt = 0;
	cnt++;
	if (cnt % 1 == 0)
	{
		//for (int i = 0; i < 5; i++)
		{
			CMouseEffect* effect = CMouseEffect::Create();
			D3DXVECTOR3 pos(FloatRandam(m_pos.x + 5.0f, m_pos.x - 5.0f), FloatRandam(m_pos.y + 5.0f, m_pos.y - 5.0f), 0.0f);
			effect->SetPos(pos);
			effect->SetSize(D3DXVECTOR3(8.0f, 8.0f, 0.0f));
			//D3DXVECTOR3 move(FloatRandam(m_pos.x + 5.0f, m_pos.x - 5.0f), FloatRandam(m_pos.y + 5.0f, m_pos.y - 5.0f), 0.0f);
			//effect->SetMove(FloatRandam(5.0f,- 5.0f), FloatRandam(5.0f, - 5.0f), 0.0f);
			effect->SetRot(m_rot);
			effect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.5f, 0.25f));
		}
	}
}

//-----------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------
CMouseObject* CMouseObject::Create()
{
	CMouseObject* object = new CMouseObject(CTaskGroup::LEVEL_2D_UI);

	if (object != nullptr)
	{
		object->Init();
	}

	return object;
}
