//=============================================================================
//
// ゲーム中のカメラ
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _CAMERA_EDIT_H_
#define _CAMERA_EDIT_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "camera.h"

//=============================================================================
// カメラクラス
//=============================================================================
class CCameraEdit : public CCamera
{
public:
	CCameraEdit(CTaskGroup::EPriority inPriority = CTaskGroup::LEVEL_3D_1);
	~CCameraEdit() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	void Rotate();
private:
	float m_fDistance;			// 距離
};
#endif		// _CAMERA_H_
