#include "common.hlsli"

struct Output_VS
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct Output_PS {
	float4 color : SV_Target0;
};

//=============================================================================
// 頂点シェーダ
//=============================================================================
Output_VS VS_main(in  float3 inPosition		: POSITION0,
	in  float3 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0
)
{
	Output_VS output;

	float4 pos = float4(inPosition, 1.0f);

	output.pos = mul(pos, Projection);
	output.normal = float4(inNormal,1.0f);
	output.texcoord = inTexCoord;

	output.color = inDiffuse;
	return output;
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


//=============================================================================
// ピクセルシェーダ
//=============================================================================
Output_PS PS_main(Output_VS a)
{
	Output_PS output;
	float4 color;

	
	color = g_Texture.Sample(g_SamplerState, a.texcoord);
	color *= a.color;
	output.color = color;

	return output;
}
