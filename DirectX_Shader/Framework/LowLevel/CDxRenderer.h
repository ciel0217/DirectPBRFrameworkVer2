#pragma once

#include "../LowLevel/DirectX.h"
#include "../Resources/DevelopStruct.h"
#include "../Resources/DevelopEnum.h"

#include <string>

//1パス目に使用するテクスチャーの数
#define RENDER_TARGET_NUM 4

class CDxRenderer {
private:
	static CDxRenderer *m_Renderer;
	D3D_FEATURE_LEVEL       m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	Microsoft::WRL::ComPtr<ID3D11Device> m_D3DDevice = NULL;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ImmediateContext = NULL;

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = NULL;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_BackBuffer = NULL;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView = NULL;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader = NULL;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader = NULL;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_VertexLayout = NULL;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStateEnable = NULL;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStateDisable = NULL;

	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStateNone = NULL;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStateAlphaBlend = NULL;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStateAdd = NULL;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStateSubtract = NULL;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterStateCullOff = NULL;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterStateCullCW = NULL;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterStateCullCCW = NULL;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_DepthStencilSRV = NULL;

	
	ID3D11Texture2D* m_DepthTexture = NULL;

	//0 : color 1 : Normal, 2 : r:roughness g:metaric b:specular 3:Emission + (lightmaps) 4 中間バッファー
	RenderTarget m_RenderTargets[RENDER_TARGET_NUM + 1];

	ID3D11RenderTargetView* m_Rtv[RENDER_TARGET_NUM];
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_IntermediateRtv;



	HWND m_hWnd;
	HINSTANCE m_hInstance;

	D3DXCOLOR m_BorderColor;
	
	BLEND_MODE				m_BlendStateParam;
	MATERIAL_CBUFFER	m_Material;
	LIGHT_CBUFFER	m_Light;

public:
	CDxRenderer() = delete;
	CDxRenderer(HWND, HINSTANCE);
	~CDxRenderer();

	//Initしてからよぶこと
	static CDxRenderer *GetRenderer() { return m_Renderer; }

	HRESULT InitRenderer();

	ID3D11InputLayout* CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * , UINT , const std::string &, const std::string &);

	ID3D11Buffer* CreateVertexBuffer(UINT VertexNum, VERTEX_3D* obj)
	{
		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		static D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
		vertexSubresourceData.pSysMem = obj;

		ID3D11Buffer* hpBuffer;

		if (!obj) {
			if (FAILED(m_D3DDevice->CreateBuffer(&bd, NULL, &hpBuffer))) {
				return nullptr;
			}
			return hpBuffer;
		}

		if (FAILED(m_D3DDevice->CreateBuffer(&bd, &vertexSubresourceData, &hpBuffer))) {
			return nullptr;
		}

		return hpBuffer;
	};

	ID3D11Buffer* CreateIndexBuffer(UINT IndexNum, UINT* Index);

	ID3D11Device* GetDevice() { return m_D3DDevice.Get(); }
	ID3D11DeviceContext *GetDeviceContext() { return m_ImmediateContext.Get(); }
	ID3D11ShaderResourceView* GetDepthStencilSRV() { return m_DepthStencilSRV.Get(); }

	void SetInputLayout(ID3D11InputLayout* VertexLayout);
	void SetVertexShader(ID3D11VertexShader* vs);
	void SetPixelShader(ID3D11PixelShader* ps);
	void SetGeometryShader(ID3D11GeometryShader* gs);

	void SetVertexBuffer(ID3D11Buffer* VertexBuffer, UINT VertexSize);
	void SetIndexBuffer(ID3D11Buffer* IndexBuffer);


	void SetDepthEnable(bool Enable);
	void SetBlendState(BLEND_MODE bm);
	void SetCullingMode(CULL_MODE cm);
	void SetAlphaTestEnable(BOOL flag);
	void SetSamplerState(FILTER_MODE fm, ADDRESS_MODE am);
	void SetSamplerBorderColor(D3DXCOLOR col);

	void UnbindShaderResourceView(UINT);
	void SetRenderTargetBackBuffor(bool );
	void SetRenderTargetByDeffard();
	//中間バッファー
	void SetRenderTargetIntermediateBuffer(bool);

	void ClearBackBuffor(bool);
	void ClearRenderTextureSceneByDeffard();
	void ClearIntermediateBuffer(bool);

	void SetViewPortDefault();

	void Present();

	RenderTarget GetSceneTexture(SCENE_TEXTURE tex) { return m_RenderTargets[tex]; }
};


