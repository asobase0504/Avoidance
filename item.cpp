//============================
//
// アイテム設定
// Author:hamada ryuuga
//
//============================

#include <stdio.h>
#include "item.h"
#include "camera.h"
#include "shadow.h"
#include "file.h"
#include "line.h"
#include "comn.h"
#include "score.h"
#include "fade.h"
#include "mesh.h"
#include "floor.h"
#include "end.h"
#include "title.h"
#include "suzu.h"
#include "Tmesh.h"
#include "player.h"
#include "ui.h"
#include "gameover.h"
#include "sound.h"
#include "paticle.h"

//マクロ
#define Attenuation	(0.5f)		//減衰係数
//#define Speed	(1.0f)			//スピード
#define WIDTH (10.0f)			//モデルの半径
#define Vtx (4)		//頂点数
//------------------------------------
// static変数
//------------------------------------
static bool s_bIsLanding;
static ITEM s_Item[MAX_ITEM];	// ポリゴンの構造体
static int s_nSet, s_nTimer2,s_nTimer, s_nSuzu,MaxSuzu; //最大数,タイマー,鈴の数,鈴の最大数
D3DXVECTOR3 posDist[MAX_ITEM];//アイテムとプレイヤーの距離
//=========================================
// 初期化処理
//=========================================
void InitItem(void)
{
	//カメラのデータ取得
	CAMERA *pCamera;
	pCamera = GetCamera();
	// タイマー
	s_nTimer = 0;
	s_nTimer2 = 0;

	s_nSet = 0;//最大数
	s_nSuzu = 0;//鈴の数
	MaxSuzu = 0;//鈴の最大数
	for (int i = 0; i < MAX_ITEM; i++)
	{
		// 初期化処理
		s_Item[i].s_pBuffItem = NULL;
		s_Item[i].s_Item = 0;
		s_Item[i].s_pVtxMesh = NULL;			//縦　　　　　　　　　　//横
		s_Item[i].RotMove = D3DXVECTOR3(D3DX_PI + pCamera->rot.y, D3DX_PI*0.5f + pCamera->rot.y, 0.0f);
		s_Item[i].consumption = 0.0f;
		s_Item[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 頂点座標
		s_Item[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転座標
		s_Item[i].nLife = 10;
		s_Item[i].bFinish = false;
		s_Item[i].dist = false;
		s_Item[i].bUse = false;
		s_Item[i].SIZ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転座標
		s_Item[i].ItemMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		s_Item[i].ItemMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		posDist[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転座標

	}
	
}

//=========================================
// 終了処理
//=========================================
void UninitItem(void)
{
	StopSound();

	for (int j = 0; j < s_nSet; j++)
	{
		for (int i = 0; i < (int)s_Item[j].s_Item; i++)
		{
			if (s_Item[j].s_pTexture[i] != NULL)
			{// テクスチャの解放
				s_Item[j].s_pTexture[i]->Release();
				s_Item[j].s_pTexture[i] = NULL;
			}
		}
	}
	for (int i = 0; i < MAX_ITEM; i++)
	{
		// 頂点バッファーの解放
		if (s_Item[i].s_pVtxMesh != NULL)
		{
			s_Item[i].s_pVtxMesh->Release();
			s_Item[i].s_pVtxMesh = NULL;
		}
		if (s_Item[i].s_pBuffItem != NULL)
		{
			s_Item[i].s_pBuffItem->Release();
			s_Item[i].s_pBuffItem = NULL;
		}
	}
}

//=========================================
// 更新処理
//=========================================
void UpdateItem(void)
{
	s_nTimer2++;
	s_nTimer++;
	s_nTimer %= 60;

	for (int nCnt = 0; nCnt < s_nSet; nCnt++)
	{
		if (s_Item[nCnt].bUse)
		{
			s_Item[nCnt].posOld = s_Item[nCnt].pos;
			if (s_Item[nCnt].bFinish)
			{
				PLAYER *pPlayer = GetPlayer();

				D3DXVECTOR3 vecPlayerDist = pPlayer->pos - s_Item[nCnt].pos;
				float distPlayer = D3DXVec3Length(&vecPlayerDist);
				if (!s_Item[nCnt].dist)
				{
					s_Item[nCnt].pos += vecPlayerDist / distPlayer * 5.0f;
				}
				if (distPlayer <= 5.0f)
				{
					s_Item[nCnt].dist = true;		
					//s_Item[nCnt].bUse = false;
				}
				if (s_Item[nCnt].dist)
				{	
					posDist[s_Item[nCnt].Numbers] = s_Item[nCnt].pos;
					
					if (s_Item[nCnt].Numbers - 1 == 0)
					{//一番最初のだけプレイヤーの体に埋め込む
						s_Item[nCnt].pos.x = pPlayer->pos.x - pPlayer->move.x*s_Item[nCnt].Numbers+ s_Item[s_nSet].SIZ.x;
						s_Item[nCnt].pos.z = pPlayer->pos.z - pPlayer->move.z*s_Item[nCnt].Numbers+ s_Item[s_nSet].SIZ.z;
						s_Item[nCnt].pos.y = pPlayer->pos.y;
					}
					else
					{//一個前のについてくる処理
						s_Item[nCnt].pos.x = posDist[s_Item[nCnt].Numbers - 1].x;//pPlayer->pos.x - pPlayer->move.x*s_Item[nCnt].Numbers+ s_Item[s_nSet].SIZ.x;
						s_Item[nCnt].pos.z = posDist[s_Item[nCnt].Numbers - 1].z;//pPlayer->pos.z - pPlayer->move.z*s_Item[nCnt].Numbers+ s_Item[s_nSet].SIZ.z;
						s_Item[nCnt].pos.y = posDist[s_Item[nCnt].Numbers - 1].y;//pPlayer->pos.y;
					}
					s_Item[nCnt].rot.y += 0.01f;
				}
			}
			else
			{
				if (s_Item[nCnt].nType == 0)
				{//鈴
					if (s_nTimer <= 30)
					{
						s_Item[nCnt].pos.y += 0.1f;
					}
					if (s_nTimer >= 30)
					{
						s_Item[nCnt].pos.y -= 0.1f;
					}
					s_Item[nCnt].rot.y += 0.01f;
				}
				else
				{
					if (s_nTimer <= 30)
					{
						s_Item[nCnt].pos.y += 0.2f;
					}
					if (s_nTimer >= 30)
					{
						s_Item[nCnt].pos.y -= 0.2f;
					}
					s_Item[nCnt].rot.y -= 0.01f;
					if (s_nTimer2 >= MaxSuzu)
					{
						s_nTimer2 = 0;
						SetParticle(s_Item[nCnt].pos, s_Item[nCnt].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 20.0f, PARTICLETYPE_STAR, 20);
					}
				}	
			}
			//正規化
			if (s_Item[nCnt].rot.y > D3DX_PI * 2)
			{
				s_Item[nCnt].rot.y += -D3DX_PI * 2;
			}
			if (s_Item[nCnt].rot.y < -D3DX_PI * 2)
			{
				s_Item[nCnt].rot.y += D3DX_PI * 2;
			}
		}
	}

}

//=========================================
// 描画処理
//=========================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// 計算用マトリックス
	D3DMATERIAL9 marDef;
	D3DXMATERIAL *pMat;
	//MESH *pMesh = GetMesh();
	for (int j = 0; j < s_nSet; j++)//出てるモデルの数だけ回す
	{
		if (s_Item[j].bUse)//使ってるやつ出す
		{
			// ワールドマトリックスの初期化
			// 行列初期化関数(第1引数の行列を単位行列に初期化)
			D3DXMatrixIdentity(&s_Item[j].MtxWorld);

			D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);

			// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
			D3DXMatrixMultiply(&s_Item[j].MtxWorld, &s_Item[j].MtxWorld, &mtxScale);

			// 向きを反映
			// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Item[j].rot.y, s_Item[j].rot.x, s_Item[j].rot.z);
			// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
			D3DXMatrixMultiply(&s_Item[j].MtxWorld, &s_Item[j].MtxWorld, &mtxRot);

			// 位置を反映
			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
			D3DXMatrixTranslation(&mtxTrans, s_Item[j].pos.x, s_Item[j].pos.y, s_Item[j].pos.z);
			// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
			D3DXMatrixMultiply(&s_Item[j].MtxWorld, &s_Item[j].MtxWorld, &mtxTrans);

			// ワールド座標行列の設定
			pDevice->SetTransform(D3DTS_WORLD, &s_Item[j].MtxWorld);

			//現在のマテリアルを保持
			pDevice->GetMaterial(&marDef);

			//マテリアルデータのポインタを取得
			pMat = (D3DXMATERIAL*)s_Item[j].s_pBuffItem->GetBufferPointer();

			for (int i = 0; i < (int)s_Item[j].s_Item; i++)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, NULL);
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[i].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture(0, s_Item[j].s_pTexture[i]);
				//モデルパーツの描画
				s_Item[j].s_pVtxMesh->DrawSubset(i);
			}
			//現在のマテリアルを保持
			pDevice->SetMaterial(&marDef);
		}
	}
	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
}
void SetItem(D3DXVECTOR3 pos, char *Filename ,int nType)
{
	//カメラのデータ取得
	CAMERA *pCamera;
	pCamera = GetCamera();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	char aFile[128] = FILE_3D_MODEL;

	strcat(aFile, &Filename[0]);//合成　aFile＜-こいつに入れる

	D3DXLoadMeshFromX(&aFile[0],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_Item[s_nSet].s_pBuffItem,
		NULL,
		&s_Item[s_nSet].s_Item,
		&s_Item[s_nSet].s_pVtxMesh
	);
	//初期化のXファイルを読み込んだ後

	//すべての頂点数を保存する関数を作る
	int NumVpx;	//頂点数
				//一つの頂点のサイズを入れる関数
	DWORD sizeFVF;
	//バイトがたのポインターchar
	BYTE *pVtxBuff;

	//ここで頂点数を参照
	NumVpx = s_Item[s_nSet].s_pVtxMesh->GetNumVertices();

	//ここでサイズを入れる
	sizeFVF = D3DXGetFVFVertexSize(s_Item[s_nSet].s_pVtxMesh->GetFVF());

	//	頂点バッファのロックしましょう
	s_Item[s_nSet].s_pVtxMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_Item[s_nSet].s_pBuffItem->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)s_Item[s_nSet].s_Item; i++)
	{
		s_Item[s_nSet].s_pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_Item[s_nSet].s_pTexture[i]);
		}
		else
		{
			s_Item[s_nSet].s_pTexture[i] = NULL;
		}
	}

	for (int i = 0; i < NumVpx; i++)
	{
		D3DXVECTOR3 pVtx = *(D3DXVECTOR3*)pVtxBuff;
		//それを使ってモデルの最大値と最小値を使って求める
		//XYZ全部求める
		if (pVtx.x <= s_Item[s_nSet].ItemMin.x)
		{
			s_Item[s_nSet].ItemMin.x = pVtx.x;
		}
		if (pVtx.x >= s_Item[s_nSet].ItemMax.x)
		{
			s_Item[s_nSet].ItemMax.x = pVtx.x;
		}
		if (pVtx.y <= s_Item[s_nSet].ItemMin.y)
		{
			s_Item[s_nSet].ItemMin.y = pVtx.y;
		}
		if (pVtx.y >= s_Item[s_nSet].ItemMax.y)
		{
			s_Item[s_nSet].ItemMax.y = pVtx.y;
		}
		if (pVtx.z <= s_Item[s_nSet].ItemMin.z)
		{
			s_Item[s_nSet].ItemMin.z = pVtx.z;
		}
		if (pVtx.z >= s_Item[s_nSet].ItemMax.z)
		{
			s_Item[s_nSet].ItemMax.z = pVtx.z;
		}

		//一個分求めたら次に生きたいので
		pVtxBuff += sizeFVF;
		//これをやれば最大最小を求められる

	}

	s_Item[s_nSet].SIZ = (s_Item[s_nSet].ItemMax - s_Item[s_nSet].ItemMin);

	//サイズを大きくして当たり判定を大きくします
	s_Item[s_nSet].ItemMax *= 10;
	s_Item[s_nSet].ItemMin *= 10;
	//	頂点バッファのロックしましょう
	s_Item[s_nSet].s_pVtxMesh->UnlockVertexBuffer();

	// 初期化処理
	s_Item[s_nSet].nType = nType;
	s_Item[s_nSet].pos = pos;	// 頂点座標
	s_Item[s_nSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転座標
	s_Item[s_nSet].bUse = true;
	s_Item[s_nSet].RotMove = D3DXVECTOR3(D3DX_PI + pCamera->rot.y, D3DX_PI*0.5f + pCamera->rot.y, 0.0f);
	
	//もしメッシュに埋まってたら埋まってないとこまで押し上げる処理
	CollisionMesh(&s_Item[s_nSet].pos);
	
	if (s_Item[s_nSet].nType == 0)
	{
		s_nSuzu++;//鈴の最大数を
		MaxSuzu = s_nSuzu;//現在の最大数に加算する
	}
	s_nSet++;
}

