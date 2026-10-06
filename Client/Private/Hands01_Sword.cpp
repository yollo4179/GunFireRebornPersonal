#include"stdafx.h"
#include "Hands01_Sword.h"
#include"GameInstance.h"
#include"Particle_Player01_On.h"
#include"Particle_Player01_Hit.h"
#include"Hands01.h"
#include"Player.h"
CHands01_Sword::CHands01_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{pDevice,pContext}
{
}

CHands01_Sword::CHands01_Sword(const CHands01_Sword& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CHands01_Sword::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHands01_Sword::Initialize(void* pArg)
{
    HANDS01_SWORD_DESC* pDesc = static_cast<HANDS01_SWORD_DESC*>(pArg);
   
    m_pState = pDesc->pState;
    m_pSocketMatrix[0] = pDesc->pSocketMatrix[0];
    m_pSocketMatrix[1] = pDesc->pSocketMatrix[1];
    m_pSocketMatrix[2] = pDesc->pSocketMatrix[2];
    m_pSocketMatrix[3] = pDesc->pSocketMatrix[3];
    m_pSocketMatrix[4] = pDesc->pSocketMatrix[4];
   
    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
    m_pTransformCom->Scaling(0.3f, 0.3f, 0.3f);// 폼크 리프트를 줄여서 작게 만듬 너무커서
    //m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90));
   // m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-0.6f, 0.f, 0.f, 1.f));//조금 떨어진 상태에서 뼈에 붙임

    //m_pModelCom->SetUp_Animation(0, true);

    
    if (FAILED(Ready_Particles()))
        return E_FAIL;
    for (int i = 0; i < g_iNumParticle_Player01_Hit; ++i)
        m_fAccParticleTime[i] = 3.5f;

    return S_OK;
}

void CHands01_Sword::Priority_Update(_float fTimeDelta)
{

    if (m_fNowAttackTime > m_fAccNowAttackTime)
    {
        m_fAccNowAttackTime+= fTimeDelta;
    }
  
    /*상대방 파티클*/
    for (int i = 0; i < g_iNumParticle_Player01_Hit; ++i)
    {
        if (m_fAccParticleTime[i] < 1.f) {
            m_fAccParticleTime[i] += fTimeDelta;
            m_pParticle_Hit[i]->Priority_Update(fTimeDelta);
        }
        else
        {
            m_matFloat4x4OfTargetPart[i] = nullptr;
        }
    }
    if (!m_isValid)
        return;
    /*입혀진 파티클*/
    for (size_t i = 0; i < 20; ++i)
    {
        m_pParticles_On[i]->Priority_Update(fTimeDelta);
    }
   
}

void CHands01_Sword::Update(_float fTimeDelta)
{
    /* 상대방 Particle Update*/
    for (int j = 0; j < g_iNumParticle_Player01_Hit; ++j)
    {
        if (m_fAccParticleTime[j] < 1.f) {
            m_pParticle_Hit[j]->Update(fTimeDelta);
            if (nullptr != m_matFloat4x4OfTargetPart[j])
                m_pParticle_Hit[j]->Set_Matrix(XMLoadFloat4x4(m_matFloat4x4OfTargetPart[j]));
        }
    }
    if (!m_isValid)
        return;
    /*변환된 부모 행렬들에 대해 나의 최종 행렬을 업데이트 해서 던져준다 */
     /*입혀진 파티클*/
    for (size_t i = 0; i < 20; ++i)
    {
        m_pParticles_On[i]->Update(fTimeDelta);
    }
   
  

    CHands01* pHands01 = static_cast<CHands01*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS01, 0));
    if (nullptr != pHands01) {
        m_isAttackChance = pHands01->Get_AttackChance();
        m_isHandsAnimEnd = (*(pHands01->Get_PartAnimEndState()));
    }

    for (size_t i=0;i<5;++i){
             _matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix[i]);
            for (size_t j = 0; j < 3; ++j)
            {
                SocketMatrix.r[j] = XMVector3Normalize(SocketMatrix.r[j]);
                //각각의 축행렬을 일단 1로 크기줄 줄인다 (노말라이즈 ) 
                //본의원래 행렬이 (그게 칼이라면)-> 크기가 납작해지게 만드는 스케일을 적용하고 있다 ->
                // 배치는 제대로 되겠지만 , 스케일의 문제로 ( 스케일은 확대 배율 크기)-> 일단 다 노멀라이즈 한다 .
                //다른 무기를 붙이려고 할때, 본의 행렬을 그대로 적용하면 똑같이 납작해지는 문제가 생기기때문에 적당히
                //교체할무기의 스케일로 조절하여 소켓 메트릭스를 설정한다 .
            }
            if (4 == i)
            {
                m_pTransformCom->Scaling(8.f, 8.f,8.f);
            }
            XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
           
            m_pColliderCom[i]->Update(XMLoadFloat4x4(&m_WorldMatrix));
            if (4 == i)
            {
                m_pTransformCom->Scaling(0.3f, 0.3f, 0.3f);
            }
            for (int j = 0; j < 4; ++j)
            {
                m_pParticles_On[i+j]->Set_Matrix(XMLoadFloat4x4(&m_WorldMatrix));
             }

    }
    for (size_t i = 0; i < 5; ++i)
    {
        if(true== m_isAttackChance &&(CPlayer::STATE_ATTACK_DOWN &*m_pState ||CPlayer::STATE_ATTACK_PRESSING & *m_pState))
             m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, pHands01 ,m_pColliderCom[i]);
    }
  
  
}

