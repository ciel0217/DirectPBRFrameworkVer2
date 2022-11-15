#pragma once
#include "../Renderer/CRenderer.h"
#include "../LowLevel/DirectX.h"

class SpriteRenderer : public CRenderer {
private:
	D3DXCOLOR m_TextureColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	std::string m_TextureName;
	int m_IndexSize;
	//TODO:pivot‚Æ‚©offset‚Æ‚©‚»‚¤‚¢‚¤‚Ì’Ç‰Á
public:
	SpriteRenderer(){}
	~SpriteRenderer();

	void SetUpSprite(CGameObject* self);
	void SetUpMaterial(std::string texture_name, std::string shader_name, std::string material_name)override;
	void Draw(unsigned int index)override;
	void SetTextureColor(D3DXCOLOR color) { m_TextureColor = color; }
};