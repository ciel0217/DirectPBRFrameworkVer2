#pragma once
#include "../Resources/DevelopStruct.h"
#include <memory>
#include <algorithm>

class Mesh {
private:
	std::vector<VERTEX_3D> m_VertexArray;
	std::vector<std::vector<unsigned int>> m_IndexArray;

	D3DXVECTOR3 m_Bounds = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//std::map<unsigned int, std::vector<unsigned int>> m_IndexArray;
	//std::vector<std::vector<unsigned int>> m_IndexArray;
	//std::shared_ptr<unsigned int**> m_IndexArray;
	//std::shared_ptr<unsigned int[]> m_StartIndex;
	//std::shared_ptr<unsigned int[]> m_SubMeshIndexNum;
	unsigned int m_SubMeshCount;
	

public:
	Mesh(){}
	~Mesh() {}

	void SetSubMeshCount(unsigned int count) { 
		m_SubMeshCount = count; 
		m_IndexArray.resize(count);
		//m_IndexArray = std::make_shared<unsigned int>(new unsigned int*[count]);
	}
	void SetVertexArray(std::vector<VERTEX_3D> data) 
	{ 
		m_VertexArray = data;

		auto x_bounds = std::minmax_element(m_VertexArray.begin(), m_VertexArray.end(),
			[](const VERTEX_3D& a, const VERTEX_3D& b) { return a.Position.x < b.Position.x; });
		auto y_bounds = std::minmax_element(m_VertexArray.begin(), m_VertexArray.end(),
			[](const VERTEX_3D& a, const VERTEX_3D& b) { return a.Position.y < b.Position.y; });
		auto z_bounds = std::minmax_element(m_VertexArray.begin(), m_VertexArray.end(),
			[](const VERTEX_3D& a, const VERTEX_3D& b) { return a.Position.z < b.Position.z; });

		D3DXVECTOR3 min = D3DXVECTOR3(x_bounds.first->Position.x, y_bounds.first->Position.y, z_bounds.first->Position.z);
		D3DXVECTOR3 max = D3DXVECTOR3(x_bounds.second->Position.x, y_bounds.second->Position.y, z_bounds.second->Position.z);

		m_Bounds = max - min;
	
	}
	bool SetIndexArray(std::vector<unsigned int> data, unsigned int subset) { 
		if (subset >= m_SubMeshCount)return false;
		m_IndexArray[subset] = data;

		return true;
	}

	void CalcNormal();

	std::vector<VERTEX_3D> GetVertexArray() { return m_VertexArray; }
	std::vector<unsigned int> GetIndexArray(unsigned int subset) { 
		return m_IndexArray[subset];
	}

	D3DXVECTOR3 GetBounds() { return m_Bounds; }

	std::vector<std::vector<unsigned int>> GetIndicesArray() { return m_IndexArray; }
};