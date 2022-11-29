//=============================================================================
//
// メッシュ設定(まっすぐ)
// Author:hamada ryuuga
//
//=============================================================================

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object_mesh.h"
#include "application.h"
#include "utility.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <nlohmann/json.hpp>
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <sstream>

nlohmann::json JMesh;//リストの生成

//=============================================================================
// コンストラクタ
//=============================================================================
CMesh::CMesh(CTaskGroup::EPriority nPriority) :
	CObjectPolygon3D(nPriority),
	m_xsiz(0),						// 面数
	m_zsiz(0),						// 面数
	m_vtxCountX(0),					// 辺の頂点数
	m_vtxCountZ(0),					// 辺の頂点数
	m_vtx(0),						// 頂点数
	m_index(0),						// インデックス
	m_polygonCount(0),
	m_NowMesh(0),
	m_Number(0),
	m_Type(0),
	m_move(5.0f),
	IsCollision(true)
{
	m_MeshSize = { 100.0f,0.0f,100.0f };
}

//=============================================================================
// デストラクタ
//=============================================================================
CMesh::~CMesh()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMesh::Init(void)
{
	// 初期化処理
	m_pVtxBuff = nullptr;		// 頂点バッファーへのポインタ
	m_pIdxBuff = nullptr;		// インデックスバッファ
	
	//m_pFileName = nullptr;

	//テクスチャの読み込み

	SetMesh(10);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMesh::Uninit(void)
{
	// 頂点バッファーの解放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMesh::NormalUpdate(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CMesh::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	//ライト設定falseにするとライトと食らわない
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	// 行列初期化関数(第1引数の行列を単位行列に初期化)
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールド座標行列の設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファ設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの取得
	CTexture* pTexture = CApplication::GetInstance()->GetTexture();

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(GetTexture()));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_vtx, 0, m_polygonCount);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// Create関数
//=============================================================================
CMesh* CMesh::Create()
{
	CMesh * pObject = nullptr;
	pObject = new CMesh;

	if (pObject != nullptr)
	{
		
		pObject->Init();

	}
	return pObject;
}


//=============================================================================
// 当たり判定
// Author: hamada ryuuga
// Aythor: Yuda Kaito
//=============================================================================
bool CMesh::CollisionMesh(D3DXVECTOR3 *pPos)
{

	bool bIsLanding = false;
	const int nTri = 3;
	// 頂点座標をロック
	VERTEX_3D* pVtx = NULL;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファのロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxWorld;

	// ワールドマトリックスの初期化
	// 行列初期化関数(第1引数の行列を単位行列に初期化)
	D3DXMatrixIdentity(&mtxWorld);

	// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	// 行列掛け算関数(第2引数×第3引数を第１引数に格納)
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	for (int nCnt = 0; nCnt < m_polygonCount; nCnt++)
	{
		D3DXVECTOR3 posLineVec[nTri];

		posLineVec[0] = pVtx[pIdx[nCnt + 0]].pos;
		posLineVec[1] = pVtx[pIdx[nCnt + 1]].pos;
		posLineVec[2] = pVtx[pIdx[nCnt + 2]].pos;

		if ((pIdx[nCnt + 0] == pIdx[nCnt + 1]) ||
			(pIdx[nCnt + 0] == pIdx[nCnt + 2]) ||
			(pIdx[nCnt + 2] == pIdx[nCnt + 1]))
		{//縮退ポリゴンを省き
			continue;
		}

		for (int i = 0; i < nTri; i++)
		{//ベクトル３座標をマトリックスで変換する（乗算）
			D3DXVec3TransformCoord(&posLineVec[i], &posLineVec[i], &mtxWorld);
		}

		int  LineCout = 0;

		for (int i = 0; i < nTri; i++)
		{
			//ベクトルS2 V2												
			D3DXVECTOR3 vecWall = posLineVec[(i + 1) % nTri] - posLineVec[i];

			//ベクトル現在のPOSと始点までの距離
			D3DXVECTOR3 vecPos = *pPos - posLineVec[i];

			//外積計算//辺１
			float vecLine = Vec2Cross(&vecPos, &vecWall);

			//三角の中に入ってるときの判定向きによって右側か左側か違うため判定を二つ用意する
			if ((nCnt % 2 == 0 && vecLine >= 0.0f) || (nCnt % 2 != 0 && vecLine <= 0.0f))
			{
				LineCout++;
			}
			else
			{
				break;
			}
		}
		if (LineCout == nTri)
		{
			D3DXVECTOR3 V1 = posLineVec[1] - posLineVec[0];
			D3DXVECTOR3 V2 = posLineVec[2] - posLineVec[0];

			D3DXVECTOR3 Normal;
			//AとBの法線を求めるやつ
			D3DXVec3Cross(&Normal, &V1, &V2);

			//vecB をノーマライズして、長さ 1にする。
			D3DXVec3Normalize(&Normal, &Normal);

			D3DXVECTOR3 VecA = *pPos - posLineVec[0];
			//プレイヤーの位置補正
			SwitchCollision(true);
			OnHit();

			if (IsCollision)
			{
				pPos->y = (posLineVec[0].y - (Normal.x*(pPos->x - posLineVec[0].x) + Normal.z*(pPos->z - posLineVec[0].z)) / Normal.y) + 10.0f;
			}
		}
	}
	// 頂点座標をアンロック
	m_pVtxBuff->Unlock();
	// 頂点インデックスをアンロック
	m_pIdxBuff->Unlock();

	return bIsLanding;
}

