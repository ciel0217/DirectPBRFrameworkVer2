#include "CPostEffect.h"

bool CPostEffect::Release()
{
	delete this;
	return true;
}
