#include "common.hlsli"

struct DummyVertex {};

struct ParticleInfo
{
	float4x4 WorldMatrix;
	float4 Color;
	float3 Size;
	float2 UV;
	float2 Offset;

	float Dummy;
};


struct Output_GS
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float2 TexCoord : TEXCOORD0;
};

StructuredBuffer<ParticleInfo> Particle : register(t0);

//何もしない頂点シェーダー
void VS_main() {}


//ジオメトリシェーダー
[maxvertextcount(4)] //quadだから6にしなきゃいけない?
void GS_main(
	point DummyVertex input[1] : POSITION,
	in uint id : SV_PrimitiveId,
	inout TriangleStream<Output_GS> output
)
{
	float half_width = Particle[id].Size.x / 2.0;
	float half_height = Particle[id].Size.y / 2.0f;

	float3 position[4];
	float2 uv[4];

	position[0] = float3(-half_width, -half_height, 0.0); //左上
	position[1] = float3(half_width, -half_height, 0.0); //右上
	position[2] = float3(-half_width, half_height, 0.0); //左下
	position[3] = float3(half_width, half_height, 0.0); //右下

	uv[0] = float2(Particle[id].UV.x, Particle[id].UV.y);
	uv[1] = float2(Particle[id].UV.x + Particle[id].Offset.x, Particle[id].UV.y);
	uv[2] = float2(Particle[id].UV.x, Particle[id].UV.y + Particle[id].Offset.y);
	uv[3] = float2(Particle[id].UV.x + Particle[id].Offset.x, Particle[id].UV.y + Particle[id].Offset.y);

	for (int i = 0; i < 4; i++)
	{
		Output_GS gs;
		matrix wvp;
		wvp = mul(Particle[id].WorldMatrix, View);
		wvp = mul(wvp, Projection);

		float4 pos = float4(position[i], 1.0f);

		gs.Position = mul(pos, wvp);
		gs.Color = Particle[id].Color;
		gs.TexCoord = uv[i];

		output.Append(gs);
	}

	output.RestartStrip();

}


struct Output_PS
{
	float4 color : SV_Target0;
};

Texture2D		g_Texture : register(t0);

SamplerState	g_SamplerState : register(s0);


Output_PS PS_main(Output_GS a)
{
	Output_PS output;
	float4 color = 0.0;

	if (Material.UseAlbedoTex == 1)
	{
		color = g_Texture.Sample(g_SamplerState, a.TexCoord);
		//color = Material.Roughness;

		//color = float4(1.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{
		//color = a.color;
		color = float4(0.5f, 0.5f, 0.5f, 1.0f);

	}



	output.color = color;

	return output;
}


