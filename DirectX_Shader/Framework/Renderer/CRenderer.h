#pragma once
#include <memory>
#include <string>
#include "DirectX.h"
#include "CMaterial.h"
#include "CBuffer.h"
#include <vector>

class CGameObject;

class CRenderer {
protected:
	bool m_IsActive = true;
	bool m_IsVisible;
	//ƒ}ƒeƒŠƒAƒ‹
	std::vector<unsigned int> m_MaterialIds;
	CGameObject* m_Self;//this‚ÌŽž‚Ç‚¤‚·‚ê‚Î‚¦‚¦‚ñ‚â...

	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_VertexBuffer;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_IndexBuffer;

public:
	static std::unique_ptr<CBuffer> m_MaterialCBuffer;
	static std::unique_ptr<CBuffer> m_WorldMatrixCBuffer;


	static void SetUpRenderer();

	CRenderer() {}
	virtual ~CRenderer() {}

	virtual void SetUpMaterial(std::string name, std::string shader_name, std::string material_name) = 0;
	virtual void Draw(unsigned int index) = 0;

	std::vector<unsigned int> GetMaterialIds() { return m_MaterialIds; }
};