#include"stdafx.h"
#include"Monster09_Mini.h"
#include"Monster09_Mini_Shot.h"
#include"GameInstance.h"
#include"Hands.h"
#include"UI_DemageFont.h"
#include"Particle_Monster09_Mini.h"
CMonster09_Mini::CMonster09_Mini(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster{pDevice, pContext}
{
}

CMonster09_Mini::CMonster09_Mini(const CMonster09_Mini& Prototype)
    :CMonster (Prototype)
{
}

HRESULT CMonster09_Mini::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster09_Mini::Initialize(void* pArg)
{
    MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);
    pDesc->iNumPartObject = 0;


    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (Mapping_Anim_States())
        return E_FAIL;

    if (FAILED(Ready_MiniShots()))
        return E_FAIL;
    if (FAILED(Ready_Particle()))
        return E_FAIL; 

    m_pModelCom->SetUp_Animation(1,true);

    m_HPInfo = { 1000.f,1000.f, 1.f,1.f };
    m_iState = M9_MINI_STATE::M9_MINI_STATE_IDLE;

    return S_OK;

}


void CMonster09_Mini::Priority_Update(_float fTimeDelta)
{
    if (false == m_bActive)
        return;

    if (m_HPInfo.x <= 0)
    {
        m_bDead = true;
        
    }
    /*Particle*/

    if (m_fAccParticleTime < m_fParticleTime)
    {
        m_fAccParticleTime += fTimeDelta;
    }
    if (m_fAccParticleTime >= m_fParticleTime)
    {
        m_isParticle_Working = false;
    }

    if (true == m_isParticle_Working)
    {
        m_pParticle_Mini->Priority_Update(fTimeDelta);
    }
    Pri_Update_LoopCall_State();
    Pri_Update_Non_LoopCall_State();

    /*m_fAccTime*/
    {

       /*스킬 시간 더해주기 (cool time 까지)*/
        if (Skill_Sign_Queue.size()<10)
        {
            if (!CONDITION_ATTACK_SHOT)
                m_fAcc_Attack_Shot_Time += fTimeDelta;
        
            if (m_fAcc_Attack_Shot_Time > m_fAttack_Shot_Cool_Time)
            {
                m_fAcc_Attack_Shot_Time = 0.f;
                Skill_Sign_Queue.push(1);
            }
        }

       
       
    }
    if ((!m_isAnimEnd && CONDITION_NON_LOOP))
    {
        if (CONDITION_ATTACK_SHOT)
        {
            m_fAcc_Attack_Shot_Timing += fTimeDelta;
        }
        
    }
    else if (m_isAnimEnd && CONDITION_NON_LOOP)
    {
        if (CONDITION_ATTACK_SHOT)
        {
            m_iState ^= M9_MINI_STATE::M9_MINI_STATE_ATTACK_SHOT;
            m_fAcc_Attack_Shot_Timing = 0.f;
            Skill_Sign_Queue.pop();
            m_bShotSign = false;
            m_bShotSignEnd = false;
            //애님 끝났으면 초기화 
            m_fAcc_Attack_Shot_Time = 0.f;
            m_iState = M9_MINI_STATE::M9_MINI_STATE_IDLE;
        }
        else if (CONDITION_POPOUT)
        {
            m_iState ^= M9_MINI_STATE::M9_MINI_STATE_POPOUT;
            m_iState = M9_MINI_STATE::M9_MINI_STATE_IDLE;
        }
    }
    /* 스킬 끝났다*/
    else
    {
        /*상태 해제*/

        /*Attack*/
        m_iState = M9_MINI_STATE::M9_MINI_STATE_IDLE;
        if (!Skill_Sign_Queue.empty())
        {
                m_iState =M9_MINI_STATE::M9_MINI_STATE_ATTACK_SHOT;
        }
    }
    /*Animation */

    if(CONDITION_ATTACK_SHOT)
    {
        if (m_bCall_Attack_Shot)
        {
            m_pModelCom->Move_NextAnim(m_vecAnimStates[_uint(M9_MINI_ANIM::M9_MINI_ANIM_SHOT)], 0.2F, false);
            m_bCall_Attack_Shot = false;
        }
    }
    else if (CONDITION_IDLE)
    {
        if (m_bCall_Idle)
        {

            m_pModelCom->Move_NextAnim(m_vecAnimStates[_uint(M9_MINI_ANIM::M9_MINI_ANIM_IDLE)], 0.2f, true);
            m_bCall_Idle = false;
        }
    }
    else if (CONDITION_DEAD)
    {
        if (m_bCall_Dead) {
            _float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);
            if (5.f >= fChance)
            {
                m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_MINI_ANIM::M9_MINI_ANIM_DEAD1], 0.2f, false);
            }
            else if (10.f >= fChance)
            {
                m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_MINI_ANIM::M9_MINI_ANIM_DEAD2], 0.2f, false);
            }
            m_bCall_Dead = false;
        }
    }

    else if (CONDITION_IDLE)
    {
        if (m_bCall_Idle)
        {
            m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_MINI_ANIM::M9_MINI_ANIM_IDLE], 0.2f, true);
            m_bCall_Idle = false;
        }
    }
   
    /* popout은 monster09_main에서 활성화 할때 만 시켜줄거임 ->그 이후로는 idle 과 공격의 반복 (queue가 비지않았으면 attack상태로*/
    else if (CONDITION_POPOUT)
    {
        if (m_bCall_PopOut)
        {
            m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_MINI_ANIM::M9_MINI_ANIM_POPOUT], 0.2F, false);
            m_bCall_PopOut = false;
        }
    }

    



    __super::Priority_Update(fTimeDelta);

    /*Attack*/
      /*For Mini SHot*/
    {
        /*For Monster09_Mini_Shot*/
        for (size_t i = 0; i < m_iNumMiniShots; ++i)
        {
            if (m_pMiniShots[i]->Get_Active())
                m_pMiniShots[i]->Priority_Update(fTimeDelta);
        }
        if (false == m_bShotSignEnd && m_fAcc_Attack_Shot_Timing >= 0.1f)
        {
            m_bShotSign = true;
        }
        
    }
}

