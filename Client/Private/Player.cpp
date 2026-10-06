#include"stdafx.h"
#include "Player.h"
#include"GameInstance.h"
#include "Navigation.h"

#include"UI_Arms.h"
#include"UI_Player_HP.h"
#include"Hands01.h"
#include"Hands01_Sword.h"
#include"Hands02.h"
#include"Hands03.h"
#include"Hands04.h"
#include"Hands05.h"
#include"Hands_Skill.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CContainerObject{ pDevice, pContext }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CContainerObject(Prototype)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
    PLAYER_DESC* pDesc = static_cast<PLAYER_DESC*>(pArg);

    CONTAINEROBJECT_DESC Desc = {};
    Desc.iNumPartObject = PART_END;
    Desc.fSpeedPerSec = 10.f;  //나의 이동 속도는 (트랜스폼)
    Desc.fRotationPerSec = XMConvertToRadians(90.f);//나의 회전 속도는 
    
    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;
    //트랜스폼 정보 넘겨주고 부모의 트랜스폼 이동 옵션 초기값을 세팅 ,
    //관리할 파트오브젝트 수만큼 -> 컨테이너 공간확보 
    if(nullptr != pDesc)
    m_fInitialHeight = pDesc->fInitial_Height;
    float InitialX = pDesc->InitialPos.x;
    float InitialZ = pDesc->InitialPos.z;
    m_iNaviMeshIndex = pDesc->iNaviMeshIndex;
    m_iNowMapStage = pDesc->iNowMapStage;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION , XMVectorSet(InitialX, m_fInitialHeight, InitialZ, 1.f));


    m_vHPInfo = g_PlayerHPInfo;
    m_ePlayerItemInfo = g_PlayerItemInfo;

    if (FAILED(Ready_Components()))
    {
        return E_FAIL;
    }

    if (FAILED(Ready_PartObjects()))
    {
        return E_FAIL;
    }
    
   
    {
        //무기 교체할때 -> 이거 바꾸면됨
        m_MyTools[0] = (_uint)PART_HANDS01;
        m_MyTools[1] = (_uint)PART_HANDS05;
        m_MyTools[2] = (_uint)PART_HANDS04;
        m_MyTools[3] = (_uint)PART_HANDS03;
        m_MyTools[4] = (_uint)PART_HANDS02;

        m_iCurTool = 0;  //첫번째 무기 장착
        static_cast<CHands*>(m_PartObjects[m_MyTools[m_iCurTool]])->SetActive(true);
    }
    Init_UIS();

    m_iState = STATE_POP_OUT;
    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{


    /*DashTime*/
    if (true ==m_bDashStart)
    {
        m_pGameInstance->Get_UI_By_Tag(TEXT("Dash_Screen"))->Set_Active();
        m_fAccDashTime += fTimeDelta;
        m_iState |= STATE_DASH;
    }
    if (m_fAccDashTime >= m_fDashTime)
    {
        m_fAccDashTime = 0.f;
        m_fAccDash_CoolTime = 0.f;
        if (STATE_DASH & m_iState)
            m_iState ^= STATE_DASH;
        m_fSpeed = 1.f;
        m_pGameInstance->Get_UI_By_Tag(TEXT("Dash_Screen"))->Set_InActive();
        m_bDashStart = false;

    }


    Key_Input_UI();
    if (false == g_Update_TimeDelta)
        return;

   

    /*Hit Time*/
    {
        if (m_fAccHitTime < 5.f)
            m_fAccHitTime += fTimeDelta;
    }
    /*BombCoolTime*/
    if (m_fAccBombCoolTime < m_fBombCoolTime)
    {
        m_fAccBombCoolTime += fTimeDelta;
    }
    /*DashCoolTime*/
    if (m_fAccDash_CoolTime < m_fDash_CoolTime)
    {
        m_fAccDash_CoolTime += fTimeDelta;
    }
  
    /*Sound*/
    if (m_bSound_StartDash)
    {
        m_pGameInstance->PlayEffect(L"Player_Dash.mp3",0.1f);
        m_pGameInstance->PlayEffect(L"Player_Dash_Voice.mp3", 0.3f);
        m_bSound_StartDash = false;
    }
    if (m_bSound_StartJump)
    {
        m_pGameInstance->PlayEffect(L"Player_Jump.mp3",0.1f);
        m_pGameInstance->PlayEffect(L"Player_JumpVoice.mp3", 0.1f);
        m_bSound_StartJump = false;
    }

    __super::Priority_Update(fTimeDelta);// ->파트 오브젝트들 업데이트 시켜준다

   // m_iState = STATE_IDLE;//STATE는 IDLE 로 고정시켜야  애님이 안변함

   
    Pri_Update_KeyInput(fTimeDelta);
    {

        /*Y축 (점프가 아닌 상테일경우)*/
        _vector vPos = m_pNavigationCom->SetUp_HeightOnNavigation(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + m_fInitialHeight);
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

       
        if (true ==m_isjumping)
        {


            m_fCurHeight += m_v0 * fTimeDelta;
            m_v0 -= m_fGravity * fTimeDelta;
            
            if ( 0.f >= m_fCurHeight)
            {
                m_fCurHeight = 0.f;
                m_isjumping = false;
                m_v0 = 5.f;
                if(STATE_JUMP & m_iState )
                    m_iState ^= STATE_JUMP;
            }
            vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + m_fCurHeight);
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
        }


    }
   
}

