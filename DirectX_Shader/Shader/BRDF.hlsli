

static const float PI = 3.14159265359;

float RadicalInverse_VdC(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 Hammersley2d(uint i, uint N) {
	return float2(float(i) / float(N), RadicalInverse_VdC(i));
}

float3 ImportanceSampleGGX(float2 st, float3 N, float roughness)
{
	float a = roughness * roughness;

	float phi = 2.0 * PI * st.x;
	float cosTheta = sqrt((1.0 - st.y) / (1.0 + (a * a - 1.0) * st.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	// from spherical coordinates to cartesian coordinates
	float3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;

	// from tangent-space vector to world-space sample vector
	float3 up = abs(N.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
	float3 tangent = normalize(cross(up, N));
	float3 bitangent = cross(N, tangent);

	float3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}

float CalculateGeometrySmithIBL(float dot, float roughness)
{
	float k = (roughness * roughness) / 2.0f;

	//float k = roughness * sqrt(2.0f/PI);
	float numerator = dot;
	float denominator = dot * (1.0f - k) + k;

	return numerator / denominator;
}

float CalculateGeometrySmith(float dot, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;


	float numerator = dot;
	float denominator = dot * (1.0f - k) + k;

	return numerator / denominator;
}

float GeometrySmith(float NdotV, float NdotL, float roughness)
{
	float ggx1 = CalculateGeometrySmith(NdotL, roughness);
	float ggx2 = CalculateGeometrySmith(NdotV, roughness);

	return ggx1 * ggx2;
}

float GeometrySmithIBL(float NdotV, float NdotL, float roughness)
{
	float ggx1 = CalculateGeometrySmithIBL(NdotL, roughness);
	float ggx2 = CalculateGeometrySmithIBL(NdotV, roughness);

	return ggx1 * ggx2;
}

float NormalDistributionGGX(float NdotH, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH2 = NdotH * NdotH;

	float denominator = NdotH2 * (a2 - 1.0f) + 1;
	denominator = denominator * denominator * PI;

	return a2 / denominator;
}

float3 FresnelSchlick(float3 F0, float VdotH)
{
	float3 ret;
	ret = F0 + (1.0f - F0) * pow(1.0f - VdotH, 5.0);
	return ret;
}

float3 FresnelSchlickRoughness(float3 F0, float NdotV, float Roughness)
{
	float a = 1.0f - Roughness;
	return F0 + (max(float3(a, a, a), F0) - F0) * pow(1.0f - NdotV, 5.0f);
}

float FresnelEffect(float3 Normal, float3 ViewDir, float Power)
{
	
	return pow((1.0f - saturate(dot(normalize(Normal), normalize(ViewDir)))), Power);
}