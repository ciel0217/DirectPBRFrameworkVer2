#pragma once
#include "../LowLevel/DirectX.h"
#include <vector>

class CGameObject;
class LinerOctree{

private:
	const unsigned int MAX_DIMENSION_LEVEL = 7;
	unsigned int m_DimensionLevel = 2;

	D3DXVECTOR3 m_Min;
	D3DXVECTOR3 m_Max;
	D3DXVECTOR3 m_UnitLen; //�ŏ��̈�̒���
	
	std::vector<std::vector<CGameObject*>> m_ObjectList;

public:
	LinerOctree() = delete;
	LinerOctree(int level, D3DXVECTOR3 min, D3DXVECTOR3 max) : m_DimensionLevel(level), m_Min(min), m_Max(max) {};

	void Init();

	std::vector<std::vector<CGameObject*>> CalcOctree();

	// �r�b�g�����֐�
	DWORD BitSeparateFor3D(BYTE n)
	{
		DWORD s = n;
		s = (s | s << 8) & 0x0000f00f;
		s = (s | s << 4) & 0x000c30c3;
		s = (s | s << 2) & 0x00249249;
		return s;
	}

	// 3D���[�g����Ԕԍ��Z�o�֐�
	DWORD Get3DMortonNumber(BYTE x, BYTE y, BYTE z)
	{
		return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
	}

	// ���W�����`8���ؗv�f�ԍ��ϊ��֐�
	DWORD GetPointElem(D3DXVECTOR3 p)
	{
		return Get3DMortonNumber(
			(BYTE)((p.x - m_Min.x) / m_UnitLen.x),
			(BYTE)((p.y - m_Min.y) / m_UnitLen.y),
			(BYTE)((p.z - m_Min.z) / m_UnitLen.z)
		);
	}
};