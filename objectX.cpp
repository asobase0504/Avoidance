//=============================================================================
//
// オブジェクトX処理 [objectX.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "objectX.h"
#include "objectX_group.h"
#include "application.h"
#include "renderer.h"
#include "main.h"
#include "texture.h"
#include "utility.h"
#include <assert.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CObjectX::CObjectX(CTaskGroup::EPriority nPriority) :
	CObject(nPriority),
	m_pParent(nullptr)
{
	//オブジェクトのタイプセット処理
	CObject::SetType(CObject::MODEL);
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectX::~CObjectX()
{
}

//=============================================================================
// オブジェクトの初期化
// Author : Hamada Ryuuga
// 概要 : 値の初期化を行う
//=============================================================================
HRESULT CObjectX::Init()
{
	return S_OK;
}

//=============================================================================
// オブジェクトの終了
// Author : Hamada Ryuuga
// 概要 : 更新を行う
//=============================================================================
void CObjectX::Uninit()
{
	//インスタンスの解放処理
	Release();
}

//=============================================================================
// オブジェクトの更新
// Author : Yuda Kaito
// 概要 : 更新を行う
//=============================================================================
void CObjectX::NormalUpdate()
{
}

//=============================================================================
// 描画
// Author : Hamada Ryuga
// 概要 : 描画を行う
//=============================================================================
void CObjectX::Draw()
{
	GiftMtx(&m_mtxWorld, m_pos, m_rot);	// マトリックスの設定

	if (m_pParent != nullptr)
	{
		D3DXMATRIX mtxParent = m_pParent->GetMtxWorld();

		// 行列掛け算関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// ワールドマトリックスの設定（ワールド座標行列の設定）
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを保持
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	D3DXMATERIAL* pMat = m_material;

	for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// クオータニオンを使用した描画
// Author : Yuda Kaito
// 概要 : 描画を行う
//=============================================================================
void CObjectX::Draw(const D3DXQUATERNION& quaternion)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;		// 計算用マトリックス
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// クォータニオンの使用した姿勢の設定
	D3DXMatrixRotationQuaternion(&mtxRot, &quaternion);			// クオータニオンによる行列回転
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);	// (※行列移動関数(第1引数にx,y,z方向の移動行列を作成))
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMtxWorld();

		// 行列掛け算関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	// ワールドマトリックスの設定（ワールド座標行列の設定）
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを保持
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = m_material;

	for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Diffuse;

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 親子関係のある描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CObjectX::Draw(D3DXMATRIX mtxParent)
{
	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);	// 行列回転関数
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);				// 行列掛け算関数 

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);		// 行列移動関数
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);			// 行列掛け算関数

	// 行列掛け算関数
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを保持
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// テクスチャポインタの取得
	CTexture *pTexture = CApplication::GetInstance()->GetTexture();

	// マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	if (m_material != nullptr)
	{// マテリアルデータへのポインタを取得
		pMat = m_material;

		for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
		{// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetTexture("NULL"));

			// モデルパーツの描画
			m_pMesh->DrawSubset(nCntMat);

			// テクスチャの設定
			pDevice->SetTexture(0, nullptr);
		}
	}

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 頂点最大小値の計算処理
//=============================================================================
void CObjectX::CalculationVtx()
{
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;

	D3DXMatrixIdentity(&mtxWorld);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);	// 行列回転関数
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);					// 行列掛け算関数

	D3DXVec3TransformCoord(&m_MaxVtx, &m_MaxVtx, &mtxWorld);
	D3DXVec3TransformCoord(&m_MinVtx, &m_MinVtx, &mtxWorld);


	if (m_MaxVtx.x < m_MinVtx.x)
	{
		float change = m_MaxVtx.x;
		m_MaxVtx.x = m_MinVtx.x;
		m_MinVtx.x = change;
	}

	if (m_MaxVtx.y < m_MinVtx.y)
	{
		float change = m_MaxVtx.y;
		m_MaxVtx.y = m_MinVtx.y;
		m_MinVtx.y = change;
	}

	if (m_MaxVtx.z < m_MinVtx.z)
	{
		float change = m_MaxVtx.z;
		m_MaxVtx.z = m_MinVtx.z;
		m_MinVtx.z = change;
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CObjectX * CObjectX::Create(D3DXVECTOR3 pos, CTaskGroup::EPriority nPriority)
{
	// ポインタ宣言
	CObjectX *pObjectX = nullptr;

	// インスタンス生成
	pObjectX = new CObjectX(nPriority);

	if (pObjectX != nullptr)
	{// ポインタが存在したら実行
		pObjectX->Init();
		pObjectX->SetPos(pos);
		pObjectX->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{// ポインタが虚無だったら実行
		assert(false);
	}

	// ポインタを返す
	return pObjectX;
}

//=============================================================================
// モデルの読み込み
//=============================================================================
void CObjectX::LoadModel(const char *aFileName)
{
	CObjectXGroup *xGroup = CApplication::GetInstance()->GetObjectXGroup();
	m_material = (D3DXMATERIAL*)xGroup->GetBuffMat(aFileName)->GetBufferPointer();
	m_MaxVtx = xGroup->GetMaxVtx(aFileName);
	m_pMesh = xGroup->GetMesh(aFileName);
	m_MinVtx = xGroup->GetMinVtx(aFileName);
	m_NumMat = xGroup->GetNumMat(aFileName);
	m_size = xGroup->GetSize(aFileName);
}

//=============================================================================
// 平行投影処理
//=============================================================================
void CObjectX::Projection(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// 変数宣言
	D3DXMATRIX mtxShadow;
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;
	D3DMATERIAL9 Material;

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	vecLight = -D3DXVECTOR4(0.2f, -0.5f, 0.3f, 0.0f);

	if (m_pos.y >= -20.0f)
	{
		pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
	}

	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);
	D3DXMatrixShadow(&mtxShadow,&vecLight, &planeField);

	// ワールドマトリックスと掛け合わせる
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定（ワールド座標行列の設定）
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// 現在のマテリアルを保持
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = m_material;

	for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		Material = pMat[nCntMat].MatD3D;

		Material.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		Material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// マテリアルの設定
		pDevice->SetMaterial(&Material);

		// モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 色味（拡散反射光）の設定
//=============================================================================
void CObjectX::SetMaterialDiffuse(unsigned int index, const D3DXCOLOR & inColor)
{
	// 変更予定のマテリアルがない場合
	if (index >= m_NumMat)
	{
		assert(false);
		return;
	}

	m_material[index].MatD3D.Diffuse = inColor;
}

//=============================================================================
// 当たり判定 (左右,奥,手前)
//=============================================================================
bool CObjectX::Collision(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pSize)
{
	if (!m_isCollision)
	{
		return false;
	}

	D3DXVECTOR3 pos = GetPos();

	// 変数宣言
	bool bIsLanding = false;

	// モデルの左側当たり判定
	if ((pPos->z - pSize->z * 0.5f < pos.z + m_MaxVtx.z) &&
		(pPos->z + pSize->z * 0.5f > pos.z + m_MinVtx.z) &&
		(pPosOld->x + pSize->x * 0.5f <= pos.x + m_MinVtx.x * 0.5f) &&
		(pPos->x + pSize->x * 0.5f > pos.x + m_MinVtx.x * 0.5f) &&
		(pPos->y + pSize->y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos->y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
		pPos->x = pos.x + m_MinVtx.x * 0.5f - pSize->x * 0.5f;
	}
	// モデルの右側当たり判定
	if ((pPos->z - pSize->z * 0.5f < pos.z + m_MaxVtx.z) &&
		(pPos->z + pSize->z * 0.5f > pos.z + m_MinVtx.z) &&
		(pPosOld->x - pSize->x * 0.5f >= pos.x + m_MaxVtx.x * 0.5f) &&
		(pPos->x - pSize->x * 0.5f < pos.x + m_MaxVtx.x * 0.5f) &&
		(pPos->y + pSize->y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos->y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
		pPos->x = pos.x + m_MaxVtx.x * 0.5f + pSize->x * 0.5f;
	}
	// モデルの奥側当たり判定
	if ((pPos->x - pSize->x * 0.5f < pos.x + m_MaxVtx.x) &&
		(pPos->x + pSize->x * 0.5f > pos.x + m_MinVtx.x) &&
		(pPosOld->z - pSize->z * 0.5f >= pos.z + m_MaxVtx.z * 0.5f) &&
		(pPos->z - pSize->z * 0.5f < pos.z + m_MaxVtx.z * 0.5f) &&
		(pPos->y + pSize->y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos->y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
		pPos->z = pos.z + m_MaxVtx.z * 0.5f + pSize->z * 0.5f;
	}
	// モデルの手前側当たり判定
	if ((pPos->x - pSize->x * 0.5f < pos.x + m_MaxVtx.x) &&
		(pPos->x + pSize->x * 0.5f > pos.x + m_MinVtx.x) &&
		(pPosOld->z + pSize->z * 0.5f <= pos.z + m_MinVtx.z * 0.5f) &&
		(pPos->z + pSize->z * 0.5f > pos.z + m_MinVtx.z * 0.5f) &&
		(pPos->y + pSize->y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos->y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
		pPos->z = pos.z + m_MinVtx.z * 0.5f - pSize->z * 0.5f;
	}

	// 値を返す
	return bIsLanding;
}

//=============================================================================
// 当たり判定 (左右,奥,手前)
//=============================================================================
bool CObjectX::Collision(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * inMaxVtx, D3DXVECTOR3 * inMinVtx)
{
	if (!m_isCollision)
	{
		return false;
	}

	D3DXVECTOR3 pos = GetPos();

	// 変数宣言
	bool bIsLanding = false;

	// モデルの乗り上げ判定
	if ((pPos->y + 20.0f > pos.y + m_MaxVtx.y))
	{
		return bIsLanding;
	}

	// モデルの左側当たり判定
	if ((pPos->z + inMinVtx->z < pos.z + m_MaxVtx.z) &&
		(pPos->z + inMaxVtx->z > pos.z + m_MinVtx.z) &&
		(pPosOld->x + inMaxVtx->x  <= pos.x + m_MinVtx.x) &&
		(pPos->x + inMaxVtx->x > pos.x + m_MinVtx.x) &&
		(pPos->y + inMaxVtx->y > pos.y + m_MinVtx.y) &&
		(pPos->y < pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->x = pos.x + m_MinVtx.x + inMinVtx->x - 5.0f;
	}

	// モデルの右側当たり判定
	if ((pPos->z + inMinVtx->z < pos.z + m_MaxVtx.z) &&
		(pPos->z + inMaxVtx->z > pos.z + m_MinVtx.z) &&
		(pPosOld->x + inMinVtx->x >= pos.x + m_MaxVtx.x) &&
		(pPos->x + inMinVtx->x < pos.x + m_MaxVtx.x) &&
		(pPos->y + inMaxVtx->y > pos.y + m_MinVtx.y) &&
		(pPos->y < pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->x = pos.x + m_MaxVtx.x + inMaxVtx->x;
		pPos->x -= 0.1f;
	}

	// モデルの奥側当たり判定
	if ((pPos->x + inMinVtx->x < pos.x + m_MaxVtx.x) &&
		(pPos->x + inMaxVtx->x > pos.x + m_MinVtx.x) &&
		(pPosOld->z + inMinVtx->z >= pos.z + m_MaxVtx.z) &&
		(pPos->z + inMinVtx->z < pos.z + m_MaxVtx.z) &&
		(pPos->y + inMaxVtx->y > pos.y + m_MinVtx.y) &&
		(pPos->y < pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->z = pos.z + m_MaxVtx.z + inMaxVtx->z;
	}

	// モデルの手前側当たり判定
	if ((pPos->x + inMinVtx->x < pos.x + m_MaxVtx.x) &&
		(pPos->x + inMaxVtx->x > pos.x + m_MinVtx.x) &&
		(pPosOld->z + inMaxVtx->z <= pos.z + m_MinVtx.z) &&
		(pPos->z + inMaxVtx->z > pos.z + m_MinVtx.z) &&
		(pPos->y + inMaxVtx->y > pos.y + m_MinVtx.y) &&
		(pPos->y < pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->z = pos.z + m_MinVtx.z + inMinVtx->z - 5.0f;
		pPos->z -= 0.1f;
	}

	// 値を返す
	return bIsLanding;
}

//=============================================================================
// 当たり判定 (上側)
//=============================================================================
bool CObjectX::UpCollision(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pSize, D3DXVECTOR3 * pMove)
{
	if (!m_isCollision)
	{
		return false;
	}

	// 変数宣言
	bool bIsLanding = false;

	// モデルの上側当たり判定
	if ((pPos->z - pSize->z * 0.5f < m_pos.z + m_MaxVtx.z) &&
		(pPos->z + pSize->z * 0.5f > m_pos.z + m_MinVtx.z) &&
		(pPos->x - pSize->x * 0.5f < m_pos.x + m_MaxVtx.x) &&
		(pPos->x + pSize->x * 0.5f > m_pos.x + m_MinVtx.x) &&
		(pPos->y <= m_pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->y = m_pos.y + m_MaxVtx.y;
		if (pPos->y == pPosOld->y)
		{
			pMove->y = 0.0f;
		}
	}

	// 値を返す
	return bIsLanding;
}

bool CObjectX::UpCollision(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * inMaxVtx, D3DXVECTOR3 * inMinVtx, D3DXVECTOR3 * pMove)
{
	if (!m_isCollision)
	{
		return false;
	}

	// 変数宣言
	bool bIsLanding = false;

	// モデルの上側当たり判定
	if ((pPos->z + inMinVtx->z < m_pos.z + m_MaxVtx.z) &&
		(pPos->z + inMaxVtx->z > m_pos.z + m_MinVtx.z) &&
		(pPos->x + inMinVtx->x < m_pos.x + m_MaxVtx.x) &&
		(pPos->x + inMaxVtx->x > m_pos.x + m_MinVtx.x) &&
		(pPos->y <= m_pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->y = m_pos.y + m_MaxVtx.y;
		if (pPos->y == pPosOld->y)
		{
			pMove->y = 0.0f;
		}
	}

	// 値を返す
	return bIsLanding;
}
