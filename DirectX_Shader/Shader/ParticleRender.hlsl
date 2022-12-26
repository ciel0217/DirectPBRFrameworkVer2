#include "common.hlsli"

struct DummyVertexBuffer{}

struct ParticleInfo
{
	float4x4 WorldMatrix;
	float4 Color;
	float3 Size;
	float2 UV;
	float2 Offset;

	float Dummy;
};


StructuredBuffer<ParticleInfo> Particle : register(t0);
