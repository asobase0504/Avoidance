//=============================================================================
//
// プレイヤー設定
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "player.h"
#include "input.h"
#include "camera.h"
#include "application.h"
#include "particle_manager.h"
#include "utility.h"
#include "collision.h"
#include "object_polygon3d.h"

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const float CPlayer::SPEED = 6.5f;			// 移動量
const float CPlayer::ATTENUATION = 0.50f;	// 移動減衰係数
const float CPlayer::JUMPING_POWER = 1.5f;	// 跳躍力
const float CPlayer::GRAVITY = 0.95f;		// 重力

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayer::CPlayer() :
	m_quaternion(D3DXQUATERNION(0.0f,0.0,0.0f,1.0f)),
	m_jumpCount(0),
	m_isGoal(false)
{
	SetType(CObject::EType::PLAYER);
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CPlayer::Init()
{
	// 現在のモーション番号の保管
	CObjectX::Init();
	LoadModel("BOX");
	SetScale(D3DXVECTOR3(1.0f,1.0f,1.0f));
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CPlayer::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayer::NormalUpdate()
{
	Move();		// 移動
	boost();	// 突進
	Jump();		// ジャンプ
	Landing();	// 落下
	OnHitGoal();	// Goalとの当たり判定
	OnHitEnemy();

	CInput* input = CInput::GetKey();

	static D3DXCOLOR color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	if (m_isGoal)
	{
		color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		SetMaterialDiffuse(0, color);
	}
	if (!m_isGoal)
	{
		color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		SetMaterialDiffuse(0, color);
	}
	if (input->Trigger(DIK_2))
	{
		color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		SetMaterialDiffuse(0, color);
	}
	if (input->Trigger(DIK_3))
	{
		color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		SetMaterialDiffuse(0, color);
	}
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

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CPlayer::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------
CPlayer* CPlayer::Create()
{
	CPlayer* player = new CPlayer;

	if (player != nullptr)
	{
		player->Init();
	}

	return player;
}

//-----------------------------------------------------------------------------
// 移動
//-----------------------------------------------------------------------------
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

	CCamera* camera = (CCamera*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(ROLE_CAMERA, CTaskGroup::LEVEL_3D_1);

	if (camera != nullptr)
	{
		moveInput = camera->VectorCombinedRot(moveInput);
	}

	if (D3DXVec3Length(&moveInput) != 0.0f)
	{
		m_move.x = moveInput.x * SPEED;
		m_move.z = moveInput.z * SPEED;
	}

	//（目的の値 - 現在の値） ＊ 減衰係数
	m_move.x += (0.0f - m_move.x) * ATTENUATION;
	m_move.z += (0.0f - m_move.z) * ATTENUATION;

	D3DXVECTOR2 horizontalAxis(m_move.x, m_move.z);

	D3DXVECTOR3 axis;	// 回転軸
	D3DXVECTOR3 inverseVec = -m_move;					// move値を反対にする
	D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&axis, &inverseVec, &vecY);			// 外積で回転軸を算出。

	// クオータニオンの計算
	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationAxis(&quaternion, &axis, D3DXVec2Length(&horizontalAxis) * 0.035f);	// 回転軸と回転角度を指定

	// クオータニオンを適用
	m_quaternion *= quaternion;

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	SetMtxQuaternion(m_quaternion);

	D3DXMATRIX mtrixMR = GetMatRot();
	D3DXMATRIX mtrixQ;
	D3DXMatrixRotationQuaternion(&mtrixQ, &m_quaternion);
	D3DXMATRIX mtrixR;
	D3DXMatrixRotationYawPitchRoll(&mtrixR, m_rot.y, m_rot.x, m_rot.z);	// 行列回転関数
}

//-----------------------------------------------------------------------------
// 跳躍
//-----------------------------------------------------------------------------
void CPlayer::Jump()
{
	// 跳躍
	if (CInput::GetKey()->Trigger(DIK_SPACE) && (m_jumpCount == 0))
	{
		m_jumpCount++;
		m_move.y = 15.0f;
	}
}

//-----------------------------------------------------------------------------
// 突進
//-----------------------------------------------------------------------------
void CPlayer::boost()
{
	// 突進
	if (CInput::GetKey()->Trigger(DIK_SPACE) && (m_jumpCount == 1))
	{
		m_jumpCount++;

		m_move.y *= 2.0f;

		m_move.x *= 7.0f;
		m_move.z *= 7.0f;
	}
}

//-----------------------------------------------------------------------------
// 落下
//-----------------------------------------------------------------------------
void CPlayer::Landing()
{

	if (OnHitPolygon())
	{
		//m_move.y = 0.0f;
	}
	else
	{
		//m_move.y -= GRAVITY;		// 重力
	}

	//// 疑似的な床表現
	//if (m_pos.y + m_move.y <= 15.0f)
	//{
	//	m_jumpCount = 0;
	//	m_move.y = 0.0f;
	//}
}

//-----------------------------------------------------------------------------
// Goalとの当たり判定
//-----------------------------------------------------------------------------
void CPlayer::OnHitGoal()
{
	CObject* object = SearchType(CObject::EType::GOAL, CTaskGroup::EPriority::LEVEL_3D_1);

	float dist = 0.0f;

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();
		
		if (OBBAndOBB((CObjectX*)object, &dist))
		{
			m_isGoal = true;	// Goal
			D3DXVECTOR3 move = GetMove();
			move.y = 0.0f;
			move *= -1.0f;
			D3DXVec3Normalize(&move, &move);
			SetMove(move * dist);
		}

		object = next;
	}
}

//-----------------------------------------------------------------------------
// Enemyとの当たり判定
//-----------------------------------------------------------------------------
void CPlayer::OnHitEnemy()
{
	CObject* object = SearchType(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1);

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();

		if (OBBAndOBB((CObjectX*)object))
		{
			m_isGoal = true;	// Goal
		}

		object = next;
	}
}

//-----------------------------------------------------------------------------
// Polygonとの当たり判定
//-----------------------------------------------------------------------------
bool CPlayer::OnHitPolygon()
{
	// 最初に見つけた指定したタイプのobjectを持ってくる
	CObject* object = SearchType(CObject::EType::POLYGON, CTaskGroup::EPriority::LEVEL_3D_1);

	float length;
	bool isHit = false;

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();	// 同じタイプのobjectを持ってくる

		CObjectPolygon3D* objectPolygon = (CObjectPolygon3D*)object;	// 変換

		if (OBBAndPolygon(objectPolygon, &length))
		{
			AddPos(objectPolygon->GetNormal() * length);
			isHit = true;	// Goal
		}

		object = next;
	}
	return isHit;
}