void CPlayer::Update(_float fTimeDelta)
{
    if (false == g_Update_TimeDelta)
        return;
  

    __super::Update(fTimeDelta);
    /*Add_ColldionGroup :CollisionCheck*/
    {
        /*Collider Pos*/
        _matrix matColliderPos = m_pTransformCom->Get_WorldMatrix();
        _vector vRight = matColliderPos.r[0] / 1.5f;
        _vector vUp = matColliderPos.r[1] / 1.5f;
        _vector vLook = matColliderPos.r[2] / 1.5f;
        _vector vPos = XMVectorSetY(matColliderPos.r[3], XMVectorGetY(matColliderPos.r[3]));
        matColliderPos.r[0] = vRight;
        matColliderPos.r[1] = vUp;
        matColliderPos.r[2] = vLook;
        matColliderPos.r[3] = vPos;
        m_pColliderCom->Update(matColliderPos);
        m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_PLAYER, this, m_pColliderCom);


        m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_CHECK_RAY, this, m_pColliderCom_CheckRay);
        pair<_vector, _vector> pairRay = Get_PickingPosInWorld();
        _float3 RaySrc, RayDir;
        XMStoreFloat3(&RaySrc, pairRay.first);
        XMStoreFloat3(&RayDir, pairRay.second);
        m_pColliderCom_CheckRay->Update(make_pair(RaySrc, RayDir));
    }
}

void CPlayer::Late_Update(_float fTimeDelta)
{
   
    /*PartObjects*/
    __super::Late_Update(fTimeDelta);
    /*Collision*/
    CGameObject* pTarget = { nullptr };
    if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER_ATTACK, &pTarget))
    {
        if (m_fHitTime <= m_fAccHitTime)
        {
            _float fDemage = 1.f;
            //Blue
            if (m_vHPInfo.x >= fDemage)
            {
                m_vHPInfo.x -= fDemage;
            }
            else
            {
                //Red
                if (0 != m_vHPInfo.x)
                {
                    m_vHPInfo.z -= (fDemage - m_vHPInfo.x);
                    m_vHPInfo.x = 0.f;
                }
                else
                {
                    m_vHPInfo.z -= (fDemage);
                    if (m_vHPInfo.z < 0)
                    {
                        m_vHPInfo.z = 0;
                    }
                }
            }
            m_fAccHitTime = 0.f;
        }

    }
    /*Collision-Picking*/
    if (m_pGameInstance->Check_Collision(m_pColliderCom_CheckRay, CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER, &pTarget))
    {

    }


    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

