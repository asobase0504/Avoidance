//=============================================================================
// 
// ポーズの作成(ヘッダーファイル)
// Author Hamada Ryuuga
// 
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object.h"
#include "object2d.h"

//=============================================================================
// ポーズクラス
//=============================================================================
class CPause :public CObject
{
public:
	//画面(モード)の種類
	enum EMode
	{
		MODE_GAME = 0,			//ゲーム画面	
		MODE_RETURN,
		MODE_TITLE,
		MODE_END,
		MODE_MAX
	};

	CPause();
	~CPause();

	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
	void Draw() override;

	// Setter
	void Set() { m_onPause = !m_onPause; };

	// Getter
	bool Get() { return m_onPause; };

private:
	bool m_onPause;
	int m_nextMode;
	CObject2d* m_pBg;
	CObject2d* m_pObject2d[4];
};

#endif 