ITEM *GetItem(void)
{
	return &s_Item[0];
}
int *GetItemMax(void)
{
	return &s_nSuzu;
}

//---------------------------------
//当たり判定
//---------------------------------
bool  CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 Siz)
{

	bool bIsLanding = false;
	{
		for (int nCnt = 0; nCnt < s_nSet; nCnt++)
		{
			if (s_Item[nCnt].bUse)
			{
				D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
				D3DXMATRIX mtxWorld;

				// ワールドマトリックスの初期化
				// 行列初期化関数(第1引数の行列を単位行列に初期化)
				D3DXMatrixIdentity(&mtxWorld);
				// 向きを反映
				// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
				D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Item[nCnt].rot.y, s_Item[nCnt].rot.x, s_Item[nCnt].rot.z);
				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

				// 位置を反映
				// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
				D3DXMatrixTranslation(&mtxTrans, s_Item[nCnt].pos.x, s_Item[nCnt].pos.y, s_Item[nCnt].pos.z);
				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

				D3DXVECTOR3 posCorner[Vtx];

				//中心から頂点
				posCorner[0] = D3DXVECTOR3(s_Item[nCnt].ItemMin.x, 0.0f, s_Item[nCnt].ItemMax.z);
				posCorner[1] = D3DXVECTOR3(s_Item[nCnt].ItemMax.x, 0.0f, s_Item[nCnt].ItemMax.z);
				posCorner[2] = D3DXVECTOR3(s_Item[nCnt].ItemMax.x, 0.0f, s_Item[nCnt].ItemMin.z);
				posCorner[3] = D3DXVECTOR3(s_Item[nCnt].ItemMin.x, 0.0f, s_Item[nCnt].ItemMin.z);

				for (int i = 0; i < Vtx; i++)
				{
					D3DXVec3TransformCoord(&posCorner[i], &posCorner[i], &mtxWorld);
				}

				int  LineCout = 0;
				for (int i = 0; i < Vtx; i++)
				{
					//V1 move
					D3DXVECTOR3 vecMove = *pPos - *pPosOld;

					D3DXVECTOR3 vecMoveN;

					D3DXVec3Normalize(&vecMoveN, &vecMove);

					//ベクトルS2 V2												
					D3DXVECTOR3 vecWall = posCorner[(i + 1) % 4] - posCorner[i] - (vecMoveN*Siz.z);

					//ベクトル現在のPOSと始点までの距離
					D3DXVECTOR3 vecPos = *pPos - posCorner[i];

					//外積計算//辺１
					float vecLine = Vec2Cross(&vecPos, &vecWall);

					if (vecLine > 0.0f)
					{
						LineCout++;
					}
				}
				if (LineCout == 4)
				{
					GAMESET *pGame = GetGameMode();//ゲームモード();//

					LEVEL level = GetLevel();		

					if (s_Item[nCnt].nType == 0)
					{//鈴
						if (!s_Item[nCnt].bFinish)
						{
							MaxSuzu -= 1;
							PlaySound(SOUND_LABEL_SE_BELL);
							SumSuzu(1);
							AddScore(100 + ((int)level * 100));
							s_Item[nCnt].bFinish = true;
							s_Item[nCnt].Numbers = s_nSuzu - MaxSuzu;
						}
					}
					else
					{//鍵
						END* pEnd = GetEnd();
						pEnd->Lst = true;
						pEnd->Time = true;
						AddScore(10000 + ((int)level * 2000));
						pGame->NoGame = false;
						pGame->Result = true;
						//ゲームオーバー初期化
						InitGameover();
						bIsLanding = true;
						//SetFade(MODE_RESULT);
						s_Item[nCnt].bUse = false;
					}	
				}
			}
		}
	}
	return bIsLanding;
}

