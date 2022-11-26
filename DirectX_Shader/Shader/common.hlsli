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
	int			UseAlbedoTex;
	int			UseOccMetalRough;
	int			UseAoMap;
	int			UseEmmisive;
	int			NormalState;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL	Material;
}

cbuffer InverseWorldBuffer : register(b6)
{
	matrix InverseWorld;
}
