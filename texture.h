//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// Author  : Yuda Kaito
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>
#include <vector>
#include <unordered_map>
#include <string>

//==================================================
// 定義
//==================================================
class CTexture
{
private: // 静的メンバー変数
	static CTexture* m_pTexture;

public: // 静的関数
	static CTexture* GetInstance();

private: // シングルトン
	CTexture();

public:
	~CTexture();	// デストラクタ

public: /* パブリック関数 */
	void LoadAll();					// 全ての読み込み
	void UnloadAll();				// 全ての破棄
	void Load(std::string inKey, std::string inFileName);	// 指定の読み込み
	void Load(std::vector<std::string> inTexture);			// 指定の読み込み
	void Unload(std::string inKey);	// 指定の破棄
	LPDIRECT3DTEXTURE9 GetTexture(std::string inKey);	// 情報の取得
	LPDIRECT3DTEXTURE9 GetTexture(int inKey);	// 情報の取得
	int SetTexture(std::string inKey);	// 情報の取得

private: /* プライベート関数 */
	bool ExistsPath(std::string inKey) { return m_texturePath.count(inKey) != 0; }	// Map内に指定されたKeyが存在するか否か
	bool ExistsKey(std::string inKey) { return m_texture.count(inKey) != 0; }		// Map内に指定されたKeyが存在するか否か

private: /* メンバ変数 */
	std::unordered_map<std::string, std::string> m_texturePath;		// パス
	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_texture;	// 情報
	std::unordered_map<int, std::string > m_createIndex;			// 生成順
	int m_maxIndex;	// 最大生成数
};

#endif // !_TEXTURE_H_
