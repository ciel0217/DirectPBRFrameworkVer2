#include "DirectXTexture2D.h"
#include "CDxRenderer.h"

DirectXTexture2D::DirectXTexture2D(ID3D11Texture2D * texture) : m_Texture(texture)
{
}

DirectXTexture2D::DirectXTexture2D(DirectXTexture2D & texture)
{
}

DirectXTexture2D::~DirectXTexture2D()
{
	m_TextureRTVs.clear();
}

DirectXTexture2D * DirectXTexture2D::CreateTexture(int width, int height, DXGI_FORMAT texFormat, UINT bindFlags, D3D11_SUBRESOURCE_DATA * data)
{

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = texFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = bindFlags;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* texturePtr;
	
	HRESULT result = CDxRenderer::GetRenderer()->GetDevice()->CreateTexture2D(&textureDesc, data, &texturePtr);
	if (FAILED(result))
	{
		return nullptr;
	}

	return new DirectXTexture2D(texturePtr);
}

DirectXTexture2D * DirectXTexture2D::CreateTextureCube(int size, DXGI_FORMAT texFormat, UINT bindFlags, UINT mipLevels, D3D11_SUBRESOURCE_DATA * data)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = size;
	textureDesc.Height = size;
	textureDesc.MipLevels = mipLevels;
	textureDesc.ArraySize = 6; // 6 faces
	textureDesc.Format = texFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = bindFlags;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* texturePtr;
	HRESULT result = CDxRenderer::GetRenderer()->GetDevice()->CreateTexture2D(&textureDesc, data, &texturePtr);
	if (FAILED(result))
	{
		return nullptr;
	}

	return new DirectXTexture2D(texturePtr);
}

bool DirectXTexture2D::CreateSRV(DXGI_FORMAT texFormat, D3D11_SRV_DIMENSION viewDimension, UINT mipLevels)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texFormat;
	srvDesc.ViewDimension = viewDimension;

	if (viewDimension == D3D11_SRV_DIMENSION_TEXTURECUBE)
	{
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = mipLevels;
	}
	else
	{
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = mipLevels;
	}
	// Create the shader resource view.
	HRESULT result = CDxRenderer::GetRenderer()->GetDevice()->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_TextureSRV);
	
	return SUCCEEDED(result);
}

bool DirectXTexture2D::CreateRTV(DXGI_FORMAT texFormat)
{
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texFormat;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	ID3D11RenderTargetView* rtv;
	HRESULT result = CDxRenderer::GetRenderer()->GetDevice()->CreateRenderTargetView(m_Texture.Get(), &rtvDesc, &rtv);
	m_TextureRTVs.push_back(rtv);
	return SUCCEEDED(result);
}

bool DirectXTexture2D::CreateTextureCubeRTVs(DXGI_FORMAT texFormat, UINT mipLevels)
{
	for (UINT mipSlice = 0; mipSlice < mipLevels; ++mipSlice)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = texFormat;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.MipSlice = mipSlice;
		rtvDesc.Texture2DArray.ArraySize = 1;
		for (UINT i = 0; i < 6; ++i)
		{
			rtvDesc.Texture2DArray.FirstArraySlice = i;
			ID3D11RenderTargetView* rtv;
			HRESULT result = CDxRenderer::GetRenderer()->GetDevice()->CreateRenderTargetView(m_Texture.Get(), &rtvDesc, &rtv);
		
			m_TextureRTVs.push_back(rtv);
			if (FAILED(result))
			{
				return false;
			}
		}

	}
	return true;
}

bool DirectXTexture2D::CreateDSV(DXGI_FORMAT texFormat)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = texFormat;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = 0;

	HRESULT result = CDxRenderer::GetRenderer()->GetDevice()->CreateDepthStencilView(m_Texture.Get(), &dsvDesc, &m_TextureDSV);
	
	return SUCCEEDED(result);
}

