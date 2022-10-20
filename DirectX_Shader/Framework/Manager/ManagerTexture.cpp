#include "../Manager/ManagerTexture.h"
#include "../LowLevel/CDxRenderer.h"
#include <sstream>
#include <istream>


std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> ManagerTexture::m_texture;


ID3D11ShaderResourceView * ManagerTexture::LoadTexture(std::string filename)
{
	if(!m_texture.count(filename)) 
	{
		
		m_texture.insert({filename, nullptr});
		
		std::stringstream ss{ filename };
		std::string buf;

		std::vector<std::string> v;

		while (std::getline(ss, buf, '.')) {
			v.push_back(buf);
		}

		std::wstring result = std::wstring(filename.begin(), filename.end());
		LPCWSTR str = result.c_str();
		
		DirectX::ScratchImage image;
		DirectX::TexMetadata metadata;
		HRESULT hr;
		if (v.back() == "hdr") {
			hr = DirectX::LoadFromHDRFile(str, &metadata, image);
		}
		else {
			hr = DirectX::LoadFromWICFile(str, DirectX::WIC_FLAGS_NONE, &metadata, image);
		}

		DirectX::CreateShaderResourceView(CDxRenderer::GetRenderer()->GetDevice(), 
			image.GetImages(), 
			image.GetImageCount(), 
			image.GetMetadata(), 
			&m_texture[filename]);
	}
	return m_texture[filename].Get();
}

ID3D11ShaderResourceView * ManagerTexture::GetTexture(std::string filename)
{
	if (!m_texture.count(filename)) {
		return nullptr;
	}
	else {
		return m_texture[filename].Get();
	}
}