//=============================================================================
// メッシュの作成
// Author:hamada ryuuga
//=============================================================================
bool CMesh::CreateMesh(D3DXVECTOR3 *pPos)
{
	bool bIsLanding = false;

	VERTEX_3D* pVtx = NULL;

	const int nTri = 3;

	// 頂点座標をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファのロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxWorld;

	// ワールドマトリックスの初期化
	// 行列初期化関数(第1引数の行列を単位行列に初期化)
	D3DXMatrixIdentity(&mtxWorld);

	// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// 行列掛け算関数(第2引数×第3引数を第１引数に格納)
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	for (int nCnt = 0; nCnt < m_polygonCount; nCnt++)
	{
		D3DXVECTOR3 posLineVec[nTri];

		posLineVec[0] = pVtx[pIdx[nCnt + 0]].pos;
		posLineVec[1] = pVtx[pIdx[nCnt + 1]].pos;
		posLineVec[2] = pVtx[pIdx[nCnt + 2]].pos;

		if ((pIdx[nCnt + 0] == pIdx[nCnt + 1]) ||
			(pIdx[nCnt + 0] == pIdx[nCnt + 2]) ||
			(pIdx[nCnt + 2] == pIdx[nCnt + 1]))
		{//縮退ポリゴンを省き
			continue;
		}

		for (int i = 0; i < nTri; i++)
		{//ベクトル３座標をマトリックスで変換する（乗算）
			D3DXVec3TransformCoord(&posLineVec[i], &posLineVec[i], &mtxWorld);
		}

		int  LineCout = 0;

		for (int i = 0; i < nTri; i++)
		{
			//ベクトルS2 V2												
			D3DXVECTOR3 vecWall = posLineVec[(i + 1) % nTri] - posLineVec[i];

			//ベクトル現在のPOSと始点までの距離
			D3DXVECTOR3 vecPos = *pPos - posLineVec[i];

			//外積計算//辺１
			float vecLine = Vec2Cross(&vecPos, &vecWall);

			//三角の中に入ってるときの判定
			if ((nCnt % 2 == 0 && vecLine > 0.0f) ||
				(nCnt % 2 != 0 && vecLine < 0.0f))
			{
				LineCout++;
			}
			else
			{
				break;
			}
		}
		if (LineCout == nTri)
		{
			/*
			CInput *CInputpInput = CInput::GetKey();

			if (CInputpInput->Trigger(CInput::KEY_MESHUP))
			{			
				SetVtxMesh(pVtx, pIdx, nCnt, true);
			}
			if (CInputpInput->Trigger(CInput::KEY_MESHDOWN))
			{
				SetVtxMesh(pVtx, pIdx, nCnt, false);
			}
			*/
		}
	}

	// 頂点座標をアンロック
	m_pVtxBuff->Unlock();
	// 頂点座標をアンロック
	m_pIdxBuff->Unlock();

	CMesh::SetVtxMeshLight();
	

	return bIsLanding;
}

//=============================================================================
// メッシュの作成
// Author:hamada ryuuga
//=============================================================================
void CMesh::SetVtxMesh(VERTEX_3D* pVtx, WORD* pIdx,int nCnt,bool isUp)
{
	int sign = isUp ? 1 : -1;

	for (int i = 0; i < 3; i++)
	{
		int index = pIdx[nCnt + i];

		pVtx[index].pos.y += (MOUNTAIN * sign);
	}
}

