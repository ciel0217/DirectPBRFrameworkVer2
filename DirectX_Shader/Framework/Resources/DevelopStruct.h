#pragma once
#include "../LowLevel/DirectX.h"


//*********************************************************
// �\����
//*********************************************************

// ���_�\����
struct VERTEX_3D
{
	D3DXVECTOR3 Position;//���W
	D3DXVECTOR3 Normal;//�@��
	D3DXVECTOR2 TexCoord;//UV�l
	D3DXCOLOR   Diffuse;//���_�J���[
	D3DXVECTOR3 Tangent;//tangent(�ڃx�N�g���H�Ȃ�Ă������������w)
	D3DXVECTOR3 Binormal;//�]�@��
	
	float Dummy[2];
};

// ���C�g�\����
struct LIGHT {
	D3DXVECTOR4 Direction;	// ���C�g�̕���
	D3DXVECTOR4 Position;	// ���C�g�̈ʒu
	D3DXCOLOR	Diffuse;	// �g�U���̐F
	D3DXCOLOR   Ambient;	// �����̐F
	float		Attenuation;// ������
	float		ConstantAtt;
	float		LinearAtt;
	float		QuadAtt;
	float		SpotAngle;  //�X�|�b�g���C�g�̎��̊p�x
	int			Status;		// ���C�g���
	int			LightType;		// �L���t���O
	float		Dummy;
};


// �}�e���A���p�萔�o�b�t�@�\����
struct MATERIAL_CBUFFER
{
	D3DXCOLOR  	BaseColor;
	D3DXCOLOR  	Normal;
	float    	Roughness;
	float    	Metaric;
	float   	Specular;
	int			UseAlbedoTex;
	int			UseOccMetalRough;
	int			UseAoMap;
	int			UseEmmisive;
	int			NormalState;
};

// ���C�g�p�t���O�\����
struct LIGHTFLAGS
{
	int			Type;		//���C�g�^�C�v�ienum LIGHT_TYPE�j
	int         OnOff;		//���C�g�̃I��or�I�t�X�C�b�`
	int			Dummy[2];
};

// ���C�g�p�萔�o�b�t�@�\����
struct LIGHT_CBUFFER
{
	D3DXCOLOR GlobalAmbient;
	LIGHT Lights[10];
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

struct WORLD_CBUFFER
{
	D3DXMATRIX World;
	D3DXMATRIX InverseWorld;
};

struct CAMERA_CBUFFER
{
	D3DXVECTOR4 CameraPos;
	D3DMATRIX TransView;
	D3DMATRIX InverseView;
	D3DMATRIX TransProjection;
	D3DMATRIX InverseProjection;
	D3DMATRIX InverseVP;
	D3DMATRIX TransWVP;
};

struct CAMERA_INFO
{
	D3DXVECTOR4 CameraPos;
	D3DMATRIX View;
	D3DMATRIX Projection;
	D3DMATRIX WVP;
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

struct ParticleStructuredBuffer
{
	D3DXMATRIX WorldMatrix;
	D3DXCOLOR Color;
	D3DXVECTOR3 Size;
	D3DXVECTOR2 UV;
	D3DXVECTOR2 Offset;

	float Dummy;
};

struct FrustumCullStructuredBuffer
{
	D3DXVECTOR4 Position;
	D3DXVECTOR4 Scale;
};

struct FrustumCullCameraCBuffer
{
	D3DXVECTOR4 Position;
	D3DMATRIX InvVPMatrix;
};