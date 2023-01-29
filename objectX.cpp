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


#define InFront( a ) \
	((a) < 0.0f)

#define Behind( a ) \
	((a) >= 0.0f)

#define On( a ) \
	((a) < 0.005f && (a) > -0.005f)

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
	extern LPD3DXEFFECT pEffect;	// シェーダー

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
	DrawOutLine();
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

	// タスクグループ情報
	CTaskGroup* taskGroup = CApplication::GetInstance()->GetTaskGroup();

	// カメラ情報
	CCamera* pCamera = (CCamera*)taskGroup->SearchRoleTop(CTask::ERole::ROLE_CAMERA, GetPriority());

	D3DMATRIX viewMatrix = pCamera->GetMtxView();
	D3DMATRIX projMatrix = pCamera->GetMtxProje();

	// ライト情報
	CLight* lightClass = (CLight*)taskGroup->SearchRoleTop(CTask::ERole::ROLE_LIGHT, GetPriority());
	D3DLIGHT9 light = lightClass->GetLight(0);

	D3DXVECTOR4 v, light_pos;

	D3DXMATRIX m;

	//-------------------------------------------------
	// シェーダの設定
	//-------------------------------------------------
	pEffect->SetTechnique(m_hTechnique);
	pEffect->Begin(NULL, 0);

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

	D3DXVec4Transform(&v, &v, &m);

	//視点をシェーダーに渡す
	pEffect->SetVector(m_hvEyePos, &v);

	//マテリアルデータのポインタを取得する
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	D3DMATERIAL9 *pMtrl = &pMat->MatD3D;

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

		pEffect->BeginPass(0);
		m_pMesh->DrawSubset(nCntMat);	//モデルパーツの描画
		pEffect->EndPass();

		pMtrl++;
	}

	pEffect->End();
}

void CObjectX::DrawOutLine()
{
	// 計算用マトリックス
	D3DXMATRIX mtxTrans;

	// ワールドマトリックスの初期化
	// 行列初期化関数(第1引数の行列を単位行列に初期化)
	D3DXMatrixIdentity(&m_mtxWorld);

	// 大きさを反映
	//D3DXMatrixScaling(&mtxTrans, -m_scale.x * 1.05f, -m_scale.y * 1.05f, -m_scale.z * 1.05f);
	D3DXMatrixScaling(&mtxTrans, -m_scale.x - 0.15f, -m_scale.y - 0.15f, -m_scale.z - 0.15f);
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

	D3DXCOLOR color = m_materialDiffuse[0];
	m_materialDiffuse[0] = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	DrawMaterial();
	m_materialDiffuse[0] = color;
}

