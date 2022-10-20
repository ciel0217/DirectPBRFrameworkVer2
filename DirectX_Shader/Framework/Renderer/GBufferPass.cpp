#include "GBufferPass.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/CRenderer.h"
#include "CMaterial.h"


void GBufferPass::Draw(std::list<std::tuple<CRenderer*, unsigned int, std::shared_ptr<CMaterial>>> gameobject)
{
	for (auto obj : gameobject) {
		std::get<0>(obj)->Draw(std::get<1>(obj));
	}
}
