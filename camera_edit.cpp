//=============================================================================
//
// Editで動かすカメラ
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "camera_edit.h"
#include "input.h"
#include "object.h"
#include "objectX.h"
#include "debug_proc.h"
#include "utility.h"
#include "light.h"
#include "application.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CCameraEdit::CCameraEdit(CTaskGroup::EPriority inPriority) : CCamera(inPriority)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CCameraEdit::~CCameraEdit()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CCameraEdit::Init()
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
void CCameraEdit::Uninit()
{
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CCameraEdit::Update()
{
	Rotate();

	m_posR = D3DXVECTOR3(0.0f, 15.0f, 30.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -250.0f);

	// 計算用マトリックス
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot, mtxTrans;

	D3DXMatrixIdentity(&mtxWorld);	// 行列初期化関数

									// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// 行列回転関数
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);						// 行列掛け算関数 

	// 位置を反映
	D3DXVECTOR3 pos = CObject::SearchType(CObject::EType::PLAYER, CTaskGroup::EPriority::LEVEL_3D_1)->GetPos();	// プレイヤーを検索
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);	// 行列移動関数
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);	// 行列掛け算関数as

	// D3DXVECTOR3に反映
	D3DXVec3TransformCoord(&m_posV, &m_posV, &mtxWorld);
	D3DXVec3TransformCoord(&m_posR, &m_posR, &mtxWorld);

	D3DXVECTOR3 dist = m_posR - m_posV;
	D3DXVec3Normalize(&dist, &dist);

	D3DXVECTOR3 outPos;

	// カメラと地面の当たり判定
	CObject::TypeAllFunc(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1, [&dist, this, &pos, &outPos](CObject* inObject)
	{
		CObjectX* objectX = (CObjectX*)inObject;
		if (objectX->SegmentAndAABB(m_posV, pos, &outPos))
		{
			CDebugProc::Print("outPos : %f,%f,%f\n", outPos.x, outPos.y, outPos.z);
			m_posV = outPos + dist * 20.0f;
			objectX->AttachOutLine(false);
		}
		else
		{
			objectX->AttachOutLine();
		}
	});

	CDebugProc::Print("PosR : %f,%f,%f\n", m_posR.x, m_posR.y, m_posR.z);
	CDebugProc::Print("PosV : %f,%f,%f\n", m_posV.x, m_posV.y, m_posV.z);
}

//-----------------------------------------------------------------------------
// 回転
//-----------------------------------------------------------------------------
void CCameraEdit::Rotate()
{
	// 入力情報の取得
	const float MIN_MOUSE_MOVED = 0.01f;	// デッドゾーン

	D3DXVECTOR3 rollDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 cursorMove = CInput::GetKey()->GetMouse()->GetMouseCursorMove();
	D3DXVECTOR3 joyPadMove = CInput::GetKey()->VectorMoveJoyStick(true);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CDebugProc::Print("CursorMove : %.2f,%.2f,%.2f\n", cursorMove.x, cursorMove.y, cursorMove.z);
	CDebugProc::Print("JoyPadMove : %.2f,%.2f,%.2f\n", joyPadMove.x, joyPadMove.y, joyPadMove.z);

	if (D3DXVec3Length(&joyPadMove) == 0.0f)
	{ // JoyPadの入力がなかった場合カーソルの移動量を代入
		move = D3DXVECTOR3(cursorMove.y, cursorMove.x, cursorMove.z) * 0.1f;
	}
	else
	{ // JoyPadの入力があった場合
		move = D3DXVECTOR3(joyPadMove.y, joyPadMove.x, joyPadMove.z) * 1.5f;
	}

	if (D3DXVec3Length(&move) > MIN_MOUSE_MOVED || D3DXVec3Length(&move) < -MIN_MOUSE_MOVED)
	{
		// マウスの移動方向のノーマライズ
		//D3DXVec3Normalize(&move, &move);

		// 移動方向の算出
		rollDir += move * (D3DX_PI / 180.0f);
	}

	// 回転
	m_rot += rollDir * 2.0f;
	m_rot.z = 0.0f;

	CDebugProc::Print("m_rot : %.2f,%.2f,%.2f\n", m_rot.x, m_rot.y, m_rot.z);

	// 正規化
	NormalizeAngle(m_rot.y);

	if (m_rot.x < -D3DX_PI * 0.5f + 0.75f)
	{// 向きが-D3DX_PI未満の時
		m_rot.x = -D3DX_PI * 0.5f + 0.75f;
	}
	if (m_rot.x > D3DX_PI * 0.5f - 0.75f)
	{// 向きがD3DX_PI以上の時
		m_rot.x = D3DX_PI * 0.5f - 0.75f;
	}
}
