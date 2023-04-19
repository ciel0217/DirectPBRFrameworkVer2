#include "CGameObject.h"
#include "../LowLevel/CDxRenderer.h"
#include "Sprite.h"
#include "../Manager/ManagerTexture.h"
#include "../Manager/ManagerModel.h"
#include "../Resources/CShader.h"
#include "CBuffer.h"


bool CGameObject::Destroy()
{
	return m_IsDestroy;
}

bool CGameObject::Release()
{
	return true;
}