//==================
//マップ処理(エクセル)現在システム変更中
//autho hamadaryuuga 
//==================
void InitItem(char *Filename)
{
	int s_aMap[MAX_MESH][MAX_MESH];

	FILE *pFile = NULL;	//ファイルポインタを宣言
	int nCntX, nCntY;

	VERTEX_3D* pVtx = NULL;
	
	//ファイルを開く

	pFile = fopen(&Filename[0], "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		for (nCntY = 0; nCntY < MAX_MESH; nCntY++)
		{
			for (nCntX = 0; nCntX < MAX_MESH; nCntX++)
			{//Mapの書き込み
				fscanf(pFile, "%d", &s_aMap[nCntY][nCntX]);
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
	}

	float posx = -1500.0f;
	float posz = -1500.0f;
	//ブロック設定
	for (nCntY = 0; nCntY < MAX_MESH; nCntY++, pVtx++)
	{
		for (nCntX = 0; nCntX < MAX_MESH; nCntX++, pVtx++)
		{//Mapの書き込み
			posx += MAX_ITEMWAID;

			switch (s_aMap[nCntY][nCntX])
			{
		
			case 1://鈴
				SetItem(D3DXVECTOR3(posx, 0.1f, posz), SUZU, 0);
				break;
			case 2://鍵
				SetItem(D3DXVECTOR3(posx, 0.1f, posz), KI, 1);
				break;
			default:
				break;
			}	
		}
		posx = -1500.0f;
		posz += MAX_ITEMWAID;
	}


}
//----------------------------
//ファイルの出力マップ情報改造
//----------------------------
void OutputMap(char *Filename)
{

	//ファイル開け
	FILE *pFile = fopen(Filename, "w");
	fprintf(pFile, "SCRIPT\n\n");
	for (int nCnt = 0; nCnt < s_nSet; nCnt++)
	{
		if (s_Item[nCnt].bUse)
		{
			if (s_Item[nCnt].nType == 0)
			{		
				fprintf(pFile, "MODEL_FILENAME " SUZU"\n");
			}

			if (s_Item[nCnt].nType == 1)
			{
				fprintf(pFile, "MODEL_FILENAME " KI"\n");
				
			}
			fprintf(pFile, "MODELSET\n");
			fprintf(pFile, "TYPE = %d\n", s_Item[nCnt].nType);
			fprintf(pFile, "POS = %.4f %.4f %.4f\n", s_Item[nCnt].pos.x,s_Item[nCnt].pos.y, s_Item[nCnt].pos.z);
			fprintf(pFile, "ROT = %.4f %.4f %.4f\n", s_Item[nCnt].rot.x, s_Item[nCnt].rot.y, s_Item[nCnt].rot.z);
			fprintf(pFile, "END_ITEMSET\n");
		}
		fprintf(pFile, "\n");
	}

	fprintf(pFile, "END_SCRIPT");
	fclose(pFile);


}
