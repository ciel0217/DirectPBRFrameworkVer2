#include "Quaternion.h"
#include <math.h>

D3DXQUATERNION AngleAxis(float angle, D3DXVECTOR3 axis);

D3DXVECTOR3 Transform(const D3DXVECTOR3 vec, const D3DXQUATERNION qua) {
	float x = qua.x + qua.x;
	float y = qua.y + qua.y;
	float z = qua.z + qua.z;
	float wx = qua.w * x;
	float wy = qua.w * y;
	float wz = qua.w * z;
	float xx = qua.x * x;
	float xy = qua.x * y;
	float xz = qua.x * z;
	float yy = qua.y * y;
	float yz = qua.y * z;
	float zz = qua.z * z;

	return D3DXVECTOR3(
		((vec.x * ((1.0f - yy) - zz)) + (vec.y * (xy - wz))) + (vec.z * (xz + wy)),
		((vec.x * (xy + wz)) + (vec.y * ((1.0f - xx) - zz))) + (vec.z * (yz - wx)),
		((vec.x * (xz - wy)) + (vec.y * (yz + wx))) + (vec.z * ((1.0f - xx) - yy)));
}

D3DXQUATERNION RotateAxis(const D3DXVECTOR3 vec, float rot, const D3DXQUATERNION qua1)
{
	float tx = vec.x;
	float ty = vec.y;
	float tz = vec.z;
	float len = tx * tx + ty * ty + tz * tz;

	// Ž²‚ª³‹K‰»‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î³‹K‰»
	if (len != 1.0f)
	{
		len = sqrt(len);
		tx /= len;
		ty /= len;
		tz /= len;
	}

	float r = rot * 0.5f;

	float sin_r = sinf(r);
	float cos_r = cosf(r);

	D3DXQUATERNION qua2 =  D3DXQUATERNION(
		sin_r * tx,
		sin_r * ty,
		sin_r * tz,
		cos_r);

	float lx = qua1.x;
	float ly = qua1.y;
	float lz = qua1.z;
	float lw = qua1.w;
	D3DXQUATERNION ret = D3DXQUATERNION(
		(qua2.x * lw + lx * qua2.w + qua2.y * lz) - (qua2.z * ly),
		(qua2.y * lw + ly * qua2.w + qua2.z * lx) - (qua2.x * lz),
		(qua2.z * lw + lz * qua2.w + qua2.x * ly) - (qua2.y * lx),
		(qua2.w * lw) - (qua2.x * lx + qua2.y * ly + qua2.z * lz));

	return ret;
}

D3DXQUATERNION AngleAxis(float angle, D3DXVECTOR3 axis)
{
	D3DXVec3Normalize(&axis, &axis);

	float rad = angle * ((D3DX_PI * 2.0f) / 360.0f) * 0.5f;

	axis *= sinf(rad);

	D3DXQUATERNION ret = D3DXQUATERNION(axis.x, axis.y, axis.z, cosf(rad));
	return ret;
}

D3DXQUATERNION FromToRotation(D3DXVECTOR3 from, D3DXVECTOR3 to)
{

	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis, &from, &to);

	float dot = D3DXVec3Dot(&from, &to);
	float from_length = D3DXVec3Length(&from);
	float to_length = D3DXVec3Length(&to);

	float angle = acosf(dot / (from_length * to_length)) * (180.0f / D3DX_PI);

	if (angle >= 179.9196f) {
		D3DXVECTOR3 r;
		D3DXVec3Cross(&r, &from, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		
		D3DXVec3Cross(&axis, &r, &from);

		if (D3DXVec3LengthSq(&axis) < 0.000001f)
			axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	D3DXVec3Normalize(&axis, &axis);


	D3DXQUATERNION ret = AngleAxis(angle, axis);

	return ret;
}

D3DXVECTOR3 QuaXVec3(D3DXQUATERNION qua, D3DXVECTOR3 vec)
{
	float x = qua.x * 2.0f;
	float y = qua.y * 2.0f;
	float z = qua.z * 2.0f;
	float xx = qua.x * x;
	float yy = qua.y * y;
	float zz = qua.z * z;
	float xy = qua.x * y;
	float xz = qua.x * z;
	float yz = qua.y * z;
	float wx = qua.w * x;
	float wy = qua.w * y;
	float wz = qua.w * z;

	D3DXVECTOR3 ret;
	ret.x =(1.0f - (yy + zz)) * vec.x + (xy - wz) * vec.y + (xz + wy) * vec.z;
	ret.y = (xy + wz) * vec.x + (1.0f - (xx + zz)) * vec.y + (yz - wx) * vec.z;
	ret.z = (xz - wy) * vec.x + (yz + wx) * vec.y + (1.0f - (xx + yy)) * vec.z;


	return ret;
}

