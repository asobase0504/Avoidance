#//=============================================================================
//
// 丸影
// Author:Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "circle_shadow.h"
#include "task_group.h"
#include "objectX.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CCircleShadow::CCircleShadow()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CCircleShadow::~CCircleShadow()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CCircleShadow::Init()
{
	CObjectPolygon3D::Init();

	SetTexture("CIRCLE_SHADOW");
	SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	SetSize(D3DXVECTOR3(15.0f, 15.0f, 0.0f));
	SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CCircleShadow::Uninit()
{
	CObjectPolygon3D::Uninit();
}

//-----------------------------------------------------------------------------
// 通常更新
//-----------------------------------------------------------------------------
void CCircleShadow::NormalUpdate()
{
	CObjectPolygon3D::NormalUpdate();

	// Y軸を最低にする
	SetPos(D3DXVECTOR3(0.0f, -FLT_MAX, 0.0f));

	// 全ての地面と当たり判定を行う
	CObject::TypeAllFunc(EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1, [this](CObject* inObject)
	{
		CObjectX* plain = (CObjectX*)inObject;

		D3DXVECTOR3 outPos;

		if (plain->RayAndAABB(m_parent->GetPos(), D3DXVECTOR3(0.0f, -1.0f, 0.0f), &outPos))
		{
			outPos.y += 5.0f;

			bool isParentThanOn = m_parent->GetPos().y > outPos.y;	// 親より上
			bool isThisThanDown = GetPos().y < outPos.y;			// 予定されてる位置より上

			if (isParentThanOn && isThisThanDown)
			{
				SetPos(outPos);
			}
		}
	});
}

//-----------------------------------------------------------------------------
// 作成
//-----------------------------------------------------------------------------
CCircleShadow * CCircleShadow::Create(CObject* inObject)
{
	CCircleShadow* shadow = new CCircleShadow;

	if (shadow != nullptr)
	{
		shadow->Init();
		shadow->m_parent = inObject;
	}

	return shadow;
}
