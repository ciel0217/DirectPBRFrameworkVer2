#include "../Resources/CommonProcess.h"

//ƒQ[ƒ€’†‚Éíœ‚·‚é‚Æ‚«
bool CommonProcess::Destroy()
{
	if (m_IsDestroy) {
		Uninit();
		//delete this;
		return true;
	}

	return false;
}

//Uninit‚Ì‚ÉŒÄ‚ÔBâ‘Îtrue
bool CommonProcess::Release()
{
	delete this;
	return true;
}
