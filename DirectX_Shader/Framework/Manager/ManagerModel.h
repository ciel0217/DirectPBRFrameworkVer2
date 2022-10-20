#pragma once
#include "../LowLevel/DirectX.h"
#include "struct.h"
#include <string>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>


#define NORMAL_MAP 1
#define BUMP_MAP 2

#define NORMAL_UNUSED 0

class Mesh;
// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL_CBUFFER			Material;
	char						TextureName[256];
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;
	unsigned int	*IndexArray;
	unsigned int	IndexNum;
	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};





struct DX11_MODEL
{
	Mesh* ModelMesh;
	std::vector<unsigned int> MaterialNums;
};

class CBuffer;

class ManagerModel {
private:
	static std::unordered_map <std::string, std::pair<std::shared_ptr<Mesh>, std::vector<unsigned int>>> m_ModelList;
	static CBuffer* m_ModelCBuffer;

	static void LoadObj(const char* FileName, MODEL *Model);
	static void LoadMaterial(char* File, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum);
public:
	static void Init();
	static std::pair<std::shared_ptr<Mesh>, std::vector<unsigned int>> Load(std::string file);
	static void Draw(DX11_MODEL *Model);
	static void Unload();
};

