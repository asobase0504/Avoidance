//=============================================================================
//
// ゲームで動かすカメラ
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "camera_game.h"
#include "input.h"
#include "object.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CCameraGame::CCameraGame(CTaskGroup::EPriority inPriority) : CCamera(inPriority)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CCameraGame::~CCameraGame()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CCameraGame::Init()
{
	CCamera::Init();
	m_posV = D3DXVECTOR3(0.0f, 150.0f, -200.0f);

	D3DXVECTOR3 distPos = m_posR - m_posV;
	m_fDistance = sqrtf(distPos.x * distPos.x + distPos.z * distPos.z);
	m_fDistance = sqrtf(distPos.y * distPos.y + (m_fDistance * m_fDistance));
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CCameraGame::Uninit()
{
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CCameraGame::Update()
{
	D3DXVECTOR3 move(0.0f,0.0f,0.0f);

	// 視点角度の回転
	if (CInput::GetKey()->Press(DIK_Q))
	{
		m_rot.y += -0.05f;		// 回転量
	}
	if (CInput::GetKey()->Press(DIK_E))
	{
		m_rot.y += 0.05f;		// 回転量
	}
	if (CInput::GetKey()->Press(DIK_R))
	{
		m_posV.y -= 10.0f;
	}
	if (CInput::GetKey()->Press(DIK_F))
	{
		m_posV.y += 10.0f;
	}

	m_posV.x = m_posR.x - sinf(m_rot.y) * m_fDistance;
	m_posV.y = CObject::SearchType(CObject::EType::PLAYER, CTaskGroup::EPriority::LEVEL_3D_1)->GetPos().y + 200.0f;
	m_posV.z = m_posR.z - cosf(m_rot.y) * m_fDistance;

	m_posR = CObject::SearchType(CObject::EType::PLAYER, CTaskGroup::EPriority::LEVEL_3D_1)->GetPos();
}
