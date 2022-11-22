#include "BRDF.hlsli"
#include "NDCFullScreen.hlsli"

struct Output_VS
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

static const uint SampleCount = 1024u;

Output_VS VS_main(
	uint VertexID : SV_VertexID
)
{
	Output_VS output;
	output.pos = float4(arrBasePos[VertexID].xy, 0.0, 1.0);

	output.texcoord = arrUV[VertexID].xy;
	
	return output;
}

float4 PS_main(Output_VS vs) : SV_Target0
{
	float3 v;
	v.x = sqrt(1.0f - (vs.texcoord.x * vs.texcoord.x));
	v.y = 0.0f;
	v.z = vs.texcoord.x;

	float a = 0;
	float b = 0;

	float3 n = float3(0.0f, 0.0f, 1.0f);
	for (uint i = 0u; i < SampleCount; i++) 
	{
		float2 xi = Hammersley2d(i, SampleCount);
		float3 h = ImportanceSampleGGX(xi, n, vs.texcoord.y);
		float3 l = 2 * dot(v, h) * h - v;

		float NdotL = saturate(l.z);
		float NdotH = saturate(h.z);
		float VdotH = saturate(dot(v, h));

		if (NdotL > 0.0f) {
			float g = GeometrySmithIBL(vs.texcoord.x, NdotL, vs.texcoord.y);
			float g_vis = g * VdotH / (NdotH * vs.texcoord.x);
			float fc = pow(1 - VdotH, 5);

			a += (1 - fc) * g_vis;
			b += fc * g_vis;
		}
	}

	float2 ret = float2(a, b) / SampleCount;

	float4 ret4 = float4(ret.x, ret.y, 0.0f, 1.0f);
	
	return ret4;
}
