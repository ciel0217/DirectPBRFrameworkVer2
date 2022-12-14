#pragma once
#include <memory>
#include <string>
#include "../LowLevel/DirectX.h"
#include "../Resources/CMaterial.h"
#include "../Resources/CBuffer.h"
#include <vector>

class CGameObject;

class CRenderer {
protected:
	bool m_IsActive = true;
	bool m_IsVisible;
	//マテリアル
	std::vector<unsigned int> m_MaterialIds;
	CGameObject* m_Self;//thisの時どうすればええんや...

	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_VertexBuffer;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_IndexBuffer;

public:
	static std::unique_ptr<CBuffer> m_MaterialCBuffer;
	static std::unique_ptr<CBuffer> m_WorldMatrixCBuffer;
	static std::unique_ptr<CBuffer> m_InverseWorldMatrixCBuffer;


	static void SetUpRenderer();

	CRenderer() {}
	virtual ~CRenderer() {}

	virtual void Draw(unsigned int index) = 0;

	std::vector<unsigned int> GetMaterialIds() { return m_MaterialIds; }
};