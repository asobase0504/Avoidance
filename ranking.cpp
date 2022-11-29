//=============================================================================
//
// 制作 ( ランキング )
// Author  : hamada ryuuga
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <stdio.h>
#include "ranking.h"
#include "main.h"
#include "input.h"
#include "application.h"
#include "texture.h"
#include "object2d.h"
#include "name.h"
#include "fade.h"
#include <windows.h>
#include <functional>
#include <iphlpapi.h>
#include <string>
#include <thread>
#include "sound.h"
#include "text.h"
#include "score.h"

// playfab
#include "playfab/PlayFabError.h"
#include "playfab/PlayFabClientDataModels.h"
#include "playfab/PlayFabClientApi.h"
#include "playfab/PlayFabSettings.h"
#include "playfab/PlayFabApiSettings.h"

#pragma comment(lib, "iphlpapi.lib")

//-----------------------------------------------------------------------------
// 名前空間の宣言
//-----------------------------------------------------------------------------
using namespace PlayFab;
using namespace ClientModels;

//-----------------------------------------------------------------------------
// 静的メンバー変数の宣言
//-----------------------------------------------------------------------------
bool finished = false;
CScore *CRanking::m_pRanking[MAX_RANK];
int CRanking::m_score;
bool CRanking::m_isStop;
std::string  CRanking::m_playName;
std::string CRanking::m_name[5];

//=============================================================================
// 
//=============================================================================
void OnLoginGet(const LoginResult& , void* )
{
	printf("Congratulations, you made your first successful API call!\n");
	CRanking::GetScore();

	Sleep(1000);
}

//=============================================================================
// 
//=============================================================================
void OnLoginSet(const LoginResult& , void* )
{
	if (CRanking::GetMyScore() != 0)
	{
		printf("Congratulations, you made your first successful API call!\n");

		CRanking::GoScore();
		CRanking::SetName();
	}
}

//=============================================================================
// 
//=============================================================================
void OnLoginFail(const PlayFabError& error, void*)
{
	printf("Something went wrong with your first API call.\n");
	printf("Here's some debug information:\n");
	printf(error.GenerateErrorReport().c_str());
	printf("\n");
}

//=============================================================================
// コンストラクタ
//=============================================================================
CRanking::CRanking()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{
}

//=============================================================================
// 初期化
//=============================================================================
inline HRESULT CRanking::Init(void)
{
	for (int i = 0; i < 3; i++)
	{
		m_name[i] = "";
	}
	D3DXVECTOR3 pos = D3DXVECTOR3(CApplication::CENTER_POS.x, 100.0f, 0.0f);
	for (int i = 0; i < MAX_RANK - 1; i++)
	{
		m_pRanking[i] = CScore::Create(pos);
		m_pRanking[i]->Set(0);
		pos.y += 100.0f;
	}

	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_RANKING);
	
	CRanking::OnlineSetScore();

	m_nowPlay = 0;
	m_namePos = D3DXVECTOR3(30.0f, 100.0f, 0.0f);

	{ // オブジェクトの作成
		m_pObject2d[0] = CObject2d::Create();
		m_pObject2d[0]->SetTexture("RANKINBG");
		m_pObject2d[0]->SetSize(CApplication::CENTER_POS);
		m_pObject2d[0]->SetPos(CApplication::CENTER_POS);

		m_pObject2d[1] = CObject2d::Create();
		m_pObject2d[1]->SetTexture("RANKIN");
		m_pObject2d[1]->SetSize(D3DXVECTOR3(100.0f, 300.0f, 0.0f));
		m_pObject2d[1]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x - 120.0f, 350.0f, 0.0f));

		m_pObject2d[2] = CObject2d::Create();
		m_pObject2d[2]->SetTexture("RANKINTITLEOFF");
		m_pObject2d[2]->SetSize(D3DXVECTOR3(200.0f, 100.0f, 0.0f));
		m_pObject2d[2]->SetPos(D3DXVECTOR3(200.0f, 150.0f, 0.0f));
	}

	finished = false;
	m_isStop = false;
	m_isRankingSet = false;

	m_pRanking[5] = CScore::Create(pos);
	m_pRanking[5]->Set(m_score);

	PlayFabSettings::staticSettings->titleId = ("323A0");

	LoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = GetMACAddr();

	PlayFabClientAPI::LoginWithCustomID(request, OnLoginGet, OnLoginFail);

	std::thread Up(CRanking::APIUp);
	//切り離す
	Up.detach();

	return E_NOTIMPL;
}