void CObjectX::SetScale(const D3DXVECTOR3& inScale)
{
	m_scale = inScale;

	m_MinVtx.x = m_scale.x * m_MinVtxOrigin.x;
	m_MinVtx.y = m_scale.y * m_MinVtxOrigin.y;
	m_MinVtx.z = m_scale.z * m_MinVtxOrigin.z;

	m_MaxVtx.x = m_scale.x * m_MaxVtxOrigin.x;
	m_MaxVtx.y = m_scale.y * m_MaxVtxOrigin.y;
	m_MaxVtx.z = m_scale.z * m_MaxVtxOrigin.z;
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
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &m_mtxRot);		// 行列掛け算関数

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
	m_MaxVtxOrigin = xGroup->GetMaxVtx(aFileName);
	m_MaxVtx = m_MaxVtxOrigin;
	m_MinVtxOrigin = xGroup->GetMinVtx(aFileName);
	m_MinVtx = m_MinVtxOrigin;
	m_pMesh = xGroup->GetMesh(aFileName);
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
// OBBとOBBの当たり判定(押し戻しを含む)
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::OBBAndOBB(CObjectX* inObjectX, D3DXVECTOR3* outPos)
{
	if (!inObjectX->IsCollision())
	{
		return false;
	}

	bool debug = true;

	if (debug)CDebugProc::Print("----------------------------------------------\n");

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

	// 逆行列
	D3DXMATRIX targetMtxRot = inObjectX->GetMtxRot();
	targetMtxRot._11 = inObjectX->GetMtxRot()._11;
	targetMtxRot._12 = inObjectX->GetMtxRot()._21;
	targetMtxRot._13 = inObjectX->GetMtxRot()._31;
	targetMtxRot._21 = inObjectX->GetMtxRot()._12;
	targetMtxRot._22 = inObjectX->GetMtxRot()._22;
	targetMtxRot._23 = inObjectX->GetMtxRot()._32;
	targetMtxRot._31 = inObjectX->GetMtxRot()._13;
	targetMtxRot._32 = inObjectX->GetMtxRot()._23;
	targetMtxRot._33 = inObjectX->GetMtxRot()._33;

	// 自身とターゲットの向きを合成
	D3DXMATRIX mtx = mtxThisRot * inObjectX->GetMtxRot();

	// 絶対値
	D3DXMATRIX mtxAds;
	mtxAds._11 = fabsf(mtx._11);
	mtxAds._12 = fabsf(mtx._21);
	mtxAds._13 = fabsf(mtx._31);
	mtxAds._21 = fabsf(mtx._12);
	mtxAds._22 = fabsf(mtx._22);
	mtxAds._23 = fabsf(mtx._32);
	mtxAds._31 = fabsf(mtx._13);
	mtxAds._32 = fabsf(mtx._23);
	mtxAds._33 = fabsf(mtx._33);

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
	D3DXVec3TransformCoord(&interval, &pos, &mtxThisRot);	// m_mtxRotではなく逆行列；

	if (debug)CDebugProc::Print("pos : %.2f,%.2f,%.2f \n", pos.x, pos.y, pos.z);
	if (debug)D3DXVec3Length(&interval) <= 50.0f ? CDebugProc::Print("★interval★ : %.2f,%.2f,%.2f \n", interval.x, interval.y, interval.z) : CDebugProc::Print("interval : %.2f,%.2f,%.2f \n", interval.x, interval.y, interval.z);

	if (debug)CDebugProc::Print("\nm_mtxRot\n");
	if (debug)CDebugProc::Print(" 1 : %.2f,%.2f,%.2f     ", m_mtxRot._11, m_mtxRot._12, m_mtxRot._13);
	if (debug)CDebugProc::Print(" 2 : %.2f,%.2f,%.2f     ", m_mtxRot._21, m_mtxRot._22, m_mtxRot._23);
	if (debug)CDebugProc::Print(" 3 : %.2f,%.2f,%.2f     ", m_mtxRot._31, m_mtxRot._32, m_mtxRot._33);
	if (debug)CDebugProc::Print("\nmtxThisRot\n");
	if (debug)CDebugProc::Print(" 1 : %.2f,%.2f,%.2f     ", mtxThisRot._11, mtxThisRot._12, mtxThisRot._13);
	if (debug)CDebugProc::Print(" 2 : %.2f,%.2f,%.2f     ", mtxThisRot._21, mtxThisRot._22, mtxThisRot._23);
	if (debug)CDebugProc::Print(" 3 : %.2f,%.2f,%.2f     ", mtxThisRot._31, mtxThisRot._32, mtxThisRot._33);
	if (debug)CDebugProc::Print("\ninObjectX->GetMtxRot()\n");
	if (debug)CDebugProc::Print(" 1 : %.2f,%.2f,%.2f     ", inObjectX->GetMtxRot()._11, inObjectX->GetMtxRot()._12, inObjectX->GetMtxRot()._13);
	if (debug)CDebugProc::Print(" 2 : %.2f,%.2f,%.2f     ", inObjectX->GetMtxRot()._21, inObjectX->GetMtxRot()._22, inObjectX->GetMtxRot()._23);
	if (debug)CDebugProc::Print(" 3 : %.2f,%.2f,%.2f     ", inObjectX->GetMtxRot()._31, inObjectX->GetMtxRot()._32, inObjectX->GetMtxRot()._33);
	if (debug)CDebugProc::Print("\nmtx\n");
	if (debug)CDebugProc::Print(" 1 : %.2f,%.2f,%.2f     ", mtx._11, mtx._12, mtx._13);
	if (debug)CDebugProc::Print(" 2 : %.2f,%.2f,%.2f     ", mtx._21, mtx._22, mtx._23);
	if (debug)CDebugProc::Print(" 3 : %.2f,%.2f,%.2f     ", mtx._31, mtx._32, mtx._33);
	if (debug)CDebugProc::Print("\n\n");

	//if (debug)CDebugProc::Print("mtxAds : %.2f,%.2f,%.2f   %.2f,%.2f,%.2f  %.2f,%.2f,%.2f│ ", mtxAds._11, mtxAds._12, mtxAds._13, mtxAds._21, mtxAds._22, mtxAds._23, mtxAds._31, mtxAds._32, mtxAds._33);
	//if (debug)CDebugProc::Print("interval : %.2f,%.2f,%.2f │", interval.x, interval.y, interval.z);

	float s;

	D3DXVECTOR3 thisScale = m_MaxVtx;
	{
		//D3DXVECTOR3 scale = m_scale * 0.5f;
		//thisScale.x *= scale.x;
		//thisScale.y *= scale.y;
		//thisScale.z *= scale.z;
		if (debug)CDebugProc::Print("thisScale : %.2f,%.2f,%.2f │", thisScale.x, thisScale.y, thisScale.z);
	}

	D3DXVECTOR3 targetScale = inObjectX->GetMaxVtx();
	{
		//D3DXVECTOR3 scale = inObjectX->GetScale() * 0.5f;
		//if (debug)CDebugProc::Print("\n");
		//if (debug)CDebugProc::Print("scale : %.2f,%.2f,%.2f │", inObjectX->GetScale().x, inObjectX->GetScale().y, inObjectX->GetScale().z);
		//if (debug)CDebugProc::Print("targetScale : %.2f,%.2f,%.2f \n", targetScale.x, targetScale.y, targetScale.z);
		//targetScale.x *= scale.x;
		//targetScale.y *= scale.y;
		//targetScale.z *= scale.z;
		//if (debug)CDebugProc::Print("scale : %.2f,%.2f,%.2f │", scale.x, scale.y, scale.z);
		if (debug)CDebugProc::Print("targetScale : %.2f,%.2f,%.2f \n", targetScale.x, targetScale.y, targetScale.z);
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
	s = fabs(interval.x) - (thisScale.x + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._11, mtxAds._12, mtxAds._13), &targetScale));
	if (TrackFaceAxis(&aAxis, 0, s, &aMax, D3DXVECTOR3(mtxThisRot._11, mtxThisRot._21, mtxThisRot._31), &nA))
	{
		if (debug)CDebugProc::Print("│A.e1\n");
		return false;
	}

	//A.e2
	s = fabs(interval.y) - (thisScale.y + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._21, mtxAds._22, mtxAds._23), &targetScale));
	if (TrackFaceAxis(&aAxis, 1, s, &aMax, D3DXVECTOR3(mtxThisRot._12, mtxThisRot._22, mtxThisRot._32), &nA))
	{
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│ A.e2\n");
		return false;
	}

	//A.e3
	s = fabs(interval.z) - (thisScale.z + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._31, mtxAds._32, mtxAds._33), &targetScale));
	if (TrackFaceAxis(&aAxis, 2, s, &aMax, D3DXVECTOR3(mtxThisRot._13, mtxThisRot._23, mtxThisRot._33), &nA))
	{
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│ A.e3\n");
		return false;
	}

	//B.e1
	s = fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._11, mtx._21, mtx._31))) - (targetScale.x + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._11, mtxAds._21, mtxAds._31), &thisScale));
	if (TrackFaceAxis(&bAxis, 3, s, &bMax, D3DXVECTOR3(targetMtxRot._11, targetMtxRot._21, targetMtxRot._31), &nB))
	{
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│ B.e1\n");
		return false;
	}

	//B.e2
	s = fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._12, mtx._22, mtx._32))) - (targetScale.y + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._12, mtxAds._22, mtxAds._32), &thisScale));
	if (TrackFaceAxis(&bAxis, 4, s, &bMax, D3DXVECTOR3(targetMtxRot._12, targetMtxRot._22, targetMtxRot._32), &nB))
	{
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│ B.e2\n");
		return false;
	}

	//B.e3
	s = fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._13, mtx._23, mtx._33))) - (targetScale.z + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._13, mtxAds._23, mtxAds._33),&thisScale));
	if (TrackFaceAxis(&bAxis, 5, s, &bMax, D3DXVECTOR3(targetMtxRot._13, targetMtxRot._23, targetMtxRot._33), &nB))
	{
		if (debug)CDebugProc::Print("s : %.2f", s);
		if (debug)CDebugProc::Print("│b : %.2f", fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._13, mtx._23, mtx._33))));
		if (debug)CDebugProc::Print("│a : %.2f", targetScale.z + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._13, mtxAds._23, mtxAds._33), &thisScale));
		if (debug)CDebugProc::Print("│a_1 : %.2f", targetScale.z);
		if (debug)CDebugProc::Print("│a_2 : %.2f", D3DXVec3Dot(&D3DXVECTOR3(mtxAds._13, mtxAds._23, mtxAds._33), &thisScale));
		if (debug)CDebugProc::Print("│ B.e3\n");
		return false;
	}

	// 平行ではない場合
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
		thisRadius = thisScale.y * mtxAds._31 + thisScale.z * mtxAds._21;
		targetRadius = targetScale.y * mtxAds._13 + targetScale.z * mtxAds._12;
		s = fabs(interval.z * mtx._21 - interval.y * mtx._31) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 6, s, &eMax, D3DXVECTOR3(0.0f, -mtx._31, mtx._21), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C11\n");
			return false;
		}

		//C12
		thisRadius = thisScale.y * mtxAds._32 + thisScale.z * mtxAds._22;
		targetRadius = targetScale.x * mtxAds._13 + targetScale.z * mtxAds._11;
		s = fabs(interval.z * mtx._22 - interval.y * mtx._32) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 7, s, &eMax, D3DXVECTOR3(0.0f, -mtx._32, mtx._22), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C12\n");
			return false;
		}

		//C13
		thisRadius = thisScale.y * mtxAds._33 + thisScale.z * mtxAds._23;
		targetRadius = targetScale.x * mtxAds._12 + targetScale.y * mtxAds._11;
		s = fabs(interval.z * mtx._23 - interval.y * mtx._33) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 8, s, &eMax, D3DXVECTOR3(0.0f, -mtx._33, mtx._23), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C13\n");
			return false;
		}

		//C21
		thisRadius = thisScale.x * mtxAds._31 + thisScale.z * mtxAds._11;
		targetRadius = targetScale.y * mtxAds._23 + targetScale.z * mtxAds._22;
		s = fabs(interval.x * mtx._31 - interval.z * mtx._11) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 9, s, &eMax, D3DXVECTOR3(mtx._31, 0.0f, -mtx._11), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f│ ", s);
			if (debug)CDebugProc::Print("a : %.2f│ ", fabs(interval.x * mtx._31 - interval.z * mtx._11));
			if (debug)CDebugProc::Print("Scale : %.2f,%.2f│ ", thisScale.x, thisScale.z);
			if (debug)CDebugProc::Print("this : %.2f│ ", thisRadius);
			if (debug)CDebugProc::Print("target : %.2f│ ", targetRadius);
			if (debug)CDebugProc::Print("C21\n");
			return false;
		}

		//C22
		thisRadius = thisScale.x * mtxAds._32 + thisScale.z * mtxAds._12;
		targetRadius = targetScale.x * mtxAds._23 + targetScale.z * mtxAds._21;
		s = fabs(interval.x * mtx._32 - interval.z * mtx._12) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 10, s, &eMax, D3DXVECTOR3(mtx._32, 0.0f, -mtx._12), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C22\n");
			return false;
		}

		//C23
		thisRadius = thisScale.x * mtxAds._33 + thisScale.z * mtxAds._13;
		targetRadius = targetScale.x * mtxAds._22 + targetScale.y * mtxAds._21;
		s = fabs(interval.x * mtx._33 - interval.z * mtx._13) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 11, s, &eMax, D3DXVECTOR3(mtx._33, 0.0f, -mtx._13), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C23\n");
			return false;
		}

		//C31
		thisRadius = thisScale.x * mtxAds._21 + thisScale.y * mtxAds._11;
		targetRadius = targetScale.y * mtxAds._33 + targetScale.z * mtxAds._32;
		s = fabs(interval.y * mtx._11 - interval.x * mtx._21) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 12, s, &eMax, D3DXVECTOR3(-mtx._21, mtx._11, 0.0f), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C31\n");
			return false;
		}

		//C32
		thisRadius = thisScale.x * mtxAds._22 + thisScale.y * mtxAds._12;
		targetRadius = targetScale.x * mtxAds._33 + targetScale.z * mtxAds._31;
		s = fabs(interval.y * mtx._12 - interval.x * mtx._22) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 13, s, &eMax, D3DXVECTOR3(-mtx._22, mtx._12, 0.0f), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C32\n");
			return false;
		}

		//C33
		thisRadius = thisScale.x * mtxAds._23 + thisScale.y * mtxAds._13;
		targetRadius = targetScale.x * mtxAds._32 + targetScale.y * mtxAds._31;
		s = fabs(interval.y * mtx._13 - interval.x * mtx._23) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 14, s, &eMax, D3DXVECTOR3(-mtx._23, mtx._13, 0.0f), &nE))
		{
			if (debug)CDebugProc::Print("s : %.2f", s);
			if (debug)CDebugProc::Print("│ C33\n");
			return false;
		}
	}

	if (debug)CDebugProc::Print("s : %.2f", s);
	if (debug)CDebugProc::Print("│ ★Hit★\n");

	/* ここまで通れば接触してる */

	// Artificial axis bias to improve frame coherence
	const float kRelTol = 0.95f;
	const float kAbsTol = 0.01f;
	int axis;
	float sMax;
	D3DXVECTOR3 n;
	
	float faceMax;
	if (aMax > bMax)
	{
		faceMax = aMax;
	}
	else
	{
		faceMax = bMax;
	}
	
	if (kRelTol * eMax > faceMax + kAbsTol)
	{
		axis = eAxis;
		sMax = eMax;
		n = nE;
	}
	else
	{
		if (kRelTol * bMax > aMax + kAbsTol)
		{
			axis = bAxis;
			sMax = bMax;
			n = nB;
		}
		else
		{
			axis = aAxis;
			sMax = aMax;
			n = nA;
		}
	}
	
	if (D3DXVec3Dot(&n, &(GetPos() - inObjectX->GetPos())) < 0.0f)
	{
		n = -n;
	}
	
	if (axis == ~0)
	{
		assert(false);
		return true;
	}
	
	if (debug)CDebugProc::Print("軸数: %d \n", axis);

	if (axis < 6)
	{
		D3DXVECTOR3 rtx_pos;
		D3DXVECTOR3 itx_pos;
		D3DXMATRIX rtx_mtxRot;
		D3DXMATRIX itx_mtxRot;
		D3DXVECTOR3 eR;
		D3DXVECTOR3 eI;
		bool flip;
	
		if (axis < 3)
		{
			rtx_pos = m_pos;
			itx_pos = inObjectX->GetPos();
			rtx_mtxRot = m_mtxRot;
			itx_mtxRot = inObjectX->GetMtxRot();
			eR = thisScale;
			eI = targetScale;
			flip = false;
		}
		else
		{
			rtx_pos = inObjectX->GetPos();
			itx_pos = m_pos;
			rtx_mtxRot = inObjectX->GetMtxRot();
			itx_mtxRot = m_mtxRot;
			eR = targetScale;
			eI = thisScale;
			flip = true;
			n = -n;
		}
	
		// Compute reference and incident edge information necessary for clipping
		ClipVertex incident[4];
		ComputeIncidentFace(itx_pos, itx_mtxRot, eI, n, incident);

		char clipEdges[4];
		D3DXMATRIX basis;
		D3DXVECTOR3 e;
		ComputeReferenceEdgesAndBasis(eR, rtx_pos, rtx_mtxRot, n, axis, clipEdges, &basis, &e);
	
		// Clip the incident face against the reference face side planes
		ClipVertex out[8];
		float depths[8];
		int outNum;
		outNum = Clip(rtx_pos, e, clipEdges, basis, incident, out, depths);
	
		if (outNum)
		{
			//m->contactCount = outNum;
			//m->normal = flip ? -n : n;
	
			if (debug)CDebugProc::Print("方向 : %.2f,%.2f,%.2f \n", flip ? -n.x : n.x, flip ? -n.y : n.y, flip ? -n.z : n.z);

			for (int i = 0; i < outNum; ++i)
			{
				if (debug)CDebugProc::Print("戻す位置？ : %.2f,%.2f,%.2f \n", out[i].v.x, out[i].v.y, out[i].v.z);
				if (debug)CDebugProc::Print("貫通してる距離？ : %.2f \n", depths[i]);
				//q3Contact* c = m->contacts + i;
	
				//FeaturePair pair = out[i].f;
	
				//if (flip)
				//{
				//	std::swap(pair.inI, pair.inR);
				//	std::swap(pair.outI, pair.outR);
				//}
	
				//c->fp = out[i].f;
				//c->position = out[i].v;
				//c->penetration = depths[i];
			}
		}
	}
	else
	{
		D3DXVECTOR3 vector = n;
		n.x = m_mtxRot._11 * vector.x + m_mtxRot._12 * vector.y + m_mtxRot._13 * vector.z;
		n.y = m_mtxRot._21 * vector.x + m_mtxRot._22 * vector.y + m_mtxRot._23 * vector.z;
		n.z = m_mtxRot._31 * vector.x + m_mtxRot._32 * vector.y + m_mtxRot._33 * vector.z;

		D3DXVECTOR3 dist = inObjectX->GetPos() - m_pos;
		if (D3DXVec3Dot(&n, &dist) < 0.0f)
		{
			n = -n;
		}

		D3DXVECTOR3 PA, QA;
		D3DXVECTOR3 PB, QB;

		auto SupportEdge = [](const D3DXVECTOR3& tx_pos, const D3DXMATRIX& tx_rot, const D3DXVECTOR3& e, D3DXVECTOR3 n, D3DXVECTOR3* aOut, D3DXVECTOR3* bOut)
		{
			D3DXVECTOR3 vector = n;
			n.x = tx_rot._11 * vector.x + tx_rot._21 * vector.y + tx_rot._31 * vector.z;
			n.y = tx_rot._12 * vector.x + tx_rot._22 * vector.y + tx_rot._32 * vector.z;
			n.z = tx_rot._13 * vector.x + tx_rot._23 * vector.y + tx_rot._33 * vector.z;

			D3DXVECTOR3 absN;
			absN.x = fabs(n.x);
			absN.y = fabs(n.y);
			absN.z = fabs(n.z);
			D3DXVECTOR3 a, b;

			// x > y
			if (absN.x > absN.y)
			{
				// x > y > z
				if (absN.y > absN.z)
				{
					a = D3DXVECTOR3(e.x, e.y, e.z);
					b = D3DXVECTOR3(e.x, e.y, -e.z);
				}

				// x > z > y || z > x > y
				else
				{
					a = D3DXVECTOR3(e.x, e.y, e.z);
					b = D3DXVECTOR3(e.x, -e.y, e.z);
				}
			}

			// y > x
			else
			{
				// y > x > z
				if (absN.x > absN.z)
				{
					a = D3DXVECTOR3(e.x, e.y, e.z);
					b = D3DXVECTOR3(e.x, e.y, -e.z);
				}

				// z > y > x || y > z > x
				else
				{
					a = D3DXVECTOR3(e.x, e.y, e.z);
					b = D3DXVECTOR3(-e.x, e.y, e.z);
				}
			}
			
			float signx;;
			if (n.x >= 0.0f)
			{
				signx = 1.0f;
			}
			else
			{
				signx = -1.0f;
			}

			float signy;
			if (n.y >= 0.0f)
			{
				signy = 1.0f;
			}
			else
			{
				signy = -1.0f;
			}

			float signz;
			if (n.z >= 0.0f)
			{
				signz = 1.0f;
			}
			else
			{
				signz = -1.0f;
			}

			a.x *= signx;
			a.y *= signy;
			a.z *= signz;
			b.x *= signx;
			b.y *= signy;
			b.z *= signz;

			aOut->x = tx_rot._11 * a.x + tx_rot._12 * a.y + tx_rot._13 * a.z;
			aOut->y = tx_rot._21 * a.x + tx_rot._22 * a.y + tx_rot._23 * a.z;
			aOut->z = tx_rot._31 * a.x + tx_rot._32 * a.y + tx_rot._33 * a.z;
			*aOut += tx_pos;

			bOut->x = tx_rot._11 * b.x + tx_rot._12 * b.y + tx_rot._13 * b.z;
			bOut->y = tx_rot._21 * b.x + tx_rot._22 * b.y + tx_rot._23 * b.z;
			bOut->z = tx_rot._31 * b.x + tx_rot._32 * b.y + tx_rot._33 * b.z;
			*bOut += tx_pos;
		};

		SupportEdge(m_pos, m_mtxRot, thisScale, n, &PA, &QA);
		SupportEdge(inObjectX->GetPos(), inObjectX->GetMtxRot(), targetScale, -n, &PB, &QB);

		D3DXVECTOR3 CA, CB;
		{
			D3DXVECTOR3 DA = QA - PA;
			D3DXVECTOR3 DB = QB - PB;
			D3DXVECTOR3 r = PA - PB;
			float a = D3DXVec3Dot(&DA, &DA);
			float e = D3DXVec3Dot(&DB, &DB);
			float f = D3DXVec3Dot(&DB, &r);
			float c = D3DXVec3Dot(&DA, &r);

			float b = D3DXVec3Dot(&DA, &DB);
			float denom = a * e - b * b;

			float TA = (b * f - c * e) / denom;
			float TB = (b * TA + f) / e;

			CA = PA + DA * TA;
			CB = PB + DB * TB;
		}

		CDebugProc::Print("戻す位置 : %.2f,%.2f,%.2f\n", (CA.x + CB.x) * 0.5f, (CA.y + CB.y) * 0.5f, (CA.z + CB.z) * 0.5f);
		CDebugProc::Print("貫通してる距離？ : %.2f \n", sMax);
		//m->normal = n;
		//m->contactCount = 1;
	
		//q3Contact* c = m->contacts;
		//FeaturePair pair;
		//pair.key = axis;
		//c->fp = pair;
		//c->penetration = sMax;
		//c->position = (CA + CB) * 0.5f;
	}

	return true;
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
		D3DXMATRIX mtxRot = m_mtxRot;

		D3DXVECTOR3 size = GetMaxVtx();

		thisNormalizeVecX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		thisNormalizeVecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		thisNormalizeVecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		D3DXVec3TransformCoord(&thisNormalizeVecX, &thisNormalizeVecX, &mtxRot);
		D3DXVec3TransformCoord(&thisNormalizeVecY, &thisNormalizeVecY, &mtxRot);
		D3DXVec3TransformCoord(&thisNormalizeVecZ, &thisNormalizeVecZ, &mtxRot);
		D3DXVec3Normalize(&thisNormalizeVecX, &thisNormalizeVecX);
		D3DXVec3Normalize(&thisNormalizeVecY, &thisNormalizeVecY);
		D3DXVec3Normalize(&thisNormalizeVecZ, &thisNormalizeVecZ);
		thisVecX = thisNormalizeVecX * size.x;
		thisVecY = thisNormalizeVecY * size.y;
		thisVecZ = thisNormalizeVecZ * size.z;
	}

	D3DXVECTOR3 targetVecX;
	D3DXVECTOR3 targetVecY;
	D3DXVECTOR3 targetVecZ;
	D3DXVECTOR3 targetNormalizeVecX;
	D3DXVECTOR3 targetNormalizeVecY;
	D3DXVECTOR3 targetNormalizeVecZ;

	{
		// 計算用マトリックス
		D3DXMATRIX mtxRot = inObjectX->GetMtxRot();

		D3DXVECTOR3 size = inObjectX->GetMaxVtx();

		targetNormalizeVecX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		targetNormalizeVecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		targetNormalizeVecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		D3DXVec3TransformCoord(&targetNormalizeVecX, &targetNormalizeVecX, &mtxRot);
		D3DXVec3TransformCoord(&targetNormalizeVecY, &targetNormalizeVecY, &mtxRot);
		D3DXVec3TransformCoord(&targetNormalizeVecZ, &targetNormalizeVecZ, &mtxRot);
		D3DXVec3Normalize(&targetNormalizeVecX, &targetNormalizeVecX);
		D3DXVec3Normalize(&targetNormalizeVecY, &targetNormalizeVecY);
		D3DXVec3Normalize(&targetNormalizeVecZ, &targetNormalizeVecZ);
		targetVecX = targetNormalizeVecX * size.x;
		targetVecY = targetNormalizeVecY * size.y;
		targetVecZ = targetNormalizeVecZ * size.z;
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
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecX, &targetVecY);
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
		targetRadius = LenSegOnSeparateAxis(&Cross, &targetVecY, &targetVecZ);
		length = fabs(D3DXVec3Dot(&interval, &Cross));
		if (length > thisRadius + targetRadius)
		{
			return false;
		}
	}

	//C32
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &thisNormalizeVecZ, &targetNormalizeVecY);
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

	//CDebugProc::Print("★HIT★\n");
	return true;
}

