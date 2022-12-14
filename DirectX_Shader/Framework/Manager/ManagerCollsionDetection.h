#pragma once
#include "../Resources/DevelopEnum.h"
#include "../Resources/CCollisionDetection.h"
#include <list>


class CGameObject;

class ManagerCollisionDetection
{
private:
	static ManagerCollisionDetection* _instance;
	CCollisionDetection* m_CollisionDetection[eCollisionTypeMax][eCollisionTypeMax];
	std::vector<std::vector<CGameObject*>> m_ObjectListByOctree;


	bool CalcOctreeCollisionList(std::vector<CGameObject*> &current_space ,std::vector<CGameObject*> &collision_stack, DWORD elem);

public:
	ManagerCollisionDetection();
	~ManagerCollisionDetection();

	static void Create() 
	{
		if (!_instance) {
			_instance = new ManagerCollisionDetection();
		}
	}
	
	static void Release()
	{
		delete _instance;
	}

	static ManagerCollisionDetection* GetInstance() { return _instance; }

	void CollisionCheck(std::list<CGameObject*> gameobject);
	void CollisionCheck(std::vector<std::vector<CGameObject*>> object_list);
};