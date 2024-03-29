#pragma once
#include "../LowLevel/DirectX.h"
#include <vector>

class CGameObject;

class LinerOctree
{
private:
	const unsigned int MAX_DIMENSION_LEVEL = 7;
	const unsigned int POWER_NUMBER[8] ={1, 8, 64, 512, 4096, 32768, 262144, 2097152};//8のべき乗

	unsigned int m_DimensionLevel = 2;

	D3DXVECTOR3 m_Min;
	D3DXVECTOR3 m_Max;
	D3DXVECTOR3 m_UnitLen; //最小領域の長さ
	unsigned int m_SpaceNum;

	std::vector<std::vector<CGameObject*>> m_ObjectList;

	// ビット分割関数
	DWORD BitSeparateFor3D(BYTE n)
	{
		DWORD s = n;
		s = (s | s << 8) & 0x0000f00f;
		s = (s | s << 4) & 0x000c30c3;
		s = (s | s << 2) & 0x00249249;
		return s;
	}

	// 3Dモートン空間番号算出関数
	DWORD Get3DMortonNumber(BYTE x, BYTE y, BYTE z)
	{
		return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
	}

	// 座標→線形8分木要素番号変換関数
	DWORD GetPointElem(D3DXVECTOR3 p)
	{
		return Get3DMortonNumber(
			(BYTE)((p.x - m_Min.x) / m_UnitLen.x),
			(BYTE)((p.y - m_Min.y) / m_UnitLen.y),
			(BYTE)((p.z - m_Min.z) / m_UnitLen.z)
		);
	}

public:
	LinerOctree() = delete;
	LinerOctree(int level, D3DXVECTOR3 min, D3DXVECTOR3 max) : m_DimensionLevel(level), m_Min(min), m_Max(max) {};
	~LinerOctree();

	void Init();

	//Calc呼ぶ前に呼ぶ(1回だけでいい)
	void ClearList();
	//update毎に呼ぶ
	void CalcOctree(CGameObject* object);

	std::vector<std::vector<CGameObject*>> GetObjectList() { return m_ObjectList; }
	
};