#ifdef _DEBUG
        m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

}

HRESULT CPlayer::Render()
{
   // m_pColliderCom->Render();
    return S_OK; //렌더는 모델이 가지는  각각의 파트오브젝트가 수행 
    
}

const _float4x4* CPlayer::Get_PlayerFloat4x4_ptr()
{
    if(nullptr != m_pTransformCom)
         return (m_pTransformCom->Get_WorldFloat4x4_Ptr());

    return nullptr;
}

void CPlayer::Init_UIS()
{
    Init_UI_TOOL();
    Init_UI_HPBar();
}

void CPlayer::Init_UI_TOOL()
{
   
    m_pArms = static_cast<CUI_Arms*>( m_pGameInstance->Get_UI_By_Tag(L"Arms"));
    Safe_AddRef(m_pArms);
    /*Mapping ARMS WITH Player*/
    m_pArms->Init_UI_MappingIndexWithUI(PART_HANDS01, 0);//ui의 애니메이션과 총을 실제로 매핑 
    m_pArms->Init_UI_MappingIndexWithUI(PART_HANDS02, 1);
    m_pArms->Init_UI_MappingIndexWithUI(PART_HANDS03, 2);
    m_pArms->Init_UI_MappingIndexWithUI(PART_HANDS04, 3);
    m_pArms->Init_UI_MappingIndexWithUI(PART_HANDS05, 4);

}

void CPlayer::Init_UI_HPBar()
{
    m_pPlayerHP = static_cast<CUI_Player_HP*>(m_pGameInstance->Get_UI_By_Tag(L"PlayerHP"));
    Safe_AddRef(m_pPlayerHP);
    m_pPlayerHP->Init_HPInfo(&m_vHPInfo);

}

/* pri Update -> 현재 트랜스폼, 이동량, 상태를 세팅해준다.*/ /* update-> anim 세팅해준다 */ /* late_Update ->애님 상태에 따른 기능 세팅해준다 */
void CPlayer::Pri_Update_KeyInput(_float fTimeDelta)
{
    /* 상태 변화를 일으키지 않는 일반 무빙 키인풋 함수들*/
   

    _bool bKeyInput = false; 
    if (Pri_Update_KeyInput_GoLeft_GoRight(fTimeDelta) || Pri_Update_KeyInput_GoStraight_GoBackward(fTimeDelta) )
        bKeyInput = true;
    if (false == bKeyInput && (m_iState & STATE_DASH))
        m_pTransformCom->Go_Straight(m_fSpeed * fTimeDelta, true, m_pNavigationCom);

    Pri_Update_KeyInput_Turn_Horize_Vertical(fTimeDelta);
    /* 상태 변화를 일으키는 키 입력 */
    Pri_Update_KeyInput_ChangeState(fTimeDelta);
   
}

_bool CPlayer::Pri_Update_KeyInput_GoStraight_GoBackward(_float fTimeDelta)
{
    _bool m_bKeyInput  = false;
    if (m_bKeyInput = KEY_PRESSING(KEY::W))
    {
      
        m_pTransformCom->Go_Straight(m_fSpeed * fTimeDelta, true, m_pNavigationCom);
    }

    else if (m_bKeyInput =KEY_PRESSING(KEY::S))
    {
       
        m_pTransformCom->Go_Straight(-1.f*m_fSpeed * fTimeDelta, true, m_pNavigationCom);

    }
   
    return m_bKeyInput;

}

_bool CPlayer::Pri_Update_KeyInput_GoLeft_GoRight(_float fTimeDelta)
{
    _bool m_bKeyInput = false;
    if (KEY_PRESSING(KEY::D))
    {
        m_pTransformCom->Go_Right(m_fSpeed * fTimeDelta, m_pNavigationCom);
      
    }

    else if (KEY_PRESSING(KEY::A))
    {
        m_pTransformCom->Go_Right(-1*m_fSpeed * fTimeDelta, m_pNavigationCom);
      

    }
   
    return m_bKeyInput;
}

