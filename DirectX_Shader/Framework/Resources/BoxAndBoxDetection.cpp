#include "../Resources/BoxAndBoxDetection.h"
#include "BoxCollision.h"

bool BoxAndBoxDetection::IsHit(CCollision * obj1, CCollision * obj2)
{
	//TODO:OBB‚ÅŽÀ‘•‚µ‚È‚¨‚µ‚½‚¢
	BoxCollision* box1 = (BoxCollision*)obj1;
	BoxCollision* box2 = (BoxCollision*)obj2;

	D3DXVECTOR3 pos1 = box1->GetCollisionCenterPosition();
	D3DXVECTOR3 pos2 = box2->GetCollisionCenterPosition();

	D3DXVECTOR3 size1 = box1->GetCollisionBoxSize();
	D3DXVECTOR3 size2 = box2->GetCollisionBoxSize();

	float Axmax = pos1.x + (size1.x / 2);
	float Axmin = pos1.x - (size1.x / 2);
	float Aymax = pos1.y + (size1.y / 2);
	float Aymin = pos1.y - (size1.y / 2);
	float Azmax = pos1.z + (size1.z / 2);
	float Azmin = pos1.z - (size1.z / 2);

	float Bxmax = pos2.x + (size2.x / 2);
	float Bxmin = pos2.x - (size2.x / 2);
	float Bymax = pos2.y + (size2.y / 2);
	float Bymin = pos2.y - (size2.y / 2);
	float Bzmax = pos2.z + (size2.z / 2);
	float Bzmin = pos2.z - (size2.z / 2);

	if ((Axmax > Bxmin) && (Axmin < Bxmax))
	{
		if ((Aymin < Bymax) && (Aymax > Bymin))
		{
			if ((Azmax > Bzmin) && (Azmin < Bzmax))
			{
				return true;
			}
		}
	}

	return false;
}
