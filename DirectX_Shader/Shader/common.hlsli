struct WORLD_BUFFER
{
	matrix World;
	matrix InverseWorld;
};

// マトリクスバッファ
cbuffer WorldBuffer : register(b0)
{
	WORLD_BUFFER WorldBuffer;
}

struct CAMERA_BUFFER
{
	float4 CameraPos;
	matrix View;
	matrix InverseView;
	matrix Projection;
	matrix InverseProjection;
	matrix InverseVP;
	matrix WVP; //2D用
};

cbuffer CameraBuffer : register(b1)
{
	CAMERA_BUFFER CameraBuffer;
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

