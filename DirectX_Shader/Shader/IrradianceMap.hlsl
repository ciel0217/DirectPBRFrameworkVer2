
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
	float3 texcoord : TEXCOORD0;
};

static const float PI = 3.14159265359;
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

	
	return output;
}

TextureCube g_CubeMap : register(t8);
SamplerState	g_SamplerState : register(s0);

float4 PS_main(Output_VS vs) : SV_Target0
{
	float3 irradiance = float3(0.0f, 0.0f, 0.0f);
	float3 normal = normalize(vs.texcoord);

	float3 up = float3(0.0, 1.0, 0.0);
	float3 right = cross(up, normal);
	up = cross(normal, right);

	float nrSamples = 0.0;
	for (float phi = 0.0; phi < 2.0 * PI; phi += SAMPLE_DELTA)
	{
		for (float theta = 0.0; theta < 0.5 * PI; theta += SAMPLE_DELTA)
		{
			// spherical to cartesian (in tangent space)
			float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// tangent space to world
			float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
			irradiance += g_CubeMap.Sample(g_SamplerState, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));
	return float4(irradiance, 1);
}