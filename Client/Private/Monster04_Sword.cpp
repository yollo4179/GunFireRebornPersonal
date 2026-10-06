#include"stdafx.h"
#include "Monster04_Sword.h"
#include"GameInstance.h"
#include"Trail_Monster04.h"
#include"Monster04.h"
CMonster04_Sword::CMonster04_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CMonster04_Sword::CMonster04_Sword(const CMonster04_Sword& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CMonster04_Sword::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster04_Sword::Initialize(void* pArg)
{
    MONSTER04_SWORD_DESC* pDesc = static_cast<MONSTER04_SWORD_DESC*>(pArg);
    m_pState = pDesc->pState;
    m_pSocketMatrix = pDesc->pSocketMatrix;
     

    /* m_pSocketMatrix[1] = pDesc->pSocketMatrix[1];*/

    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
    m_pTransformCom->Scaling(0.7f, 0.7f, 0.7f);

    if (FAILED(Ready_Trail()))
        return E_FAIL;

    return S_OK;
}

void CMonster04_Sword::Priority_Update(_float fTimeDelta)
{
    m_fAccVerticesAddTiming += fTimeDelta;
    m_pTrail->Priority_Update(fTimeDelta);
}

void CMonster04_Sword::Update(_float fTimeDelta)
{


    if (m_pSocketMatrix)
    { 
        _matrix matLow = {};
        _matrix matHigh = {};
        m_isAttackDoing = false;
        _matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
        for (size_t j = 0; j < 3; ++j)
        {
            SocketMatrix.r[j] = XMVector3Normalize(SocketMatrix.r[j]);

        }
        size_t i = 0;
        for (i = 0; i < 4; ++i) {

            m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, -0.5f * i, 1.f));

            XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
            m_pColliderCom[i]->Update(XMLoadFloat4x4(&m_WorldMatrix));
            if (CONDITION_ATTACK_SWORD_M4)
            m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_MONSTER_ATTACK, this, m_pColliderCom[i]);

            if (m_fAccVerticesAddTiming >=m_fVerticesAddingTerm && (3 == i && ((M4_STATE::M4_STATE_BATTLE_SLASH & *m_pState) || (M4_STATE::M4_STATE_RUSH_SLASH & *m_pState))))
            {
                

                m_isAttackDoing = true;
                m_bAttackDone = false;
                matLow = XMLoadFloat4x4(&m_WorldMatrix);
                matHigh = XMLoadFloat4x4(&m_WorldMatrix);
               // matHigh = XMLoadFloat4x4(&m_WorldMatrix);
       

            }
        
          
        }
    
        
    
        if (m_fAccVerticesAddTiming > m_fVerticesAddingTerm)
        {
            m_fAccVerticesAddTiming = 0;
            if (m_isAttackDoing)
            {
                m_pTrail->Set_BufferMatrix(matHigh, matLow);
            }
        }
        if (false ==m_isAttackDoing&&false ==m_bAttackDone &&!((M4_STATE::M4_STATE_BATTLE_SLASH & *m_pState) || (M4_STATE::M4_STATE_RUSH_SLASH & *m_pState)))
        {
           m_pTrail->Reset_VerticesCnt();
           m_bAttackDone = true;
        }

        m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet( 0.f, 0.f, 0.5f * i, 1.f));
    }
    
    
    m_pTrail->Update(fTimeDelta);
    _bool isChanging = false;
}

void CMonster04_Sword::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
    for (size_t i = 0; i < 4; ++i)
    {
        CGameObject* pTarget = { nullptr };
        if(CONDITION_ATTACK_SWORD_M4)
        m_pGameInstance->Check_Collision(m_pColliderCom[i], CCollider_Manager::COLLISION_PLAYER, &pTarget);
    }
    
    if(true==m_isAttackDoing)
         m_pTrail->Late_Update(fTimeDelta);
   

#ifdef _DEBUG
    for (int i = 0; i < 4; ++i)
    {
        if (m_pColliderCom[i])
            m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);
    }
#endif

}

HRESULT CMonster04_Sword::Render()
{
    return S_OK;
}

HRESULT CMonster04_Sword::Ready_Components()
{
    CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
    ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
    ColliderDesc.fRadius = 0.5f;

    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom[0]), &ColliderDesc)))
        return E_FAIL;
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderCom[1]), &ColliderDesc)))
        return E_FAIL;
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider2"), reinterpret_cast<CComponent**>(&m_pColliderCom[2]), &ColliderDesc)))
        return E_FAIL;
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider4"), reinterpret_cast<CComponent**>(&m_pColliderCom[3]), &ColliderDesc)))
        return E_FAIL;
    


    return S_OK;
}

HRESULT CMonster04_Sword::Bind_ShaderResources()
{


    return S_OK;
}

HRESULT CMonster04_Sword::Ready_Trail()
{
   
    m_pTrail = static_cast<CTrail_Monster04*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Trail_Monster04"), nullptr));

    return S_OK;
}

CMonster04_Sword* CMonster04_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster04_Sword* pInstance = new CMonster04_Sword(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMonster04_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster04_Sword::Clone(void* pArg)
{
    CMonster04_Sword* pInstance = new CMonster04_Sword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CMonster04_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster04_Sword::Free()
{
    __super::Free();
    for (size_t i = 0; i < 4; ++i)
    {
        Safe_Release(m_pColliderCom[i]);
    }
    Safe_Release(m_pTrail);
}
