#include"stdafx.h"
#include "Monster05_Bullet.h"
#include"GameInstance.h"

#include"Trail_Monster05.h"
#include"Effect_Monster05_Shot.h"

CMonster05_Bullet::CMonster05_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CMonster05_Bullet::CMonster05_Bullet(const CMonster05_Bullet& Prototype)
    :CGameObject (Prototype)
{
}

HRESULT CMonster05_Bullet::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster05_Bullet::Initialize(void* pArg)
{
   

    
    if (FAILED(__super::Initialize(pArg)))
    {
        return E_FAIL;
    }

    _matrix matWorld = XMMatrixIdentity();
   // Reset_Bullets(matWorld);

    if (FAILED(Ready_Components()))
        return E_FAIL;
    if (FAILED(Ready_Effects()))
        return E_FAIL;
    
    return S_OK;
}

void CMonster05_Bullet::Priority_Update(_float fTimeDelta)
{

    if (m_isActive &&m_fAccNowLifeTime < m_fLifeTimeLimit )
    {
        m_fAccNowLifeTime += fTimeDelta;
    }
    else
    {
        m_isActive = false;
    }
 

    if (m_fAccTrailTime < m_fTrailTiming)
    {
        m_fAccTrailTime += fTimeDelta;
    }
    if (m_isActive)
    {
        if (true==m_bTracingMode)
        {
            CTransform* pTargetTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), TEXT("Com_Transform")));

            if (nullptr == pTargetTransform)
                return;
            _vector vPlayerPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
            _vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
            _vector vTracingDir = XMVector3Normalize(vPlayerPos - vMyPos);

            m_pTransformCom->Go_Pushed( fTimeDelta, vTracingDir);

        }
        else
        {
           
           m_pTransformCom->Go_Straight(2.f * fTimeDelta);
        }
        m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
        m_pEffect_Shot->Set_Matrix(m_pTransformCom->Get_WorldMatrix());

        if ((true == m_bTracingMode)&&m_fAccTrailTime >= m_fTrailTiming)
        {
            _matrix matHigh = m_pTransformCom->Get_WorldMatrix();
            _matrix matLow = m_pTransformCom->Get_WorldMatrix();
            m_pTrail_Shot->Set_BufferMatrix(matHigh, matLow);

        }
        if (m_fAccTrailTime >= m_fTrailTiming)
        {
            m_fAccTrailTime = 0.f;
        }
    }

}

void CMonster05_Bullet::Update(_float fTimeDelta)
{
    if (m_isActive)
    {
        m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER_ATTACK, this, m_pColliderCom);
        m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER, this, m_pColliderCom);
       
    }

}

void CMonster05_Bullet::Late_Update(_float fTimeDelta)
{
  
   

    if (m_isActive)
    {
        CGameObject* pTargetObject = nullptr;
        if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER, &pTargetObject))
        {
            m_isActive = false;
        }

        if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, &pTargetObject))
        {
            m_isActive = false;
        }

        m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
   
        if (true == m_bTracingMode)
            m_pTrail_Shot->Late_Update(fTimeDelta);
        m_pEffect_Shot->Late_Update(fTimeDelta);
      
    }

  
#ifdef _DEBUG
    if(m_isActive)
        m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

}

HRESULT CMonster05_Bullet::Render()
{
    return S_OK;
}

void CMonster05_Bullet::Reset_Bullets(_fmatrix matInitialShootingPos)
{
    
   


    CTransform* pTargetTransform =static_cast<CTransform*>( m_pGameInstance->Get_Component(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), TEXT("Com_Transform")));

    if (nullptr == pTargetTransform)
        return;
    _vector vPlayerPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
    _vector vMyPos = matInitialShootingPos.r[3];

    _vector vShotLook = XMVector3Normalize(vPlayerPos - vMyPos);
    _vector vShotRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vShotLook));
    _vector vShotUp = XMVector3Normalize(XMVector3Cross(vShotLook, vShotRight));
  
    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vShotRight));
    m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vShotUp));
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vShotLook));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialShootingPos.r[3]);
    m_fAccNowLifeTime = 0.f;
    m_fAccTrailTime = 0.f;
  
    m_pTrail_Shot->Reset_VerticesCnt();
}

HRESULT CMonster05_Bullet::Ready_Components()
{
    CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
    ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
    ColliderDesc.fRadius = 0.5f;

    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CMonster05_Bullet::Ready_Effects()
{
    
    m_pTrail_Shot = static_cast<CTrail_Monster05*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Trail_Monster05"), nullptr));
    m_pEffect_Shot =static_cast<CEffect_Monster05_Shot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Monster05_Shot"), nullptr));
    return S_OK;
}

CMonster05_Bullet* CMonster05_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster05_Bullet* pInstance = new CMonster05_Bullet(pDevice, pContext);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to create: CMonster05_Bullet");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CMonster05_Bullet::Clone(void* pArg)
{
    CMonster05_Bullet* pInstance = new CMonster05_Bullet(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Clone: CMonster05_Bullet");
        return nullptr;
    }

    return pInstance;
}

void CMonster05_Bullet::Free()
{
    /*Effect*/
    Safe_Release(m_pTrail_Shot);
    Safe_Release(m_pEffect_Shot);

    Safe_Release(m_pColliderCom);
    __super::Free();
}
