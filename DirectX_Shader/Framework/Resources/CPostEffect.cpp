#include "../Resources/CPostEffect.h"

bool CPostEffect::Release()
{
	delete this;
	return true;
}
