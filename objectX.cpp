//=============================================================================
//
// オブジェクトX処理 [objectX.cpp]
// Author : Yuda Kaito
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "objectX.h"
#include "objectX_group.h"
#include "object_polygon3d.h"
#include "application.h"
#include "renderer.h"
#include "main.h"
#include "texture.h"
#include "utility.h"
#include "camera.h"
#include "light.h"
#include <assert.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CObjectX::CObjectX(CTaskGroup::EPriority nPriority) :
	CObject(nPriority),
	m_pParent(nullptr),
	m_scale(1.0f,1.0f,1.0f),
	m_colorAlpha(1.0f)
{
	//オブジェクトのタイプセット処理
	CObject::SetType(CObject::MODEL);
	D3DXMatrixIdentity(&m_mtxRot);
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectX::~CObjectX()
{
}

//=============================================================================
// オブジェクトの初期化
// Author : Yuda Kaito
// 概要 : 初期化を行う
//=============================================================================
HRESULT CObjectX::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	extern LPD3DXEFFECT pEffect;		// シェーダー

	m_hTechnique = pEffect->GetTechniqueByName("Diffuse");			// エフェクト
	m_hTexture = pEffect->GetParameterByName(NULL, "Tex");			// テクスチャ
	m_hmWVP = pEffect->GetParameterByName(NULL, "mWVP");			// ローカル-射影変換行列
	m_hmWIT = pEffect->GetParameterByName(NULL, "mWIT");			// ローカル-ワールド変換行列
	m_hvLightDir = pEffect->GetParameterByName(NULL, "vLightDir");	// ライトの方向
	m_hvCol = pEffect->GetParameterByName(NULL, "vColor");			// 頂点カラー
	m_hvEyePos = pEffect->GetParameterByName(NULL, "vEyePos");

	return S_OK;
}

