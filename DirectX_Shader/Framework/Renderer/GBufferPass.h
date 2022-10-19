#pragma once
#include <list>
#include <memory>

class CRenderer;
class CMaterial;

class GBufferPass {
public:

	GBufferPass() {};
	~GBufferPass() {};

	void Draw(std::list<std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>>> gameobject);
};