#pragma once
#include "../LowLevel/DirectX.h"

class CCollision;

class CCollisionDetection
{



public:
	CCollisionDetection() {};
	virtual ~CCollisionDetection(){};

	virtual bool IsHit(CCollision* obj1, CCollision* obj2) = 0;
};