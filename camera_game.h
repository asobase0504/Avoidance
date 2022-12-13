//=============================================================================
//
// ゲーム中のカメラ
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _CAMERA_GAME_H_
#define _CAMERA_GAME_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "camera.h"

//=============================================================================
// カメラクラス
//=============================================================================
class CCameraGame : public CCamera
{
public:
	CCameraGame(CTaskGroup::EPriority inPriority = CTaskGroup::LEVEL_3D_1);
	~CCameraGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
private:
	float m_fDistance;			// 距離
};
#endif		// _CAMERA_H_