//=============================================================================
// RayとOBBの当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::RayAndAABB(const D3DXVECTOR3 & inPos, const D3DXVECTOR3 & inNormal, D3DXVECTOR3* outPos)
{
	// 光線を境界ボックスの空間へ移動
	D3DXMATRIX invMat;
	D3DXMatrixInverse(&invMat, 0, &m_mtxWorld);

	D3DXVECTOR3 p_l, dir_l;
	D3DXVec3TransformCoord(&p_l, &inPos, &invMat);
	invMat._41 = 0.0f;
	invMat._42 = 0.0f;
	invMat._43 = 0.0f;
	D3DXVec3TransformCoord(&dir_l, &inNormal, &invMat);

	// 交差判定
	float p[3], d[3], min[3], max[3];
	memcpy(p, &p_l, sizeof(D3DXVECTOR3));
	memcpy(d, &dir_l, sizeof(D3DXVECTOR3));
	memcpy(min, &(m_MinVtx * 0.5f), sizeof(D3DXVECTOR3));
	memcpy(max, &(m_MaxVtx * 0.5f), sizeof(D3DXVECTOR3));

	float t = -FLT_MAX;
	float t_max = FLT_MAX;

	for (int i = 0; i < 3; ++i)
	{
		if (abs(d[i]) < FLT_EPSILON)
		{
			if (p[i] < min[i] || p[i] > max[i])
			{
				return false; // 交差していない
			}
		}
		else
		{
			// スラブとの距離を算出
			// t1が近スラブ、t2が遠スラブとの距離
			float odd = 1.0f / d[i];
			float t1 = (min[i] - p[i]) * odd;
			float t2 = (max[i] - p[i]) * odd;
			if (t1 > t2) {
				float tmp = t1; t1 = t2; t2 = tmp;
			}

			if (t1 > t) t = t1;
			if (t2 < t_max) t_max = t2;

			// スラブ交差チェック
			if (t >= t_max)
			{
				return false;
			}
		}
	}

	if (outPos != nullptr)
	{
		*outPos = inPos + (t * inNormal);
	}
	CDebugProc::Print("★Hit★\n");
	return true;
}

