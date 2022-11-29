//**************************************************************************************************
//
// モーション処理(motion.h)
// Auther：唐﨑結斗
// 概要 : モーションクラスの設定
//
//**************************************************************************************************
#ifndef _MOTION_H_			// このマクロ定義がされてなかったら
#define _MOTION_H_			// 二重インクルード防止のマクロ定義

//***************************************************************************
// インクルード
//***************************************************************************
#include "main.h"
#include "objectX.h"
#include "renderer.h"
#include <vector>

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CParts;
class CModel3D;

//=============================================================================
// モーションクラス
// Author : 唐﨑結斗
// 概要 : モーション設定を行うクラス
//=============================================================================
class CMotion
{
public:
	//***************************************************************************
	// 定数定義
	//***************************************************************************
	static const unsigned int MAX_MOTION = (128);			// モーション数の最大数
	static const unsigned int MAX_MODEL_PARTS = (128);		// モデル数の最大数
	static const unsigned int MAX_KEY = (64);				// キーの最大数
	static const unsigned int MAX_KEYSET = (64);			// キー設定の最大数
	static const unsigned int MOTION_BLEND_FRAM = (12);		// モーションブレンドのフレーム数	

	//***************************************************************
	// キー構造体を定義
	//***************************************************************
	typedef struct
	{
		D3DXVECTOR3		pos;	// 現在位置
		D3DXVECTOR3		rot;	// 現在の向き
	}MyKey;

	//***************************************************************
	// キー設定構造体を定義
	//***************************************************************
	typedef struct
	{
		int		nFrame;				// フレーム数
		std::vector<MyKey>	pKey;	// キー情報
	}MyKeySet;

	//***************************************************************
	// モーション設定構造体を定義
	//***************************************************************
	typedef struct
	{
		int nNumKey;		// キー数
		int nCntKeySet;		// キーセットカウント
		int nCntFrame;		// フレームカウント
		bool bLoop;			// ループ使用状況
		bool bMotion;		// モーションを行っているか
		std::vector<MyKeySet> pKeySet;	// キー設定情報
	}MyMotion;

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMotion(const char *pFileName);
	~CMotion();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Update();	// 更新
	void Uninit(void);	// 終了

	void ReloadMotion(const char *pFileName);	// モーションの再読み込み

	// Setter
	void SetPartsOrigin();							// パーツをもとの場所に配置する
	void SetMotion(const int nCntMotionSet);		// モーションの初期設定
	void SetParts(D3DXMATRIX mtxWorld);				// パーツの設定
	void SetNumMotion(const int nNumMotion);		// モーション番号の設定
	void SetUseMotion(bool isMotion) { m_bMotion = isMotion; }			// モーションを行っているか設定
	void SetMotionBlend(bool isBlend) { m_bMotionBlend = isBlend; }		// モーションブレンドを行っているか設定

	// Getter
	int GetMaxParts() { return m_nMaxParts; }				// パーツの最大数の取得
	bool GetMotion() { return m_bMotion; }					// モーションを行っているか取得
	bool GetMotionBlend() { return m_bMotionBlend; }		// モーションブレンドを行っているか取得
	CParts* GetParts(int index) { return m_parts[index]; }	// モーションブレンドを行っているか取得

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Init();	// 初期化

	void PlayMotion();	// モーションの再生
	void MotionBlend();	// モーションブレンド
	void LoodSetMotion(const char *pFileName);	// モーション読み込み
	void CntReset(const int nNumMotionOld);		// カウントリセット

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CModel3D	*m_pParent;						// 親
	std::vector<MyMotion> m_motion;				// モーション
	std::vector<CParts*> m_parts;				// パーツ
	std::vector<char*> m_partsFile;	// パーツのXファイル名
	int m_nMaxParts;							// パーツ数
	int m_nNumMotion;							// 扱うモーション
	bool m_bMotion;								// モーションを行うか
	bool m_bMotionBlend;						// モーションブレンド
};
#endif

