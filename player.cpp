//============================
//
// プレイヤー設定
// Author:Yuda Kaito
//
//============================
#include <assert.h>
#include "player.h"
#include "input.h"
#include "camera.h"
#include "application.h"
#include "particle_manager.h"
#include "utility.h"

//------------------------------------
// static変数
//------------------------------------
const float CPlayer::ATTENUATION = 0.5f;	// 移動減衰係数
const float CPlayer::SPEED = 1.0f;			// 移動量

//------------------------------------
// コンストラクタ
//------------------------------------
CPlayer::CPlayer() :
	m_quaternion(D3DXQUATERNION(0.0f,0.0,0.0f,1.0f))
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
	LoadModel("BOX");

	m_MoveSpeed = 7.0f;
	m_rot.y += -D3DX_PI * 0.5f;

	D3DXVECTOR3	Size(2.0f, 2.0f, 2.0f);
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
	Move();	// 移動
	Jump();	// ジャンプ

	CInput* input = CInput::GetKey();
	if (input->Trigger(DIK_0))
	{
		SetMaterialDiffuse(0, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	if (input->Trigger(DIK_1))
	{
		SetMaterialDiffuse(0, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
	if (input->Trigger(DIK_2))
	{
		SetMaterialDiffuse(0, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}
	if (input->Trigger(DIK_3))
	{
		SetMaterialDiffuse(0, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	}
	static D3DXCOLOR color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	if (input->Press(DIK_4))
	{
		color.r += 0.01f;
		SetMaterialDiffuse(0, color);
	}
	if (input->Press(DIK_5))
	{
		color.r -= 0.01f;
		SetMaterialDiffuse(0, color);
	}
}

//------------------------------------
// 描画
//------------------------------------
void CPlayer::Draw()
{
	CObjectX::Draw(m_quaternion);
}

//------------------------------------
// create
//------------------------------------
CPlayer *CPlayer::Create()
{
	CPlayer * player = new CPlayer;

	if (player != nullptr)
	{
		player->Init();
	}

	return player;
}

//------------------------------------
// 動きセット
//------------------------------------
void CPlayer::Move()
{
	CInput* input = CInput::GetKey();
	D3DXVECTOR3 moveInput = D3DXVECTOR3(0.0f,0.0f,0.0f);

	if (input->Press(KEY_RIGHT))
	{
		moveInput.x += 1.0f;
	}
	if (input->Press(KEY_LEFT))
	{
		moveInput.x -= 1.0f;
	}
	if (input->Press(KEY_DOWN))
	{	
		moveInput.z -= 1.0f;
	}
	if (input->Press(KEY_UP))
	{	
		moveInput.z += 1.0f;
	}

	moveInput = CRenderer::GetInstance()->GetCamera()->VectorCombinedRot(moveInput);

	m_move.x = moveInput.x * SPEED * m_MoveSpeed;
	m_move.z = moveInput.z * SPEED * m_MoveSpeed;

	//（目的の値-現在の値）＊減衰係数
	m_move.x += (0.0f - m_move.x) * ATTENUATION;
	m_move.z += (0.0f - m_move.z) * ATTENUATION;

	D3DXVECTOR3 axis;	// 回転軸
	D3DXVECTOR3 inverseVec = -m_move;					// move値を反対にする
	D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&axis, &inverseVec, &vecY);			// 外積で回転軸を算出。

	// クオータニオンの計算
	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationAxis(&quaternion, &axis, 0.15f);	// 回転軸と回転角度を指定

	// クオータニオンを適用
	m_quaternion *= quaternion;

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
}

void CPlayer::Jump()
{
	if (CInput::GetKey()->Trigger(DIK_SPACE))
	{
		m_move.y = 20.0f;
	}
	m_move.y -= m_move.y * ATTENUATION;
}
