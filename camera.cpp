//=============================================================================
//
// カメラ
// Author : Hamada Ryuuga
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "application.h"

#include "camera.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera(CTaskGroup::EPriority inPriority) :
	CTask(inPriority,CTaskGroup::EPushMethod::PUSH_TOP)
{
	SetRole(ROLE_CAMERA);
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CCamera::Init(void)
{
	// 視点　注視点　上方向　設定
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CCamera::Update(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CCamera::Draw()
{
	Set(0);
}

//=============================================================================
// 描画
//=============================================================================
void CCamera::Set(int Type)
{
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
			D3DXToRadian(90.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f,
			20000.0f);
	}
	else
	{
		// プロジェクションマトリックスの作成(平行投影)
		D3DXMatrixOrthoLH(&m_MtxProject,					// プロジェクションマトリックス
			(float)SCREEN_WIDTH,							// 幅
			(float)SCREEN_HEIGHT,							// 高さ
			-100.0f,										// ニア
			2000.0f);										// ファー
	}

	// 適用
	pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProject);
}

//=============================================================================
// カメラの向きに合わせたベクトルに変換する
//=============================================================================
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