_bool CPlayer::Pri_Update_KeyInput_Turn_Horize_Vertical(_float fTimeDelta)
{

    _long	MouseMove = {};
    if (MouseMove = GET_MOUSE_MOVE(MOUSE_MOVE::DIMS_X))
    {


        m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * fTimeDelta * m_fMouseSensor);
    }

    else if (MouseMove = GET_MOUSE_MOVE(MOUSE_MOVE::DIMS_Y))
    {

        m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * fTimeDelta * m_fMouseSensor);
    }

    if (MouseMove)
        return true;

}

void CPlayer::Pri_Update_KeyInput_ChangeState(_float fTimeDelta)
{
    if (m_iState & STATE_GRANADE_END)
    {
        static_cast<CHands*>(m_PartObjects[PART_HANDS_SKILL])->SetActive(false);
        m_iState =  STATE_POP_OUT;
    }


    if (KEY_DOWN(KEY::ONE)) 
    {
        
        m_iState = STATE_POP_OUT;
        m_iCurTool = 0;
    }
    if (KEY_DOWN(KEY::TWO))
    {
       
        m_iState = STATE_POP_OUT;
        m_iCurTool = 1;
    }
    if (KEY_DOWN(KEY::THREE))
    {
       
        m_iState = STATE_POP_OUT;
        m_iCurTool = 2;
        
    }
 
    if (STATE_POP_OUT == m_iState)
    {

        for (auto Hand : m_MyTools)
        {
            static_cast<CHands*>(m_PartObjects[Hand])->SetActive(false);
        }
        static_cast<CHands*>(m_PartObjects[m_MyTools[m_iCurTool]])->SetActive(true);
        m_pArms->Set_CurArmIndex(m_MyTools[m_iCurTool],m_iCurTool);

    }

    /*공격은 몇초 지나야 프레싱이 가능하다 , 0.2초 간격을줘서 1번만 공격할수 있게 한다.*/
   /* if (STATE_ATTACK_DOWN == m_iState && m_fAccAttackTerm < 0.5f)
        m_fAccAttackTerm += fTimeDelta;*/
    /*애니메이션이 끝난 후  끝나기 전의 틱의 상태를 받아온다.*/
   


    /* 공격(중복 x)*/
    if (m_fAccBombCoolTime >= m_fBombCoolTime &&0 < m_ePlayerItemInfo.iNumGrenades  && KEY_UP(KEY::Q))
    {
        m_iState = STATE_GRANADE;
        m_pGameInstance->PlayEffect(L"Dash.ogg", 0.1f);
        m_pGameInstance->PlayEffect(L"Player_BombVoice.mp3", 0.2f);
        static_cast<CHands*>(m_PartObjects[PART_HANDS_SKILL])->SetActive(true);
        static_cast<CHands*>(m_PartObjects[m_MyTools[m_iCurTool]])->SetActive(false);
        m_ePlayerItemInfo.iNumGrenades-= 1;
        m_fAccBombCoolTime = 0.f;

      
    }
    if(!(m_iState& STATE_GRANADE)){
        if (MOUSE_DOWN(MOUSE::DIM_LB))
        {

            m_iState |= STATE_ATTACK_DOWN;
        }
        ///* 연속 공격(중복 x)*//* 일단 마우스를 계속 누르면 (상태변화 없이)조건문 안에 ATTACK_DOWN인 상태로 계속 조건문에 들어올 것-> 0.2초 지나야 상태인정*/
        else if (MOUSE_PRESSING(MOUSE::DIM_LB))
        {
            m_iState |= STATE_ATTACK_PRESSING;

        }
        ///* 마우스 때면 누적시간 초기화 */
        else if (MOUSE_UP(MOUSE::DIM_LB))
        {
            m_iState |= STATE_ATTACK_UP;
        }
        /*장전 (중복 x)*/
        if (MOUSE_DOWN(MOUSE::DIM_RB))
        {

            m_iState |= STATE_LOAD;
        }
        /* 그 외의 움직임이 없다면 idle 로 대기*/
        else if (STATE_POP_OUT & m_iState)
        {
            m_iState = STATE_POP_OUT;
        }

     }

   
    if (!(STATE_DASH&m_iState) && m_fAccDash_CoolTime >= m_fDash_CoolTime && KEY_DOWN(KEY::SHIFT))
    {
        m_iState |= STATE_DASH;
        m_fSpeed = 3.f;
        m_fAccDash_CoolTime = 0.f;
        m_bSound_StartDash = true;
        m_fAccDashTime = 0.f;
        m_bDashStart = true;

    }
   
   
    if ( !(STATE_JUMP&m_iState) &&KEY_DOWN(KEY::SPACE))
    {
        m_iState |= STATE_JUMP;
        m_isjumping = true;
        m_bSound_StartJump = true;
    }
}

