#include "../Manager/ManagerCollsionDetection.h"
#include "../Resources/CommonProcess.h"
#include "../Resources/CCollision.h"
#include "../Resources/CGameObject.h"
#include "../Resources/BoxAndBoxDetection.h"

#define TARGET_RADIUS 200.0f

ManagerCollisionDetection* ManagerCollisionDetection::_instance = nullptr;



ManagerCollisionDetection::ManagerCollisionDetection()
{
	m_CollisionDetection[eBoxCollision][eBoxCollision] = new BoxAndBoxDetection();
}

ManagerCollisionDetection::~ManagerCollisionDetection()
{
	for (int i = 0; i < eCollisionTypeMax; i++) {
		for (int j = 0; j < eCollisionTypeMax; j++) {
			delete m_CollisionDetection[i][j];
		}
	}
}

void ManagerCollisionDetection::CollisionCheck(std::list<CGameObject*> gameobject)
{
	int count = 0;
	for (auto itr = gameobject.cbegin(); itr != gameobject.cend(); itr++) 
	{
		count++;
		if ((*itr)->GetCollision()) 
		{
			//要素数-1で終了
			auto tmp = itr;
			if (++tmp == gameobject.cend())break;

			for(auto it = tmp; it != gameobject.cend(); it++)
			{
				//collisionがない
				if (!(*it)->GetCollision())continue;

				//当たり判定をとる範囲にいない
				D3DXVECTOR3 distance = (*it)->GetPosition() - (*itr)->GetPosition();
				float length = D3DXVec3Length(&distance);
				if (length > TARGET_RADIUS)continue;

				//tagで当たり判定をとる物体か判定
				if ((*it)->GetCollision()->GetTagMask() & (*itr)->GetTag())
				{
					if (m_CollisionDetection[(*it)->GetCollision()->GetCollisionType()][(*itr)->GetCollision()->GetCollisionType()]
						->IsHit((*itr)->GetCollision(), (*it)->GetCollision()))
					{
						//相手にHit通知
						(*itr)->GetCollision()->GetSelf()->OnCollisionEnter((*it)->GetCollision()->GetSelf());
						(*it)->GetCollision()->GetSelf()->OnCollisionEnter((*itr)->GetCollision()->GetSelf());
						break;
					}
				}

			}
		}
	}
}

//線形8分木を用いた当たり判定
void ManagerCollisionDetection::CollisionCheck(std::vector<std::vector<CGameObject*>> object_list)
{

	std::vector<CGameObject* > collision_stack;//当たり判定を取る上位空間のオブジェクト
	collision_stack.reserve(6);//先にちょっと確保
	m_ObjectListByOctree = object_list;

	DWORD current_elem = 0;
	CalcOctreeCollisionList(m_ObjectListByOctree[0], collision_stack, current_elem);

}


bool ManagerCollisionDetection::CalcOctreeCollisionList(std::vector<CGameObject*>& current_space, std::vector<CGameObject*>& collision_stack, DWORD elem)
{
	
	for (int i = 0; i < current_space.size(); i++)
	{
		CCollision* collision1 = current_space[i]->GetCollision();
		if (!collision1) continue;
		//同空間内で当たり判定
		for (int j = i + 1; j < current_space.size(); j++)
		{
			CCollision* collision2 = current_space[j]->GetCollision();
			if (!collision2) continue;

			//当たり判定マスク
			if (collision1->GetTagMask() & collision2->GetTagMask())
			{
				//ヒット
				if (m_CollisionDetection[collision1->GetCollisionType()][collision2->GetCollisionType()]
					->IsHit(collision1, collision2))
				{
					collision1->GetSelf()->OnCollisionEnter(current_space[j]);
					collision2->GetSelf()->OnCollisionEnter(current_space[i]);
				}
			}
		}

		//上位空間との当たり判定
		for (auto object : collision_stack)
		{
			CCollision* collision3 = object->GetCollision();
			if (!collision3) continue;

			//当たり判定マスク
			if (collision1->GetTagMask() & collision3->GetTagMask())
			{
				//ヒット
				if (m_CollisionDetection[collision1->GetCollisionType()][collision3->GetCollisionType()]
					->IsHit(collision1, collision3))
				{
					collision1->GetSelf()->OnCollisionEnter(object);
					collision3->GetSelf()->OnCollisionEnter(current_space[i]);
				}
			}
		}
	}

	bool once = false;
	//子空間があるかどうか
	for (int i = 0; i < 8; i++)
	{
		DWORD next_elem = elem * 8 + 1 + i;
		std::vector<CGameObject*> next_space = m_ObjectListByOctree[next_elem];

		if (next_space.size() != 0)
		{
			//子空間がある場合は一度だけstackに詰む処理をする
			if (!once)
			{
				for (auto object : current_space)
				{
					collision_stack.push_back(object);
				}
			}
			once = true;
			CalcOctreeCollisionList(m_ObjectListByOctree[next_elem], collision_stack, next_elem);
		}

	}

	//自分が持っている子空間全部終わったら、Stackに詰んである自身の空間のオブジェクトを削除
	if (once)
	{
		for (int i = 0; i < current_space.size(); i++)
		{
			collision_stack.pop_back();
		}
	}

	return true;
}