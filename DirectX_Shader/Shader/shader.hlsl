#include "common.hlsli"
#include "Lighting.hlsli"

struct Output_VS 
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;

	float4 light_pos : POSITION4;
};

struct Output_PS {
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 rough_meta_spe : SV_Target2;
	float4 emmision : SV_Target3;
	float4 depth_shadow : SV_Target4;
};


StructuredBuffer<LIGHT> LightSBuffer : register(t12)
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
	wvp = mul(WorldBuffer.World, CameraBuffer.View);
	wvp = mul(wvp, CameraBuffer.Projection);

	float4 pos = float4(inPosition, 1.0f);
	
	output.pos = mul(pos, wvp);

	float4 worldNormal, normal;
	normal = float4(inNormal, 0.0f);
	worldNormal = mul(normal, WorldBuffer.InverseWorld);
	worldNormal = normalize(worldNormal);
	output.normal = worldNormal.xyz;
	
	
	output.texcoord = inTexCoord;

	output.color = inDiffuse;
	return output;
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register(t0);
Texture2D		g_AlbedoTexture : register(t1);
Texture2D		g_OccMetalRoughTexture : register(t2);
Texture2D		g_AoTexture : register(t3);
Texture2D		g_EmissiveTexture : register(t4);
Texture2D		g_NormalTexture : register(t5);

SamplerState	g_SamplerState : register(s0);


//=============================================================================
// ピクセルシェーダ
//=============================================================================
Output_PS PS_main(Output_VS a)
{
	Output_PS output;
	float4 color = 0.0;

	if (Material.UseAlbedoTex == 1)
	{
		color = g_Texture.Sample(g_SamplerState, a.texcoord);
		//color = Material.Roughness;

		//color = float4(1.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{
		//color = a.color;
		color = float4(0.5f, 0.5f, 0.5f, 1.0f);
		
	}

	
	
	output.color = color;
	output.normal = float4(a.normal* 0.5f + 0.5f, 1);
	//output.normal = float4(a.normal, 1);
	output.rough_meta_spe = float4(Material.Roughness, Material.Metaric, Material.Specular, 1.0f);
	//output.color = float4(Material.Roughness, Material.Roughness, Material.Roughness, 1.0f);
	//output.color = float4(.0f, 0.0f, 1.0f, 1.0f);
	return output;
}
