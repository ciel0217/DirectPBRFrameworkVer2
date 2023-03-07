#include "common.hlsli"

struct FrustumCullStructuredBuffer
{
	float4 Position;
	float4 Scale;
};


struct CSInput
{
	uint3 groupThread : SV_GroupThreadID;
	uint3 group : SV_GroupID;
	uint groupIndex : SV_GroupIndex;
	uint3 dispatch : SV_DispatchThreadID;
};

StructuredBuffer<FrustumCullStructuredBuffer> SBuffer : register(t0);
RWStructuredBuffer<int> Result : register(u0);

#define size_x    32
#define size_y      1
#define size_z      1

[numthreads(size_x, size_y, size_z)]

void CS_Main(const CSInput input)
{
	uint id = input.dispatch.x;

	Result[id] = id;
}