//=============================================================================
// 描画
// Author : Yuda Kaito
// 概要 : 描画を行う
//=============================================================================
void CObjectX::Draw()
{
	// 計算用マトリックス
	D3DXMATRIX mtxTrans;

	// ワールドマトリックスの初期化
	// 行列初期化関数(第1引数の行列を単位行列に初期化)
	D3DXMatrixIdentity(&m_mtxWorld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxTrans, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != nullptr)
	{
		D3DXMATRIX mtxParent = m_pParent->GetMtxWorld();

		// 行列掛け算関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	DrawMaterial();
}

//=============================================================================
// 描画
// Author : Yuda Kaito
// 概要 : 描画を行う
//=============================================================================
void CObjectX::DrawMaterial()
{
	extern LPD3DXEFFECT pEffect;		// シェーダー

	if (pEffect == nullptr)
	{
		assert(false);
		return;
	}

	/* pEffectに値が入ってる */

	CCamera* pCamera = (CCamera*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(CTask::ERole::ROLE_CAMERA, GetPriority());

	D3DMATRIX viewMatrix = pCamera->GetMtxView();
	D3DMATRIX projMatrix = pCamera->GetMtxProje();

	CLight* lightClass = (CLight*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(CTask::ERole::ROLE_LIGHT, GetPriority());	// カメラ情報
	D3DLIGHT9 light = lightClass->GetLight(0);

	D3DXVECTOR4 v, light_pos;

	D3DXMATRIX m;

	//-------------------------------------------------
	// シェーダの設定
	//-------------------------------------------------
	pEffect->SetTechnique(m_hTechnique);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	D3DXMatrixTranslation(&m, 1.0f, 0.0f, 0.0f);

	// ローカル-射影変換行列
	D3DXMatrixInverse(&m, NULL, &m_mtxWorld);
	D3DXMatrixTranspose(&m, &m);
	pEffect->SetMatrix(m_hmWIT, &m);

	// ローカル-射影変換行列
	m = m_mtxWorld * viewMatrix * projMatrix;
	pEffect->SetMatrix(m_hmWVP, &m);

	// ライトの方向
	light_pos = D3DXVECTOR4(light.Direction.x, light.Direction.y, light.Direction.z, 0);

	D3DXMatrixInverse(&m, NULL, &m_mtxWorld);
	D3DXVec4Transform(&v, &-light_pos, &m);

	D3DXVec3Normalize((D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v);

	//環境光の大きさ
	v.w = -0.8f;
	pEffect->SetVector(m_hvLightDir, &v);

	// 視点
	m = m_mtxWorld *viewMatrix;
	D3DXMatrixInverse(&m, NULL, &m);

	//環境光
	v = D3DXVECTOR4(0, 0, 0, 1);

	//マテリアルデータのポインタを取得する
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	D3DMATERIAL9 *pMtrl = &pMat->MatD3D;

	D3DXVec4Transform(&v, &v, &m);

	//視点をシェーダーに渡す
	pEffect->SetVector(m_hvEyePos, &v);

	for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
	{
		// モデルの色の設定 
		{
			D3DXVECTOR4 Diffuse;
			if (m_materialDiffuse.count(nCntMat) != 0)
			{
				Diffuse = D3DXVECTOR4(m_materialDiffuse[nCntMat].r, m_materialDiffuse[nCntMat].g, m_materialDiffuse[nCntMat].b, m_materialDiffuse[nCntMat].a);
			}
			else
			{
				Diffuse = D3DXVECTOR4(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, pMat[nCntMat].MatD3D.Diffuse.a);
			}

			// モデルの透明度を設定
			/*
			// ※現在適応されません。
			// 原因：.fxファイルにてAmbientColorのalpha値を1.0fに固定しているため
			*/
			Diffuse.w = m_colorAlpha;

			pEffect->SetVector(m_hvCol, &Diffuse);
		}

		if (CTexture::GetInstance()->GetTexture(m_textureKey) != nullptr)
		{// テクスチャの適応
			pTex0 = CTexture::GetInstance()->GetTexture(m_textureKey);
		}

		// テクスチャの設定
		pEffect->SetTexture(m_hTexture, pTex0);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);

		pMtrl++;
	}

	pEffect->EndPass();
	pEffect->End();
}

void CObjectX::SetScale(const D3DXVECTOR3& inScale)
{
	m_scale = inScale;

	m_MinVtx.x *= m_scale.x;
	m_MinVtx.y *= m_scale.y;
	m_MinVtx.z *= m_scale.z;

	m_MaxVtx.x *= m_scale.x;
	m_MaxVtx.y *= m_scale.y;
	m_MaxVtx.z *= m_scale.z;

	MulSize(m_scale);
}

//=============================================================================
// 向きの設定
// Author : Yuda Kaito
//=============================================================================
void CObjectX::SetRot(const D3DXVECTOR3 & inRot)
{
	if (m_rot != inRot)
	{
		SetMtxRot(inRot);
	}
	m_rot = inRot;
}

//=============================================================================
// 頂点最大小値の計算処理
// Author : Yuda Kaito
//=============================================================================
void CObjectX::SetMtxRot(const D3DXVECTOR3 & inRot)
{
	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationYawPitchRoll(&quaternion, inRot.y, inRot.x, inRot.z);
	SetMtxQuaternion(quaternion);
}

//=============================================================================
// 頂点最大小値の計算処理
// Author : Yuda Kaito
//=============================================================================
void CObjectX::CalculationVtx()
{
	D3DXMATRIX mtxTrans, mtxWorld;

	D3DXMatrixIdentity(&mtxWorld);

	// 向きの反映
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &m_mtxRot);					// 行列掛け算関数

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
// Author : Yuda Kaito
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
// Author : Yuda Kaito
//=============================================================================
void CObjectX::LoadModel(const char *aFileName)
{
	CObjectXGroup *xGroup = CApplication::GetInstance()->GetObjectXGroup();
	m_pBuffMat = xGroup->GetBuffMat(aFileName);
	m_MaxVtx = xGroup->GetMaxVtx(aFileName);
	m_pMesh = xGroup->GetMesh(aFileName);
	m_MinVtx = xGroup->GetMinVtx(aFileName);
	m_NumMat = xGroup->GetNumMat(aFileName);
	m_size = xGroup->GetSize(aFileName);
}

//=============================================================================
// 平行投影処理
// Author : Yuda Kaito
//=============================================================================
void CObjectX::Projection(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxTrans;	// 計算用マトリックス

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

	if (m_pBuffMat != nullptr)
	{
		// マテリアルデータへのポインタを取得
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

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
	}

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 色味（拡散反射光）の設定
// Author : Yuda Kaito
//=============================================================================
void CObjectX::SetMaterialDiffuse(unsigned int index, const D3DXCOLOR & inColor)
{
	// 変更予定のマテリアルがない場合
	if (index >= m_NumMat)
	{
		assert(false);
		return;
	}

	if (m_materialDiffuse.count(index) == 0)
	{
		m_materialDiffuse.emplace(index, inColor);
	}
	else
	{
		m_materialDiffuse[index] = inColor;
	}
}

//=============================================================================
// 当たり判定 (左右,奥,手前)
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::Collision(const D3DXVECTOR3& pPos, const D3DXVECTOR3& pPosOld, const D3DXVECTOR3& pSize)
{
	if (!m_isCollision)
	{
		return false;
	}

	D3DXVECTOR3 pos = GetPos();

	// 変数宣言
	bool bIsLanding = false;

	// モデルの左側当たり判定
	if ((pPos.z - pSize.z * 0.5f < pos.z + m_MaxVtx.z) &&
		(pPos.z + pSize.z * 0.5f > pos.z + m_MinVtx.z) &&
		(pPosOld.x + pSize.x * 0.5f <= pos.x + m_MinVtx.x ) &&
		(pPos.x + pSize.x * 0.5f > pos.x + m_MinVtx.x * 0.5f) &&
		(pPos.y + pSize.y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos.y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
	}
	// モデルの右側当たり判定
	if ((pPos.z - pSize.z * 0.5f < pos.z + m_MaxVtx.z) &&
		(pPos.z + pSize.z * 0.5f > pos.z + m_MinVtx.z) &&
		(pPosOld.x - pSize.x * 0.5f >= pos.x + m_MaxVtx.x) &&
		(pPos.x - pSize.x * 0.5f < pos.x + m_MaxVtx.x * 0.5f) &&
		(pPos.y + pSize.y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos.y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
	}
	// モデルの奥側当たり判定
	if ((pPos.x - pSize.x * 0.5f < pos.x + m_MaxVtx.x) &&
		(pPos.x + pSize.x * 0.5f > pos.x + m_MinVtx.x) &&
		(pPosOld.z - pSize.z * 0.5f >= pos.z + m_MaxVtx.z) &&
		(pPos.z - pSize.z * 0.5f < pos.z + m_MaxVtx.z * 0.5f) &&
		(pPos.y + pSize.y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos.y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
	}
	// モデルの手前側当たり判定
	if ((pPos.x - pSize.x * 0.5f < pos.x + m_MaxVtx.x) &&
		(pPos.x + pSize.x * 0.5f > pos.x + m_MinVtx.x) &&
		(pPosOld.z + pSize.z * 0.5f <= pos.z + m_MinVtx.z) &&
		(pPos.z + pSize.z * 0.5f > pos.z + m_MinVtx.z * 0.5f) &&
		(pPos.y + pSize.y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos.y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
	}

	// 値を返す
	return bIsLanding;
}

//=============================================================================
// 当たり判定 (左右,奥,手前)
// Author : Yuda Kaito
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
// Author : Yuda Kaito
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

//=============================================================================
// OBBとOBBの当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::OBBAndOBB(CObjectX* inObjectX)
{
	if (!inObjectX->IsCollision())
	{
		return false;
	}

	// 逆行列
	D3DXMATRIX mtxThisRot = m_mtxRot;
	mtxThisRot._11 = m_mtxRot._11;
	mtxThisRot._12 = m_mtxRot._21;
	mtxThisRot._13 = m_mtxRot._31;
	mtxThisRot._21 = m_mtxRot._12;
	mtxThisRot._22 = m_mtxRot._22;
	mtxThisRot._23 = m_mtxRot._32;
	mtxThisRot._31 = m_mtxRot._13;
	mtxThisRot._32 = m_mtxRot._23;
	mtxThisRot._33 = m_mtxRot._33;

	// 自身とターゲットの向きを合成
	D3DXMATRIX mtx = mtxThisRot * inObjectX->GetMatRot();

	// 絶対値
	D3DXMATRIX mtxAds;
	mtxAds._11 = fabs(mtx._11);
	mtxAds._12 = fabs(mtx._21);
	mtxAds._13 = fabs(mtx._31);
	mtxAds._21 = fabs(mtx._12);
	mtxAds._22 = fabs(mtx._22);
	mtxAds._23 = fabs(mtx._32);
	mtxAds._31 = fabs(mtx._13);
	mtxAds._32 = fabs(mtx._23);
	mtxAds._33 = fabs(mtx._33);

	const float etc = 1.0e-6f;	// めっちゃ小さな数字(100万分の1)
	bool parallel = false;

	if (mtxAds._11 + etc >= 1.0f ||
		mtxAds._12 + etc >= 1.0f || 
		mtxAds._13 + etc >= 1.0f || 
		mtxAds._21 + etc >= 1.0f || 
		mtxAds._22 + etc >= 1.0f || 
		mtxAds._23 + etc >= 1.0f || 
		mtxAds._31 + etc >= 1.0f || 
		mtxAds._32 + etc >= 1.0f || 
		mtxAds._33 + etc >= 1.0f)
	{
		parallel = true;
	}

	D3DXVECTOR3 interval;
	D3DXVECTOR3 pos = (GetPos() - inObjectX->GetPos());
	D3DXVec3TransformCoord(&interval, &pos, &m_mtxRot);

	bool debug = true;

	if (D3DXVec3Length(&pos) < 100.0f)
	{
		debug = true;
	}

	debug = true;

	if (debug)
	{
		//CDebugProc::Print("mtxAds : %.2f,%.2f,%.2f   %.2f,%.2f,%.2f  %.2f,%.2f,%.2f│ ", mtxAds._11, mtxAds._12, mtxAds._13, mtxAds._21, mtxAds._22, mtxAds._23, mtxAds._31, mtxAds._32, mtxAds._33);
		CDebugProc::Print("pos : %.2f,%.2f,%.2f │ ", pos.x, pos.y, pos.z);
		CDebugProc::Print("interval : %.2f,%.2f,%.2f │", interval.x, interval.y, interval.z);
	}

	float s;

	D3DXVECTOR3 thisScale = m_MaxVtx;
	{
		D3DXVECTOR3 scale = m_scale * 0.5f;
		thisScale.x *= scale.x;
		thisScale.y *= scale.y;
		thisScale.z *= scale.z;
		//CDebugProc::Print("thisScale : %.2f,%.2f,%.2f │", thisScale.x, thisScale.y, thisScale.z);
	}

	D3DXVECTOR3 targetScale = inObjectX->GetMaxVtx();
	{
		D3DXVECTOR3 scale = inObjectX->GetScale() * 0.5f;
		targetScale.x *= scale.x;
		targetScale.y *= scale.y;
		targetScale.z *= scale.z;
		CDebugProc::Print("targetScale : %.2f,%.2f,%.2f │", targetScale.x, targetScale.y, targetScale.z);
	}

	float aMax = -FLT_MAX;
	float bMax = -FLT_MAX;
	float eMax = -FLT_MAX;
	int aAxis = ~0;
	int bAxis = ~0;
	int eAxis = ~0;
	D3DXVECTOR3 nA;
	D3DXVECTOR3 nB;
	D3DXVECTOR3 nE;

	auto TrackFaceAxis = [](int* axis,int nCnt,float s,float* max,const D3DXVECTOR3& normal, D3DXVECTOR3* axisNormal)
	{
		if (s > 0.0f)
		{
			return true;
		}

		if (s > *max)
		{
			*max = s;
			*axis = nCnt;
			*axisNormal = normal;
		}
		return false;
	};

	//A.e1
	s = fabs(interval.x) - (thisScale.x + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._11, mtxAds._21, mtxAds._31), &targetScale));
	if (debug)CDebugProc::Print("%s",parallel ? "│ parallel" : "");
	if (TrackFaceAxis(&aAxis, 0, s, &aMax, D3DXVECTOR3(m_mtxRot._11, m_mtxRot._12, m_mtxRot._13), &nA))
	{
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│A.e1\n");
		return false;
	}

	//A.e2
	s = fabs(interval.y) - (thisScale.y + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._12, mtxAds._22, mtxAds._32), &targetScale));
	if (TrackFaceAxis(&aAxis, 1, s, &aMax, D3DXVECTOR3(m_mtxRot._21, m_mtxRot._22, m_mtxRot._23), &nA))
	{
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│ A.e2\n");
		return false;
	}

	//A.e3
	s = fabs(interval.z) - (thisScale.z + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._13, mtxAds._23, mtxAds._33), &targetScale));
	if (TrackFaceAxis(&aAxis, 2, s, &aMax, D3DXVECTOR3(m_mtxRot._31, m_mtxRot._32, m_mtxRot._33), &nA))
	{
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│ A.e3\n");
		return false;
	}

	D3DXMATRIX targetMtxRot = inObjectX->GetMatRot();

	//B.e1
	s = fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._11, mtx._12, mtx._13))) - (targetScale.x + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._11, mtxAds._12, mtxAds._13), &thisScale));
	if (TrackFaceAxis(&bAxis, 3, s, &bMax, D3DXVECTOR3(targetMtxRot._11, targetMtxRot._12, targetMtxRot._13), &nB))
	{
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│ B.e1\n");
		return false;
	}

	//B.e2
	s = fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._21, mtx._22, mtx._23))) - (targetScale.y + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._21, mtxAds._22, mtxAds._23), &thisScale));
	if (TrackFaceAxis(&bAxis, 4, s, &bMax, D3DXVECTOR3(targetMtxRot._21, targetMtxRot._22, targetMtxRot._23), &nB))
	{
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│ B.e2\n");
		return false;
	}

	//B.e3
	s = fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._31, mtx._32, mtx._33))) - (targetScale.z + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._31, mtxAds._32, mtxAds._33),&thisScale));
	if (TrackFaceAxis(&bAxis, 5, s, &bMax, D3DXVECTOR3(targetMtxRot._31, targetMtxRot._32, targetMtxRot._33), &nB))
	{
		if (debug)CDebugProc::Print("a : %.2f", fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._31, mtx._32, mtx._33))));
		if (debug)CDebugProc::Print("b : %.2f", (targetScale.z + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._31, mtxAds._32, mtxAds._33), &thisScale)));
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│ B.e3\n");
		return false;
	}

	if (!parallel)
	{
		float thisRadius;
		float targetRadius;

		auto TrackEdgeAxis = [](int* axis, int nCnt, float s, float* max, const D3DXVECTOR3& normal, D3DXVECTOR3* axisNormal)
		{
			if (s > 0.0f)
			{
				return true;
			}

			float i = 1.0f / D3DXVec3Length(&normal);
			s *= i;

			if (s > *max)
			{
				*max = s;
				*axis = nCnt;
				*axisNormal = normal * i;
			}

			return false;
		};

		//C11
		thisRadius = thisScale.y * mtxAds._13 + thisScale.z * mtxAds._12;
		targetRadius = targetScale.y * mtxAds._31 + targetScale.z * mtxAds._21;
		s = fabs(interval.z * mtx._12 - interval.y * mtx._13) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 6, s, &eMax, D3DXVECTOR3(0.0f, -mtx._13, mtx._12), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C11\n");
			return false;
		}

		//C12
		thisRadius = thisScale.y * mtxAds._23 + thisScale.z * mtxAds._22;
		targetRadius = targetScale.x * mtxAds._31 + targetScale.z * mtxAds._11;
		s = fabs(interval.z * mtx._22 - interval.y * mtx._23) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 7, s, &eMax, D3DXVECTOR3(0.0f, -mtx._23, mtx._22), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C12\n");
			return false;
		}

		//C13
		thisRadius = thisScale.y * mtxAds._33 + thisScale.z * mtxAds._32;
		targetRadius = targetScale.x * mtxAds._21 + targetScale.y * mtxAds._11;
		s = fabs(interval.z * mtx._32 - interval.y * mtx._33) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 8, s, &eMax, D3DXVECTOR3(0.0f, -mtx._33, mtx._32), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C13\n");
			return false;
		}

		//C21
		thisRadius = thisScale.x * mtxAds._13 + thisScale.z * mtxAds._11;
		targetRadius = targetScale.y * mtxAds._31 + targetScale.z * mtxAds._22;
		s = fabs(interval.x * mtx._13 - interval.z * mtx._11) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 9, s, &eMax, D3DXVECTOR3(mtx._13, 0.0f, -mtx._11), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C21\n");
			return false;
		}

		//C22
		thisRadius = thisScale.x * mtxAds._23 + thisScale.z * mtxAds._21;
		targetRadius = targetScale.x * mtxAds._32 + targetScale.z * mtxAds._12;
		s = fabs(interval.x * mtx._23 - interval.z * mtx._21) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 10, s, &eMax, D3DXVECTOR3(mtx._23, 0.0f, -mtx._21), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C22\n");
			return false;
		}

		//C23
		thisRadius = thisScale.x * mtxAds._33 + thisScale.z * mtxAds._31;
		targetRadius = targetScale.x * mtxAds._22 + targetScale.y * mtxAds._12;
		s = fabs(interval.x * mtx._33 - interval.z * mtx._31) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 11, s, &eMax, D3DXVECTOR3(mtx._33, 0.0f, -mtx._31), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C23\n");
			return false;
		}

		//C31
		thisRadius = thisScale.x * mtxAds._12 + thisScale.y * mtxAds._11;
		targetRadius = targetScale.y * mtxAds._33 + targetScale.z * mtxAds._23;
		s = fabs(interval.y * mtx._11 - interval.x * mtx._12) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 12, s, &eMax, D3DXVECTOR3(-mtx._13, mtx._12, 0.0f), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C31\n");
			return false;
		}

		//C32
		thisRadius = thisScale.x * mtxAds._22 + thisScale.y * mtxAds._21;
		targetRadius = targetScale.x * mtxAds._33 + targetScale.z * mtxAds._13;
		s = fabs(interval.y * mtx._21 - interval.x * mtx._22) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 13, s, &eMax, D3DXVECTOR3(-mtx._22, mtx._21, 0.0f), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C32\n");
			return false;
		}

		//C33
		thisRadius = thisScale.x * mtxAds._32 + thisScale.y * mtxAds._31;
		targetRadius = targetScale.x * mtxAds._23 + targetScale.y * mtxAds._13;
		s = fabs(interval.y * mtx._31 - interval.x * mtx._32) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 14, s, &eMax, D3DXVECTOR3(-mtx._32, mtx._31, 0.0f), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C33\n");
			return false;
		}
	}

	if (debug)CDebugProc::Print("s : %.2f", s);
	if (debug)CDebugProc::Print("│ ★Hit★\n");
	return true;
}