void CMonster09_Mini::Update(_float fTimeDelta)
{
    if (false == m_bActive)
        return;
    if (true == m_isParticle_Working)
    {
        m_pParticle_Mini->Update(fTimeDelta);
    }

    m_isAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);
    __super::Update(fTimeDelta);
    
    /*총알 발사*/
    {
        
        if (m_pSocketMatrix_For_Attack)
        {
            /*소켓 메트릭스는 머즐본의(위치)*/
            _matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix_For_Attack);
            for (size_t j = 0; j < 3; ++j)
            {
                SocketMatrix.r[j] = XMVector3Normalize(SocketMatrix.r[j]);
            }
            XMStoreFloat4x4(&m_WorldMatrix_For_Attack, SocketMatrix * m_pTransformCom->Get_WorldMatrix());
        }
        /*For Monster09_Mini_Shot*/
        for (size_t i = 0; i < m_iNumMiniShots; ++i)
        {
            if (m_pMiniShots[i]->Get_Active())
                m_pMiniShots[i]->Update(fTimeDelta);
        }
        if (true == m_bShotSign && CONDITION_ATTACK_SHOT)
        {
            CTransform* pTargetTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY2, TEXT("Layer_APlayer"), TEXT("Com_Transform"), 0));
            if (nullptr == pTargetTransform)
                return;

            _vector vPlayerPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
          
            _vector vMyPos = XMLoadFloat4((_float4*)&m_WorldMatrix_For_Attack.m[3]);
           // _vector vMyLook = XMLoadFloat4((_float4*)&m_WorldMatrix_For_Attack.m[2]);
            _vector vShotLook = XMVector3Normalize(vPlayerPos - vMyPos);
            _vector vShotRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f,1.f,0.f,0.f), vShotLook));
            _vector vShotUp = XMVector3Normalize(XMVector3Cross(vShotLook, vShotRight));
            _vector vShotPos = vMyPos;


            m_bShotSign = false;
            m_bShotSignEnd = true;
           
            _matrix  MiniShotMatrix;
            MiniShotMatrix.r[0] = vShotRight;
            MiniShotMatrix.r[1] = vShotUp;
            MiniShotMatrix.r[2] = vShotLook;
            MiniShotMatrix.r[3] = vShotPos;
            m_pMiniShots[m_iNowMiniShotIndex]->Reset_Bullets(MiniShotMatrix);
            m_pMiniShots[m_iNowMiniShotIndex]->Set_Active(true);
            m_iNowMiniShotIndex = (1 + m_iNowMiniShotIndex)%(m_iNumMiniShots);

        }


    }
    /*Collider Pos*/
    {


        _matrix matColliderPos = m_pTransformCom->Get_WorldMatrix();
        _vector vRight = matColliderPos.r[0] * 3;// 1.5f;
            _vector vUp = matColliderPos.r[1] * 3;// 1.5f;
            _vector vLook = matColliderPos.r[2] * 3; // 1.5f;
        _vector vPos = XMVectorSetY(matColliderPos.r[3], XMVectorGetY(matColliderPos.r[3]) + 5.f );
        matColliderPos.r[0] = vRight;
        matColliderPos.r[1] = vUp;
        matColliderPos.r[2] = vLook;
        matColliderPos.r[3] = vPos;
        m_pColliderCom->Update(matColliderPos);
        m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_MONSTER, this, m_pColliderCom);

    }


}

