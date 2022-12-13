//=============================================================================
//
// タイトルカメラ
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _CAMERA_TITLE_H_
#define _CAMERA_TITLE_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "camera.h"

//=============================================================================
// カメラクラス
//=============================================================================
class CCameraTitle : public CCamera
{
public:
	CCameraTitle(CTaskGroup::EPriority inPriority = CTaskGroup::LEVEL_3D_1);
	~CCameraTitle() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
};
#endif		// _CAMERA_H_
