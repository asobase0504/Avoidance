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
float4 vColor;		// ライト＊メッシュの色
float3 vEyePos;		// カメラの位置（ローカル座標系）

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos			: POSITION;			// 位置
	float4 Color		: COLOR0;			// 色
	float2 Tex			: TEXCOORD0;		// テクスチャ
	float3 N			: TEXCOORD2;		// 法線
	float3 X			: TEXCOORD1;
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
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// 出力データ

	// 座標変換
	Out.Pos = mul(Pos, mWVP);

	// テクスチャ座標
	Out.Tex = Tex;

	// 拡散光＋環境光
	float amb = -vLightDir.w;	// 環境光の強さ
	float3 L = -vLightDir;		// ローカル座標系でのライトベクトル
	Out.Color = vColor * max(amb, dot(Normal, -vLightDir));

	// 鏡面反射用のベクトル
	Out.N = Normal.xyz;
	Out.X = Pos.xyz;

	return Out;
}

//=========================================
//ピクセルシェーダー
//=========================================
float4 PS(VS_OUTPUT In) : COLOR
{
	float3 L = vLightDir.xyz;				// ライトベクトル
	float3 N = normalize(In.N);				// 法線ベクトル
	float3 V = normalize(vEyePos - In.X);	// 視線ベクトル
	float3 H = normalize(L + V);			// ハーフベクトル

	// 計算に使うそれぞれの角度
	float NV = dot(N,V);
	float NH = dot(N,H);
	float VH = dot(V,H);
	float NL = dot(N,L);
	float LH = dot(L,H);

	float4 Ambient = { 0.0f,0.0f,0.0f, 1.0f };

	return In.Color + Ambient;		// 拡散光＋環境光(テクスチャの色)
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique Diffuse
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
