#pragma once
#include "CCollisionDetection.h"

class CCollision;

class ObbAndObbDetection : public CCollisionDetection
{
private:
	float CalcLenSepAxis(D3DXVECTOR3 *sep,D3DXVECTOR3 *a, D3DXVECTOR3 *b, D3DXVECTOR3 *c = 0);
public:
	ObbAndObbDetection() {};
	~ObbAndObbDetection() {};

	bool IsHit(CCollision* obj1, CCollision* obj2)override;
};
