#pragma once
#include "../LowLevel/DirectX.h"
#include "../Resources/DevelopStruct.h"

class CDxRenderer;
class CBuffer;
class Sprite2D {
private:
	static Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	static CBuffer * m_MatrixCBuffer;
	static CBuffer * m_MaterialCBuffer;

public :

	static void InitSprite();
	static void Draw2D(ID3D11ShaderResourceView* texture, D3DXVECTOR3 pos, D3DXVECTOR3 wh, D3DXCOLOR color, D3DXVECTOR2 uv, D3DXVECTOR2 uv_wh);
	static void Draw2D(D3DXVECTOR3 pos, D3DXVECTOR3 wh, D3DXCOLOR color, D3DXVECTOR2 uv, D3DXVECTOR2 uv_wh);
	static void DrawBillboard(ID3D11ShaderResourceView* texture, D3DXVECTOR3 pos, D3DXVECTOR3 wh, D3DXCOLOR color, 
		D3DXVECTOR2 uv, D3DXVECTOR2 uv_wh, MATERIAL_CBUFFER material);

};