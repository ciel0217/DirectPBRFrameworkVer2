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
	//���`8����
	int count = 0;
	for (auto itr = gameobject.cbegin(); itr != gameobject.cend(); itr++) 
	{
		count++;
		if ((*itr)->GetCollision()) 
		{
			//�v�f��-1�ŏI��
			auto tmp = itr;
			if (++tmp == gameobject.cend())break;

			for(auto it = tmp; it != gameobject.cend(); it++)
			{
				//collision���Ȃ�
				if (!(*it)->GetCollision())continue;

				//�����蔻����Ƃ�͈͂ɂ��Ȃ�
				D3DXVECTOR3 distance = (*it)->GetPosition() - (*itr)->GetPosition();
				float length = D3DXVec3Length(&distance);
				if (length > TARGET_RADIUS)continue;

				//tag�œ����蔻����Ƃ镨�̂�����
				if ((*it)->GetCollision()->GetTagMask() & (*itr)->GetTag())
				{
					if (m_CollisionDetection[(*it)->GetCollision()->GetCollisionType()][(*itr)->GetCollision()->GetCollisionType()]
						->IsHit((*itr)->GetCollision(), (*it)->GetCollision()))
					{
						//�����Hit�ʒm
						(*itr)->GetCollision()->GetSelf()->OnCollisionEnter((*it)->GetCollision()->GetSelf());
						(*it)->GetCollision()->GetSelf()->OnCollisionEnter((*itr)->GetCollision()->GetSelf());
						break;
					}
				}

			}
		}
	}
}

void ManagerCollisionDetection::CollisionCheckByOctree(std::vector<std::vector<CGameObject*>> object_list)
{
	bool is_finish = false;
	bool is_newcomming = true;
	std::vector<CGameObject* > current_space;//�����݂̋��
	std::vector<CGameObject* > collision_stack;//�����蔻�������ʋ�Ԃ̃I�u�W�F�N�g
	collision_stack.reserve(6);//��ɂ�����Ɗm��
	//������root��Ԃ��Z�b�g
	std::copy(object_list[0].begin(), object_list[0].end(), std::back_inserter(current_space));


	DWORD current_elem = 0;
	while (!is_finish)
	{
		if (is_newcomming)
		{
			for (int i = 0; i < current_space.size(); i++)
			{
				CCollision* collision1 = current_space[i]->GetCollision();
				if (!collision1) continue;

				//����ԓ��œ����蔻��
				for (int j = i + 1; j < current_space.size(); j++)
				{
					CCollision* collision2 = current_space[j]->GetCollision();
					if (!collision2) continue;

					//�����蔻��}�X�N
					if (collision1->GetTagMask() & collision2->GetTagMask())
					{
						//�q�b�g
						if (m_CollisionDetection[collision1->GetCollisionType()][collision2->GetCollisionType()]
							->IsHit(collision1, collision2))
						{
							collision1->GetSelf()->OnCollisionEnter(current_space[j]);
							collision2->GetSelf()->OnCollisionEnter(current_space[i]);
						}
					}
				}

				//��ʋ�ԂƂ̓����蔻��
				for (auto object : collision_stack)
				{
					CCollision* collision3 = object->GetCollision();
					if (!collision3) continue;

					//�����蔻��}�X�N
					if (collision1->GetTagMask() & collision3->GetTagMask())
					{
						//�q�b�g
						if (m_CollisionDetection[collision1->GetCollisionType()][collision3->GetCollisionType()]
							->IsHit(collision1, collision3))
						{
							collision1->GetSelf()->OnCollisionEnter(object);
							collision3->GetSelf()->OnCollisionEnter(current_space[i]);
						}
					}
				}

				//�q��Ԃ����邩�ǂ���
				for(int z = 0; z < 8; z++)
				{

				}

				//��A�̏������I�������A������o�^
				collision_stack.push_back(current_space[i]);
			}


		}
	}
}