//=============================================================================
// 線分とAABBの当たり判定
// Author : Yuda Kaito
//=============================================================================
bool CObjectX::SegmentAndAABB(const D3DXVECTOR3 & inPos, const D3DXVECTOR3 & inPos2)
{
	D3DXVECTOR3 dist = inPos - inPos2;
	D3DXVECTOR3 hitPos(0.0f,0.0f,0.0f);

	D3DXVec3Normalize(&dist,&dist);

	D3DXVECTOR3 min;
	D3DXVECTOR3 max;

	if (inPos.x < inPos2.x)
	{
		min.x = inPos.x;
		max.x = inPos2.x;
	}
	else
	{
		min.x = inPos2.x;
		max.x = inPos.x;
	}

	if (inPos.y < inPos2.y)
	{
		min.y = inPos.y;
		max.y = inPos2.y;
	}
	else
	{
		min.y = inPos2.y;
		max.y = inPos.y;
	}

	if (inPos.z < inPos2.z)
	{
		min.z = inPos.z;
		max.z = inPos2.z;
	}
	else
	{
		min.z = inPos2.z;
		max.z = inPos.z;
	}

	if (RayAndAABB(inPos, dist, &hitPos))
	{
		if (min.x < hitPos.x &&
			min.y < hitPos.y &&
			min.z < hitPos.z &&
			hitPos.x < max.x &&
			hitPos.y < max.y &&
			hitPos.z < max.z)
		{
			return true;
		}
	}
	return false;
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

void CObjectX::ComputeIncidentFace(const D3DXVECTOR3 & itx_pos, const D3DXMATRIX & itx_mtxRot, const D3DXVECTOR3 & e, D3DXVECTOR3 n, ClipVertex * out)
{
	D3DXVECTOR3 normal = n;
	n.x = itx_mtxRot._11 * normal.x + itx_mtxRot._21 * normal.y + itx_mtxRot._31 * normal.z;
	n.y = itx_mtxRot._12 * normal.x + itx_mtxRot._22 * normal.y + itx_mtxRot._32 * normal.z;
	n.z = itx_mtxRot._13 * normal.x + itx_mtxRot._23 * normal.y + itx_mtxRot._33 * normal.z;

	D3DXVECTOR3 absN;
	absN.x = fabs(n.x);
	absN.y = fabs(n.y);
	absN.z = fabs(n.z);

	if (absN.x > absN.y && absN.x > absN.z)
	{
		if (n.x > 0.0f)
		{
			out[0].v = D3DXVECTOR3(e.x, e.y, -e.z);
			out[1].v = D3DXVECTOR3(e.x, e.y, e.z);
			out[2].v = D3DXVECTOR3(e.x, -e.y, e.z);
			out[3].v = D3DXVECTOR3(e.x, -e.y, -e.z);

			out[0].f.ri.inI = 9;
			out[0].f.ri.outI = 1;
			out[1].f.ri.inI = 1;
			out[1].f.ri.outI = 8;
			out[2].f.ri.inI = 8;
			out[2].f.ri.outI = 7;
			out[3].f.ri.inI = 7;
			out[3].f.ri.outI = 9;
		}

		else
		{
			out[0].v = D3DXVECTOR3(-e.x, -e.y, e.z);
			out[1].v = D3DXVECTOR3(-e.x, e.y, e.z);
			out[2].v = D3DXVECTOR3(-e.x, e.y, -e.z);
			out[3].v = D3DXVECTOR3(-e.x, -e.y, -e.z);

			out[0].f.ri.inI = 5;
			out[0].f.ri.outI = 11;
			out[1].f.ri.inI = 11;
			out[1].f.ri.outI = 3;
			out[2].f.ri.inI = 3;
			out[2].f.ri.outI = 10;
			out[3].f.ri.inI = 10;
			out[3].f.ri.outI = 5;
		}
	}

	else if (absN.y > absN.x && absN.y > absN.z)
	{
		if (n.y > 0.0f)
		{
			out[0].v = D3DXVECTOR3(-e.x, e.y, e.z);
			out[1].v = D3DXVECTOR3(e.x, e.y, e.z);
			out[2].v = D3DXVECTOR3(e.x, e.y, -e.z);
			out[3].v = D3DXVECTOR3(-e.x, e.y, -e.z);

			out[0].f.ri.inI = 3;
			out[0].f.ri.outI = 0;
			out[1].f.ri.inI = 0;
			out[1].f.ri.outI = 1;
			out[2].f.ri.inI = 1;
			out[2].f.ri.outI = 2;
			out[3].f.ri.inI = 2;
			out[3].f.ri.outI = 3;
		}

		else
		{
			out[0].v = D3DXVECTOR3(e.x, -e.y, e.z);
			out[1].v = D3DXVECTOR3(-e.x, -e.y, e.z);
			out[2].v = D3DXVECTOR3(-e.x, -e.y, -e.z);
			out[3].v = D3DXVECTOR3(e.x, -e.y, -e.z);

			out[0].f.ri.inI = 7;
			out[0].f.ri.outI = 4;
			out[1].f.ri.inI = 4;
			out[1].f.ri.outI = 5;
			out[2].f.ri.inI = 5;
			out[2].f.ri.outI = 6;
			out[3].f.ri.inI = 5;
			out[3].f.ri.outI = 6;
		}
	}

	else
	{
		if (n.z > 0.0f)
		{
			out[0].v = D3DXVECTOR3(-e.x, e.y, e.z);
			out[1].v = D3DXVECTOR3(-e.x, -e.y, e.z);
			out[2].v = D3DXVECTOR3(e.x, -e.y, e.z);
			out[3].v = D3DXVECTOR3(e.x, e.y, e.z);

			out[0].f.ri.inI = 0;
			out[0].f.ri.outI = 11;
			out[1].f.ri.inI = 11;
			out[1].f.ri.outI = 4;
			out[2].f.ri.inI = 4;
			out[2].f.ri.outI = 8;
			out[3].f.ri.inI = 8;
			out[3].f.ri.outI = 0;
		}

		else
		{
			out[0].v = D3DXVECTOR3(e.x, -e.y, -e.z);
			out[1].v = D3DXVECTOR3(-e.x, -e.y, -e.z);
			out[2].v = D3DXVECTOR3(-e.x, e.y, -e.z);
			out[3].v = D3DXVECTOR3(e.x, e.y, -e.z);

			out[0].f.ri.inI = 9;
			out[0].f.ri.outI = 6;
			out[1].f.ri.inI = 6;
			out[1].f.ri.outI = 10;
			out[2].f.ri.inI = 10;
			out[2].f.ri.outI = 2;
			out[3].f.ri.inI = 2;
			out[3].f.ri.outI = 9;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		D3DXVECTOR3 vector = out[i].v + itx_pos;
		out[i].v.x = itx_mtxRot._11 * vector.x + itx_mtxRot._12 * vector.y + itx_mtxRot._13 * vector.z;
		out[i].v.y = itx_mtxRot._21 * vector.x + itx_mtxRot._22 * vector.y + itx_mtxRot._23 * vector.z;
		out[i].v.z = itx_mtxRot._31 * vector.x + itx_mtxRot._32 * vector.y + itx_mtxRot._33 * vector.z;
	}
}

void CObjectX::ComputeReferenceEdgesAndBasis(const D3DXVECTOR3 & eR, const D3DXVECTOR3 & rtx_pos, const D3DXMATRIX & rtx_mtxRot, D3DXVECTOR3 n, int axis, char * out, D3DXMATRIX * basis, D3DXVECTOR3 * e)
{
	D3DXVECTOR3 normal = n;
	n.x = rtx_mtxRot._11 * normal.x + rtx_mtxRot._21 * normal.y + rtx_mtxRot._31 * normal.z;
	n.y = rtx_mtxRot._12 * normal.x + rtx_mtxRot._22 * normal.y + rtx_mtxRot._32 * normal.z;
	n.z = rtx_mtxRot._13 * normal.x + rtx_mtxRot._23 * normal.y + rtx_mtxRot._33 * normal.z;

	if (axis >= 3)
		axis -= 3;

	switch (axis)
	{
	case 0:
		if (n.x > 0.0f)
		{
			out[0] = 1;
			out[1] = 8;
			out[2] = 7;
			out[3] = 9;

			*e = D3DXVECTOR3(eR.y, eR.z, eR.x);

			basis->_11 = rtx_mtxRot._12;
			basis->_21 = rtx_mtxRot._22;
			basis->_31 = rtx_mtxRot._32;

			basis->_12 = rtx_mtxRot._13;
			basis->_22 = rtx_mtxRot._23;
			basis->_32 = rtx_mtxRot._33;

			basis->_13 = rtx_mtxRot._11;
			basis->_23 = rtx_mtxRot._21;
			basis->_33 = rtx_mtxRot._31;
		}

		else
		{
			out[0] = 11;
			out[1] = 3;
			out[2] = 10;
			out[3] = 5;

			*e = D3DXVECTOR3(eR.z, eR.y, eR.x);

			basis->_11 = rtx_mtxRot._13;
			basis->_21 = rtx_mtxRot._23;
			basis->_31 = rtx_mtxRot._33;

			basis->_12 = rtx_mtxRot._12;
			basis->_22 = rtx_mtxRot._22;
			basis->_32 = rtx_mtxRot._32;

			basis->_13 = -rtx_mtxRot._11;
			basis->_23 = -rtx_mtxRot._21;
			basis->_33 = -rtx_mtxRot._31;
		}
		break;

	case 1:
		if (n.y > 0.0f)
		{
			out[0] = 0;
			out[1] = 1;
			out[2] = 2;
			out[3] = 3;

			*e = D3DXVECTOR3(eR.z, eR.x, eR.y);

			basis->_11 = rtx_mtxRot._13;
			basis->_21 = rtx_mtxRot._23;
			basis->_31 = rtx_mtxRot._33;

			basis->_12 = rtx_mtxRot._11;
			basis->_22 = rtx_mtxRot._21;
			basis->_32 = rtx_mtxRot._31;

			basis->_13 = rtx_mtxRot._12;
			basis->_23 = rtx_mtxRot._22;
			basis->_33 = rtx_mtxRot._32;
		}

		else
		{
			out[0] = 4;
			out[1] = 5;
			out[2] = 6;
			out[3] = 7;

			*e = D3DXVECTOR3(eR.z, eR.x, eR.y);

			basis->_11 = rtx_mtxRot._13;
			basis->_21 = rtx_mtxRot._23;
			basis->_31 = rtx_mtxRot._33;

			basis->_12 = -rtx_mtxRot._11;
			basis->_22 = -rtx_mtxRot._21;
			basis->_32 = -rtx_mtxRot._31;

			basis->_13 = -rtx_mtxRot._12;
			basis->_23 = -rtx_mtxRot._22;
			basis->_33 = -rtx_mtxRot._32;
		}
		break;

	case 2:
		if (n.z > 0.0f)
		{
			out[0] = 11;
			out[1] = 4;
			out[2] = 8;
			out[3] = 0;

			*e = D3DXVECTOR3(eR.y, eR.x, eR.z);

			basis->_11 = -rtx_mtxRot._12;
			basis->_21 = -rtx_mtxRot._22;
			basis->_31 = -rtx_mtxRot._32;

			basis->_12 = rtx_mtxRot._11;
			basis->_22 = rtx_mtxRot._21;
			basis->_32 = rtx_mtxRot._31;

			basis->_13 = rtx_mtxRot._13;
			basis->_23 = rtx_mtxRot._23;
			basis->_33 = rtx_mtxRot._33;
		}

		else
		{
			out[0] = 6;
			out[1] = 10;
			out[2] = 2;
			out[3] = 9;

			*e = D3DXVECTOR3(eR.y, eR.x, eR.z);

			basis->_11 = -rtx_mtxRot._12;
			basis->_21 = -rtx_mtxRot._22;
			basis->_31 = -rtx_mtxRot._32;

			basis->_12 = -rtx_mtxRot._11;
			basis->_22 = -rtx_mtxRot._21;
			basis->_32 = -rtx_mtxRot._31;

			basis->_13 = -rtx_mtxRot._13;
			basis->_23 = -rtx_mtxRot._23;
			basis->_33 = -rtx_mtxRot._33;
		}
		break;
	}
}

int CObjectX::Clip(const D3DXVECTOR3 & rPos, const D3DXVECTOR3 & e, char * clipEdges, const D3DXMATRIX & basis, ClipVertex * incident, ClipVertex * outVerts, float * outDepths)
{
	int inCount = 4;
	int outCount;
	ClipVertex in[8];
	ClipVertex out[8];

	for (int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 dist = incident[i].v - rPos;
		in[i].v.x = basis._11 * dist.x + basis._21 * dist.y + basis._31 * dist.z;
		in[i].v.y = basis._12 * dist.x + basis._22 * dist.y + basis._32 * dist.z;
		in[i].v.z = basis._13 * dist.x + basis._23 * dist.y + basis._33 * dist.z;
	}

	auto Orthographic = [](float sign, float e, int axis, int clipEdge, ClipVertex* in, int inCount, ClipVertex* out)
	{
		int outCount = 0;
		ClipVertex a = in[inCount - 1];

		for (int i = 0; i < inCount; ++i)
		{
			ClipVertex b = in[i];

			float da = sign * a.v[axis] - e;
			float db = sign * b.v[axis] - e;

			ClipVertex cv;

			// B
			if (((InFront(da) && InFront(db)) || On(da) || On(db)))
			{
				assert(outCount < 8);
				out[outCount++] = b;
			}

			// I
			else if (InFront(da) && Behind(db))
			{
				cv.f = b.f;
				cv.v = a.v + (b.v - a.v) * (da / (da - db));
				cv.f.ri.outR = (char)clipEdge;
				cv.f.ri.outI = 0;
				assert(outCount < 8);
				out[outCount++] = cv;
			}

			// I, B
			else if (Behind(da) && InFront(db))
			{
				cv.f = a.f;
				cv.v = a.v + (b.v - a.v) * (da / (da - db));
				cv.f.ri.inR = (char)clipEdge;
				cv.f.ri.inI = 0;
				assert(outCount < 8);
				out[outCount++] = cv;

				assert(outCount < 8);
				out[outCount++] = b;
			}

			a = b;
		}

		return outCount;
	};

	outCount = Orthographic(1.0f, e.x, 0, clipEdges[0], in, inCount, out);

	if (!outCount)
		return 0;

	inCount = Orthographic(1.0f, e.y, 1, clipEdges[1], out, outCount, in);

	if (!inCount)
		return 0;

	outCount = Orthographic(-1.0f, e.x, 0, clipEdges[2], in, inCount, out);

	if (!outCount)
		return 0;

	inCount = Orthographic(-1.0f, e.y, 1, clipEdges[3], out, outCount, in);

	// Keep incident vertices behind the reference face
	outCount = 0;
	for (int i = 0; i < inCount; ++i)
	{
		float d = in[i].v.z - e.z;

		if (d <= 0.0f)
		{
			D3DXVECTOR3 vector = in[i].v;
			outVerts[outCount].v .x = basis._11 * vector.x + basis._12 * vector.y + basis._13 * vector.z;
			outVerts[outCount].v .y = basis._21 * vector.x + basis._22 * vector.y + basis._23 * vector.z;
			outVerts[outCount].v .z = basis._31 * vector.x + basis._32 * vector.y + basis._33 * vector.z;
			outVerts[outCount].v += rPos;
			outVerts[outCount].f = in[i].f;
			outDepths[outCount++] = d;
		}
	}

	assert(outCount <= 8);

	return outCount;
}
