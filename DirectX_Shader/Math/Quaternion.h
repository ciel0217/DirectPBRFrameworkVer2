#pragma once
#include "../Framework/LowLevel/DirectX.h"

D3DXVECTOR3 Transform(const D3DXVECTOR3, const D3DXQUATERNION);

D3DXQUATERNION RotateAxis(const D3DXVECTOR3, float, const D3DXQUATERNION);


D3DXQUATERNION FromToRotation(D3DXVECTOR3 from, D3DXVECTOR3 to);