#pragma once
#include "struct.h"
#include <memory>

class MeshFilter {
private:
	VERTEX_3D* m_VertexArray;
	unsigned int* m_IndexArray;
	unsigned int m_StartIndex;

public:
	MeshFilter() {}
	MeshFilter(std::string modelname);
	~MeshFilter(){}
	
	

	

};