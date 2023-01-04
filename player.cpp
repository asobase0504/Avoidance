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
#include "player_afterimage.h"
#include "goal.h"

#include "color.h"
//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const float CPlayer::SPEED = 6.5f;			// 移動量
const float CPlayer::ATTENUATION = 0.25f;	// 移動減衰係数
const float CPlayer::JUMPING_POWER = 1.5f;	// 跳躍力
const float CPlayer::GRAVITY = 0.75f;		// 重力

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayer::CPlayer() :
	m_quaternion(D3DXQUATERNION(0.0f,0.0,0.0f,1.0f)),
	m_quaternionOld(D3DXQUATERNION(0.0f, 0.0, 0.0f, 1.0f)),
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
	m_quaternionOld = m_quaternion;
	Move();		// 移動
	boost();	// 突進
	Jump();		// ジャンプ
	//Landing();	// 落下
	OnHitGoal();	// Goalとの当たり判定
	OnHitEnemy();

	CInput* input = CInput::GetKey();

	if (input->Press(DIK_0))
	{
		SetPos(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	if (input->Press(DIK_9))
	{
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (m_pos.y < -5000.0f)
	{
		SetPos(D3DXVECTOR3(m_pos.x, 1200.0f, m_pos.z));
	}

	CDebugProc::Print("quaternion  : %.2f,%.2f,%.2f\n", m_quaternion.x, m_quaternion.y, m_quaternion.z);
	CDebugProc::Print("quaternionOld  : %.2f,%.2f,%.2f\n", m_quaternionOld.x, m_quaternionOld.y, m_quaternionOld.z);
	CDebugProc::Print("pos  : %.2f,%.2f,%.2f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("move : %.2f,%.2f,%.2f\n", m_move.x, m_move.y, m_move.z);

	static int time = 0;
	time++;
	if (time % 1 == 0)
	{
		CPlayerAfterimage* afterimage = CPlayerAfterimage::Create(m_pos);
		afterimage->SetMtxQuaternion(m_quaternion);
		afterimage->SetMaterialDiffuse(0,GetMaterialDiffuse(0));
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
		//m_pos.y = m_posOld.y;
	}
	else
	{
		m_move.y -= GRAVITY;	// 重力
	}
}

//-----------------------------------------------------------------------------
// Goalとの当たり判定
//-----------------------------------------------------------------------------
void CPlayer::OnHitGoal()
{
	CObject* object = SearchType(CObject::EType::GOAL, CTaskGroup::EPriority::LEVEL_3D_1);

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();
		
		if (OBBAndOBB((CObjectX*)object))
		{
			CGoal* goal = (CGoal*)object;	// Goal
			goal->Goal(true);
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
			//m_isGoal = true;	// Goal
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
	CObject* object = SearchType(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1);

	bool hit = false;

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();	// 同じタイプのobjectを持ってくる

		CObjectX* objectX = (CObjectX*)object;	// 変換

		if (OBBAndOBB(objectX))
		{
			D3DXVECTOR3 move = m_posOld - m_pos;
			CDebugProc::Print("dist : %.2f,%.2f,%.2f\n", move.x, move.y, move.z);

			if (D3DXVec3Length(&(m_posOld - m_pos)) == 0.0f)
			{
				AddMove(m_move);
			}
			else
			{
				AddMove(m_posOld - m_pos);
			}

			AddPos(GetMove());
			SetMtxQuaternion(m_quaternionOld);

			if (m_move.y < -20.0f)
			{
				m_move.y = 0.0f;
			}
			hit = true;
		}
		object = next;
	}
	return hit;
}
