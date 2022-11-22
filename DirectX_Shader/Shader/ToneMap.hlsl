#include "NDCFullScreen.hlsli"
//*****************************************************************************
// 定数バッファ
//*****************************************************************************

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


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_IntermediateTex			: register(t0);
SamplerState	g_SamplerState	: register(s0);


Output_VS VS_main(uint VertexID : SV_VertexID
)
{
	Output_VS output;
	output.pos = float4(arrBasePos[VertexID].xy, 0.0, 1.0);

	output.texcoord = arrUV[VertexID].xy;

	return output;
}


float CalcLuminance(float3 color)
{
	return dot(color.xyz, float3(0.299f, 0.587f, 0.114f));
}


//=============================================================================
// ピクセルシェーダ
//=============================================================================
Output_PS PS_main(Output_VS vs)
{
	Output_PS output;
	
	float3 color = g_IntermediateTex.Sample(g_SamplerState, vs.texcoord).rgb;
	float luminance = CalcLuminance(color);
	float reinhard = luminance / (luminance + 1);
	output.color = float4(color * (reinhard / luminance), 1.0f);

	

	output.color = g_IntermediateTex.Sample(g_SamplerState, vs.texcoord);

	return output;

}