void CMonster09_Mini::Late_Update(_float fTimeDelta)
{

    if (false == m_bActive)
        return;

    if (true == m_isParticle_Working)
    {
        m_pParticle_Mini->Late_Update(fTimeDelta);
    }

    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND,this);
    __super::Late_Update(fTimeDelta);

    /*For Collision*/
    CGameObject* pTarget = { nullptr };
    if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, &pTarget))
    {
        if (m_fHitTime <= m_fAccHitTime)
        {
            m_bHit_NormalMonster = true;
            CHands* pHands = static_cast<CHands*>(pTarget);
            HandsInfo eHandsInfo = pHands->Get_HandsInfo();
            _bool isCritical = false;

            _float fDemage = eHandsInfo.fDemage;
            _float fExtraDemage = m_pGameInstance->Compute_Random(0.f, fDemage * 0.1f);
            _float fCriticalDemage = eHandsInfo.fCriticalDemage;
            _float fCriticalRate = eHandsInfo.fCriticalRate;

            _float fRate = m_pGameInstance->Compute_Random(0.f, 100.f);

            if (fRate <= fCriticalRate)isCritical = true;

            fDemage += fExtraDemage;

            if (isCritical)
            {
                fDemage = fDemage * (1 + fCriticalDemage / 100.f);
            }
           
            
            
             (m_HPInfo).x -= fDemage;
            
             if (m_HPInfo.x <= 0)
             {
                 m_HPInfo.x = 0.f;
             }
           
           m_fAccHitTime = 0.f;
            /*Add Demage Font*/
            {
                CUI_DemageFont* pDemageFont = nullptr;
                pDemageFont = static_cast<CUI_DemageFont*>(m_pGameInstance->Get_Now_UI_DemageFont());
                if (nullptr == pDemageFont)
                {
                    return;
                }
                _matrix matColliderWorld = m_pColliderCom->Get_WorldMatrix();
                _float fRandomOffsetX = m_pGameInstance->Compute_Random(0.f, XMVectorGetX(XMVector3Length(matColliderWorld.r[0])) * 0.5f);
                _float fRandomOffsetY = m_pGameInstance->Compute_Random(0.f, XMVectorGetX(XMVector3Length(matColliderWorld.r[1])) * 0.5f);
                matColliderWorld.r[3] = XMVectorSetY(matColliderWorld.r[3], XMVectorGetY(matColliderWorld.r[3]) + fRandomOffsetY);
                matColliderWorld.r[3] = XMVectorSetX(matColliderWorld.r[3], XMVectorGetX(matColliderWorld.r[3]) + fRandomOffsetX);

                pDemageFont->Set_Active(true);
                pDemageFont->Set_WorldMatrix(matColliderWorld);
                pDemageFont->Set_Demage(fDemage, isCritical);


            }
        }
    }

    /*For Monster09_Mini_Shot*/
    for (size_t i = 0; i < m_iNumMiniShots; ++i)
    {
        if (m_pMiniShots[i]->Get_Active())
            m_pMiniShots[i]->Late_Update(fTimeDelta);
    }

    if (m_isRemoving)
    {
        m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
    }

    
  

#ifdef _DEBUG
    m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CMonster09_Mini::Render()
{

    __super::Render();

    

    return S_OK;
}

HRESULT CMonster09_Mini::Mapping_Anim_States()
{
    m_vecAnimStates.resize((_uint)M9_MINI_ANIM::M9_MINI_ANIM_END);

    /*Attack*/
    m_vecAnimStates[(_uint)M9_MINI_ANIM::M9_MINI_ANIM_SHOT] = 3;
    /*PopOut*/
    m_vecAnimStates[(_uint)M9_MINI_ANIM::M9_MINI_ANIM_POPOUT] = 1;
    /*Dead*/
    m_vecAnimStates[(_uint)M9_MINI_ANIM::M9_MINI_ANIM_DEAD1] = 4;
    m_vecAnimStates[(_uint)M9_MINI_ANIM::M9_MINI_ANIM_DEAD2] = 2;
    /*Idle*/
    m_vecAnimStates[(_uint)M9_MINI_ANIM::M9_MINI_ANIM_IDLE] = 0;


    return S_OK;
}

void CMonster09_Mini::Pri_Update_Cancle_LoopState()
{
    if (CONDITION_IDLE)
        m_iState ^= M9_MINI_STATE::M9_MINI_STATE_IDLE;


}

