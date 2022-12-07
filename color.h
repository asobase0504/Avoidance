//=============================================================================
// 
// 色管理クラス
// Author YudaKaito
// 
//=============================================================================
#ifndef _THEME_COLOR_H_
#define _THEME_COLOR_H_

#include <d3dx9.h>
#include <map>
#include <string>

//-----------------------------------------------------------------------------
// 色管理クラス
//-----------------------------------------------------------------------------
class CColor
{
public:	// 列挙型
	enum EColorConfig
	{
		COLOR_0,
		COLOR_1,
		COLOR_2,
		COLOR_3,
		COLOR_MAX,
	};

public:	// メンバー関数
	CColor();
	~CColor();

	HRESULT Init();
	void Uninit();

	// Getter
	const D3DXCOLOR& GetColor(EColorConfig inConfig);
	int GetSize() { return typeNumber; }

	// Setter
	void SetTheme(int idx);

private: // 動的メンバー変数
	std::map<int, D3DXCOLOR> m_color;
	int typeNumber;
};

#endif // !_COLOR_H_
