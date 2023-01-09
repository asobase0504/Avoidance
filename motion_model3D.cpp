//=============================================================================
//
// モーションキャラクター3Dクラス(model3D.h)
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3D生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "motion_model3D.h"
#include "motion.h"
#include "renderer.h"
#include "application.h"
#include "objectX.h"
#include "parts.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3Dを生成する
//=============================================================================
CMotionModel3D * CMotionModel3D::Create()
{
	// オブジェクトインスタンス
	CMotionModel3D *pMotionModel3D = nullptr;

	// メモリの解放
	pMotionModel3D = new CMotionModel3D;

	// メモリの確保ができなかった
	assert(pMotionModel3D != nullptr);

	// 数値の初期化
	pMotionModel3D->Init();

	// インスタンスを返す
	return pMotionModel3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CMotionModel3D::CMotionModel3D(CTaskGroup::EPriority nPriority) :
	CObjectX(nPriority),
	m_pMotion(nullptr),			// モーション情報
	m_posOld(D3DXVECTOR3()),	// 過去位置
	m_isUpdateStop(false)
{
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CMotionModel3D::~CMotionModel3D()
{
	assert(m_pMotion == nullptr);
}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CMotionModel3D::Init()
{
	// 変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 過去位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CMotionModel3D::Uninit()
{
	if (m_pMotion != nullptr)
	{// 終了処理
		m_pMotion->Uninit();

		// メモリの解放
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// オブジェクト2Dの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CMotionModel3D::NormalUpdate()
{
	if (m_pMotion != nullptr)
	{// モーション番号の設定
		if(!m_isUpdateStop)
		{
			m_pMotion->Update();
		}
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// Author : Yuda Kaito
// 概要 : 2D描画を行う
//=============================================================================
void CMotionModel3D::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxWorld = GetMtxWorld();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);											// 行列初期化関数

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// 行列回転関数
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);						// 行列掛け算関数 

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);			// 行列移動関数
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);					// 行列掛け算関数

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	SetMtxWorld(mtxWorld);

	if (m_pMotion != nullptr)
	{// パーツの描画設定
		
		for (int i = 0; i < m_pMotion->GetMaxParts(); i++)
		{
			if (m_pMotion->GetParts(i)->GetParent() == nullptr)
			{
				m_pMotion->GetParts(i)->SetParent(this);
			}
		}
	}
}

//=============================================================================
// モーションの設定
// Author : 唐﨑結斗
// 概要 : モーションの読み込みを行う
//=============================================================================
void CMotionModel3D::SetMotion(const char * pName)
{
	if (m_pMotion != nullptr)
	{// 終了処理
		m_pMotion->Uninit();

		// メモリの解放
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// モーション情報
	m_pMotion = new CMotion(pName);
	assert(m_pMotion != nullptr);

	// モーションの初期設定
	m_pMotion->SetMotion(0);

	// モーション番号の設定
	m_pMotion->SetNumMotion(0);

	SetMaxMinVtx();
}


void CMotionModel3D::SetMaxMinVtx()
{

	D3DXVECTOR3 minVtx(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 maxVtx(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (int nCntParts = 0; nCntParts < m_pMotion->GetMaxParts(); nCntParts++)
	{
		D3DXVECTOR3 partsMinVtx = m_pMotion->GetParts(nCntParts)->GetMinVtx();
		D3DXVECTOR3 partsMaxVtx = m_pMotion->GetParts(nCntParts)->GetMaxVtx();

		//X
		if (partsMinVtx.x < minVtx.x)
		{//最小値
			minVtx.x = partsMinVtx.x;
		}
		if (partsMaxVtx.x > maxVtx.x)
		{//最大値
			maxVtx.x = partsMaxVtx.x;
		}

		//Y
		if (partsMinVtx.y < minVtx.y)
		{//最小値
			minVtx.y = partsMinVtx.y;
		}
		if (partsMaxVtx.y > maxVtx.y)
		{//最大値
			maxVtx.y = partsMaxVtx.y;
		}

		//Z
		if (partsMinVtx.z < minVtx.z)
		{//最小値
			minVtx.z = partsMinVtx.z;
		}
		if (partsMaxVtx.z > maxVtx.z)
		{//最大値
			maxVtx.z = partsMaxVtx.z;
		}
	}

	SetMinVtx(minVtx);
	SetMaxVtx(maxVtx);
}

//=============================================================================
// 線分の当たり判定
//=============================================================================
bool CMotionModel3D::SegmentCollision(CObjectX* inObjectX)
{
	if (!inObjectX->IsCollision())
	{
		return false;
	}

	// 変数宣言

	D3DXVECTOR3 interval = GetPos() - inObjectX->GetPos();

	D3DXVECTOR3 thisVecX;
	D3DXVECTOR3 thisVecY;
	D3DXVECTOR3 thisVecZ;
	D3DXVECTOR3 thisNormalizeVecX;
	D3DXVECTOR3 thisNormalizeVecY;
	D3DXVECTOR3 thisNormalizeVecZ;

	{
		// 計算用マトリックス
		D3DXMATRIX mtxRot;

		D3DXMATRIX mtxWorld;
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);	// 行列回転関数
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);				// 行列掛け算関数 

		thisVecX = D3DXVECTOR3(m_size.x, 0.0f, 0.0f);
		thisVecY = D3DXVECTOR3(0.0f, m_size.y, 0.0f);
		thisVecZ = D3DXVECTOR3(0.0f, 0.0f, m_size.z);

		D3DXVec3TransformCoord(&thisVecX, &thisVecX, &mtxWorld);
		D3DXVec3TransformCoord(&thisVecY, &thisVecY, &mtxWorld);
		D3DXVec3TransformCoord(&thisVecZ, &thisVecZ, &mtxWorld);

		D3DXVec3Normalize(&thisNormalizeVecX, &thisVecX);
		D3DXVec3Normalize(&thisNormalizeVecY, &thisVecY);
		D3DXVec3Normalize(&thisNormalizeVecZ, &thisVecZ);
	}

	D3DXVECTOR3 targetVecX;
	D3DXVECTOR3 targetVecY;
	D3DXVECTOR3 targetVecZ;

	D3DXVECTOR3 targetNormalizeVecX;
	D3DXVECTOR3 targetNormalizeVecY;
	D3DXVECTOR3 targetNormalizeVecZ;

	{
		// 計算用マトリックス
		D3DXMATRIX mtxRot;

		D3DXMATRIX mtxWorld;
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		D3DXVECTOR3 rot = inObjectX->GetRot();
		D3DXVECTOR3 size = inObjectX->GetSize();

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);	// 行列回転関数
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);			// 行列掛け算関数 

		targetVecX = D3DXVECTOR3(size.x, 0.0f, 0.0f);
		targetVecY = D3DXVECTOR3(0.0f, size.y, 0.0f);
		targetVecZ = D3DXVECTOR3(0.0f, 0.0f, size.z);
		D3DXVec3TransformCoord(&targetVecX, &targetVecX, &mtxWorld);
		D3DXVec3TransformCoord(&targetVecY, &targetVecY, &mtxWorld);
		D3DXVec3TransformCoord(&targetVecZ, &targetVecZ, &mtxWorld);
		D3DXVec3Normalize(&targetNormalizeVecX, &targetVecX);
		D3DXVec3Normalize(&targetNormalizeVecY, &targetVecY);
		D3DXVec3Normalize(&targetNormalizeVecZ, &targetVecZ);
	}

	float thisRadius;
	float targetRadius;
	float length;

	//A.e1
	thisRadius = D3DXVec3Length(&thisVecX);
	targetRadius = LenSegOnSeparateAxis(&thisNormalizeVecX, &targetVecX, &targetVecY, &targetVecZ);
	length = fabs(D3DXVec3Dot(&interval, &thisNormalizeVecX));
	if (length > thisRadius + targetRadius)
	{
		return false;
	}

	//A.e2
	thisRadius = D3DXVec3Length(&thisVecY);
	targetRadius = LenSegOnSeparateAxis(&thisNormalizeVecY, &targetVecX, &targetVecY, &targetVecZ);
	length = fabs(D3DXVec3Dot(&interval, &thisNormalizeVecY));
	if (length > thisRadius + targetRadius)
	{
		return false;
	}

	//A.e3
	thisRadius = D3DXVec3Length(&thisVecZ);
	targetRadius = LenSegOnSeparateAxis(&thisNormalizeVecZ, &targetVecX, &targetVecY, &targetVecZ);
	length = fabs(D3DXVec3Dot(&interval, &thisNormalizeVecZ));
	if (length > thisRadius + targetRadius)
	{
		return false;
	}

	//B.e1
	thisRadius = D3DXVec3Length(&targetVecX);
	targetRadius = LenSegOnSeparateAxis(&targetNormalizeVecX, &thisVecX, &thisVecY, &thisVecZ);
	length = fabs(D3DXVec3Dot(&interval, &targetNormalizeVecX));
	if (length > thisRadius + targetRadius)
	{
		return false;
	}

	//B.e2
	thisRadius = D3DXVec3Length(&targetVecY);
	targetRadius = LenSegOnSeparateAxis(&targetNormalizeVecY, &thisVecX, &thisVecY, &thisVecZ);
	length = fabs(D3DXVec3Dot(&interval, &targetNormalizeVecY));
	if (length > thisRadius + targetRadius)
	{
		return false;
	}

	//B.e3
	thisRadius = D3DXVec3Length(&targetVecZ);
	targetRadius = LenSegOnSeparateAxis(&targetNormalizeVecZ, &thisVecX, &thisVecY, &thisVecZ);
	length = fabs(D3DXVec3Dot(&interval, &targetNormalizeVecZ));
	if (length > thisRadius + targetRadius)
	{
		return false;
	}

	//C11
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &thisNormalizeVecX, &targetNormalizeVecX);
		thisRadius = LenSegOnSeparateAxis(&Cross, &thisVecY, &thisVecZ);
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecY, &targetVecZ);
		length = fabs(D3DXVec3Dot(&interval, &Cross));
		if (length > thisRadius + targetRadius)
		{
			return false;
		}
	}

	//C12
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &thisNormalizeVecX, &targetNormalizeVecY);
		thisRadius = LenSegOnSeparateAxis(&Cross, &thisVecY, &thisVecZ);
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecX, &targetVecZ);
		length = fabs(D3DXVec3Dot(&interval, &Cross));
		if (length > thisRadius + targetRadius)
		{
			return false;
		}
	}

	//C13
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &thisNormalizeVecX, &targetNormalizeVecZ);
		thisRadius = LenSegOnSeparateAxis(&Cross, &thisVecY, &thisVecZ);
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecX, &targetVecZ);
		length = fabs(D3DXVec3Dot(&interval, &Cross));
		if (length > thisRadius + targetRadius)
		{
			return false;
		}
	}

	//C21
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &thisNormalizeVecY, &targetNormalizeVecX);
		thisRadius = LenSegOnSeparateAxis(&Cross, &thisVecX, &thisVecZ);
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecY, &targetVecZ);
		length = fabs(D3DXVec3Dot(&interval, &Cross));
		if (length > thisRadius + targetRadius)
		{
			return false;
		}
	}

	//C22
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &thisNormalizeVecY, &targetNormalizeVecY);
		thisRadius = LenSegOnSeparateAxis(&Cross, &thisVecX, &thisVecZ);
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecX, &targetVecZ);
		length = fabs(D3DXVec3Dot(&interval, &Cross));
		if (length > thisRadius + targetRadius)
		{
			return false;
		}
	}

	//C23
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &thisNormalizeVecY, &targetNormalizeVecZ);
		thisRadius = LenSegOnSeparateAxis(&Cross, &thisVecX, &thisVecZ);
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecX, &targetVecY);
		length = fabs(D3DXVec3Dot(&interval, &Cross));
		if (length > thisRadius + targetRadius)
		{
			return false;
		}
	}

	//C31
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &thisNormalizeVecZ, &targetNormalizeVecX);
		thisRadius = LenSegOnSeparateAxis(&Cross, &thisVecX, &thisVecY);
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecX, &targetVecZ);
		length = fabs(D3DXVec3Dot(&interval, &Cross));
		if (length > thisRadius + targetRadius)
		{
			return false;
		}
	}

	//C32
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &thisNormalizeVecZ, &targetNormalizeVecX);
		thisRadius = LenSegOnSeparateAxis(&Cross, &thisVecX, &thisVecY);
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecX, &targetVecZ);
		length = fabs(D3DXVec3Dot(&interval, &Cross));
		if (length > thisRadius + targetRadius)
		{
			return false;
		}
	}

	//C33
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &thisNormalizeVecZ, &targetNormalizeVecZ);
		thisRadius = LenSegOnSeparateAxis(&Cross, &thisVecX, &thisVecY);
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecX, &targetVecY);
		length = fabs(D3DXVec3Dot(&interval, &Cross));
		if (length > thisRadius + targetRadius)
		{
			return false;
		}
	}
	return true;
}

float CMotionModel3D::LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	float r1 = fabs(D3DXVec3Dot(Sep, e1));
	float r2 = fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}
