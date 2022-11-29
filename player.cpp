//============================
//
// プレイヤー設定
// Author:hamada ryuuga
//
//============================

#include <stdio.h>
#include <assert.h>
#include "player.h"
#include "input.h"
#include "camera.h"
#include "motion.h"
#include "application.h"
#include "motion.h"
#include "particle_manager.h"
#include "utility.h"
#include "game.h"
#include "tutorial.h"

//------------------------------------
// static変数
//------------------------------------
const float CPlayer::ATTENUATION = 0.5f;	// 移動減衰係数
const float CPlayer::SPEED = 1.0f;			// 移動量
const float CPlayer::WIDTH = 10.0f;			// モデルの半径
const int CPlayer::MAX_PRAYER = 16;			// 最大数
const int CPlayer::MAX_MOVE = 9;			// アニメーションの最大数
const int CPlayer::INVINCIBLE = 30;			// 無敵時間

//------------------------------------
// コンストラクタ
//------------------------------------
CPlayer::CPlayer()
{
}

//------------------------------------
// デストラクタ
//------------------------------------
CPlayer::~CPlayer()
{

}

//------------------------------------
// 初期化
//------------------------------------
HRESULT CPlayer::Init()
{
	// 現在のモーション番号の保管
	CObjectX::Init();
	//CObjectX::Set(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "Data/system/Gon/Fox.txt");
	D3DXVECTOR3	Size(2.0f, 2.0f, 2.0f);
	m_MoveSpeed = 7.0f;
	m_rot.y += (-D3DX_PI*0.5f);
	SetSize(Size);
	return S_OK;
}

//------------------------------------
// 終了
//------------------------------------
void CPlayer::Uninit()
{
	// 現在のモーション番号の保管
	CObjectX::Uninit();
}

//------------------------------------
// 更新
//------------------------------------
void CPlayer::NormalUpdate()
{
	switch (*CApplication::GetInstance()->GetMode())
	{
	case CApplication::MODE_TITLE:
		TitleMove();	//動きセット
		break;
	case CApplication::MODE_GAME:
		Move();	//動きセット	
		break;
	case CApplication::MODE_RESULT:
		ResetMove();
		break;
	case CApplication::MODE_RANKING:
		break;
	case CApplication::MODE_TUTORIAL:
		TutorialMove();
		break;
	default:
		break;
	}

	// 現在のモーション番号の保管
	CObjectX::Update();
}

//------------------------------------
// 描画
//------------------------------------
void CPlayer::Draw()
{
	CObjectX::Draw();
}

//------------------------------------
// create
//------------------------------------
CPlayer *CPlayer::Create()
{
	CPlayer * pObject = nullptr;
	pObject = new CPlayer;

	if (pObject != nullptr)
	{
		pObject->Init();
	}

	return pObject;
}

