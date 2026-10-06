#include"stdafx.h"
#include "Monster03_Bullet.h"
#include"GameInstance.h"
#include"Trail_Monster03.h"
#include"Effect_Monster03_Shot.h"
#include"Particle_Monster03_Shot.h"
CMonster03_Bullet::CMonster03_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CMonster03_Bullet::CMonster03_Bullet(const CMonster03_Bullet& Prototype)
    :CGameObject(Prototype)
{
}

HRESULT CMonster03_Bullet::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster03_Bullet::Initialize(void* pArg)
{



    if (FAILED(__super::Initialize(pArg)))
    {
        return E_FAIL;
    }

    _matrix matWorld = XMMatrixIdentity();
    Reset_Bullets(matWorld);

    if (FAILED(Ready_Components()))
        return E_FAIL;
    if (FAILED(Ready_Effects()))
        return E_FAIL;
    return S_OK;
}

void CMonster03_Bullet::Priority_Update(_float fTimeDelta)
{

    if (m_isActive && m_fAccNowLifeTime < m_fLifeTimeLimit)
    {
        m_fAccNowLifeTime += fTimeDelta;
    }
    else
    {
        m_isActive = false;
        for (size_t i = 0; i < 3; ++i)
            m_pTrail_Circle[i]->Reset_VerticesCnt();
    }

    if (m_isActive)
    {
        for (size_t i = 0; i < 3; ++i)
            m_pTrail_Circle[i]->Priority_Update(fTimeDelta);

        m_pTransformCom->Go_Straight(1.5f * fTimeDelta);
        m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
        m_pParticle_Shot->Priority_Update(fTimeDelta);
    }

}

void CMonster03_Bullet::Update(_float fTimeDelta)
{

    
    if (m_isActive)
    {
        m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER_ATTACK, this, m_pColliderCom);
        m_pParticle_Shot->Update(fTimeDelta);
    }

}

void CMonster03_Bullet::Late_Update(_float fTimeDelta)
{

    if (m_isActive)
    {
        CGameObject* pTargetObject = nullptr;
        if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER, &pTargetObject))
        {
            for (size_t i = 0; i < 3; ++i)
                 m_pTrail_Circle[i]->Reset_VerticesCnt();
            m_isActive = false;
        }

        m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
        _matrix mat_World = m_pTransformCom->Get_WorldMatrix();
        for (size_t i = 0; i < 3; ++i)
        {
            int j = i+1;
            _float3 Size = { 0.5f*j, 0.5f * j,0.5f * j };
            
            mat_World.r[3] -= mat_World.r[2] * 0.3f*(j);
            m_pTrail_Circle[i]->Set_Matrix(mat_World, Size);
            m_pTrail_Circle[i]->Late_Update(fTimeDelta);
           
        }
        m_pEffect_Shot->Late_Update(fTimeDelta);
        m_pEffect_Shot->Set_Matrix(m_pTransformCom->Get_WorldMatrix());

        m_pParticle_Shot->Late_Update(fTimeDelta);
        m_pParticle_Shot->Set_Matrix(m_pTransformCom->Get_WorldMatrix());
    }
#ifdef _DEBUG
    if (m_isActive)
    m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

}

HRESULT CMonster03_Bullet::Render()
{
 
    return S_OK;
}

void CMonster03_Bullet::Set_Active(_bool isActive)
{
    m_isActive = isActive; 
    for (size_t i = 0; i < 3; ++i)
        m_pTrail_Circle[i]->Reset_VerticesCnt();
}

void CMonster03_Bullet::Reset_Bullets(_fmatrix matInitialShootingPos)
{
    CTransform* pTargetTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), TEXT("Com_Transform")));

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
   


}

HRESULT CMonster03_Bullet::Ready_Components()
{
    CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
    ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
    ColliderDesc.fRadius = 0.5f;

    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CMonster03_Bullet::Ready_Effects()
{
    for(size_t i=0;i<3;++i)
    m_pTrail_Circle[i] = static_cast<CTrail_Monster03*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Trail_Monster03"), nullptr));

    m_pEffect_Shot  = static_cast<CEffect_Monster03_Shot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Monster03_Shot"), nullptr));

    m_pParticle_Shot = static_cast<CParticle_Monster03_Shot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Monster03_Shot"), nullptr));

    return S_OK;
}

CMonster03_Bullet* CMonster03_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster03_Bullet* pInstance = new CMonster03_Bullet(pDevice, pContext);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to create: CMonster03_Bullet");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CMonster03_Bullet::Clone(void* pArg)
{
    CMonster03_Bullet* pInstance = new CMonster03_Bullet(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Clone: CMonster03_Bullet");
        return nullptr;
    }

    return pInstance;
}

void CMonster03_Bullet::Free()
{
    for(size_t i=0;i<3;++i)
        Safe_Release(m_pTrail_Circle[i]);
    Safe_Release(m_pEffect_Shot);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pParticle_Shot);
    __super::Free();
}
