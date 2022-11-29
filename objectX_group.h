//=============================================================================
//
// オブジェクトXを読み込んで保存する処理 [objectX_group.h]
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _OBJECTX_GROUP_H_
#define _OBJECTX_GROUP_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "main.h"
#include <string>
#include <vector>
#include <map>

//=============================================================================
// クラスの定義
//=============================================================================
class CObjectXGroup
{
private: /* プライベート定数 */
	static const wchar_t* filePath;

public:

	// モデルデータ
	struct SModelData
	{
		LPD3DXMESH pMesh;
		LPD3DXBUFFER pBuffMat;
		DWORD numMat;
		D3DXVECTOR3 minVtx;
		D3DXVECTOR3 maxVtx;
		D3DXVECTOR3 size;
	};
public:
	//-------------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//-------------------------------------------------------------------------
	CObjectXGroup();
	~CObjectXGroup();

	//-------------------------------------------------------------------------
	// メンバー関数
	//-------------------------------------------------------------------------
	void LoadAll();		// 全ての読み込み
	void UnloadAll();	// 全ての破棄
	void Load(std::string inKey, std::string inFileName);	// 指定の読み込み
	void Load(std::vector<std::string> inModel) { Load(inModel[0], inModel[1]); }			// 指定の読み込み
	void Unload(std::string inKey);				// 指定の破棄

	// Getter
	LPD3DXMESH GetMesh(std::string inKey);		// メッシュの取得
	LPD3DXBUFFER GetBuffMat(std::string inKey);	// バッファーの取得
	DWORD GetNumMat(std::string inKey);			// マテリアル数の取得
	D3DXVECTOR3 GetMinVtx(std::string inKey);	// 最小頂点の取得
	D3DXVECTOR3 GetMaxVtx(std::string inKey);	// 最大頂点の取得
	D3DXVECTOR3 GetSize(std::string inKey);		// 大きさの取得

private: /* プライベート関数 */
	bool ExistsKey(std::string inKey) { return m_model.count(inKey) != 0; }	// Map内に指定されたKeyが存在するか否か

private:
	std::map<std::string, SModelData, std::less<>> m_model;	// モデルの情報
};
#endif