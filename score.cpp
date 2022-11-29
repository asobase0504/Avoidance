//============================
//
// スコア設定
// Author:hamada ryuuga
//
//============================
#include "score.h"
#include "application.h"
#include "number.h"

int CScore::m_nScore;

CScore::CScore()
{
}

CScore::~CScore()
{
}

//===================
//初期化処理
//===================
HRESULT CScore::Init()
{
	//m_posScore = D3DXVECTOR3(30.0f, 50.0f, 0.0f);

	//頂点バッファをロックし頂点情報へのポインタを取得
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{	
		m_pNumber[nCntScore] = CNumber::Create();
		m_pNumber[nCntScore]->SetPos(m_posScore);
		m_pNumber[nCntScore]->SetSize(D3DXVECTOR3 (30.0f, 30.0f, 0.0f));
		m_posScore += D3DXVECTOR3(50.0f, 0.0f, 0.0f);
	}

	return S_OK;
}

//===================
//破棄処理
//===================
void  CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		m_pNumber[nCntScore]->Uninit();
		m_pNumber[nCntScore] = nullptr;
	}

}

//===================
//更新処理
//===================
void  CScore::NormalUpdate(void)
{
}

//===================
//描画処理
//===================
void  CScore::Draw(void)
{
}

//===================
//セット
//===================
void  CScore::Set(int nScore)
{
	int nCntScore;
	int aPosTexU[MAX_SCORE]; //各桁の数字を格納

	m_nScore = nScore;


	int nModScore = m_nScore;

	for (int i = 7; i >= 0; i--)
	{
		aPosTexU[i] = (nModScore % 10);
		nModScore /= 10;
	}
	
	//頂点バッファをロックし頂点情報へのポインタを取得
	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		if (aPosTexU[nCntScore] != 0)
		{
			m_pNumber[nCntScore]->SetSpeed(5.0f);
			m_pNumber[nCntScore]->SetNumber(aPosTexU[nCntScore]);
		}
		else
		{
			m_pNumber[nCntScore]->SetSpeed(0.0f);
			m_pNumber[nCntScore]->SetNumber(0);
		}
		m_pNumber[nCntScore]->SetTex(PositionVec4(0.1f * aPosTexU[nCntScore], 0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f, 1.0f));
	}
}

//===================
//加算
//===================
void CScore::Add(int nValue)
{//スコアの設定とPointItemの生成＆設定
	if (*CApplication::GetInstance()->GetMode() == CApplication::MODE_GAME)
	{
		m_nScore += nValue;
		Set(m_nScore);
	}
}

//===================
//情報取得
//===================
int &CScore::GetScore(void)
{
	return m_nScore;
}

CScore *CScore::Create(const D3DXVECTOR3 pos)
{
	CScore * pObject = nullptr;
	pObject = new CScore;

	if (pObject != nullptr)
	{
		pObject->SetPos(pos);
		pObject->Init();
	}

	return pObject;
}
