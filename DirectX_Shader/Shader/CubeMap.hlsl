#include "common.hlsli"

struct Output_VS
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct Output_PS
{
	float4 color : SV_TARGET0;
};

Output_VS VS_main(in  float3 inPosition		: POSITION0,
	in  float3 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0)
{
	Output_VS output;
	matrix wvp;
	wvp = mul(WorldBuffer.World, CameraBuffer.View);
	wvp = mul(wvp, CameraBuffer.Projection);

	float4 pos = float4(inPosition, 1.0f);
	float4 nor = float4(inNormal, 1.0f);

	output.pos = mul(pos, wvp);
	output.normal = nor;
	output.texcoord = inTexCoord;

	output.color = inDiffuse;
	return output;
}


//*****************************************************************************
// ÉOÉçÅ[ÉoÉãïœêî
//*****************************************************************************
Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

Output_PS PS_main(Output_VS input) : SV_Target0
{
	Output_PS output;
	output.color = g_Texture.Sample(g_SamplerState, input.texcoord);


	return output;
	
}