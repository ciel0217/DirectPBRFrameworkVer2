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
	//線形8分木
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
