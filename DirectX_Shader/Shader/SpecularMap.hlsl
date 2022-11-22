#include "BRDF.hlsli"


// マトリクスバッファ
cbuffer WorldBuffer : register(b0)
{
	matrix World;
}

cbuffer ViewBuffer : register(b1)
{
	matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

struct Output_VS {
	float4 pos : SV_POSITION;
	float4 normal : NORMAL0;
	float3 texcoord : TEXCOORD0;
};

static const float SAMPLE_DELTA = 0.05;

Output_VS VS_main(
	in  float3 inPosition		: POSITION0,
	in  float3 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0)
{
	Output_VS output;
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float4 pos = float4(inPosition, 1.0f);


	output.pos = mul(pos, wvp);
	output.texcoord = inPosition;
	output.normal = float4(inNormal, 1.0f);

	return output;
}

TextureCube g_CubeMap : register(t8);
SamplerState	g_SamplerState : register(s0);

cbuffer Roughness : register(b4)
{
	float4 roughness;//16byte以上じゃないと確保できないらしい... xにroughnessそれ以外は使用なし
}

float4 PS_main(Output_VS vs) : SV_Target0
{
	float3 N = normalize(vs.texcoord);
	float3 R = N;
	float3 V = R;

	float totalWeight = 0.0;
	float3 prefilteredColor = float3(0.0, 0.0, 0.0);
	for (uint i = 0u; i < 1024u; ++i)
	{
		float2 st = Hammersley2d(i, 1024u);
		float3 H = ImportanceSampleGGX(st, N, roughness.x);
		float3 L = normalize(2.0 * dot(V, H) * H - V);

		float NdotL = saturate(dot(N, L));
		float NdotV = saturate(dot(N, V));
		if (NdotL > 0.0)
		{
			prefilteredColor += g_CubeMap.Sample(g_SamplerState, L).rgb * NdotL;
			totalWeight += NdotL;//加重平均
		}
	}
	prefilteredColor = prefilteredColor / totalWeight;

	return float4(prefilteredColor, 1);
}