//=============================================================================
//今あるオブジェクトの読み込み
//=============================================================================
void CMesh::Loadfile(const char * pFileName)
{
	Uninit();
	//NotRelease();
	std::ifstream ifs(pFileName);

	int nIndex = 0;
	VERTEX_3D* pVtx = NULL;
	// 頂点座標をロック	
	std::string str;
	if (ifs)
	{
		ifs >> JMesh;
		nIndex = JMesh["INDEX"];
		m_move = JMesh["MOVE"];
		D3DXVECTOR3 pos;
		D3DXVECTOR3 size;
		D3DXVECTOR3 rot;
		std::string Type;

		//初期化
		if (JMesh["MESHSIZE"] == NULL)
		{
			CMesh::SetVtxMeshSize(0);
		}
		else
		{
			CMesh::SetVtxMeshSize(JMesh["MESHSIZE"]);
		}

		str = JMesh["TEXPASS"];
		CMesh::SetTexture(str.c_str());

		m_pos = D3DXVECTOR3(JMesh["POSORIGIN"]["X"], JMesh["POSORIGIN"]["Y"], JMesh["POSORIGIN"]["Z"]);
		m_MeshSize = D3DXVECTOR3(JMesh["MESHDATASIZE"]["X"], JMesh["MESHDATASIZE"]["Y"], JMesh["MESHDATASIZE"]["Z"]);
		
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < m_vtx; nCnt++)
		{
			float posx = ((nCnt % m_vtxCountX) - 1.0f);
			float posz = ((nCnt / m_vtxCountZ) - 1.0f) * -1.0f;

			//めっしゅを真ん中にする補正
			//m_pos = D3DXVECTOR3(-(posx - 1)*MAX_SIZEMESH * 0.5f, 0.0f, -posz * MAX_SIZEMESH * 0.5f) + m_pos;

			std::string name = "MESH";
			std::string Number = std::to_string(nCnt);
			name += Number;

			pos = D3DXVECTOR3(JMesh[name]["POS"]["X"], JMesh[name]["POS"]["Y"], JMesh[name]["POS"]["Z"]);

			//座標の補正
			pVtx[nCnt].pos = D3DXVECTOR3(pos.x, pos.y, pos.z);

			// 各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// 頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// 頂点座標をアンロック
		m_pVtxBuff->Unlock();
		CMesh::SetVtxMeshLight();
	}
}

//=============================================================================
//今あるオブジェクトの保存
//=============================================================================
void CMesh::Savefile(const char * pFileName)
{
	VERTEX_3D* pVtx = NULL;

	// 頂点座標をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	int nIndex = 0;
	
	for (int i = 0; i < m_vtx; i++)
	{
		std::string name = "MESH";
		std::string Number = std::to_string(nIndex);
		name += Number;

		JMesh[name] = {
			{ "POS",{
				{ "X", pVtx[i].pos.x } ,
				{ "Y", pVtx[i].pos.y } ,
				{ "Z", pVtx[i].pos.z } } } ,};

		nIndex++;
	}

	// 頂点座標をアンロック
	m_pVtxBuff->Unlock();

	JMesh["INDEX"] = nIndex;
	JMesh["MOVE"] = m_move;
	JMesh["MESHDATASIZE"] = {
		{ "X", m_MeshSize.x } ,
		{ "Y", m_MeshSize.y } ,
		{ "Z", m_MeshSize.z } };
	
	JMesh["TEXPASS"] = m_pFileName;
	JMesh["MESHSIZE"] = m_xsiz;
	JMesh["POSORIGIN"] = {
			{ "X", m_pos.x } ,
			{ "Y", m_pos.y } ,
			{ "Z", m_pos.z } };

	JMesh["ANIMATION"] = false;

	auto jobj = JMesh.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = pFileName;
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}

