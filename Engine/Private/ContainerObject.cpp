#include "ContainerObject.h"


CContainerObject::CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CContainerObject::CContainerObject(const CContainerObject& Prototype)
    :CGameObject(Prototype)
{
}

HRESULT CContainerObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CContainerObject::Initialize(void* pArg)
{
    CONTAINEROBJECT_DESC* pDesc = static_cast<CONTAINEROBJECT_DESC*>(pArg);

    //컨테이너 구현체(FINAL)에서 정해준 파트 오브젝트의 개수만큼 미리 할당할 파트오브젝트들을 위한 공간을 마련해둔다.
    // RESIZE -> 공간과 메모리 크기를 둘다 확보해둠  
    m_PartObjects.resize(pDesc->iNumPartObject);
    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    return S_OK;
}

//파트 오브젝트들은 오브젝트 매니저에 넣지 않으니까 자동으로 업데이트가 호출되지 않는다 
// 그러니까 컨테이너에서 파트오브젝트들을 들고 있고, 
// 개별 파트오브젝트들을 컨테이너 안에서 순회하면서 다 업데이트 시킨다
void CContainerObject::Priority_Update(_float fTimeDelta)
{
    for (auto& pPartObject : m_PartObjects)
    {
        if (nullptr != pPartObject)
        {
            pPartObject->Priority_Update(fTimeDelta);
        }
    }
}

void CContainerObject::Update(_float fTimeDelta)
{
    for (auto& pPartObject : m_PartObjects)
    {
        if (nullptr != pPartObject)
        {
            pPartObject->Update(fTimeDelta);
        }
    }
}

void CContainerObject::Late_Update(_float fTimeDelta)
{
    for (auto& pPartObject : m_PartObjects)
    {
        if (nullptr != pPartObject)
        {
            pPartObject->Late_Update(fTimeDelta);
        }
    }
}

//구현체에서 처리한다 .
HRESULT CContainerObject::Render()
{
    return S_OK;
}

CComponent* CContainerObject::Find_Component(_uint iPartObjectIndex, const _wstring& strPartComponentTag)
{
    if (iPartObjectIndex >= m_PartObjects.size())
        return nullptr;
    return m_PartObjects[iPartObjectIndex]->Find_Component(strPartComponentTag);
}

CGameObject* CContainerObject::Find_GameObject(_uint iPartObjectIndex)
{

    if (iPartObjectIndex >= m_PartObjects.size())
        return nullptr;
    return m_PartObjects[iPartObjectIndex];
}


void CContainerObject::Free()
{
    __super::Free();
    for (auto& pPartObject : m_PartObjects)
        Safe_Release(pPartObject);
    m_PartObjects.clear();
}
