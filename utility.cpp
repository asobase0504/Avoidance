//===========================================================================
// 
// utility.cpp
// Author  : katsuki mizuki
// Author  : Hamada Ryuuga
// Author  : Yuda Kaito
// 
//===========================================================================

//---------------------------------------------------------------------------
// include
//---------------------------------------------------------------------------
#include "main.h"
#include "utility.h"
#include "camera.h"
#include "task_group.h"
#include "application.h"

//---------------------------------------------------------------------------
// 角度の正規化
//---------------------------------------------------------------------------
float NormalizeAngle(float& pAngle)
{
	if (pAngle >= D3DX_PI)
	{// 3.14より大きい
		pAngle -= D3DX_PI * 2.0f;
	}
	else if (pAngle <= -D3DX_PI)
	{// -3.14より小さい
		pAngle += D3DX_PI * 2.0f;
	}

	return pAngle;
}

//---------------------------------------------------------------------------
// ホーミング
//---------------------------------------------------------------------------
bool Homing(D3DXVECTOR3 *pPosOut, const D3DXVECTOR3 &posNow, const D3DXVECTOR3 &posDest, float fSpeed)
{
	D3DXVECTOR3 vecDiff = posDest - posNow;

	float fLength = D3DXVec3Length(&vecDiff);

	if (fLength <= fSpeed)
	{// 速さより長さが小さい時
		*pPosOut = posDest;
		return true;
	}
	else
	{// 速さより長さが大きい時
		*pPosOut = posNow + ((vecDiff / fLength) * fSpeed);
		return false;
	}
}

//---------------------------------------------------------------------------
// 小数点のランダム
//---------------------------------------------------------------------------
float FloatRandam(float fMax, float fMin)
{
	return ((rand() / (float)RAND_MAX) * (fMax - fMin)) + fMin;
}

//---------------------------------------------------------------------------
// 整数のランダム
//---------------------------------------------------------------------------
int IntRandam(int nMax, int nMin)
{
	return (int)((rand() / (float)RAND_MAX) * (nMax - nMin)) + nMin;
}

//---------------------------------------------------------------------------
// sinカーブの値が1.0f〜0.0fで帰ってくる
//---------------------------------------------------------------------------
float SinCurve(int nTime, float fCycle)
{
	return  (sinf((nTime * fCycle) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
}

//---------------------------------------------------------------------------
// cosカーブの値が1.0f〜0.0fで帰ってくる
//---------------------------------------------------------------------------
float CosCurve(int nTime, float fCycle)
{
	return  (cosf((nTime * fCycle) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
}

//---------------------------------------------------------------------------
// カーブの値が帰ってくる
//---------------------------------------------------------------------------
float Curve(float fCurve, float fMax, float fMin)
{
	return (fCurve * (fMax - fMin)) + fMin;
}

//---------------------------------------------------------------------------
// スクリーン座標をワールド座標へのキャスト
//---------------------------------------------------------------------------
D3DXVECTOR3 WorldCastScreen(D3DXVECTOR3 *screenPos,		// スクリーン座標
	D3DXVECTOR3 screenSize,								// スクリーンサイズ
	D3DXMATRIX* mtxView,								// ビューマトリックス
	D3DXMATRIX* mtxProjection)							// プロジェクションマトリックス
{
	// 変数宣言
	D3DXVECTOR3 ScreenPos;

	// 計算用マトリックスの宣言
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;

	// 各行列の逆行列を算出
	D3DXMatrixInverse(&InvView, NULL, mtxView);
	D3DXMatrixInverse(&InvPrj, NULL, mtxProjection);
	D3DXMatrixIdentity(&VP);
	VP._11 = screenSize.x * 0.5f; VP._22 = -screenSize.y * 0.5f;
	VP._41 = screenSize.x * 0.5f; VP._42 = screenSize.y * 0.5f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// ワールド座標へのキャスト
	D3DXMATRIX mtxWorld = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(&ScreenPos, screenPos, &mtxWorld);

	return ScreenPos;
}

//---------------------------------------------------------------------------
// スクリーン座標をワールド座標へのキャスト
//---------------------------------------------------------------------------
D3DXVECTOR3 ScreenCastWorld(const D3DXVECTOR3 &screenPos, D3DXVECTOR3 screenSize)
{
	D3DXVECTOR3 pos = screenPos;

	CCamera* camera = (CCamera*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(0, CTaskGroup::LEVEL_3D_1);
	D3DXVECTOR3 Camerapos = camera->GetPos();

	pos.y *= -1;

	pos -= (Camerapos - D3DXVECTOR3(screenSize.x * 0.5f, screenSize.y * 0.5f, 0.0f));

	return pos;
}

//---------------------------------------------------------------------------
// マトリックスの計算
// Author : Hamada Ryuuga
//---------------------------------------------------------------------------
D3DXMATRIX * GiftMtx(D3DXMATRIX * pOut, const D3DXVECTOR3& inPos, const D3DXVECTOR3& inRot)
{
	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	// 行列初期化関数(第1引数の行列を単位行列に初期化)
	D3DXMatrixIdentity(pOut);

	D3DXMatrixRotationYawPitchRoll(&mtxRot, inRot.y, inRot.x, inRot.z);
	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	D3DXMatrixMultiply(pOut, pOut, &mtxRot);

	// 位置を反映
	// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, inPos.x, inPos.y, inPos.z);
	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	D3DXMatrixMultiply(pOut, pOut, &mtxTrans);

	return pOut;
}

//---------------------------------------------------------------------------
// イージングサイン計算
// Author : Hamada Ryuuga
//---------------------------------------------------------------------------
float easeInSine(float X)
{
	return 1 - cos((X * D3DX_PI) * 0.5f);
}

//---------------------------------------------------------------------------
// イージング累乗計算
// Author : Hamada Ryuuga
//---------------------------------------------------------------------------
float easeInQuad(float X)
{
	return X * X;
}

//---------------------------------------------------------------------------
// 2Dベクトルの外積
// Author: Yuda Kaito
//---------------------------------------------------------------------------
float Vec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->z - v1->z * v2->x;
}

//---------------------------------------------------------------------------
// 2Dベクトルの内積
// Author: hamada ryuuga
//---------------------------------------------------------------------------
float Vec2Dot(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->x + v1->z * v2->z;
}

D3DXVECTOR3 ConvertQuaternionfromRadian(const D3DXQUATERNION& inQuaternion)
{
	D3DXVECTOR3 rot;
	float z1 = 2 * (inQuaternion.x * inQuaternion.y + inQuaternion.z * inQuaternion.w);
	float z2 = inQuaternion.x * inQuaternion.x - inQuaternion.y * inQuaternion.y - inQuaternion.z * inQuaternion.z - inQuaternion.w * inQuaternion.w;
	float z3 = z1 / z2;
	rot.z = atanf(z3);

	float y1 = 2 * (inQuaternion.x * inQuaternion.z - inQuaternion.y * inQuaternion.w);
	rot.y = atanf(y1);

	float x1 = 2 * (inQuaternion.x * inQuaternion.w + inQuaternion.y * inQuaternion.z);
	float x2 = inQuaternion.x * inQuaternion.x + inQuaternion.y * inQuaternion.y - inQuaternion.z * inQuaternion.z - inQuaternion.w * inQuaternion.w;
	float x3 = x1 / x2;
	rot.x = atanf(x3);

	return rot;
}
