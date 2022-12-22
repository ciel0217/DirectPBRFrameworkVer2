#pragma once
#include "../Renderer/CRenderer.h"

class Mesh;
class CBuffer;
class CMaterial;

class MeshRenderer : public CRenderer {
protected:
	std::string m_ModelName;
	//mesh‚ÆmaterialID‚Ìpair
	std::shared_ptr<Mesh> m_ModelMesh;
	std::vector<CMaterial*> m_Material;

private:
	void CalcBuffer();
	void SetMaterialCopy(unsigned int);

public:
	MeshRenderer(){}
	~MeshRenderer();
	
	void AddMaterial(unsigned int material_id);
	bool ChangeMaterial(unsigned int index, unsigned int change_material_id);
	void DeleteMaterial(unsigned int index);


	void SetUpModel(std::string model_name, CGameObject* self);
	void SetMeshAndMaterial(Mesh* mesh, std::vector<unsigned int> material_ids);

	void Draw(unsigned int index)override;
};