//=============================================================================
// OBBとBoxの当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::OBBAndBoxTop(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().x + inObjectX->GetMaxVtx().x > m_pos.x ) || !(m_pos.x > inObjectX->GetPos().x + inObjectX->GetMinVtx().x))
	{
		return false;
	}
	
	if (!(inObjectX->GetPos().z + inObjectX->GetMaxVtx().z > m_pos.z) || !(m_pos.z > inObjectX->GetPos().z + inObjectX->GetMinVtx().z))
	{
		return false;
	}
	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.y += inObjectX->GetMaxVtx().y;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBBとBoxの当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::OBBAndBoxDown(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().x + inObjectX->GetMaxVtx().x > m_pos.x) || !(m_pos.x > inObjectX->GetPos().x + inObjectX->GetMinVtx().x))
	{
		return false;
	}

	if (!(inObjectX->GetPos().z + inObjectX->GetMaxVtx().z > m_pos.z) || !(m_pos.z > inObjectX->GetPos().z + inObjectX->GetMinVtx().z))
	{
		return false;
	}
	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.y += inObjectX->GetMinVtx().y;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBBとBoxの当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::OBBAndBoxLeft(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().y + inObjectX->GetMaxVtx().y > m_pos.x) || !(m_pos.y > inObjectX->GetPos().y + inObjectX->GetMinVtx().y))
	{
		return false;
	}

	if (!(inObjectX->GetPos().z + inObjectX->GetMaxVtx().z > m_pos.z) || !(m_pos.z > inObjectX->GetPos().z + inObjectX->GetMinVtx().z))
	{
		return false;
	}
	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.y += inObjectX->GetMinVtx().x;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBBとBoxの当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::OBBAndBoxRight(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().y + inObjectX->GetMaxVtx().y > m_pos.x) || !(m_pos.y > inObjectX->GetPos().y + inObjectX->GetMinVtx().y))
	{
		return false;
	}

	if (!(inObjectX->GetPos().z + inObjectX->GetMaxVtx().z > m_pos.z) || !(m_pos.z > inObjectX->GetPos().z + inObjectX->GetMinVtx().z))
	{
		return false;
	}
	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.x += inObjectX->GetMaxVtx().x;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBBとBoxの当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::OBBAndBoxFront(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().x + inObjectX->GetMaxVtx().x > m_pos.x) || !(m_pos.x > inObjectX->GetPos().x + inObjectX->GetMinVtx().x))
	{
		return false;
	}

	if (!(inObjectX->GetPos().y + inObjectX->GetMaxVtx().y > m_pos.z) || !(m_pos.y > inObjectX->GetPos().y + inObjectX->GetMinVtx().y))
	{
		return false;
	}

	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.z += inObjectX->GetMaxVtx().z;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBBとBoxの当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::OBBAndBoxBack(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().x + inObjectX->GetMaxVtx().x > m_pos.x) || !(m_pos.x > inObjectX->GetPos().x + inObjectX->GetMinVtx().x))
	{
		return false;
	}

	if (!(inObjectX->GetPos().y + inObjectX->GetMaxVtx().y > m_pos.z) || !(m_pos.y > inObjectX->GetPos().y + inObjectX->GetMinVtx().y))
	{
		return false;
	}
	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.z += inObjectX->GetMinVtx().z;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBBと3DPolygonの当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::OBBAndPolygon(const CObjectPolygon3D * inObjectPolgon, float* outLength)
{
	// 平面の法線に対するOBBの射影線の長さを算出
	float r = 0.0f;		// 近接距離
	D3DXVECTOR3 polygonNormal = inObjectPolgon->GetNormal();	// 平面の法線ベクトル

	// X軸
	D3DXVECTOR3 thisNormalizeVecX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecX, &thisNormalizeVecX, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecX, &thisNormalizeVecX);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecX * (this->GetSize().x * 0.5f)), &polygonNormal));

	// Y軸
	D3DXVECTOR3 thisNormalizeVecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecY, &thisNormalizeVecY, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecY, &thisNormalizeVecY);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecY * (this->GetSize().y * 0.5f)), &polygonNormal));

	// Z軸
	D3DXVECTOR3 thisNormalizeVecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecZ, &thisNormalizeVecZ, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecZ, &thisNormalizeVecZ);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecZ * (this->GetSize().z * 0.5f)), &polygonNormal));

	// 平面とOBBの距離を算出
	D3DXVECTOR3 ObbPos = this->GetPos();
	D3DXVECTOR3 PlanePos = inObjectPolgon->GetPos();

	float dist = D3DXVec3Dot(&(ObbPos - PlanePos), &polygonNormal);

	// 戻し距離を算出
	if (outLength != nullptr)
	{
		if (dist > 0)
		{
			*outLength = r - fabs(dist);
		}
		else
		{
			*outLength = r + fabs(dist);
		}
	}

	// 衝突判定
	if (fabs(dist) - r < 0.0f)
	{
		return true; // 衝突している
	}

	return false; // 衝突していない
}

