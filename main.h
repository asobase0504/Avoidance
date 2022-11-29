//=============================================================================
//
// メイン
// Author : 浜田琉雅
//
//=============================================================================
#ifndef _MAIN_H_			// このマクロ定義がされてなかったら
#define _MAIN_H_			// 二重インクルード防止のマクロ定義

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef _DEBUG
#define CHECK_MEM_LEAK
#endif // _DEBUG

//普段使用禁止
#ifdef CHECK_MEM_LEAK
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif // CHECK_MEM_LEAK

#endif
