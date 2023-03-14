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

struct CameraPlanes
{
	float4 Planes[6];
};

cbuffer CameraPlanes : register(b10)
{
	CameraPlanes CameraInfo;
}

StructuredBuffer<FrustumCullStructuredBuffer> SBuffer : register(t0);
RWStructuredBuffer<int> Result : register(u0);

#define SIZE_X    32
#define SIZE_Y     1
#define SIZE_Z     1

[numthreads(SIZE_X, SIZE_Y, SIZE_Z)]

float3 GetNegativePoint(uint obj_id, uint plane_id)
{
	FrustumCullStructuredBuffer obj = SBuffet[obj_id];
	float4 plane = CameraInfo.Planes[plane_id];

	float4 min = obj.Position - obj.Scale / 2.0;

	float3 ret = min;
	
	if (plane.x < 0.0)
	{
		ret.x += obj.Scale.x;
	}

	if (plane.y < 0.0)
	{
		ret.y += obj.Scale.y;
	}

	if (plane.z < 0.0)
	{
		ret.z += obj.Scale.z;
	}

	return ret;
}

float3 GetPositivePoint(uint id)
{
	FrustumCullStructuredBuffer obj = SBuffet[obj_id];
	float4 plane = CameraInfo.Planes[plane_id];

	float4 min = obj.Position - obj.Scale / 2.0;

	float3 ret = float3(min);

	if (plane.x > 0.0)
	{
		ret.x += obj.Scale.x;
	}

	if (plane.y > 0.0)
	{
		ret.y += obj.Scale.y;
	}

	if (plane.z > 0.0)
	{
		ret.z += obj.Scale.z;
	}

	return ret;
}


void CS_Main(const CSInput input)
{
	uint id = input.dispatch.x;

	for (int i = 0; i < 6; i++)
	{
		float3 np = GetNegativePosition(id, i);
	}
	
	Result[id] = id;
}