void CPlayer::Key_Input_UI()
{
    if (KEY_DOWN(KEY::TAB))
    {
        if (!m_bInvenActive)
        {
            m_pGameInstance->Set_Active_A_UI(TEXT("Inventory"));
            m_bInvenActive = true;
        }
        else
        {
            m_pGameInstance->Set_InActive_A_UI(TEXT("Inventory"));
            m_bInvenActive = false;
        }
    }

    if (KEY_DOWN(KEY::O))
    {
        if (!m_bShopActive)
        {
            m_pGameInstance->Set_Active_A_UI(TEXT("Victory"));
            m_bShopActive = true;
        }
        else
        {
            m_pGameInstance->Set_InActive_A_UI(TEXT("Victory"));
            m_bShopActive = false;
        }
    }
}

void CPlayer::Set_ToolNoByIndex(_uint iIndex, _uint iPlayerToolIndex)
{
    m_MyTools[iIndex] = iPlayerToolIndex;
}

_uint CPlayer::Get_ToolNoByIndex(_uint iIndex)
{
    if (0 <= iIndex && g_iNumTools > iIndex)
        return m_MyTools[iIndex];
    return -1; 
}

void CPlayer::Set_ExtraGrenades(_int iNumExtraGrenades)
{
    if (m_ePlayerItemInfo.iNumGrenades + iNumExtraGrenades < m_ePlayerItemInfo.iNumMaxGrenades) m_ePlayerItemInfo.iNumGrenades += iNumExtraGrenades;
}



