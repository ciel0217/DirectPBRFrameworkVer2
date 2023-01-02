#include "common.hlsli"

struct DummyVertex {};

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

StructuredBuffer<ParticleInfo> Particle : register(t0);

////何もしない頂点シェーダー
//void VS_main() {}

//=============================================================================
// 頂点シェーダ
//=============================================================================
Output_VS VS_main(in  float3 inPosition		: POSITION0,
	in  float3 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0,
	in	float3 inTangent : TANGENT,
	in  float3 inBinormal : BINORMAL,
	in uint id : SV_InstanceID
)
{
	Output_VS output;
	matrix wvp;
	wvp = mul(Particle[id].WorldMatrix, View);
	wvp = mul(wvp, Projection);
	//inPosition.y = inPosition.y + 10.0 * id;
	float4 pos = float4(inPosition, 1.0f);

	output.pos = mul(pos, wvp);
	output.normal = normalize(mul(float4(inNormal, .0), World).xyz);
	output.texcoord = inTexCoord;

	output.color = inDiffuse;
	return output;
}


//ジオメトリシェーダー
[maxvertexcount(6)] //quadだから6にしなきゃいけない?

void GS_Main(
	triangle Output_VS input[3],
	inout TriangleStream<Output_GS> output
)
{
	Output_GS gs;
	gs.Position = input[0].pos + float4(50.0f, 0.0f, 0.0f, 0.0f);
	gs.Color = input[0].color;
	gs.TexCoord = input[0].texcoord;

	output.Append(gs);

	gs.Position = input[1].pos + float4(50.0f, 0.0f, 0.0f, 0.0f);
	gs.Color = input[1].color;
	gs.TexCoord = input[1].texcoord;

	output.Append(gs);

	gs.Position = input[2].pos + float4(50.0f, 0.0f, 0.0f, 0.0f);
	gs.Color = input[2].color;
	gs.TexCoord = input[2].texcoord;

	output.Append(gs);
	output.RestartStrip();
	/*float half_width = Particle[id].Size.x / 2.0;
	float half_height = Particle[id].Size.y / 2.0f;

	float3 position[4];
	float2 uv[4];
	float4 a[3];

	matrix wvp;
	wvp = mul(Particle[id].WorldMatrix, View);
	wvp = mul(wvp, Projection);
	Output_GS gs;

	a[0] = mul(input[0].pos, wvp);
	a[1] = mul(input[1].pos, wvp);
	a[2] = mul(input[2].pos, wvp);

	gs.Position = a[0] + float4(1.0f, 0.0f, 0.0f, 0.0f);
	gs.Color = input[0].color;
	gs.TexCoord = input[0].texcoord;

	output.Append(gs);

	gs.Position = a[1] + float4(1.0f, 0.0f, 0.0f, 0.0f);
	gs.Color = input[1].color;
	gs.TexCoord = input[1].texcoord;

	output.Append(gs);

	gs.Position = a[2] + float4(1.0f, 0.0f, 0.0f, 0.0f);
	gs.Color = input[2].color;
	gs.TexCoord = input[2].texcoord;

	output.Append(gs);

	gs.Position = a[0] + float4(1.0f, 0.0f, 0.0f, 0.0f);
	gs.Color = input[0].color;
	gs.TexCoord = input[0].texcoord;

	output.Append(gs);

	gs.Position = a[1] + float4(1.0f, 0.0f, 0.0f, 0.0f);
	gs.Color = input[1].color;
	gs.TexCoord = input[1].texcoord;

	output.Append(gs);

	gs.Position = a[2] + float4(1.0f, 0.0f, 0.0f, 0.0f);
	gs.Color = input[2].color;
	gs.TexCoord = input[2].texcoord;

	output.Append(gs);

	

	output.RestartStrip();*/

	//position[0] = float3(-half_width, -half_height, 0.0); //左上
	//position[1] = float3(half_width, -half_height, 0.0); //右上
	//position[2] = float3(-half_width, half_height, 0.0); //左下
	//position[3] = float3(half_width, half_height, 0.0); //右下

	//uv[0] = float2(Particle[id].UV.x, Particle[id].UV.y);
	//uv[1] = float2(Particle[id].UV.x + Particle[id].Offset.x, Particle[id].UV.y);
	//uv[2] = float2(Particle[id].UV.x, Particle[id].UV.y + Particle[id].Offset.y);
	//uv[3] = float2(Particle[id].UV.x + Particle[id].Offset.x, Particle[id].UV.y + Particle[id].Offset.y);

	//float

	///*for (int i = 0; i < 6; i++)
	//{
	//	Output_GS gs;
	//	matrix wvp;
	//	wvp = mul(Particle[id].WorldMatrix, View);
	//	wvp = mul(wvp, Projection);

	//	float4 pos = float4(position[i], 1.0f);

	//	gs.Position = mul(pos, wvp);
	//	gs.Color = Particle[id].Color;
	//	gs.TexCoord = uv[i];

	//	output.Append(gs);
	//}*/

	//output.RestartStrip();

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
		//color = Material.Roughness;

		//color = float4(1.0f, 0.0f, 1.0f, 1.0f);
		//color = float4(a.texcoord.x, a.texcoord.y, 0.0f, 1.0f);
	}
	else
	{
		//color = a.color;
		color = float4(0.5f, 0.5f, 0.5f, 1.0f);

	}

	//color.w = a.color.w;

	//color = float4(0.5f, 0.5f, 0.5f, 1.0f);
	output.color = color;

	return output;
}


