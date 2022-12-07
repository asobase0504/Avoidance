//=============================================================================
//
// ゲーム画面
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "game.h"
/* Sestem系統 */
#include "application.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
/* 3D系統 */
#include "camera.h"
#include "light.h"
/* Object系統*/
#include "object_polygon3d.h"
#include "player.h"
#include "pause.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数の宣言
//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// ゲームの初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	CLight* light = new CLight;
	light->Init();

	CCamera* camera = new CCamera;
	camera->Init();

	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	{
		CPlayer* player = CPlayer::Create();
		player->LoadModel("BOX");
		player->SetPos(D3DXVECTOR3(0.0f, 15.0f, 0.0f));
		player->SetMaterialDiffuse(0, D3DCOLOR(0xff46bdc6));
		player->CalculationVtx();
	}

	{
		CObjectPolygon3D* floor = CObjectPolygon3D::Create();
		floor->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		floor->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.25f, 0.0f));
		floor->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		floor->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		floor->SetColor(D3DCOLOR(0xff4285f4));
	}
	{
		CObjectPolygon3D* wall = CObjectPolygon3D::Create();
		wall->SetPos(D3DXVECTOR3(75.0f, 100.0f, 75.0f));
		wall->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.25f, 0.0f));
		wall->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		wall->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		wall->SetColor(D3DCOLOR(0xff4285f4));
	}
	{
		CObjectPolygon3D* wall = CObjectPolygon3D::Create();
		wall->SetPos(D3DXVECTOR3(-75.0f, 100.0f, 75.0f));
		wall->SetRot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.25f, 0.0f));
		wall->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		wall->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		wall->SetColor(D3DCOLOR(0xff4285f4));
	}

	return S_OK;
}

//=============================================================================
// ゲームの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();
	Release();
}

//=============================================================================
// ゲームの更新処理
//=============================================================================
void CGame::Update(void)
{
	if (CInput::GetKey()->Trigger(DIK_RETURN))
	{
		CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_RESULT);
	}
}

//=============================================================================
// ゲームの描画処理
//=============================================================================
void CGame::Draw(void)
{

}
