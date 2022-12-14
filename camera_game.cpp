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
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -200.0f);

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

	Rotate();

	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -300.0f);

	// 計算用マトリックス
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot, mtxTrans;

	D3DXMatrixIdentity(&mtxWorld);										// 行列初期化関数

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// 行列回転関数
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);						// 行列掛け算関数 

	// 位置を反映
	D3DXVECTOR3 pos = CObject::SearchType(CObject::EType::PLAYER, CTaskGroup::EPriority::LEVEL_3D_1)->GetPos();
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);			// 行列移動関数
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);			// 行列掛け算関数

	D3DXVec3TransformCoord(&m_posV, &m_posV, &mtxWorld);
	D3DXVec3TransformCoord(&m_posR, &m_posR, &mtxWorld);
}

//-----------------------------------------------------------------------------
// 回転
//-----------------------------------------------------------------------------
void CCameraGame::Rotate()
{
	// 入力情報の取得
	const float MIN_MOUSE_MOVED = 1.0f;	// デッドゾーン

	D3DXVECTOR3 rollDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 cursorMove = CInput::GetKey()->GetMouse()->GetMouseCursorMove();
	D3DXVECTOR3 move = D3DXVECTOR3(cursorMove.y, cursorMove.x, cursorMove.z);

	if (D3DXVec3Length(&move) > MIN_MOUSE_MOVED || D3DXVec3Length(&move) < -MIN_MOUSE_MOVED)
	{
		// マウスの移動方向のノーマライズ
		D3DXVec3Normalize(&move, &move);

		// 移動方向の算出
		rollDir = move * (D3DX_PI / 180.0f);
	}

	// 回転
	m_rot += rollDir * 2.0f;

	if (m_rot.y < -D3DX_PI)
	{// 向きが-D3DX_PI未満の時
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.y > D3DX_PI)
	{// 向きがD3DX_PI以上の時
		m_rot.y -= D3DX_PI * 2;
	}
	if (m_rot.x < -D3DX_PI * 0.5f + 0.1f)
	{// 向きが-D3DX_PI未満の時
		m_rot.x = -D3DX_PI * 0.5f + 0.1f;
	}
	if (m_rot.x > D3DX_PI * 0.5f - 0.1f)
	{// 向きがD3DX_PI以上の時
		m_rot.x = D3DX_PI * 0.5f - 0.1f;
	}
}
