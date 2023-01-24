

//*****************************************************************************
// 定数バッファ
//*****************************************************************************



cbuffer ConstantBuffer : register(b2)
{
	matrix wvp;
}


struct Output_VS
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct Output_PS 
{
	float4 color : SV_Target0;
};

//=============================================================================
// 頂点シェーダ
//=============================================================================
Output_VS VS_main(in  float3 inPosition		: POSITION0,
	in  float3 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0
)
{
	Output_VS output;
	
	float4 pos = float4(inPosition, 1.0f);
	float4 nor = float4(inNormal, 1.0f);

	output.pos = mul(pos, wvp);
	output.normal = nor;
	output.texcoord = inTexCoord;

	output.color = inDiffuse;
	return output;
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


//=============================================================================
// ピクセルシェーダ
//=============================================================================
Output_PS PS_main(Output_VS vs)
{
	Output_PS output;
	float4 color;

	
	color = g_Texture.Sample(g_SamplerState, vs.texcoord);

		
	color *= vs.color;

	output.color = color;
	//output.color = float4(.0f, 0.0f, 1.0f, 1.0f);
	//output.color.a = 1.0f;
	return output;
}
