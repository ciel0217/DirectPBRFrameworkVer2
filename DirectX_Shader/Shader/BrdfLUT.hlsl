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
	float3 V;
	V.x = sqrt(1.0f - vs.texcoord.x);
	V.y = 0.0f;
	V.z = vs.texcoord.x;

	float A = 0;
	float B = 0;

	float3 N = float3(0.0f, 0.0f, 1.0f);
	for (uint i = 0u; i < SampleCount; i++) {
		float2 Xi = Hammersley2d(i, SampleCount);
		float3 H = ImportanceSampleGGX(Xi, N, vs.texcoord.y);
		float3 L = 2 * dot(V, H) * H - V;

		float NdotL = saturate(L.z);
		float NdotH = saturate(H.z);
		float VdotH = saturate(dot(V, H));

		if (NdotL > 0.0f) {
			float G = GeometrySmithIBL(vs.texcoord.x, NdotL, vs.texcoord.y);
			float G_Vis = G * VdotH / (NdotH * vs.texcoord.x);
			float Fc = pow(1 - VdotH, 5);

			A += (1 - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}
	float4 ret4 = float4(1.0f, .0f, .0f, 1.0f);
	float2 ret = float2(A, B) / SampleCount;

	ret4 = float4(ret.x, ret.y, .0f, 1.0f);
	
	return ret4;
}
