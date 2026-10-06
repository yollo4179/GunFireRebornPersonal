#include "..\Public\Layer.h"
#include"ContainerObject.h"

CLayer::CLayer()
{
}

CComponent* CLayer::Get_Component(const _wstring strComponentTag, _uint iObjectIndex)
{

	if (iObjectIndex >= m_GameObjects.size())
		return nullptr;
	
	auto iter = m_GameObjects.begin();
	for (size_t i = 0; i < iObjectIndex; ++i)
	{
		++iter;
	}

	return (*iter)->Find_Component(strComponentTag);

}
/* 파트 오브젝트의 컴포넌트 (콜라이더)를 가져오자 */
CComponent* CLayer::Get_Component(_uint iPartObjectIndex, const _wstring& strPartComponentTag, _uint iObjectIndex)
{
	if (iObjectIndex >= m_GameObjects.size())
		return nullptr;
	
	auto iter = m_GameObjects.begin();
	for (size_t i = 0; i < iObjectIndex; ++i)
	{
		++iter;
	}
	return static_cast<CContainerObject*>(*iter)->Find_Component(iPartObjectIndex, strPartComponentTag);
}

CGameObject* CLayer::Get_GameObject(_uint iObjectIndex)
{

	if (iObjectIndex >= m_GameObjects.size())
		return nullptr;
	auto iter = m_GameObjects.begin();

	for (size_t i = 0; i < iObjectIndex; ++i)
	{
		++iter;
	}
	return (*iter);

}

CGameObject* CLayer::Get_GameObject(_uint iPartObjectIndex, _uint iObjectIndex)
{
	if (iObjectIndex >= m_GameObjects.size())
		return nullptr;
	auto iter = m_GameObjects.begin();

	for (size_t i = 0; i < iObjectIndex; ++i)
	{
		++iter;
	}
	return static_cast<CContainerObject*>(*iter)->Find_GameObject(iPartObjectIndex);
}


HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Priority_Update(fTimeDelta);

}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Update(fTimeDelta);
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Late_Update(fTimeDelta);
}

void CLayer::Remove_From_Layer(CGameObject* pGameObject)
{

	if (m_GameObjects.empty())
	{
		MSG_BOX("LAYER you given IS EMPTY, NO OBJECTS IN LAYER");
			return;
	}
	auto iter = find_if(m_GameObjects.begin(), m_GameObjects.end(),[&pGameObject](const CGameObject* pNowObject) {
		
		if (pNowObject == pGameObject)
			return true;
		else 
			return false;
		});

	if (iter != m_GameObjects.end())
	{
		Safe_Release(*iter);// 게임 인스턴스 클리어 할때 안지우고 지금 미리 지우는 거니까....
		m_GameObjects.remove(*iter);// 같은 주소가 중복되어 들어오면 안되지마 .. 일단 여기있는거  다 지우자 

	}
	else {
		MSG_BOX("THERE'S NO OBJECT LIKE THAT");
		return;
	}

}

CLayer * CLayer::Create()
{
	 return new CLayer();
}


void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
