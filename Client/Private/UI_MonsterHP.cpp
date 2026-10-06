#include"stdafx.h"
#include "UI_MonsterHP.h"
#include"GameInstance.h"

#include"UI_MonsterHP_BG.h"
#include"UI_MonsterHP_Red.h"
#include"UI_MonsterHP_Blue.h"
#include"UI_MonsterHP_Gray_BG.h"

CUI_MonsterHP::CUI_MonsterHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI_Parent{pDevice, pContext}
{
}

CUI_MonsterHP::CUI_MonsterHP(const CUI_MonsterHP& Prototype)
    : CUI_Parent(Prototype)
{
}



HRESULT CUI_MonsterHP::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_MonsterHP::Initialize(void* pArg)
{
    UI_MONSTER_HP_DESC* pDesc = static_cast<UI_MONSTER_HP_DESC*>(pArg);
    m_UIWorlFloat4x4 = pDesc->UIWorlFloat4x4;
    m_pViewZDistance = pDesc->pViewZDistance;
    m_pHPInfo = pDesc->pHPInfo;
    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    if (FAILED(Ready_ChildUIs()))
        return E_FAIL;

    for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
        m_EventFunction[i] = nullptr;

    m_bActive = false;
    return S_OK;
}

void CUI_MonsterHP::Priority_Update(_float fTimeDelta)
{
    if (m_bActive)
    {
        for (auto& Child : m_ChildUIs)
            Child.second->Priority_Update(fTimeDelta);
    }
}

void CUI_MonsterHP::Update(_float fTimeDelta)
{
    if (m_bActive)
    {
        for (auto& Child : m_ChildUIs)
            Child.second->Update(fTimeDelta);
    }
}

void CUI_MonsterHP::Late_Update(_float fTimeDelta)
{

    if (m_bActive)
    {
        for (auto& Child : m_ChildUIs)
            Child.second->Late_Update(fTimeDelta);
    }
   /* if (m_bActive)
    {
        m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
    }*/
}

