#pragma once
#include "CCollisionDetection.h"

class CCollision;

class BoxAndBoxDetection : public CCollisionDetection
{

public:
	BoxAndBoxDetection() {};
	~BoxAndBoxDetection() {};

	bool IsHit(CCollision* obj1, CCollision* obj2)override;
};