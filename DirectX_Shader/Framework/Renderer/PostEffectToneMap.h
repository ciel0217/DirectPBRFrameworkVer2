#pragma once
#include "CPostEffect.h"
#include <vector>
#include "struct.h"

class PostEffectToneMap : public CPostEffect {
private:
	/*std::vector<VERTEX_3D> m_MeshData;
	std::vector<UINT>	   m_IndexData;*/


	//Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override {};
	void Draw()override;
};