//=============================================================================
// OBBと平行の当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::OBBAndPolygon(const D3DXVECTOR3 & inPos, float * outLength)
{
	// 平面の法線に対するOBBの射影線の長さを算出
	float r = 0.0f;		// 近接距離
	D3DXVECTOR3 polygonNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 平面の法線ベクトル

	// X軸
	D3DXVECTOR3 thisNormalizeVecX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecX, &thisNormalizeVecX, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecX, &thisNormalizeVecX);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecX * (this->GetSize().x * 0.5f)), &polygonNormal));

	// Y軸
	D3DXVECTOR3 thisNormalizeVecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecY, &thisNormalizeVecY, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecY, &thisNormalizeVecY);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecY * (this->GetSize().y * 0.5f)), &polygonNormal));

	// Z軸
	D3DXVECTOR3 thisNormalizeVecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecZ, &thisNormalizeVecZ, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecZ, &thisNormalizeVecZ);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecZ * (this->GetSize().z * 0.5f)), &polygonNormal));

	// 平面とOBBの距離を算出
	D3DXVECTOR3 ObbPos = this->GetPos();

	float dist = D3DXVec3Dot(&(ObbPos - inPos), &polygonNormal);

	// 戻し距離を算出
	if (outLength != nullptr)
	{
		if (dist > 0)
		{
			*outLength = r - fabs(dist);
		}
		else
		{
			*outLength = r + fabs(dist);
		}
	}

	// 衝突判定
	if (fabs(dist) - r < 0.0f)
	{
		return true; // 衝突している
	}

	return false; // 衝突していない
}

//=============================================================================
// 3つの内積の絶対値の和で投影線分長を計算
// Author : Yuda Kaito
// 事項：分離軸Sepは標準化されていること
//=============================================================================
float CObjectX::LenSegOnSeparateAxis(D3DXVECTOR3 * Sep, D3DXVECTOR3 * e1, D3DXVECTOR3 * e2, D3DXVECTOR3 * e3)
{
	float r1 = fabs(D3DXVec3Dot(Sep, e1));
	float r2 = fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0.0f;
	return r1 + r2 + r3;
}

// Author : Yuda Kaito
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
