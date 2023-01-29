//=============================================================================
//
// プレイヤー設定
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "plain.h"
#include "utility.h"

#include "application.h"
#include "task_group.h"
#include "camera.h"
#include "color.h"
#include "line.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlain::CPlain()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlain::~CPlain()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CPlain::Init()
{
	begin = false;
	m_display = true;
	m_endCnt = 0;
	// 現在のモーション番号の保管
	CObjectX::Init();
	LoadModel("BOX");
	SetType(EType::PLAIN);
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CPlain::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlain::NormalUpdate()
{
	CObject* player = SearchType(CObject::EType::PLAYER, CTaskGroup::EPriority::LEVEL_3D_1);
	CCamera* camera = (CCamera*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(CTask::ERole::ROLE_CAMERA, CTaskGroup::EPriority::LEVEL_3D_1);

	m_display = !SegmentAndAABB(player->GetPos(), camera->GetPos());
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlain::EndUpdate()
{
	if(!begin)
	{
		SetMove(D3DXVECTOR3(FloatRandam(5.0f, -5.0f), 0.0f, FloatRandam(5.0f, -5.0f)));
		SetMoveRot(D3DXVECTOR3(FloatRandam(0.015f, -0.015f), 0.0f, FloatRandam(0.0125f, -0.015f)));
		begin = true;
	}

	m_endCnt++;

	if (m_endCnt > 320)
	{
		Release();
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CPlain::Draw()
{
	if (m_display)
	{
		CObjectX::Draw();
	}
}

//-----------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------
CPlain* CPlain::Create()
{
	CPlain* plain = new CPlain;

	if (plain != nullptr)
	{
		plain->Init();
	}

	return plain;
}
