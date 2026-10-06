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
		MSG_BOX("COLLISION GROUP INDEX OUT RANGED");
		return E_FAIL;
	}
	if (!pGameObject)
	{
		MSG_BOX("NO GameObject to Make Pair");
		return E_FAIL;
	}
	if (!pCollider)
	{
		MSG_BOX("NO Collider To Make Pair");
		return E_FAIL;
	}

	m_CollisionPairList[eMyGroup].push_back(make_pair(pGameObject, pCollider));
	Safe_AddRef(pGameObject);
	Safe_AddRef(pCollider);
	return S_OK;
}

void CCollider_Manager::Clear_Collision_Group()
{
	for (size_t i = 0; i < COLLISION_END; ++i)
	{
		for (auto& Collision_Pair : m_CollisionPairList[i])
		{
			Safe_Release(Collision_Pair.first);
			Safe_Release(Collision_Pair.second);
		}
		m_CollisionPairList[i].clear();
	}


}

_bool CCollider_Manager::Check_Collision(CCollider* pCollider, COLLISION_GROUP eTargetGroup, CGameObject** ppOutTargetObject)
{
	if (COLLISION_END <= eTargetGroup)
	{
		MSG_BOX("tARGET: COLLISION GROUP INDEX OUT RANGED");
		return false;
	}
	CGameObject* pGameObject = { nullptr };
	for (auto& Collision_TargetPair : m_CollisionPairList[eTargetGroup])
	{
		if (pCollider == Collision_TargetPair.second)
			continue;

		if (pCollider->Intersect(Collision_TargetPair.second))
		{
			pGameObject = Collision_TargetPair.first;
			*ppOutTargetObject = pGameObject;
			return true;
		}
	}


	return false;
}

CCollider_Manager* CCollider_Manager::Create()
{
	CCollider_Manager* pInstance = new CCollider_Manager();
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("FAILED TO CREATE CCollider_Manager");
		return nullptr;
	}
	return pInstance;
}

void CCollider_Manager::Free()
{

	Clear_Collision_Group();
	__super::Free();
}