//=============================================================================
// 破棄
//=============================================================================
void CRanking::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();
	m_isStop = true;
}

//=============================================================================
// 更新
//=============================================================================
void CRanking::Update(void)
{
	if (finished)
	{
		m_pObject2d[2]->SetTexture("RANKINTITLEON");

		if (!m_isRankingSet)
		{
			std::string Name;
			m_isRankingSet = true;
			for (int i = 0; i < 3;i++)
			{
				Name += m_name[i];
				Name += "\n";
			}
			CText::Create(CText::GON, 300, 10, Name.c_str());
		}

		CInput *CInputpInput = CInput::GetKey();

		if (CInputpInput->Trigger(CInput::KEY_DECISION))
		{
			//モードの設定
			if (m_score == 0)
			{
				CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
				return;
			}
			else
			{
				CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_RESULT);
				return;
			}
		}
	}
}

//=============================================================================
// 描画
//=============================================================================
void CRanking::Draw()
{
}

//=============================================================================
// 設定
//=============================================================================
void CRanking::GoScore()
{
	UpdatePlayerStatisticsRequest req;
	StatisticUpdate statistic;
	statistic.StatisticName = "ScoreFox";	// ゲームマネージャーでランキング名

	statistic.Value = m_score;	// 小さい順にするためにの-１かける

	req.Statistics.push_back(statistic);

	PlayFabClientAPI::UpdatePlayerStatistics(req, [](const ClientModels::UpdatePlayerStatisticsResult&, void*) {});
}

//=============================================================================
// OnlineSetScore
//=============================================================================
void CRanking::OnlineSetScore()
{
	PlayFabSettings::staticSettings->titleId = ("323A0");

	LoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = m_playName;//GetMACAddr();

	PlayFabClientAPI::LoginWithCustomID(request, OnLoginSet, OnLoginFail);
	CRanking::APIUp();
}

//=============================================================================
// GetScore
//=============================================================================
void CRanking::GetScore()
{
	GetLeaderboardRequest req;

	req.StatisticName = "ScoreFox";//ゲームマネージャーでランキング名のやつ

	PlayFabClientAPI::GetLeaderboard(req, [](const ClientModels::GetLeaderboardResult& resul, void*)
	{
		for (auto item : resul.Leaderboard)
		{
			if (item.Position <= 4)
			{
				switch (item.Position)
				{
				case 0:
					m_name[item.Position] += "いちいは";
					break;
				case 1:
					m_name[item.Position] += "にいいは";
					break;
				case 2:
					m_name[item.Position] += "さんいは";
					break;
				case 3:
					break;
				default:
					break;
				}

				m_name[item.Position] += item.DisplayName;	// 名前をchar型に変換
				// 表示
				m_pRanking[item.Position]->Set(item.StatValue);
			}
		}
		finished = true;
	});
}

//========================
// rankingの名まえのSet
//========================
void CRanking::SetName()
{
	UpdateUserTitleDisplayNameRequest req;

	req.DisplayName = m_playName;
	PlayFabClientAPI::UpdateUserTitleDisplayName(req, [](const UpdateUserTitleDisplayNameResult result, void*)
	{
		//成功

	});
}

//========================
// 任意のネットワークアダプタのMACアドレスを取得
//========================
std::string CRanking::GetMACAddr()
{
	PIP_ADAPTER_INFO adapterInfo;
	DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);

	adapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	if (adapterInfo == NULL)
	{
		return "";
	}

	if (GetAdaptersInfo(adapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(adapterInfo);
		adapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
		if (adapterInfo == NULL) {
			return "";
		}
	}

	char pOutMacAddr[64];
	if (GetAdaptersInfo(adapterInfo, &dwBufLen) == NO_ERROR)
	{
		PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
		sprintf(pOutMacAddr, "%02X:%02X:%02X:%02X:%02X:%02X",
				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

		pAdapterInfo = pAdapterInfo->Next;
	}
	else
	{
		// macアドレスが見つからない場合は固定文字列
		sprintf(pOutMacAddr, "user");
	}
	free(adapterInfo);
	return std::string(pOutMacAddr);
}

//========================
//ランキングの値をねじ込む
//========================
void CRanking::SetScore(int nScore)
{
	m_score = nScore;
}

void CRanking::APIUp()
{
	Sleep(2000);
	while (PlayFabClientAPI::Update() != 0 && !m_isStop)
		Sleep(1);
}
