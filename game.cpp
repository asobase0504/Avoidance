//=============================================================================
//
// ゲーム画面
// Author : Hamada Ryuuga
//
//=============================================================================

//-----------------------------------------------------------------------------
// インクルード
//-----------------------------------------------------------------------------
#include "game.h"
#include "sound.h"
#include "input.h"
#include "application.h"
#include "object.h"
#include "light.h"
#include "player.h"
#include "file.h"
#include "fade.h"
#include "multiply.h"
#include "particle_manager.h"
#include "ranking.h"
#include "score.h"
#include "bg.h"
#include "pause.h"
#include "camera.h"
#include "task_group.h"
#include "text.h"
#include "object_mesh.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数の宣言
//-----------------------------------------------------------------------------
CParticleManager*CGame::m_pPaticleManager = nullptr;
CPlayer*CGame::m_pPlayer = nullptr;
CPause *CGame::m_pPause = nullptr;

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

	//{
	//	CMesh* test = CMesh::Create();
	//	test->SetNumber(0);
	//	test->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//	test->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//	test->SetMesh(10);
	//	test->SetType(0);
	//}

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

	if (m_pPaticleManager != nullptr)
	{
		m_pPaticleManager->Uninit();
		delete m_pPaticleManager;
		m_pPaticleManager = nullptr;
	}

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
