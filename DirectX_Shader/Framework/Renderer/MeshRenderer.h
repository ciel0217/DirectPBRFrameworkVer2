#pragma once
#include "../Renderer/CRenderer.h"

class Mesh;
class CBuffer;
class CMaterial;

class MeshRenderer : public CRenderer {
protected:
	std::string m_ModelName;
	std::shared_ptr<Mesh> m_ModelMesh;

private:
	void CalcBuffer();

public:
	MeshRenderer(){}
	~MeshRenderer();
	
	void AddMaterial(CMaterial* material);
	bool ChangeMaterial(unsigned int index, CMaterial* material);
	void DeleteMaterial(unsigned int index);

	D3DXVECTOR3 GetBounds()override;

	void SetUpModel(std::string model_name, std::vector<std::string> material_name);
	void SetMeshAndMaterial(Mesh* mesh, std::vector<CMaterial*> materials );

	void Draw(unsigned int index)override;
};