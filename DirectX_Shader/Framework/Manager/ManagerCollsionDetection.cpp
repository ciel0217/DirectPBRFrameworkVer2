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

//���`8���؂�p���������蔻��
void ManagerCollisionDetection::CollisionCheck(std::vector<std::vector<CGameObject*>> object_list)
{

	std::vector<CGameObject* > collision_stack;//�����蔻�������ʋ�Ԃ̃I�u�W�F�N�g
	collision_stack.reserve(6);//��ɂ�����Ɗm��
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
	}

	bool once = false;
	//�q��Ԃ����邩�ǂ���
	for (int i = 0; i < 8; i++)
	{
		DWORD next_elem = elem * 8 + 1 + i;
		std::vector<CGameObject*> next_space = m_ObjectListByOctree[next_elem];

		if (next_space.size() != 0)
		{
			//�q��Ԃ�����ꍇ�͈�x����stack�ɋl�ޏ���������
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

	//�����������Ă���q��ԑS���I�������AStack�ɋl��ł��鎩�g�̋�Ԃ̃I�u�W�F�N�g���폜
	if (once)
	{
		for (int i = 0; i < current_space.size(); i++)
		{
			collision_stack.pop_back();
		}
	}

	return true;
}