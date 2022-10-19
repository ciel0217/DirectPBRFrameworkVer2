#include "Lighting.hlsli"

struct SurfaceInfo
{
	float4 WorldPosition;
	float3 Normal;
	//float3 T;
	//float3 B;
	float3 EyeV;
	float NdotV;
};

struct LightingInfo
{
	// general terms
	float3 LightV;

	float3 HalfV;
	float NdotH;
	float NdotL;

	//currently for directional light source
	float ShadowFactor;

	// for point sources
	
	float Distance;
	float Attenuation;
};

void CalcCommonLightInfo(SurfaceInfo surf,  inout LightingInfo info)
{
	info.HalfV = normalize(info.LightV + surf.EyeV);
	info.NdotH = dot(surf.Normal, info.HalfV);
	info.NdotL = dot(surf.Normal, info.LightV);
}


void CalcDirectionalLight(SurfaceInfo surf, LIGHT light, inout LightingInfo info)
{
	info.LightV = normalize(light.Direction.xyz);
	info.Distance = -1;
	info.Attenuation = 1;
	//info.ShadowFactor = (light.Status == LIGHT_ENABLED_SHADOW) ? 1 - ShadowFactor(surf.WorldPosition) : 1;
	CalcCommonLightInfo(surf, info);
}

void CalcSpotLight(SurfaceInfo surf, LIGHT light, inout LightingInfo info)
{
	info.LightV = light.Position.xyz - surf.WorldPosition.xyz;
	info.Distance = length(info.LightV);
	info.LightV = normalize(info.LightV);
	info.Attenuation = 1.0f / (light.ConstantAtt + light.LinearAtt * info.Distance + light.QuadAtt * (info.Distance * info.Distance));

	//float minCos = cos(light.spotAngle);
	//float maxCos = (minCos + 1.0f) / 2.0f; // squash between [0, 1]
	//float cosAngle = dot(light.direction.xyz, -li.L);
	//float intensity = smoothstep(minCos, maxCos, cosAngle);
	//li.attenuation = intensity * li.attenuation;

	//li.shadowFactor = 1;
	CalcCommonLightInfo(surf, info);
}

void CalcPointLight(SurfaceInfo surf, LIGHT light, inout LightingInfo info)
{
	info.LightV = light.Position.xyz - surf.WorldPosition.xyz;
	info.Distance = length(info.LightV);
	info.LightV = normalize(info.LightV);
	info.Attenuation = 1.0f / (light.ConstantAtt + light.LinearAtt * info.Distance + light.QuadAtt * (info.Distance * info.Distance));
	
	CalcCommonLightInfo(surf, info);
}

void CalcLight(SurfaceInfo surf, LIGHT light, inout LightingInfo info)
{
	info = (LightingInfo)0;
	if (light.LightType == DIRECTIONAL_LIGHT) {
		CalcDirectionalLight(surf, light, info);
	}
	else if (light.LightType == SPOT_LIGHT) {
		CalcSpotLight(surf, light, info);
	}
	else if (light.LightType == POINT_LIGHT) {
		CalcPointLight(surf, light, info);
	}
}