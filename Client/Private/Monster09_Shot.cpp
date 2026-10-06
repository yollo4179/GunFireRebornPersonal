#include"stdafx.h"
#include "Monster09_Shot.h"
#include"GameInstance.h"
#include"Particle_Monster09_Shot.h"
#include"Effect_Monster09_ShotDown.h"
#include"Monster09_WarningSign.h"

CMonster09_Shot::CMonster09_Shot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CAlphaObject{ pDevice, pContext }
{
}

CMonster09_Shot::CMonster09_Shot(const CMonster09_Shot& Prototype)
    :CAlphaObject(Prototype)
{
}

HRESULT CMonster09_Shot::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster09_Shot::Initialize(void* pArg)
{
    CMonster09_Shot::MONSTER09_SHOT_DESC* pDesc =static_cast<MONSTER09_SHOT_DESC*>(pArg);

    m_fSpeed = 1.f;//pDesc->fSpeed;


    if (FAILED(__super::Initialize(pArg)))
    {
        return E_FAIL;
    }

    _matrix matWorld = XMMatrixIdentity();
    Reset_Bullets(matWorld);

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Readt_Particles()))
        return E_FAIL;

    if (FAILED(Ready_WarningSign()))
        return E_FAIL;
    return S_OK;
}

void CMonster09_Shot::Priority_Update(_float fTimeDelta)
{

    if(true ==m_bParticle_ShotDown)
    {
        m_fAccParticle_Shot_DownTime += fTimeDelta;
        if (m_fAccParticle_Shot_DownTime > m_fParticle_Shot_DownTime) {
            m_bParticle_ShotDown = false;
            m_fAccParticle_Shot_DownTime = 0.f;
        }
    }
    if (true == m_bEffectShotDown)
    {
        m_fAccEffect_ShotDownTime += fTimeDelta;
        if (m_fAccEffect_ShotDownTime > m_fEffect_ShotDownTime)
        {
            m_bEffectShotDown = false; 
            m_fAccEffect_ShotDownTime = 0.f;

        }
     }
    if (true == m_bEffectShotDown)
    {
        m_pEffect_ShotDown->Priority_Update(fTimeDelta);
    }


    if(true == m_bParticle_ShotDown )
        m_pParticle_Shot_Down->Priority_Update(fTimeDelta);

    if (m_isActive && m_fAccNowLifeTime < m_fLifeTimeLimit)
    {
        m_fAccNowLifeTime += fTimeDelta;
    }
    else
    {
        m_isActive = false;
    }

    if (m_isActive)
    {
        m_pTransformCom->Go_Straight(1.5f * fTimeDelta*m_fSpeed);
        m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
    }
    m_pWarningSign->Priority_Update(fTimeDelta);

}

void CMonster09_Shot::Update(_float fTimeDelta)
{

    if (true == m_bParticle_ShotDown)
        m_pParticle_Shot_Down->Update(fTimeDelta);


    if (true == m_bEffectShotDown)
        m_pEffect_ShotDown->Update(fTimeDelta);


    if (m_isActive)
    {
        m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER_ATTACK, this, m_pColliderCom);
    }
    Compute_ViewZ();

    m_pWarningSign->Priority_Update(fTimeDelta);

}

void CMonster09_Shot::Late_Update(_float fTimeDelta)
{

    if (true == m_bParticle_ShotDown)
        m_pParticle_Shot_Down->Late_Update(fTimeDelta);
    if (true == m_bEffectShotDown)
        m_pEffect_ShotDown->Late_Update(fTimeDelta);

    if (m_isActive)
    {
        _matrix matMyPos = m_pTransformCom->Get_WorldMatrix();

        CGameObject* pTargetObject = nullptr;
        if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER, &pTargetObject))
        {
            m_pParticle_Shot_Down->Set_Matrix(matMyPos);
            m_pParticle_Shot_Down->Reset_Vertices(1);
            m_isActive = false;
            m_bParticle_ShotDown = true;

            m_pEffect_ShotDown->Set_Matrix(matMyPos);
            m_bEffectShotDown = true;

        }

        m_pGameInstance->Add_RenderObject(CRenderer::RG_DISTORTION, this);
        m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);

      
        if (XMVectorGetY(matMyPos.r[3]) <= 3.8f)
        {
            m_pParticle_Shot_Down->Set_Matrix(matMyPos);
            m_pParticle_Shot_Down->Reset_Vertices(1);
            m_isActive = false;
            m_bParticle_ShotDown = true;

            m_pEffect_ShotDown->Set_Matrix(matMyPos);
            m_bEffectShotDown = true;
            m_pGameInstance->PlayEffect(L"Mosnter09_ShotEnd.mp3", 0.05F);
            
        }
       
        m_pWarningSign->Late_Update(fTimeDelta);

    }

    

#ifdef _DEBUG
    if(m_isActive)
        m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif   

}

