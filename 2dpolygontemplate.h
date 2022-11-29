//=============================================================================
//
// 説明書
// Author : 浜田琉雅
//
//=============================================================================


#ifndef _2DPOLYGONTEMPLATE_H_			// このマクロ定義がされてなかったら
#define _2DPOLYGONTEMPLATE_H_			// 二重インクルード防止のマクロ定義

#include "renderer.h"
#include "object2d.h"


class CTest : public CObject2d
{

public:
	
	enum PosType
	{
		TYPE_2D = 0,	// 2d座標で表示
		TYPE_3D,		// 3d座標で表示
		MAX			// あんただれや？
	};

	CTest();
	~CTest() override;
	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
	void Draw() override;
	static CTest* Create(D3DXVECTOR3 pos, bool b3D);
private:

	void move();
};
#endif
