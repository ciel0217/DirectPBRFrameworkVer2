#include "BRDF.hlsli"

#define NORMAL_UNUSED 0
#define NORMAL_MAP 1
#define BUMP_MAP 2



// マトリクスバッファ
cbuffer WorldBuffer : register(b0)
{
	matrix World;
}

cbuffer ViewBuffer : register(b1)
{
	matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

// マテリアルバッファ
struct MATERIAL
{
	float4  	BaseColor;
	float4  	Normal;
	float    	Roughness;
	float    	Metaric;
	float   	Specular;
	int			UseAlbedoMap;
	int			UseOccMetalRough;
	int			UseAoMap;
	int			UseEmmisive;
	int			NormalState;
	int			Dummy[3];
	//float		Dummy[2];	//16byte境界用
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}

struct OCEAN_BUFFER
{
	float4 MixColor;
	float Time;
	float WaveSpeed;

	float Dummy[2];
};

cbuffer OceanBuffer : register(b4)
{
	OCEAN_BUFFER Ocean;
};

cbuffer CameraPos : register(b5)
{
	float4 CameraPos;
}

struct Output_VS
{
	float4 pos : SV_POSITION;
	float4 wpos : POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct Output_PS {
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 rough_meta_spe : SV_Target2;
	float4 emmision : SV_Target3;
};




//=============================================================================
// 頂点シェーダ
//=============================================================================
Output_VS VS_main(in  float3 inPosition		: POSITION0,
	in  float3 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0,
	in	float3 inTangent : TANGENT,
	in  float3 inBinormal : BINORMAL
)
{
	Output_VS output;
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float4 pos = float4(inPosition, 1.0f);
	output.wpos = mul(pos, World);
	output.pos = mul(pos, wvp);
	output.normal = normalize(mul(float4(inNormal, .0), World).xyz);
	output.texcoord = inTexCoord;
	output.color = inDiffuse;
	output.tangent = inTangent;
	output.binormal = inBinormal;
	return output;
}

Texture2D		g_Texture : register(t0);
Texture2D		g_AlbedoTexture : register(t1);
Texture2D		g_OccMetalRoughTexture : register(t2);
Texture2D		g_AoTexture : register(t3);
Texture2D		g_EmissiveTexture : register(t4);
Texture2D		g_NormalTexture : register(t5);

SamplerState	g_SamplerState : register(s0);



float3 NormalStrength(float3 Normal, float Strength)
{
	float3 ret = float3(Normal.rg * Strength, lerp(1.0f, Normal.b, saturate(Strength)));
	return ret;
}

float3 CalcNormalMap(float2 Uv, float3 Tangent, float3 Binormal, float3 Normal)
{
	float3 normal = g_NormalTexture.Sample(g_SamplerState, Uv).xyz;
	float strength = 0.7f;
	normal = normalize((normal * 2.0f) - 1.0f);
	normal = NormalStrength(normal, strength);

	float3 ret = (normal.x * Tangent) + (normal.y * Binormal) + (normal.z * Normal);

	return normalize(ret);
}

float2 TilingAndOffset(float2 Uv, float2 Tiling, float2 Offset)
{
	float2 ret = Uv * Tiling + Offset;
	return ret;
}

float3 NormalBlend(float3 N1, float3 N2)
{
	float3 ret = normalize(float3(N1.rg + N2.rg, N1.b * N2.b));
	return ret;
}

//=============================================================================
// ピクセルシェーダ
//=============================================================================
Output_PS PS_main(Output_VS vs)
{
	Output_PS output;
	
	//色の計算
	float3  viewdir = (CameraPos.xyz - vs.wpos.xyz);
	float f = FresnelEffect(vs.normal, viewdir, 1.0f);
	float3 color = lerp(Material.BaseColor.xyz, Ocean.MixColor.xyz,  f);

	//時間計算andUVSCROLL
	float2 time = Ocean.Time * float2(-1.0f, 0.0f);
	float2 divide = time / 20.0f;
	float2 offset = divide * Ocean.WaveSpeed;
	float2 tiling = float2(1.0f, 1.0f);
	
	float2 base_uv = float2(vs.wpos.x, vs.wpos.z);
	//一つ目の法線
	float2 tiling_uv = TilingAndOffset(base_uv, tiling, offset);
	tiling_uv = tiling_uv * 0.71f;
	float3 N1 = CalcNormalMap(tiling_uv, vs.tangent, vs.binormal, vs.normal);
	//float3 normal1 = normalize(N1) * 0.5f + 0.5f;

	//二つ目の法線
	float2 normal_uv = base_uv;
	float3 N2 = CalcNormalMap(normal_uv, vs.tangent, vs.binormal, vs.normal);
	
	float3 normal = NormalBlend(N1,N2);
	normal = normalize(normal) * 0.5f + 0.5f;

	output.color = float4(color, 1.0f);
	output.normal = float4(normal, 1.0f);
	output.rough_meta_spe = float4(Material.Roughness, Material.Metaric, Material.Specular, 1.0f);
	return output;
}