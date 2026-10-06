#include "PartObject.h"

CPartObject::CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CPartObject::CPartObject(const CPartObject& Prototype)
    :CGameObject(Prototype)
{
}

HRESULT CPartObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPartObject::Initialize(void* pArg)
{
    PARTOBJECT_DESC* pDesc = static_cast <PARTOBJECT_DESC*>(pArg);
    //부모의 트랜스폼(월드) 행렬을 받아야 되는데 그건 파트오브젝트 clone 할때 채워주고 여기서 그 주소를 받는다
   //컨테이너 월드 matrix 주소 
    m_pParentMatrix = pDesc->pParentMatrix;
    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    return S_OK;
}

void CPartObject::Priority_Update(_float fTimeDelta)
{
    
}

void CPartObject::Update(_float fTimeDelta)
{

}

void CPartObject::Late_Update(_float fTimeDelta)
{

}

HRESULT CPartObject::Render()
{
    //렌더는 실제 파트오브젝트가 모델과 셰이더를 가지므로 
    // abstract 가 아니라 실제 구현체에서 가지고 있도록 만든다 
    return S_OK;
}

void CPartObject::Free()
{
    __super::Free();
}