void CMonster09_Mini::Pri_Update_LoopCall_State()
{
    if (!CONDITION_IDLE && false == m_bCall_Idle)
        m_bCall_Idle = true;

    
}

void CMonster09_Mini::Pri_Update_Non_LoopCall_State()
{
    if (!CONDITION_POPOUT && false == m_bCall_PopOut)
        m_bCall_PopOut = true;

    if (!CONDITION_DEAD && false == m_bCall_Dead)
        m_bCall_Dead = true;
    if (!CONDITION_ATTACK_SHOT && false == m_bCall_Attack_Shot)
        m_bCall_Attack_Shot = true;
    


}

void CMonster09_Mini::Reset_Position(_fmatrix matInitialShootingPos)
{
    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(matInitialShootingPos.r[0]));
    m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(matInitialShootingPos.r[1]));
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(matInitialShootingPos.r[2]));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialShootingPos.r[3]);
    m_pTransformCom->Scaling(1.f, 1.f, 1.f);
    if (nullptr != m_pParticle_Mini)
    {

        m_pParticle_Mini->Reset_Vertices(1);
        _matrix matColliderPos = m_pTransformCom->Get_WorldMatrix();
        _vector vRight = matColliderPos.r[0] * 3;// 1.5f;
        _vector vUp = matColliderPos.r[1] * 3;// 1.5f;
        _vector vLook = matColliderPos.r[2] * 3; // 1.5f;
        _vector vPos = XMVectorSetY(matColliderPos.r[3], XMVectorGetY(matColliderPos.r[3]) + 3.f);
        matColliderPos.r[0] = vRight;
        matColliderPos.r[1] = vUp;
        matColliderPos.r[2] = vLook;
        matColliderPos.r[3] = vPos;
        m_pParticle_Mini->Set_Matrix(matColliderPos);
        m_isParticle_Working = true;
        m_fAccParticleTime = 0.f;

    }
}

void CMonster09_Mini::Set_Active(_bool isActive)
{
    if (true == isActive)
    {
        m_isRemoving = false;
        m_bDead = false;
        m_HPInfo = { 1000.f,1000.f, 100.f,100.f };
        m_fAccDesolveTime = 0.f;
        m_pGameInstance->PlayEffect(L"Monster09_MiniActive.wav", 0.1f);
        
    }

     m_bActive = isActive ;
}



HRESULT CMonster09_Mini::Ready_Components()
{
   
    /* Com_Shader */
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Model_Monster09_Mini"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;
   
    m_pSocketMatrix_For_Attack = Get_BoneMatrix("muzzle");

    CBounding_Sphere::BOUND_SPHERE_DESC  ColliderDesc = {};
    ColliderDesc.vCenter = { 0.f,0.f,0.f };
    ColliderDesc.fRadius = 0.5f;
    /*ColliderCom*/
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom),&ColliderDesc)))
        return E_FAIL;
    

    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Desolve"),
        TEXT("Com_TextureDissolve"), reinterpret_cast<CComponent**>(&m_pTextureCom_Desolve))))
        return E_FAIL;
}

HRESULT CMonster09_Mini::Bind_ShaderResources()
{
    __super::Bind_ShaderResources();

    return S_OK;
}

HRESULT CMonster09_Mini::Ready_MiniShots()
{
    m_pMiniShots.resize(m_iNumMiniShots);
    CMonster09_Mini_Shot::MONSTER09_MINI_SHOT_DESC desc = {};
    desc.fRotationPerSec = XMConvertToRadians(90.f);
    desc.fSpeedPerSec = 10.f;
    desc.fSpeed = 2.f;

    for (size_t i = 0; i < m_iNumMiniShots; ++i)
    {
        m_pMiniShots[i] = static_cast<CMonster09_Mini_Shot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Mini_Shot"), &desc));
    }
    return S_OK;
}

HRESULT CMonster09_Mini::Ready_Particle()
{

    m_pParticle_Mini = static_cast<CParticle_Monster09_Mini*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Mini"), nullptr));
   

    return S_OK;
}

CMonster09_Mini* CMonster09_Mini::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster09_Mini* pInstance = new CMonster09_Mini(pDevice, pContext);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Created CMonster09_Mini Failed");
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

CGameObject* CMonster09_Mini::Clone(void* pArg)
{
    CMonster09_Mini* pInstance = new CMonster09_Mini(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Clone CMonster09_Mini Failed");
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CMonster09_Mini::Free()
{
    for (auto& MiniShot : m_pMiniShots)
        Safe_Release(MiniShot);
    m_pMiniShots.clear();
    Safe_Release(m_pColliderCom);

    Safe_Release(m_pParticle_Mini);
    __super::Free();
}
