//**************************************************
//
// sound.h
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _SOUND_H_	//このマクロ定義がされてなかったら
#define _SOUND_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#pragma push_macro("new")
#undef new
#include <d3dx9.h>
#pragma pop_macro("new")
#include <xaudio2.h>

//==================================================
// 定義
//==================================================
class CSound
{
	/* ↓定義↓ */
public:
	enum ELabel
	{
		LABEL_NONE = -1,
		LABEL_BGM_TITLE = 0,	// タイトル
		LABEL_BGM_GAME,
		LABEL_BGM_BOSS1,
		LABEL_BGM_BOSS2,
		LABEL_BGM_RESET,
		LABEL_BGM_TUTORIAL,
		LABEL_BGM_RANKING,
		LABEL_SE_HIT,			// HIT
		LABEL_SE_SHOT,			// SHOT
		LABEL_SE_FLARE,			// FLARE
		LABEL_SE_ICE,			// ICE
		LABEL_SE_STORM,			// 
		LABEL_SE_SUN,			// 
		LABEL_SE_MOVE,			// 
		LABEL_SE_NO,			// キャンセル
		LABEL_SE_ON,			// 決定
		LABEL_SE_KIKEN,			// 
		LABEL_SE_TAIKO,				// 
		LABEL_SE_RAION,				// 
		LABEL_SE_ROBO,				
		LABEL_SE_BOOTH,				// 
		LABEL_SE_YOBI,				// 
		LABEL_SE_GOLD,				// 
		LABEL_MAX,	
	};

	struct SParam
	{
		char *pFileName;	// ファイル名
		int loop;			// ループ
	};

	static const SParam PARAM[];	// パラメータの情報

	/* ↓メンバ関数↓ */
public:
	CSound();	// デフォルトコンストラクタ
	~CSound();	// デストラクタ

public:
	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT Play(ELabel sound);
	void Stop(ELabel label);
	void Stop();

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);	// チャンクのチェック
	HRESULT LoadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// チャンクデータの読み込み

	/* ↓メンバ変数↓ */
private: 
	IXAudio2* m_pXAudio2;							// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;		// マスターボイス
	IXAudio2SourceVoice* m_pSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE* m_pDataAudio[LABEL_MAX];					// オーディオデータ
	DWORD m_sizeAudio[LABEL_MAX];					// オーディオデータサイズ
};

#endif // !_SOUND_H_
