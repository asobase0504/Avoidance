//=============================================================================
//
// プレイヤー設定
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "player.h"
#include "player_afterimage.h"
#include "player_afterimage_fall.h"
#include "player_died.h"

#include "input.h"
#include "camera.h"
#include "application.h"
#include "utility.h"
#include "collision.h"
#include "goal.h"
#include "color.h"

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const float CPlayer::SPEED = 6.5f;			// 移動量
const float CPlayer::ATTENUATION = 0.45f;	// 移動減衰係数
const float CPlayer::JUMPING_POWER = 8.5f;	// 跳躍力
const float CPlayer::GRAVITY = 0.15f;		// 重力

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayer::CPlayer() :
	m_quaternion(0.0f,0.0,0.0f,1.0f),
	m_quaternionOld(0.0f, 0.0, 0.0f, 1.0f),
	m_isGoal(false),
	m_isMove(false),
	jumpDirection(0.0f, 0.0, 0.0f),
	m_isJump(false),
	m_jumpCount(0),
	m_jumpTime(0),
	m_isDied(false)
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
	LoadModel("BOX");

	CObjectX::Init();

	AttachOutLine();

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
	// デバッグ
	CDebugProc::Print("jumpDirection : %.2f,%.2f,%.2f\n", jumpDirection.x, jumpDirection.y, jumpDirection.z);
	CDebugProc::Print("quaternion  : %.2f,%.2f,%.2f\n", m_quaternion.x, m_quaternion.y, m_quaternion.z);
	CDebugProc::Print("quaternionOld  : %.2f,%.2f,%.2f\n", m_quaternionOld.x, m_quaternionOld.y, m_quaternionOld.z);
	CDebugProc::Print("pos  : %.2f,%.2f,%.2f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("move : %.2f,%.2f,%.2f\n", m_move.x, m_move.y, m_move.z);

	if (!m_isDied)
	{
		m_quaternionOld = m_quaternion;
		Move();			// 移動
		boost();		// 突進
		Jump();			// ジャンプ
		Landing();		// 落下
		OnHitGoal();	// Goalとの当たり判定
		OnHitEnemy();	// Enemyとの当たり判定

		NextStageWait();

		CInput* input = CInput::GetKey();

#ifdef _DEBUG
		if (input->Press(DIK_0))
		{
			SetPos(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
			SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		if (input->Press(DIK_9))
		{
			SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
#endif // _DEBUG

		// 何かの拍子で下に行った場合の対処法
		if (m_pos.y < -5000.0f)
		{
			SetPos(D3DXVECTOR3(m_pos.x, 1200.0f, m_pos.z));
		}

		// 軌跡パーティクル
		static int time = 0;
		time++;
		if (m_isGoal)
		{
			// 落下軌跡
			//if (time % 2 == 0)
			{
				time = 0;
				CPlayerAfterimageFall* afterimage = CPlayerAfterimageFall::Create(m_pos);
				afterimage->SetMtxRot(GetMtxRot());
				afterimage->SetMaterialDiffuse(0, GetMaterialDiffuse(0));
			}
		}
		else
		{
			// 移動軌跡
			if (time % 2 == 0)
			{
				time = 0;
				CPlayerAfterimage* afterimage = CPlayerAfterimage::Create(m_pos);
				afterimage->SetMtxRot(GetMtxRot());
				afterimage->SetMaterialDiffuse(0, GetMaterialDiffuse(0));
			}
		}
	}
	else
	{
		static int cnt = 0;

		cnt++;
		if (cnt >= CPlayerDied::MAX_LIFE)
		{
			cnt = 0;
			m_isDied = false;
		}
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CPlayer::Draw()
{
	if (!m_isDied)
	{
		CObjectX::Draw();
	}
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
	if (!m_isMove)
	{
		return;
	}

	CInput* input = CInput::GetKey();
	D3DXVECTOR3 moveInput = D3DXVECTOR3(0.0f,0.0f,0.0f);

	// キーボード入力
	if (input->Press(DIK_D))
	{
		moveInput.x += 1.0f;
	}
	if (input->Press(DIK_A))
	{
		moveInput.x -= 1.0f;
	}
	if (input->Press(DIK_S))
	{	
		moveInput.z -= 1.0f;
	}
	if (input->Press(DIK_W))
	{	
		moveInput.z += 1.0f;
	}

	// JoyPadの入力があった場合はJoyPadに変更する
	D3DXVECTOR3 joypadInput = input->VectorMoveJoyStick(false);
	if (D3DXVec3LengthSq(&joypadInput) != 0.0f)
	{
		moveInput.x = joypadInput.x;
		moveInput.z = joypadInput.y * -1.0f;
	}

	// カメラの向きをInputに反映
	CCamera* camera = (CCamera*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(ROLE_CAMERA, CTaskGroup::LEVEL_3D_1);
	if (camera != nullptr)
	{
		moveInput = camera->VectorCombinedRot(moveInput);
	}

	// 入力があった場合移動量に反映
	if (D3DXVec3Length(&moveInput) != 0.0f)
	{
		if (fabs(m_move.x) < fabs(moveInput.x * SPEED))
		{
			m_move.x = moveInput.x * SPEED;
		}

		if (fabs(m_move.z) < fabs(moveInput.z * SPEED))
		{
			m_move.z = moveInput.z * SPEED;
		}
	}

	//（目的の値 - 現在の値） ＊ 減衰係数
	m_move.x += (0.0f - m_move.x) * ATTENUATION;
	m_move.z += (0.0f - m_move.z) * ATTENUATION;

	// 移動量に合わせて回転
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

	// 回転行列に反映
	SetMtxQuaternion(m_quaternion);
}

//-----------------------------------------------------------------------------
// 跳躍
//-----------------------------------------------------------------------------
void CPlayer::Jump()
{
	if (!m_isMove)
	{
		return;
	}

	// 跳躍
	if (CInput::GetKey()->Trigger(DIK_SPACE) && (m_jumpCount == 0))
	{
		m_jumpCount++;
		m_isJump = true;

		//jumpDirection.x *= 6.0f;
		//jumpDirection.z *= 6.0f;
		m_pos += 1.0f * jumpDirection;
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_move += JUMPING_POWER * jumpDirection;
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
		m_move.x *= 7.0f;
		m_move.z *= 7.0f;
	}
}

//-----------------------------------------------------------------------------
// 落下
//-----------------------------------------------------------------------------
void CPlayer::Landing()
{
	m_move.y -= GRAVITY;	// 重力

	if (!OnHitPlain())
	{
		jumpDirection = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
}

//-----------------------------------------------------------------------------
// 次のステージまで待機させる
//-----------------------------------------------------------------------------
void CPlayer::NextStageWait()
{
	if (!m_isGoal)
	{
		return;
	}
}

//-----------------------------------------------------------------------------
// Goalとの当たり判定
//-----------------------------------------------------------------------------
void CPlayer::OnHitGoal()
{
	// 指定したタイプを全て検索して指定した関数を呼ぶ
	TypeAllFunc(CObject::EType::GOAL, CTaskGroup::EPriority::LEVEL_3D_1, [this](CObject* inObject)
	{
		if (OBBAndOBB((CObjectX*)inObject))
		{
			CGoal* goal = (CGoal*)inObject;	// Goal
			goal->Goal(true);
			m_isGoal = true;
		}
	});
}

//-----------------------------------------------------------------------------
// Enemyとの当たり判定
//-----------------------------------------------------------------------------
void CPlayer::OnHitEnemy()
{
	// 指定したタイプを全て検索して指定した関数を呼ぶ
	TypeAllFunc(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1, [this](CObject* inObject)
	{
		if (m_isDied)
		{
			return;
		}

		if (OBBAndOBB((CObjectX*)inObject))
		{
			// 死亡演出
			for (int i = 0; i < 50; i++)
			{
				D3DXVECTOR3 pos = m_pos;
				pos.x += FloatRandom(-20.0f, 20.0f);
				pos.y += FloatRandom(-20.0f, 20.0f);
				pos.z += FloatRandom(-20.0f, 20.0f);
				CPlayerDied::Create(pos);
			}

			SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//CApplication::GetInstance()->Delay(60, 2);
			m_isDied = true;	// Goal
		}
	});
}

//-----------------------------------------------------------------------------
// Plainとの当たり判定
//-----------------------------------------------------------------------------
bool CPlayer::OnHitPlain()
{
	bool hit = false;
	D3DXVECTOR3 dist(0.0f, 0.0f, 0.0f);

	// 指定したタイプを全て検索して指定した関数を呼ぶ
	TypeAllFunc(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1, [this, &dist, &hit](CObject* inObject)
	{
		dist = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (SphereAndAABB((CObjectX*)inObject, &dist))
		{
			jumpDirection = -dist;
			jumpDirection.y += 2.0f;
			D3DXVec3Normalize(&jumpDirection, &jumpDirection);
			m_jumpCount = 0;
			//m_pos = m_posOld;

			if (dist.y < 0.0f)
			{
				m_move.y = 0.0f;
			}

			hit = true;
		}
	});

	return hit;
}