void CHands01_Sword::Late_Update(_float fTimeDelta)
{
    /*상대방 파티클*/
    for (int j = 0; j < g_iNumParticle_Player01_Hit; ++j)
    {
        if (m_fAccParticleTime[j] < 1.f)
            m_pParticle_Hit[j]->Late_Update(fTimeDelta);
    }
    
    if (!m_isValid)
        return;
    /*입혀진 파티클*/
    for (size_t i = 0; i < 20; ++i)
    {
        m_pParticles_On[i]->Late_Update(fTimeDelta);
    }
   
    /*LateUpdate CollidionCheck*/
    for (size_t i = 0; i < 5; ++i)
    {
        CGameObject* pTarget = { nullptr };
        if (true==m_isAttackChance&&(CPlayer::STATE_ATTACK_DOWN & *m_pState || CPlayer::STATE_ATTACK_PRESSING & *m_pState)
            &&m_pGameInstance->Check_Collision(m_pColliderCom[i], CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER, &pTarget))
        {
            
            /*_vector vTargetPos = static_cast<CPartObject*>(pTarget)->Get_WorldMatrixOfPartObject().r[3];
            _float vTargetLength =  XMVectorGetX(XMVector3Length(static_cast<CPartObject*>(pTarget)->Get_WorldMatrixOfPartObject().r[0]));

            _vector vCamPos = XMLoadFloat4(m_pGameInstance->Get_CamPosition());

            _vector vParticlePos = XMVector3Normalize (vCamPos - vTargetPos)*vTargetLength +vTargetPos;*/
            const _float4x4* pTargetFloat4x4 = static_cast<CPartObject*>(pTarget)->Get_Float4x4_Ptr_OfPartObject();
            m_matFloat4x4OfTargetPart[m_iNowParticleIndex] = pTargetFloat4x4;
            _matrix matTargetMatrix = XMLoadFloat4x4(pTargetFloat4x4);
            _float3 mvRange = {
                      XMVectorGetX(XMVector3Length(matTargetMatrix.r[0]))
                    , XMVectorGetX(XMVector3Length(matTargetMatrix.r[1]))
                    , XMVectorGetX(XMVector3Length(matTargetMatrix.r[2]))
            };
            m_pParticle_Hit[m_iNowParticleIndex]->Set_Range(mvRange);
            m_pParticle_Hit[m_iNowParticleIndex]->Reset_Vertices(fTimeDelta);
            m_pParticle_Hit[m_iNowParticleIndex]->Set_Matrix(XMLoadFloat4x4(m_matFloat4x4OfTargetPart[m_iNowParticleIndex]));
            m_fAccParticleTime[m_iNowParticleIndex] = 0.f;
            m_iNowParticleIndex = (m_iNowParticleIndex + 1) % g_iNumParticle_Player01_Hit;


        }
    }
   if(true == m_isAttackChance)
    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

   

#ifdef _DEBUG
    for (int i=0;i<5;++i)
    m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);
#endif
}


HRESULT CHands01_Sword::Render()
{
    if (true == m_isAttackChance&& m_isHandsAnimEnd||m_fAccNowAttackTime >= m_fNowAttackTime)
    {
        CHands01* pHands01 = static_cast<CHands01*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS01, 0));
        if (nullptr != pHands01)
            pHands01->Set_HandsInfo(g_HandsInfo[0]);
        m_fAccNowAttackTime = 0.f;
        m_isAttackChance = false;
        pHands01->Reset_AttackChance();
    }
    return S_OK;
  
}

HRESULT CHands01_Sword::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;
    
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
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider5"), reinterpret_cast<CComponent**>(&m_pColliderCom[4]), &ColliderDesc)))
        return E_FAIL;


    return S_OK;
}

HRESULT CHands01_Sword::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
         return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);


    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;



    return S_OK;
}

HRESULT CHands01_Sword::Ready_Particles()
{

    for (int i = 0; i < 20; ++i)
    {
        m_pParticles_On[i]= static_cast<CParticle_Player01_On*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player01_On"), nullptr));
        m_pParticles_On[i]->Set_TextureIndex( i%4);
    }
    for (int i = 0; i < g_iNumParticle_Player01_Hit; ++i)
    {
        m_pParticle_Hit[i]= static_cast<CParticle_Player01_Hit*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player01_Hit"), nullptr));
    }

    return S_OK;
}

CHands01_Sword* CHands01_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHands01_Sword* pInstance = new CHands01_Sword(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CHands01_Sword::Clone(void* pArg)
{
    CHands01_Sword* pInstance = new CHands01_Sword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CHands01_Sword::Free()
{
    __super::Free();

    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    for (size_t i = 0; i < 20; ++i)
    {
        Safe_Release(m_pParticles_On[i]);
    }
    for(size_t i=0;i< g_iNumParticle_Player01_Hit;++i)
    {
        Safe_Release(m_pParticle_Hit[i]);
    }
    for (size_t i = 0; i < 5; ++i) 
    {
        Safe_Release(m_pColliderCom[i]);
    }
}
