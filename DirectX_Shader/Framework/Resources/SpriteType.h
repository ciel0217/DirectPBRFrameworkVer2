#pragma once
#include "../LowLevel/DirectX.h"
#include <vector>
#include "struct.h"

class SpriteType {
protected:
	std::vector<VERTEX_3D> m_Verticies;
	std::vector<UINT> m_Indices;
	D3DXVECTOR2 m_Uv;


public:
	SpriteType() {}
	virtual ~SpriteType(){}

	std::vector<VERTEX_3D> GetVerticies() { return m_Verticies; }
	std::vector<UINT> GetIndices() { return m_Indices; }
};