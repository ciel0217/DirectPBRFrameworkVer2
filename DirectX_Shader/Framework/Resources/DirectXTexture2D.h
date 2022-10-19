#pragma once
#include "DirectX.h"

class DirectXTexture2D
{
public:
	DirectXTexture2D(ID3D11Texture2D* texture);
	DirectXTexture2D(DirectXTexture2D& texture);
	~DirectXTexture2D();

	static DirectXTexture2D* CreateTexture(int width, int height, DXGI_FORMAT texFormat, UINT bindFlags, D3D11_SUBRESOURCE_DATA* data = NULL);
	static DirectXTexture2D* CreateTextureCube(int size, DXGI_FORMAT texFormat, UINT bindFlags, UINT mipLevels = 1, D3D11_SUBRESOURCE_DATA* data = NULL);
	bool CreateSRV(DXGI_FORMAT texFormat, D3D11_SRV_DIMENSION viewDimension = D3D11_SRV_DIMENSION_TEXTURE2D, UINT mipLevels = 1);
	bool CreateRTV(DXGI_FORMAT texFormat);
	bool CreateTextureCubeRTVs(DXGI_FORMAT texFormat, UINT mipLevels = 1);
	bool CreateDSV(DXGI_FORMAT texFormat);

	ID3D11RenderTargetView* GetRTV(UINT index = 0) { return m_TextureRTVs[index].Get(); }
	ID3D11RenderTargetView** GetRTVPointerToPointer(UINT index = 0) { return m_TextureRTVs[index].GetAddressOf(); }
	ID3D11ShaderResourceView* GetSRV() {return  m_TextureSRV.Get(); }



	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture;
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_TextureSRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_TextureDSV;


	std::string m_Name;
private:
	std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> m_TextureRTVs;
	
};