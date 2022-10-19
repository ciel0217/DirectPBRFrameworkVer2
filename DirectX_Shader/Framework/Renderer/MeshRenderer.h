#pragma once
#include "CRenderer.h"

class Mesh;
class CBuffer;

class MeshRenderer : public CRenderer {
protected:
	std::string m_ModelName;
	//mesh‚ÆmaterialID‚Ìpair
	std::shared_ptr<Mesh> m_ModelMesh;

private:
	void CalcBuffer();

public:
	MeshRenderer(){}
	~MeshRenderer(){}
	
	void AddMaterial(unsigned int material_id);
	bool ChangeMaterial(unsigned int index, unsigned int change_material_id);
	void DeleteMaterial(unsigned int index);


	void SetUpModel(std::string model_name, CGameObject* self);
	void SetMeshAndMaterial(Mesh* mesh, std::vector<unsigned int> material_ids);
	void SetUpMaterial(std::string texture_name, std::string shader_name, std::string material_name)override;

	void Draw(unsigned int index)override;
};