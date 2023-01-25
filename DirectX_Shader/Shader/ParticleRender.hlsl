#include "common.hlsli"

struct DummyVertex 
{
	uint id : SV_InstanceID;
};

struct Output_VS
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct ParticleInfo
{
	matrix WorldMatrix;
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

StructuredBuffer<ParticleInfo> Particle : register(t2);

//ＩDだけ渡す頂点シェーダー
DummyVertex VS_main(in uint id : SV_InstanceID) 
{ 
	DummyVertex dv;
	dv.id = id; 
	return dv;
}



//ジオメトリシェーダー
[maxvertexcount(6)] //quadだから6にしなきゃいけない?

void GS_Main(
	triangle DummyVertex input[3],
	inout TriangleStream<Output_GS> output
)
{
	uint id = input[0].id;
	
	float half_width = Particle[id].Size.x / 2.0;
	float half_height = Particle[id].Size.y / 2.0f;


	float4 position[4];
	float2 uv[4];

	
	position[0] = float4(-half_width, half_height, 0.0, 1.0f); //左上
	position[1] = float4(half_width, half_height, 0.0, 1.0f); //右上
	position[2] = float4(-half_width, -half_height, 0.0, 1.0f); //左下
	position[3] = float4(half_width, -half_height, 0.0, 1.0f); //右下

	uv[0] = float2(Particle[id].UV.x, Particle[id].UV.y);
	uv[1] = float2(Particle[id].UV.x + Particle[id].Offset.x, Particle[id].UV.y);
	uv[2] = float2(Particle[id].UV.x, Particle[id].UV.y + Particle[id].Offset.y);
	uv[3] = float2(Particle[id].UV.x + Particle[id].Offset.x, Particle[id].UV.y + Particle[id].Offset.y);

	matrix wvp;
	wvp = mul(Particle[id].WorldMatrix, CameraBuffer.View);
	//wvp = mul(World, View);
	wvp = mul(wvp, CameraBuffer.Projection);

	for (int i = 0; i < 4; i++)
	{
		Output_GS gs;

		float4 pos = position[i];

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
	float4 color = 1.0;

	if (Material.UseAlbedoTex == 1)
	{
		color = g_Texture.Sample(g_SamplerState, a.TexCoord);
		
	}
	else
	{
		color = float4(0.0f, 0.5f, 0.5f, 1.0f);
	}

	color = color * a.Color;
	output.color = color;

	return output;
}


