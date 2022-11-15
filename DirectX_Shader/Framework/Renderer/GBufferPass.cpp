#include "GBufferPass.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Renderer/CRenderer.h"
#include "../Resources/CMaterial.h"


void GBufferPass::Draw(std::list<std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>>> gameobject)
{
	for (auto obj : gameobject) {
		std::get<0>(obj)->Draw(std::get<1>(obj));
	}
}
