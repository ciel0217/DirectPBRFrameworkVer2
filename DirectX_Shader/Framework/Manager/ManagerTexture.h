#pragma once
#include "DirectX.h"
#include <map>
#include <string>

class CDxRenderer;

class ManagerTexture {
private:
	static std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;


public:
	
	static ID3D11ShaderResourceView *LoadTexture(std::string);
	static ID3D11ShaderResourceView *GetTexture(std::string);
	
};