#pragma once
#include "../LowLevel/DirectX.h"
#include "../Resources/DevelopStruct.h"
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


class CBuffer;

class ManagerModel {
private:
	static std::unordered_map <std::string, std::shared_ptr<Mesh>> m_ModelList;
	static CBuffer* m_ModelCBuffer;

	static void LoadObj(const char* FileName, MODEL *Model);
	static void LoadMaterial(char* File, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum);
public:
	static void Init();
	static std::shared_ptr<Mesh> Load(std::string file);
	static void Unload();
};