//------------------------------------
// 動きセット
//------------------------------------
void CPlayer::Move()
{
	CInput *CInputpInput = CInput::GetKey();
	D3DXVECTOR3 *Camerarot = CRenderer::GetInstance()->GetCamera()->GetRot();
	float consumption = 0.0f;

	if (CInputpInput->Press(CInput::KEY_RIGHT))
	{
		m_move.x += sinf(D3DX_PI * 0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
		m_move.z += cosf(D3DX_PI * 0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
		//consumption = m_rotMove.x + (D3DX_PI*0.5f) - m_rot.y + Camerarot->y;
	}
	if (CInputpInput->Press(CInput::KEY_LEFT))
	{
		m_move.x += sinf(-D3DX_PI * 0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
		m_move.z += cosf(-D3DX_PI * 0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
	}
	if (CInputpInput->Press(CInput::KEY_DOWN))
	{	
		m_move.y -= m_MoveSpeed;
	}
	if (CInputpInput->Press(CInput::KEY_UP))
	{	
		m_move.y += m_MoveSpeed;
	}
	
	//弾のクリエイト
	if (CInputpInput->Press(CInput::KEY_SHOT))
	{
		
	}


 	else
	{
		m_Pow = 20;
	}

	if (m_pos.y <= -SCREEN_HEIGHT * 0.5f + 20.0f)
	{
		m_pos.y = (-SCREEN_HEIGHT * 0.5f)+20.0f;
	}

	if (m_pos.y >= SCREEN_HEIGHT * 0.5f - 100.0f)
	{
		m_pos.y = SCREEN_HEIGHT * 0.5f - 100.0f;
	}
	if (m_pos.x <= -SCREEN_WIDTH * 0.5f + 100.0f)
	{
		m_pos.x = -SCREEN_WIDTH*0.5f + 100.0f;
	}
	if (m_pos.x >= SCREEN_WIDTH * 0.5f - 80.0f)
	{
		m_pos.x = SCREEN_WIDTH*0.5f - 80.0f;
	}

	m_move.x += (0.0f - m_move.x)*ATTENUATION;	//（目的の値-現在の値）＊減衰係数
	m_move.z += (0.0f - m_move.z)*ATTENUATION;
	m_move.y += (0.0f - m_move.y)*ATTENUATION;

	m_pos += m_move;//移動を加算

	//正規化
	NormalizeAngle(consumption);

	//減算設定（感性）
	m_rot.y += (consumption)* ATTENUATION;//目的の値-現在の値）＊減衰係数

	 //正規化
	NormalizeAngle(m_rot.y);
}

//------------------------------------
// TitleのときのMove
//------------------------------------
void CPlayer::TitleMove()
{

	D3DXVECTOR3 *Camerarot = CRenderer::GetInstance()->GetCamera()->GetRot();
	float consumption = 0.0f;
	
	m_move.x += sinf(-D3DX_PI *0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
	m_move.z += cosf(-D3DX_PI *0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
	//consumption = m_rotMove.x + -(D3DX_PI*0.5f) - m_rot.y + Camerarot->y;
	
	m_pos.y = 250.0f;

	m_move.x += (0.0f - m_move.x)*ATTENUATION;//（目的の値-現在の値）＊減衰係数
	m_move.z += (0.0f - m_move.z)*ATTENUATION;
	m_move.y += (0.0f - m_move.y)*ATTENUATION;

	m_pos += m_move;//移動を加算

	NormalizeAngle(consumption);	//正規化

	//減算設定（感性）
	m_rot.y += (consumption)* ATTENUATION;//目的の値-現在の値）＊減衰係数

	NormalizeAngle(m_rot.y);	//正規化

	if (m_pos.x <= -SCREEN_WIDTH * 0.5f-100.0f)
	{
		m_pos.x = SCREEN_WIDTH * 0.5f;
	}
}

//------------------------------------
// ResetのときのMove
//------------------------------------
void CPlayer::ResetMove()
{

	D3DXVECTOR3 *Camerarot = CRenderer::GetInstance()->GetCamera()->GetRot();
	float consumption = 0.0f;

	m_move.x += sinf(-D3DX_PI *0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
	m_move.z += cosf(-D3DX_PI *0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
	//consumption = m_rotMove.x + -(D3DX_PI*0.5f) - m_rot.y + Camerarot->y;

	m_pos.y = 250.0f;

	m_move.x += (0.0f - m_move.x)*ATTENUATION;//（目的の値-現在の値）＊減衰係数
	m_move.z += (0.0f - m_move.z)*ATTENUATION;
	m_move.y += (0.0f - m_move.y)*ATTENUATION;

	m_pos += m_move;//移動を加算

	NormalizeAngle(consumption);	//正規化

	//減算設定（感性）
	m_rot.y += (consumption) * ATTENUATION;//目的の値-現在の値）＊減衰係数

	NormalizeAngle(m_rot.y);	//正規化

	if (m_pos.x <= -SCREEN_WIDTH*0.5f - 100.0f)
	{
		m_pos.x = SCREEN_WIDTH*0.5f;
	}
}

//------------------------------------
// Move
//------------------------------------
void CPlayer::TutorialMove()	//動きセット
{
	CInput *CInputpInput = CInput::GetKey();
	D3DXVECTOR3 *Camerarot = CRenderer::GetInstance()->GetCamera()->GetRot();
	float consumption = 0.0f;

	if (CInputpInput->Press(CInput::KEY_RIGHT))
	{
		m_move.x += sinf(D3DX_PI * 0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
		m_move.z += cosf(D3DX_PI * 0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
		//consumption = m_rotMove.x + (D3DX_PI*0.5f) - m_rot.y + Camerarot->y;
	}
	if (CInputpInput->Press(CInput::KEY_LEFT))
	{
		m_move.x += sinf(-D3DX_PI * 0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
		m_move.z += cosf(-D3DX_PI * 0.5f + Camerarot->y) * SPEED * m_MoveSpeed;
	}
	if (CInputpInput->Press(CInput::KEY_DOWN))
	{
		m_move.y -= m_MoveSpeed;
	}
	if (CInputpInput->Press(CInput::KEY_UP))
	{
		m_move.y += m_MoveSpeed;
	}

	//弾のクリエイト
	if (CInputpInput->Press(CInput::KEY_SHOT))
	{
	
	}

	if (m_pos.y <= -SCREEN_HEIGHT * 0.5f + 20.0f)
	{
		m_pos.y = (-SCREEN_HEIGHT * 0.5f) + 20.0f;
	}

	if (m_pos.y >= SCREEN_HEIGHT * 0.5f - 100.0f)
	{
		m_pos.y = SCREEN_HEIGHT * 0.5f - 100.0f;
	}
	if (m_pos.x <= -SCREEN_WIDTH * 0.5f + 100.0f)
	{
		m_pos.x = -SCREEN_WIDTH * 0.5f + 100.0f;
	}
	if (m_pos.x >= SCREEN_WIDTH * 0.5f - 80.0f)
	{
		m_pos.x = SCREEN_WIDTH * 0.5f - 80.0f;
	}

	m_move.x += (0.0f - m_move.x) * ATTENUATION;	//（目的の値-現在の値）* 減衰係数
	m_move.z += (0.0f - m_move.z) * ATTENUATION;	//（目的の値-現在の値）* 減衰係数
	m_move.y += (0.0f - m_move.y) * ATTENUATION;	//（目的の値-現在の値）* 減衰係数

	m_pos += m_move;//移動を加算

	NormalizeAngle(consumption);	//正規化

	//減算設定（感性）
	m_rot.y += (consumption)* ATTENUATION;//目的の値-現在の値）＊減衰係数

	NormalizeAngle(m_rot.y);	//正規化
}