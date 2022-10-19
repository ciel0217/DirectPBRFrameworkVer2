#pragma once
#include "DirectX.h"
//*********************************************************
// マクロ定義
//*********************************************************
#define LIGHT_MAX		(10)


enum BLEND_MODE
{
	BLEND_MODE_NONE,		//ブレンド無し
	BLEND_MODE_ALPHABLEND,	//αブレンド
	BLEND_MODE_ADD,			//加算ブレンド
	BLEND_MODE_SUBTRACT,	//減算ブレンド

	BLEDD_MODE_NUM
};

enum CULL_MODE
{
	CULL_MODE_NONE,			//カリング無し
	CULL_MODE_FRONT,		//表のポリゴンを描画しない(CW)
	CULL_MODE_BACK,			//裏のポリゴンを描画しない(CCW)

	CULL_MODE_NUM
};

enum ADDRESS_MODE
{
	ADDRESS_MODE_WRAP,
	ADDRESS_MODE_MIRROR,
	ADDRESS_MODE_CLAMP,
	ADDRESS_MODE_BORDER,

	ADDRESS_MODE_NUM
};

enum FILTER_MODE
{
	FILTER_MODE_POINT,
	FILTER_MODE_LINEAR,
	FILTER_MODE_ANISOTROPIC,

	FILTER_MODE_NUM
};

enum SCENE_TEXTURE 
{
	eBaseColor = 0,
	eNormal,
	eRoughnessAndMeralicAndSpecular,
	eEmmision,

	eToneMap,
	eSceneMax,
};

//*********************************************************
// 構造体
//*********************************************************

// 頂点構造体
struct VERTEX_3D
{
	D3DXVECTOR3 Position;//座標
	D3DXVECTOR3 Normal;//法線
	D3DXVECTOR2 TexCoord;//UV値
	D3DXCOLOR   Diffuse;//頂点カラー
	D3DXVECTOR3 Tangent;//tangent(接ベクトル？なんていうか分からんw)
	D3DXVECTOR3 Binormal;//従法線
	
};

// ライト構造体
struct LIGHT {
	D3DXVECTOR4 Direction;	// ライトの方向
	D3DXVECTOR4 Position;	// ライトの位置
	D3DXCOLOR	Diffuse;	// 拡散光の色
	D3DXCOLOR   Ambient;	// 環境光の色
	float		Attenuation;// 減衰率
	float		ConstantAtt;
	float		LinearAtt;
	float		QuadAtt;
	float		SpotAngle;  //スポットライトの時の角度
	int			Status;		// ライト種別
	int			LightType;		// 有効フラグ
	float		Dummy;
};


// マテリアル用定数バッファ構造体
struct MATERIAL_CBUFFER
{
	D3DXCOLOR  	BaseColor;
	D3DXCOLOR  	Normal;
	float    	Roughness;
	float    	Metaric;
	float   	Specular;
	int			UseAlbedoMap;
	int			UseOccMetalRough;
	int			UseAoMap;
	int			UseEmmisive;
	int			NormalState;
};

// ライト用フラグ構造体
struct LIGHTFLAGS
{
	int			Type;		//ライトタイプ（enum LIGHT_TYPE）
	int         OnOff;		//ライトのオンorオフスイッチ
	int			Dummy[2];
};

// ライト用定数バッファ構造体
struct LIGHT_CBUFFER
{
	D3DXCOLOR GlobalAmbient;
	LIGHT Lights[LIGHT_MAX];
};

struct DEFERRED_CBUFFER
{
	int UseAO = 0;
	int UseEnvMap = 0;
	int Dummy[2];
};

struct OCEAN_CBUFFER
{
	D3DXCOLOR MixColor;
	float Time;
	float WaveSpeed;

	float Dummy[2];
};

struct RenderTarget
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTargetTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> UnorderedAccessView;
};

struct VertexShaderRelated {
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  Layout;
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> ShaderRF;
};

