#include "common.hlsli"

struct DummyVertex{}

struct ParticleInfo
{
	float4x4 WorldMatrix;
	float4 Color;
	float3 Size;
	float2 UV;
	float2 Offset;

	float Dummy;
};


struct GS_Output
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float2 TexCoord : TEXCOORD0;
};

StructuredBuffer<ParticleInfo> Particle : register(t0);

//�������Ȃ����_�V�F�[�_�[
void VS_main(){}


//�W�I���g���V�F�[�_�[
[maxvertextcount(4)] //quad������6�ɂ��Ȃ��Ⴂ���Ȃ�?
void GS_main(
	point DummyVertex input[1] : POSITION,
	in uint id : SV_PrimitiveId,
	inout TriangleStream<GS_Output> output
	)
{
	float half_width = Particle[id].Size.x / 2.0;
	float half_height = Particle[id].Size.y / 2.0f;

	float3 position[4];
	float2 uv[4];

	position[0] = float3(-half_width, -half_height, 0.0); //����
	position[1] = float3(half_width, -half_height, 0.0);�@//�E��
	position[2] = float3(-half_width, half_height, 0.0); //����
	position[3] = float3(half_width, half_height, 0.0); //�E��

	uv[0] = float2(Particle[id].UV.x, Particle[id].UV.y);
	uv[1] = float2(Particle[id].UV.x + Particle[id].Offset.x, Particle[id].UV.y);
	uv[2] = float2(Particle[id].UV.x, Particle[id].UV.y + Particle[id].Offset.y);
	uv[3] = float2(Particle[id].UV.x + Particle[id].Offset.x, Particle[id].UV.y + Particle[id].Offset.y);

	for (int i = 0; i < 4; i++)
	{
		GS_Output gs;
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

