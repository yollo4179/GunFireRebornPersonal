#include "Collider_Manager.h"

CCollider_Manager::CCollider_Manager()
{
}

HRESULT CCollider_Manager::Initialize()
{
	return S_OK;
}

HRESULT CCollider_Manager::Add_Collision_Group(COLLISION_GROUP eMyGroup, CGameObject* pGameObject, CCollider* pCollider)
{
	if (COLLISION_END <= eMyGroup)
	{
		MSG_BOX("Exceeded The Collision Group Range");
		return E_FAIL;
	}

	if (nullptr == pGameObject)
	{
		MSG_BOX(" You need to Add GameObject To Make Pair");
		return E_FAIL;
	}
	if (nullptr == pCollider)
	{
		MSG_BOX(" You need to Add Collider To Make Pair");
		return E_FAIL;
	}

	m_CollisionPairList[eMyGroup].push_back(make_pair(pGameObject, pCollider));
	Safe_AddRef(pCollider);
	Safe_AddRef(pGameObject);
	return S_OK;
}

void CCollider_Manager::Clear_Collision_Group()
{
	for (size_t i = 0; i < COLLISION_END; ++i)
	{
		for (auto& CollisionPair : m_CollisionPairList[i])
		{
			Safe_Release(CollisionPair.first);
			Safe_Release(CollisionPair.second);
		}
		m_CollisionPairList[i].clear();
	}
	
}

_bool CCollider_Manager::Check_Collision(CCollider* pCollider, COLLISION_GROUP eTargetGroup, CGameObject** ppOutTargetObject)
{

	CGameObject* pTargetObject = { nullptr };
	for (auto CollisionPair : m_CollisionPairList[eTargetGroup])
	{
		/*나 자신은 비교하지 않는다*/
		if (CollisionPair.second == pCollider)
			continue;
		

		if (pCollider->Intersect(CollisionPair.second))
		{
			pTargetObject = CollisionPair.first;
			
			if(nullptr != pTargetObject)
				ppOutTargetObject = &pTargetObject;
		}
			
	}



	return false;
}

CCollider_Manager* CCollider_Manager::Create()
{
	CCollider_Manager* pInstance = new CCollider_Manager();
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("FAILED To Created: CCollider_Manager");
		return nullptr;
	}

	return pInstance;
}

void CCollider_Manager::Free()
{

	Clear_Collision_Group();
	__super::Free();

}
