#pragma once
#include "struct.h"
#include <memory>

class Mesh {
private:
	std::vector<VERTEX_3D> m_VertexArray;
	std::vector<std::vector<unsigned int>> m_IndexArray;
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
	void SetVertexArray(std::vector<VERTEX_3D> data) { m_VertexArray = data;}
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

	std::vector<std::vector<unsigned int>> GetIndicesArray() { return m_IndexArray; }
};