HRESULT CMonster09_Shot::Render()
{
    if (true == m_bGlow)
    {
        if (FAILED(Bind_SHaderResources_Glow()))
            return E_FAIL;

        _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

        for (size_t i = 2; i <= 2; i++)//1번 인덱스 : 구,( diffuse )  2번 인덱스 ( noise )
        {
            //(메시마다의 diffuse텍스처 던지기)
            if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
                return E_FAIL;
            //셰이더 제출
            m_pShaderCom->Begin(2);

            //정점 그리기
            m_pModelCom->Render(i);
        }
    }
    else {
      
        if (FAILED(Bind_ShaderResources()))
            return E_FAIL;

        _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

        for (size_t i = 2; i <= 2; i++)//1번 인덱스 : 구,( diffuse )  2번 인덱스 ( noise )
        {
            //(메시마다의 diffuse텍스처 던지기)
            if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
                return E_FAIL;
            //셰이더 제출
            m_pShaderCom->Begin(1);

            //정점 그리기
            m_pModelCom->Render(i);
        }
    }

   
    

    return S_OK;
}

void CMonster09_Shot::Reset_Bullets(_fmatrix matInitialShootingPos)
{

    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(matInitialShootingPos.r[0]));
    m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(matInitialShootingPos.r[1]));
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(matInitialShootingPos.r[2]));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialShootingPos.r[3]);
    m_pTransformCom-> Scaling(5.f, 5.f, 5.f);
    if(nullptr !=m_pColliderCom)
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
    m_fAccNowLifeTime = 0.f;
    m_pGameInstance->PlayEffect(L"Monster09_Shot.mp3", 0.3f);

    if (m_pWarningSign)
    {
        _vector vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
        _vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

        _float fMulti = XMVectorGetY(vOriginPos) - 3.8;

        fMulti /= XMVectorGetY(vLook);

        vOriginPos -= fMulti * vLook;
        m_pWarningSign->Set_Vector(vOriginPos);
    }

}

HRESULT CMonster09_Shot::Ready_Components()
{
    CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
    ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
    ColliderDesc.fRadius = 0.5f;

    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;
    /*Model*/
    if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Model_Monster09_Shot"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Com_Shader */
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;
  

    /*ComTexture*/
    if(FAILED(Add_Component(LEVEL_GAMEPLAY2,TEXT("Prototype_Component_Texture_Monster09_Shot_Noise"),
        TEXT("Com_Texture1"),reinterpret_cast<CComponent**>(&m_pTextureCom_Noise))))
        return E_FAIL; 



    return S_OK;
}

HRESULT CMonster09_Shot::Readt_Particles()
{
    m_pParticle_Shot_Down = static_cast<CParticle_Monster09_Shot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Shot_Down"), nullptr));
    m_pEffect_ShotDown = static_cast<CEffect_Monster09_ShotDown*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Effect_Monster09_Shot_Down"), nullptr));

    return S_OK;
}

HRESULT CMonster09_Shot::Ready_WarningSign()
{

    
    m_pWarningSign = static_cast<CMonster09_WarningSign*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_WarningSign_Shot"), nullptr));
    return S_OK;
}

HRESULT CMonster09_Shot::Bind_ShaderResources()
{

    _matrix ModelMatrix = m_pTransformCom->Get_WorldMatrix();

    ModelMatrix.r[0] *= 0.1f;
    ModelMatrix.r[1] *= 0.1f;
    ModelMatrix.r[2] *= 0.1f;
    _float4x4 ModelFloat4x4;
    XMStoreFloat4x4(&ModelFloat4x4, ModelMatrix);


    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShaderCom, "g_ScreenTexture", TEXT("Target_Final"))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &ModelFloat4x4)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;





    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccNowLifeTime, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pTextureCom_Noise->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
        return E_FAIL;

 

    _float2 vWinSize = { (_float)g_iWinSizeX,(_float)g_iWinSizeY };
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vWinSize", &vWinSize, sizeof(_float2))))




    return S_OK;
}

HRESULT CMonster09_Shot::Bind_SHaderResources_Glow()
{
    _matrix ModelMatrix = m_pTransformCom->Get_WorldMatrix();

    ModelMatrix.r[0] *= 0.1f;
    ModelMatrix.r[1] *= 0.1f;
    ModelMatrix.r[2] *= 0.1f;
    _float4x4 ModelFloat4x4;
    XMStoreFloat4x4(&ModelFloat4x4, ModelMatrix);


    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShaderCom, "g_ScreenTexture", TEXT("Target_Final"))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &ModelFloat4x4)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccNowLifeTime, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pTextureCom_Noise->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
        return E_FAIL;

    _float2 vWinSize = { (_float)g_iWinSizeX,(_float)g_iWinSizeY };
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vWinSize", &vWinSize, sizeof(_float2))))
        return E_FAIL;
    
    _float3 vThreshold = {0.0f,0.0f,0.1f};
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fThreshold", &vThreshold, sizeof(_float3)))) 
        return E_FAIL;
    _float vIntensity = 2.f;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", &vIntensity, sizeof(_float))))
        return E_FAIL; 

    return S_OK;
}

CMonster09_Shot* CMonster09_Shot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster09_Shot* pInstance = new CMonster09_Shot(pDevice, pContext);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to create: CMonster09_Shot");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CMonster09_Shot::Clone(void* pArg)
{
    CMonster09_Shot* pInstance = new CMonster09_Shot(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Clone: CMonster09_Shot");
        return nullptr;
    }

    return pInstance;
}

void CMonster09_Shot::Free()
{

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pTextureCom_Noise);
    Safe_Release(m_pParticle_Shot_Down);
    Safe_Release(m_pEffect_ShotDown);
    Safe_Release(m_pWarningSign);
    __super::Free();
}