HRESULT CPlayer::Ready_Components()
{


    /* Com_Navigation */

    CNavigation::NAVIGATION_DESC		NaviDesc{};

    NaviDesc.iStartIndex = m_iNaviMeshIndex;

    if (1 == m_iNowMapStage) {
        if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
            TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
            return E_FAIL;
    }
    else if (2 == m_iNowMapStage)
    {
        if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Navigation"),
            TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
            return E_FAIL;
    }
    /*ComCollider*/
    CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
    ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
    ColliderDesc.fRadius = 2.f;
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    {
     //검색용으로 일단 넣어두자.
        if(m_pTransformCom)
            m_Components.emplace(TEXT("Com_Transform"), m_pTransformCom);
        Safe_AddRef(m_pTransformCom);
    }


    //m_pColliderCom_CheckRay

    /*Com_Collider_Ray*/
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Ray"),
        TEXT("Com_ColliderPlayerCheckRay"), reinterpret_cast<CComponent**>(&m_pColliderCom_CheckRay), nullptr)))
        return E_FAIL;



    return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
    CHands01::HANDS01_DESC Hands01Desc{};

    Hands01Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
    Hands01Desc.pState = &m_iState;
    Hands01Desc.pPartObjects = &m_PartObjects;
    Hands01Desc.fSpeedPerSec = 10.f;
    Hands01Desc.fRotationPerSec = XMConvertToRadians(90.f);

     m_PartObjects[PART_HANDS01] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands01"), &Hands01Desc));
   
    if (nullptr == m_PartObjects[PART_HANDS01])
        return E_FAIL;
    m_pAnimEnd[PART_HANDS01] = static_cast<CHands01*>(m_PartObjects[PART_HANDS01])->Get_PartAnimEndState();

    CHands01_Sword::HANDS01_SWORD_DESC Hands01SwordDesc = {};
    Hands01SwordDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
    Hands01SwordDesc.pState = &m_iState;
    Hands01SwordDesc.pSocketMatrix[0] = static_cast<CHands01*>(m_PartObjects[PART_HANDS01])->Get_BoneMatrix("Bone003");//354
    Hands01SwordDesc.pSocketMatrix[1] = static_cast<CHands01*>(m_PartObjects[PART_HANDS01])->Get_BoneMatrix("Bone004");
    Hands01SwordDesc.pSocketMatrix[2] = static_cast<CHands01*>(m_PartObjects[PART_HANDS01])->Get_BoneMatrix("Bone005");
    Hands01SwordDesc.pSocketMatrix[3] = static_cast<CHands01*>(m_PartObjects[PART_HANDS01])->Get_BoneMatrix("Bone006");
    Hands01SwordDesc.pSocketMatrix[4] = static_cast<CHands01*>(m_PartObjects[PART_HANDS01])->Get_BoneMatrix("Bone008");
  
    m_PartObjects[PART_HANDS01_SWORD] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands01_Sword"), &Hands01SwordDesc));
    if (nullptr == m_PartObjects[PART_HANDS01_SWORD])
        return E_FAIL;


    CHands02::HANDS02_DESC Hands02Desc{};

    Hands02Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
    Hands02Desc.pState = &m_iState;
    Hands02Desc.fRotationPerSec = 10.f;
    Hands02Desc.fSpeedPerSec = XMConvertToRadians(90.f);

   
         m_PartObjects[PART_HANDS02] = static_cast <CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC,TEXT("Prototype_GameObject_Hands02"), &Hands02Desc));

    CHands03::HANDS03_DESC Hands03Desc{};

    Hands03Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
    Hands03Desc.pState = &m_iState;
    Hands03Desc.fRotationPerSec = 10.f;
    Hands03Desc.fSpeedPerSec = XMConvertToRadians(90.f);
    m_PartObjects[PART_HANDS03] = static_cast <CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands03"), &Hands03Desc));

    CHands04::HANDS04_DESC Hands04Desc{};
    Hands04Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
    Hands04Desc.pState = &m_iState;
    Hands04Desc.fRotationPerSec = 10.f;
    Hands04Desc.fSpeedPerSec = XMConvertToRadians(90.f);
    m_PartObjects[PART_HANDS04] = static_cast <CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands04"), &Hands04Desc));


    CHands05::HANDS05_DESC Hands05Desc{};

    Hands05Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
    Hands05Desc.pState = &m_iState;
    Hands05Desc.fRotationPerSec = 10.f;
    Hands05Desc.fSpeedPerSec = XMConvertToRadians(90.f);
    m_PartObjects[PART_HANDS05] = static_cast <CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands05"), &Hands05Desc));



  

    CHands_Skill::HANDS_SKILL_DESC Hands_SkillDesc = {};
    Hands_SkillDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
    Hands_SkillDesc.pState = &m_iState;
    Hands_SkillDesc.fRotationPerSec = 10.f;
    Hands_SkillDesc.fSpeedPerSec = XMConvertToRadians(90.f);
    Hands_SkillDesc.iNowMapStage = m_iNowMapStage;
    m_PartObjects[PART_HANDS_SKILL] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands_Skill"), &Hands_SkillDesc));
    

    return S_OK;
}


CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayer* pInstance = new CPlayer(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPlayer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CPlayer* pInstance = new CPlayer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to  Cloned : CPlayer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer::Free()
{
   
    __super::Free();
    
    g_PlayerHPInfo= m_vHPInfo;
    g_PlayerItemInfo = m_ePlayerItemInfo;

    Safe_Release(m_pColliderCom_CheckRay);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pArms);
    Safe_Release(m_pPlayerHP);
}
