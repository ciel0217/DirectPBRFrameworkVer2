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

struct Plane
{
	float4 Normal;
	float Distance;
	float Dummy[3];
};


StructuredBuffer<FrustumCullStructuredBuffer> SBuffer : register(t3);
StructuredBuffer<Plane> CameraPlaneSBuffer : register(t4);
RWStructuredBuffer<int> Result : register(u0);



//指定された平面に一番近い点を計算するための関数
float3 GetNegativePoint(uint obj_id, float4 plane)
{
	FrustumCullStructuredBuffer obj = SBuffer[obj_id];

	float4 min = obj.Position - obj.Scale / 2.0;

	float3 ret = min.xyz;
	
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

//指定された平面から一番遠い点を計算するための関数
float3 GetPositivePoint(uint obj_id, float4 plane)
{
	FrustumCullStructuredBuffer obj = SBuffer[obj_id];

	float4 min = obj.Position - obj.Scale / 2.0;

	float3 ret = min.xyz;

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

float GetDistanceToPoint(float3 obj_point, float4 normal, float distance)
{
	return dot(normal.xyz, obj_point) + distance;
}

#define SIZE_X    32
#define SIZE_Y     1
#define SIZE_Z     1

[numthreads(SIZE_X, SIZE_Y, SIZE_Z)]
void CS_Main(const CSInput input)
{
	uint id = input.dispatch.x;
	int count = 0;
	float3 np, pp;
	for (int i = 0; i < 6; i++)
	{
		int c = 0;
		float4 normal = CameraPlaneSBuffer[i].Normal;
		float distance = CameraPlaneSBuffer[i].Distance;
		np = GetNegativePoint(id, normal);
		pp = GetPositivePoint(id, normal);

		if (GetDistanceToPoint(np, normal, distance) > 0)
			c = 1;

		if (GetDistanceToPoint(pp, normal, distance) > 0)
			c= 1;
		count += c;
	}
	
	Result[id] = count;
	

}