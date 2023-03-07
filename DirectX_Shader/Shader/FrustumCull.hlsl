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

#define SIZE_X    32
#define SIZE_Y     1
#define SIZE_Z     1

[numthreads(SIZE_X, SIZE_Y, SIZE_Z)]

void CS_Main(const CSInput input)
{
	uint id = input.dispatch.x;

	Result[id] = id;
}