//=============================================================================
//サイズ初期化
//=============================================================================
void CMesh::SetVtxMeshSize(int Size)
{
	//CMesh::Uninit();
	//NotRelease();

	m_xsiz = Size;
	m_zsiz = Size;
	m_vtxCountX = m_xsiz + 1;	// 1多い数字
	m_vtxCountZ = m_zsiz + 1;	// 1多い数字

	// 頂点数
	m_vtx = m_vtxCountX* m_vtxCountZ;	// 頂点数を使ってるよ

	// インデックス数
	m_index = (2 * m_vtxCountX * m_zsiz + 2 * (m_zsiz - 1));

	m_polygonCount = m_index - 2;

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_vtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファ生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_index,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	// 頂点座標をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int i = 0; i < m_vtx; i++)
	{
		pVtx[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float posx = ((i % m_vtxCountX) - 1.0f);
		float posz = ((i / m_vtxCountZ) - 1.0f) * -1.0f;

		float texU = 1.0f / m_xsiz * (i % m_vtxCountX);
		float texV = 1.0f / m_zsiz * (i / m_vtxCountZ);

		// メッシュを真ん中にする補正
		m_pos = (D3DXVECTOR3(-(posx - 1) * m_MeshSize.x * 0.5f, 0.0f, -posz * m_MeshSize.z * 0.5f)) + m_pos;

		// 座標の補正
		pVtx[i].pos += D3DXVECTOR3(posx*m_MeshSize.x, 0.0f, posz * m_MeshSize.z);


		// 各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 頂点カラーの設定
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[i].tex = D3DXVECTOR2(texU, texV);

	}

	// 頂点座標をアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//法線とIndexの成立
//=============================================================================
void CMesh::SetVtxMeshLight()
{
	VERTEX_3D* pVtx = NULL;
	//インデックスバッファのロック
	WORD* pIdx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int z = 0; z < m_zsiz; z++)
	{
		int linetop = z * (m_vtxCountX * 2 + 2);
		for (int x = 0; x < m_vtxCountX; x++)
		{
			int nIdxData = x * 2 + linetop;
			pIdx[nIdxData + 1] = (WORD)(x + (z * m_vtxCountX));
			pIdx[nIdxData] = (WORD)(pIdx[nIdxData + 1] + m_vtxCountX);
		}
		//縮退ポリゴン設定
		if (z < m_xsiz - 1)
		{
			pIdx[m_vtxCountX * 2 + 0 + linetop] = (WORD)(m_xsiz + m_vtxCountX*z);
			pIdx[m_vtxCountX * 2 + 1 + linetop] = (WORD)(m_vtxCountX * 2 + m_vtxCountX * z);
		}
	}

	//---------------------------------------
	//ここから法線
	//---------------------------------------

	//三角の頂点数
	const int nTri = 3;

	D3DXVECTOR3 posLineVec[nTri];//ベクトル

	for (int nCnt = 0; nCnt < m_polygonCount; nCnt++) // プリミティブの数だけまわす。
	{
		//ベクトルを求める
		posLineVec[0] = pVtx[pIdx[nCnt + 0]].pos;
		posLineVec[1] = pVtx[pIdx[nCnt + 1]].pos;
		posLineVec[2] = pVtx[pIdx[nCnt + 2]].pos;

		if ((pIdx[nCnt + 0] == pIdx[nCnt + 1]) ||
			(pIdx[nCnt + 0] == pIdx[nCnt + 2]) ||
			(pIdx[nCnt + 2] == pIdx[nCnt + 1]))
		{
			continue;
		}

		D3DXVECTOR3 V1 = posLineVec[1] - posLineVec[0];
		D3DXVECTOR3 V2 = posLineVec[2] - posLineVec[0];

		D3DXVECTOR3 Normal;

		if (nCnt % 2 == 0)
		{
			//AとBの法線を求めるやつ
			D3DXVec3Cross(&Normal, &V1, &V2);
		}
		else
		{
			//BとAの法線を求めるやつ
			D3DXVec3Cross(&Normal, &V2, &V1);
		}

		//Normalをノーマライズして、長さ 1にする。
		D3DXVec3Normalize(&Normal, &Normal);

		for (int i = 0; i < nTri; i++)
		{//法線計算
			pVtx[pIdx[nCnt + i]].nor += Normal;
		}
	}

	for (int nCnt = 0; nCnt < m_vtx; nCnt++)
	{
		//norをノーマライズして、長さ 1にする。
		D3DXVec3Normalize(&pVtx[nCnt].nor, &pVtx[nCnt].nor);
	}

	// 頂点座標をアンロック
	m_pVtxBuff->Unlock();
	m_pIdxBuff->Unlock();
}

//=============================================================================
// メッシュの枚数決めるやつ
//=============================================================================
void CMesh::SetMesh(const int Size)
{
	m_NowMesh = Size;//枚数保存
	SetVtxMeshSize(Size);//サイズ決定
	SetVtxMeshLight();//法線設定
}
