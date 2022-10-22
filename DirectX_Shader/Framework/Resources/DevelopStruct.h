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
	int			UseAlbedoMap;
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