HRESULT CUI_MonsterHP::Render()
{
    if (nullptr == m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Monster"), 0))
        return S_OK;

    if (m_bActive)
    {
        for (auto& Child : m_ChildUIs)
            Child.second->Render();
    }
    return S_OK;
}

HRESULT CUI_MonsterHP::Ready_ChildUIs()
{


    CUI_MonsterHP_BG::UI_MONSTERHP_BG_DESC HP_BG_Desc = {};
   
    HP_BG_Desc.fRotationPerSec = XMConvertToRadians(90.f);
    HP_BG_Desc.fSpeedPerSec = 10.f;
    HP_BG_Desc.fSizeX = 250.f;
    HP_BG_Desc.fSizeY = 100.f;
    HP_BG_Desc.fX = 0;
    HP_BG_Desc.fY = 0;
    HP_BG_Desc.eType = UI_TYPE::UI_MONSTER;
    HP_BG_Desc.pMonsterHpFloat4x4 = m_UIWorlFloat4x4;
    HP_BG_Desc.pViewZDistance = m_pViewZDistance; 

    CUIObject* pObject = { nullptr };
    pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MonsterHP_BG"), &HP_BG_Desc));
    m_ChildUIs.emplace(L"AHP_BG", pObject);

    CUI_MonsterHP_Blue::UI_MONSTER_HP_BLUE_DESC HP_Blue_Desc = {};

    HP_Blue_Desc.fRotationPerSec = XMConvertToRadians(90.f);
    HP_Blue_Desc.fSpeedPerSec = 10.f;
    HP_Blue_Desc.fSizeX = 220.f;
    HP_Blue_Desc.fSizeY = 12.f;
    HP_Blue_Desc.fX = 0;
    HP_Blue_Desc.fY = 0;
    HP_Blue_Desc.eType = UI_TYPE::UI_MONSTER;
    HP_Blue_Desc.pMonsterHpFloat4x4 = m_UIWorlFloat4x4;
    HP_Blue_Desc.pViewZDistance = m_pViewZDistance;
    HP_Blue_Desc.pMonsterHPInfo = m_pHPInfo;

    pObject = { nullptr };
    pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MonsterHP_Blue"), &HP_Blue_Desc));
    m_ChildUIs.emplace(L"CHP_BLUE", pObject);

    CUI_MonsterHP_Red::UI_MONSTER_HP_RED_DESC HP_Red_Desc = {};

    HP_Red_Desc.fRotationPerSec = XMConvertToRadians(90.f);
    HP_Red_Desc.fSpeedPerSec = 10.f;
    HP_Red_Desc.fSizeX = 220.f;
    HP_Red_Desc.fSizeY = 14.f;
    HP_Red_Desc.fX = 0;
    HP_Red_Desc.fY = 0;
    HP_Red_Desc.eType = UI_TYPE::UI_MONSTER;
    HP_Red_Desc.pMonsterHpFloat4x4 = m_UIWorlFloat4x4;
    HP_Red_Desc.pViewZDistance = m_pViewZDistance;
    HP_Red_Desc.pMonsterHPInfo = m_pHPInfo;
    pObject = { nullptr };
    pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MonsterHP_Red"), &HP_Red_Desc));
    m_ChildUIs.emplace(L"BHP_RED", pObject);


   

    CUI_MonsterHP_Gray_BG::UI_MONSTERHP_GRAY_BG_DESC HP_Gray_BG_Desc = {};

    HP_Gray_BG_Desc.fRotationPerSec = XMConvertToRadians(90.f);
    HP_Gray_BG_Desc.fSpeedPerSec = 10.f;
    HP_Gray_BG_Desc.fSizeX = 220.f;
    HP_Gray_BG_Desc.fSizeY = 15.f;
    HP_Gray_BG_Desc.fX = 0;
    HP_Gray_BG_Desc.fY = 0;
    HP_Gray_BG_Desc.eType = UI_TYPE::UI_MONSTER;
    HP_Gray_BG_Desc.pMonsterHpFloat4x4 = m_UIWorlFloat4x4;
    HP_Gray_BG_Desc.pViewZDistance = m_pViewZDistance;
    HP_Gray_BG_Desc.fOffset = -12.f;

     pObject = { nullptr };
    pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterHP_Gray_BG"), &HP_Gray_BG_Desc));
    m_ChildUIs.emplace(L"AHP_GRAY_BG_ForBlue", pObject);


    HP_Gray_BG_Desc.fRotationPerSec = XMConvertToRadians(90.f);
    HP_Gray_BG_Desc.fSpeedPerSec = 10.f;
    HP_Gray_BG_Desc.fSizeX = 220.f;
    HP_Gray_BG_Desc.fSizeY = 15.f;
    HP_Gray_BG_Desc.fX = 0;
    HP_Gray_BG_Desc.fY = 0;
    HP_Gray_BG_Desc.eType = UI_TYPE::UI_MONSTER;
    HP_Gray_BG_Desc.pMonsterHpFloat4x4 = m_UIWorlFloat4x4;
    HP_Gray_BG_Desc.pViewZDistance = m_pViewZDistance;
    HP_Gray_BG_Desc.fOffset = 8.f;

        pObject = { nullptr };
    pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterHP_Gray_BG"), &HP_Gray_BG_Desc));
    m_ChildUIs.emplace(L"AHP_GRAY_BG_ForRed", pObject);
    
    return S_OK;
}

HRESULT CUI_MonsterHP::Bind_ShaderResources()
{
    return S_OK;
}

CUI_MonsterHP* CUI_MonsterHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_MonsterHP* pInstance = new  CUI_MonsterHP(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created :  CUI_MonsterHP");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_MonsterHP::Clone(void* pArg)
{
    CUI_MonsterHP* pInstance = new  CUI_MonsterHP(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned :  CUI_MonsterHP");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_MonsterHP::Free()
{
    __super::Free();
}