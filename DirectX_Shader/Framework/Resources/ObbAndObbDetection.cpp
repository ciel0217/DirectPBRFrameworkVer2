#include "ObbAndObbDetection.h"
#include "ObbCollision.h"


float ObbAndObbDetection::CalcLenSepAxis(D3DXVECTOR3 * sep, D3DXVECTOR3 * a, D3DXVECTOR3 * b, D3DXVECTOR3 * c)
{
	float r1 = fabsf(D3DXVec3Dot(sep, a));
	float r2 = fabsf(D3DXVec3Dot(sep, b));
	float r3 = c ? (fabsf(D3DXVec3Dot(sep, c))) : 0;

	return r1 + r2 + r3;
}

bool ObbAndObbDetection::IsHit(CCollision * obj1, CCollision * obj2)
{
	ObbCollision* obb1 = (ObbCollision*)obj1;
	ObbCollision* obb2 = (ObbCollision*)obj2;

	D3DXVECTOR3 x_unit_axis1 = obb1->GetAxisX();
	D3DXVECTOR3 x_unit_axis2 = obb2->GetAxisX();
	D3DXVECTOR3 y_unit_axis1 = obb1->GetAxisY();
	D3DXVECTOR3 y_unit_axis2 = obb2->GetAxisY();
	D3DXVECTOR3 z_unit_axis1 = obb1->GetAxisZ();
	D3DXVECTOR3 z_unit_axis2 = obb2->GetAxisZ();

	D3DXVECTOR3 size1 = obb1->GetObbSize();
	D3DXVECTOR3 size2 = obb2->GetObbSize();

	D3DXVECTOR3 interval = obb1->GetCenterPosition() - obb2->GetCenterPosition();

	D3DXVECTOR3 x_axis1 = x_unit_axis1 * size1.x;
	D3DXVECTOR3 x_axis2 = x_unit_axis2 * size2.x;
	D3DXVECTOR3 y_axis1 = y_unit_axis1 * size1.y;
	D3DXVECTOR3 y_axis2 = y_unit_axis2 * size2.y;
	D3DXVECTOR3 z_axis1 = z_unit_axis1 * size1.z;
	D3DXVECTOR3 z_axis2 = z_unit_axis2 * size2.z;

	float ra, rb, l;

	//•ª—£²:x•ûŒü(obb1)
	ra = D3DXVec3Length(&x_axis1);
	rb = CalcLenSepAxis(&x_unit_axis1, &x_axis2, &y_axis2, &z_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &x_unit_axis1));
	if (l > ra + rb)return false;

	//•ª—£²:y•ûŒü(obb1)
	ra = D3DXVec3Length(&y_axis1);
	rb = CalcLenSepAxis(&y_unit_axis1, &x_axis2, &y_axis2, &z_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &y_unit_axis1));
	if (l > ra + rb)return false;

	//•ª—£²:z•ûŒü(obb1)
	ra = D3DXVec3Length(&z_axis1);
	rb = CalcLenSepAxis(&z_unit_axis1, &x_axis2, &y_axis2, &z_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &z_unit_axis1));
	if (l > ra + rb)return false;

	//•ª—£²:x•ûŒü(obb2)
	ra = CalcLenSepAxis(&x_unit_axis2, &x_axis1, &y_axis1, &z_axis1);
	rb = D3DXVec3Length(&x_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &x_unit_axis2));
	if (l > ra + rb)return false;

	//•ª—£²:y•ûŒü(obb2)
	ra = CalcLenSepAxis(&y_unit_axis2, &x_axis1, &y_axis1, &z_axis1);
	rb = D3DXVec3Length(&y_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &y_unit_axis2));
	if (l > ra + rb)return false;

	//•ª—£²:z•ûŒü(obb2)
	ra = CalcLenSepAxis(&z_unit_axis2, &x_axis1, &y_axis1, &z_axis1);
	rb = D3DXVec3Length(&z_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &z_unit_axis2));
	if (l > ra + rb)return false;

	D3DXVECTOR3 cross;

	//•ª—£²:x•ûŒü(‚’¼¬•ª(obb1_x, obb2_x))
	D3DXVec3Cross(&cross, &x_unit_axis1, &x_unit_axis2);
	ra = CalcLenSepAxis(&cross, &y_axis1, &z_axis1);
	rb = CalcLenSepAxis(&cross, &y_axis2, &z_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &cross));
	if (l > ra + rb)return false;

	//•ª—£²:y•ûŒü(‚’¼¬•ª(obb1_x, obb2_y))
	D3DXVec3Cross(&cross, &x_unit_axis1, &y_unit_axis2);
	ra = CalcLenSepAxis(&cross, &y_axis1, &z_axis1);
	rb = CalcLenSepAxis(&cross, &x_axis2, &z_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &cross));
	if (l > ra + rb)return false;

	//•ª—£²:z•ûŒü(‚’¼¬•ª(obb1_x, obb2_z))
	D3DXVec3Cross(&cross, &x_unit_axis1, &z_unit_axis2);
	ra = CalcLenSepAxis(&cross, &y_axis1, &z_axis1);
	rb = CalcLenSepAxis(&cross, &x_axis2, &y_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &cross));
	if (l > ra + rb)return false;

	//•ª—£²:x•ûŒü(‚’¼¬•ª(obb1_y, obb2_x))
	D3DXVec3Cross(&cross, &y_unit_axis1, &x_unit_axis2);
	ra = CalcLenSepAxis(&cross, &x_axis1, &z_axis1);
	rb = CalcLenSepAxis(&cross, &y_axis2, &z_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &cross));
	if (l > ra + rb)return false;

	//•ª—£²:y•ûŒü(‚’¼¬•ª(obb1_y, obb2_y))
	D3DXVec3Cross(&cross, &y_unit_axis1, &y_unit_axis2);
	ra = CalcLenSepAxis(&cross, &x_axis1, &z_axis1);
	rb = CalcLenSepAxis(&cross, &x_axis2, &z_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &cross));
	if (l > ra + rb)return false;

	//•ª—£²:y•ûŒü(‚’¼¬•ª(obb1_y, obb2_z))
	D3DXVec3Cross(&cross, &y_unit_axis1, &z_unit_axis2);
	ra = CalcLenSepAxis(&cross, &x_axis1, &z_axis1);
	rb = CalcLenSepAxis(&cross, &x_axis2, &y_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &cross));
	if (l > ra + rb)return false;

	//•ª—£²:x•ûŒü(‚’¼¬•ª(obb1_z, obb2_x))
	D3DXVec3Cross(&cross, &z_unit_axis1, &x_unit_axis2);
	ra = CalcLenSepAxis(&cross, &x_axis1, &y_axis1);
	rb = CalcLenSepAxis(&cross, &y_axis2, &z_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &cross));
	if (l > ra + rb)return false;

	//•ª—£²:y•ûŒü(‚’¼¬•ª(obb1_z, obb2_y))
	D3DXVec3Cross(&cross, &z_unit_axis1, &y_unit_axis2);
	ra = CalcLenSepAxis(&cross, &x_axis1, &y_axis1);
	rb = CalcLenSepAxis(&cross, &x_axis2, &z_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &cross));
	if (l > ra + rb)return false;

	//•ª—£²:y•ûŒü(‚’¼¬•ª(obb1_z, obb2_z))
	D3DXVec3Cross(&cross, &z_unit_axis1, &z_unit_axis2);
	ra = CalcLenSepAxis(&cross, &x_axis1, &y_axis1);
	rb = CalcLenSepAxis(&cross, &x_axis2, &y_axis2);
	l = fabsf(D3DXVec3Dot(&interval, &cross));
	if (l > ra + rb)return false;

	return true;
}
