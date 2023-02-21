// -------------------------------------------------------------
// 鏡面反射光
// 
// Copyright (c) 2003 IMAGIRE Takashi. All rights reserved.
// -------------------------------------------------------------

// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------

float4x4 mWVP;

float4 vLightDir;	// ライトの方向
float4 vDiffuse;	// ライト＊メッシュの色
float4 vAmbient;	// 色
float3 vEyePos;		// カメラの位置（ローカル座標系）

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos			: POSITION;			// 位置
	float4 Color		: COLOR0;			// 色
	float2 Tex			: TEXCOORD0;		// テクスチャ
	float3 Normal		: TEXCOORD1;
};

// -------------------------------------------------------------
// テクスチャ
// -------------------------------------------------------------
texture Tex;
sampler Samp = sampler_state
{
	Texture = <Tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = WRAP;
	AddressV = WRAP;
};

//=========================================
// 頂点シェーダー
//=========================================
VS_OUTPUT VS(
	float4 Pos    : POSITION,	// ローカル位置座標
	float4 Normal : NORMAL,		// 法線ベクトル
	float2 Tex : TEXCOORD		// テクスチャの法線ベクトル
)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	// 座標変換
	Out.Pos = mul(Pos, mWVP);

	// テクスチャ座標
	Out.Tex = Tex;

	// 拡散光＋環境光
	float3 L = -(vLightDir.xyz);		// ローカル座標系でのライトベクトル

	//法線ベクトル。
	float3 N = normalize( Normal.xyz );

	Out.Normal = N;
	Out.Color = vDiffuse * (max(vAmbient, dot(N, L)));

	return Out;
}

//=========================================
//ピクセルシェーダー
//=========================================
float4 PS(VS_OUTPUT In) : COLOR
{
	return In.Color;		// 拡散光＋環境光(テクスチャの色)
}

//=========================================
//トゥーン頂点シェーダー
//=========================================
VS_OUTPUT ToonVS(float4 Pos    : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	Out.Normal = normalize(Normal.xyz);

	// 座標変換
	Out.Pos = mul(Pos, mWVP);

	// テクスチャ座標
	Out.Tex = Tex;

	// 拡散光＋環境光
	float3 L = -(vLightDir.xyz);	// ローカル座標系でのライトベクトル

	//法線ベクトル。
	float3 N = normalize(Normal.xyz);

	Out.Color = vDiffuse;

	return Out;
}

//=========================================
//トゥーンピクセルシェーダー
//=========================================
float4 ToonPS(VS_OUTPUT In) : COLOR0
{
	float4 Out;

	//ハーフランバート拡散照明によるライティング計算
	float3 L = -(vLightDir.xyz);	// ローカル座標系でのライトベクトル

	//法線ベクトル。
	float3 N = In.Normal;

	float p = (max(vAmbient, dot(N, L))) + (max(vAmbient, dot(N, -L)));
	p = p * 0.5f + 0.5f;
	p = p * p;
	
	//色情報をテクセルのＵ成分とし、トゥーンマップテクスチャーから光の反射率を取得する
	float4 Col = In.Color * tex2D(Samp, float2(p, 0.0f));
	
	//色情報を格納する
	Out = Col;
	
	return Out;
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique Diffuse
{
	pass P0
	{
		VertexShader = compile vs_2_0 ToonVS();
		PixelShader = compile ps_2_0 ToonPS();
	}
}
