//============================
//
// アイテム設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

#define MAX_ITEM (128)			//最大数
#define ITEMPS "Data/system/item.txt"
#define ITEMPSHD "Data/system/itemHd.txt"
#define ITEMPSEZ "Data/system/itemEz.txt"

#define KI "kagi.x"
#define SUZU "suzu.x"

#define MAX_ITEMWAID (50.0f)	//マップのサイズ/分割数で出たマップサイズ計算例MIN-１５００MAX+１５００はなる３０００になる
#define MINX (-1500.0f)			
#define MINY (-1500.0f)
#define MAXX (1500.0f)
#define MAXY (1500.0f)
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posOld;	//過去座標
	D3DXVECTOR3 rot;	//回転
	D3DXVECTOR3 RotMove;//回転ムーブ
	D3DXVECTOR3 ItemMin;
	D3DXVECTOR3 ItemMax;
	D3DXVECTOR3 SIZ;
	D3DXMATRIX MtxWorld; //マトリックス//ポリゴンの位置や回転行列すべてをつめてるナニカ
	int nType;			//タイプ
	int nLife;			//ライフ
	int Numbers;			//くっついた時の番号
	bool dist;
	bool bFinish;			//消える前の演出
	bool bUse;			//使ってるか使ってないか
	LPD3DXMESH s_pVtxMesh;	// メッシュのポインタ
	LPD3DXBUFFER s_pBuffItem; //バッファのポインタ
	LPDIRECT3DTEXTURE9	s_pTexture[MAX_ITEM];		// テクスチャへのポインタ
	DWORD s_Item;		//マテリアル情報
	float consumption;
}ITEM;

//プロトタイプ宣言
void InitItem(void);//初期化
void UninitItem(void);//破棄
void UpdateItem(void);//更新
void DrawItem(void);//描画
void SetItem(D3DXVECTOR3 pos, char *Filename, int nType);//セット引数座標と読み込むファイル名
ITEM *GetItem(void);//アイテムの構造体をゲットします
int *GetItemMax(void);//アイテムの最大数をゲットします
bool  CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 Siz);
void InitItem(char *Filename);
void OutputMap(char *Filename);
#endif