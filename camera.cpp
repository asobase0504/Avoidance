//=============================================================================
//
// カメラ
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "application.h"
#include "camera.h"
#include "utility.h"
#include "delay_process.h"


//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const float CCamera::VIEWING_ANGLE = 55.0f;	// 視野角

//-----------------------------------------------------------------------------
// コンストラクタ
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CCamera::CCamera(CTaskGroup::EPriority inPriority) :
	CTask(inPriority,CTaskGroup::EPushMethod::PUSH_TOP)
{
	SetRole(ROLE_CAMERA);
}

//-----------------------------------------------------------------------------
// デストラクタ
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CCamera::~CCamera()
{
}

//-----------------------------------------------------------------------------
// 初期化
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CCamera::Init(void)
{
	// 視点　注視点　上方向　設定
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CCamera::Uninit(void)
{
}

//-----------------------------------------------------------------------------
// 更新
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CCamera::Update(void)
{
}

//-----------------------------------------------------------------------------
// 描画
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CCamera::Draw()
{
	Set(0);
}

//-----------------------------------------------------------------------------
// 描画
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CCamera::Set(int Type)
{
	// 揺れカウンターを減らす
	m_nCntFrame--;

	if (m_nCntFrame >= 0)
	{
		D3DXVECTOR3 adjustPos = {};

		adjustPos.x = FloatRandom(m_Magnitude, -m_Magnitude);
		adjustPos.y = FloatRandom(m_Magnitude, -m_Magnitude);
		adjustPos.z = FloatRandom(m_Magnitude, -m_Magnitude);

		m_posV += adjustPos;
		m_posR += adjustPos;
	}

	m_Type = Type;
	LPDIRECT3DDEVICE9  pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// ビューマトリックスを初期化
	D3DXMatrixIdentity(&m_MtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_MtxView, &m_posV, &m_posR, &m_vecU);

	// 適用
	pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);

	// プロジェクションマトリックスを初期化
	D3DXMatrixIdentity(&m_MtxProject);

	if (Type == 0)
	{
		//プロジェクションマトリックス作成
		D3DXMatrixPerspectiveFovLH(&m_MtxProject,
			D3DXToRadian(VIEWING_ANGLE),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f,
			20000.0f);
	}
	else
	{
		// プロジェクションマトリックスの作成(平行投影)
		D3DXMatrixOrthoLH(&m_MtxProject,	// プロジェクションマトリックス
			(float)SCREEN_WIDTH,			// 幅
			(float)SCREEN_HEIGHT,			// 高さ
			-100.0f,						// ニア
			2000.0f);						// ファー
	}

	// 適用
	pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProject);
}

//-----------------------------------------------------------------------------
// カメラの向きに合わせたベクトルに変換する
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
const D3DXVECTOR3 & CCamera::VectorCombinedRot(const D3DXVECTOR3& inVector)
{
	if (D3DXVec3Length(&inVector) <= 0.0f)
	{
		return inVector;
	}

	D3DXVECTOR3 vector = inVector;

	D3DXVec3Normalize(&vector, &vector);

	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXVec3TransformCoord(&vector, &vector, &mtxRot);

	vector.y = 0.0f;

	D3DXVec3Normalize(&vector, &vector);

	return vector;
}

//-----------------------------------------------------------------------------
// 揺れを設定する
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CCamera::Shake(float ShakeFrame, float Magnitude)
{
	m_nCntFrame = ShakeFrame;	// 揺れるフレームカウント
	m_Magnitude = Magnitude;	// 揺れ
}
