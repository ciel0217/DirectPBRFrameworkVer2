

#define LIGHT_MAX 10

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define LIGHT_DISABLE 0
#define LIGHT_ENABLED 1
#define LIGHT_ENABLED_SHADOW 2

// ライト用バッファ
struct LIGHT
{
	float4		Direction;
	float4		Position;
	float4		Diffuse;
	float4		Ambient;
	float		Attenuation;
	float		ConstantAtt;
	float		LinearAtt;
	float		QuadAtt;
	float		SpotAngle;
	int			Status;
	int			LightType;
	float	    Dummy;
};

cbuffer  LightSettingBuffer : register(b4)
{
	float4 GlobalAmbient;
	LIGHT Lights[LIGHT_MAX];
};

