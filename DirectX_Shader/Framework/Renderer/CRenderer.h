#pragma once
#include <memory>
#include <string>
#include "../LowLevel/DirectX.h"
#include "../Resources/CMaterial.h"
#include "../Resources/CBuffer.h"
#include <vector>
#include "../Resources/CGameObject.h"

class CGameObject;
class CMaterial;

class CRenderer {
protected:
	bool m_IsActive = true;
	bool m_IsVisible;
	//ƒ}ƒeƒŠƒAƒ‹
	std::vector<CMaterial*> m_Materials;
	CGameObject* m_Self;//this‚ÌŽž‚Ç‚¤‚·‚ê‚Î‚¦‚¦‚ñ‚â...

	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_VertexBuffer;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_IndexBuffer;

public:
	static std::unique_ptr<CBuffer> m_MaterialCBuffer;
	static std::unique_ptr<CBuffer> m_WorldCBuffer;


	static void SetUpRenderer();

	CRenderer(){}
	virtual ~CRenderer() {}

	virtual void Draw(unsigned int index) = 0;
	virtual D3DXVECTOR3 GetBounds() { return D3DXVECTOR3(1.0f, 1.0f, 1.0f); }

	std::vector<CMaterial*> GetMaterials() { return m_Materials; }
	void SetSelf(CGameObject* self) { m_Self = self; }
	CGameObject* GetSelf() { return m_Self; }
};