#include"stdafx.h"
#include "UI_Inventory.h"

#include"UI_Inventory_Button_Hero.h"
#include"UI_Inventory_Button_Weapons.h"
#include"UI_Inventory_Weapons.h"
#include"GameInstance.h"
#include"Camera_Player.h"
CUI_Inventory::CUI_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI_Parent{pDevice, pContext}
{
}

CUI_Inventory::CUI_Inventory(const CUI_Inventory& Prototype)
    :CUI_Parent(Prototype)
{
}

HRESULT CUI_Inventory::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Inventory::Initialize(void* pArg)
{
    UI_INVENTORY_DESC* pDesc = static_cast<UI_INVENTORY_DESC*>(pArg);

    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    if (FAILED(Ready_ChildUIs()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
        m_EventFunction[i] = nullptr;


    return S_OK;
}

void CUI_Inventory::Priority_Update(_float fTimeDelta)
{
    if (m_bActive)
    {
        for (auto& Child : m_ChildUIs)
            Child.second->Priority_Update(fTimeDelta);
    }
    if (STATE_WEAPON & m_ShowSelection)
    {
        m_ChildUIs[L"Inventory_Weapon"]->Priority_Update(fTimeDelta);
    }


}

void CUI_Inventory::Update(_float fTimeDelta)
{
    if (m_bActive)
    {
        for (auto& Child : m_ChildUIs)
            Child.second->Update(fTimeDelta);
    }
    if (STATE_WEAPON & m_ShowSelection)
    {
        m_ChildUIs[L"Inventory_Weapon"]->Update(fTimeDelta);
    }
}

void CUI_Inventory::Late_Update(_float fTimeDelta)
{
    if (m_bActive)
    {
        m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
    }
    
    /*update에서 마우스 상태 받아 왔으니까 show Selection 처리*/
    /* 클릭됐으면 해당 장면 을 보여주는 식별자로 변경-> 클릭된경우만 식별자로 변경하니까 따로 처리 필요 x  */
    if (m_ChildUIs[L"Button_Hero"]->Get_Button())
    {
       
        m_ShowSelection = STATE_HERO;
        m_ChildUIs[L"Button_Hero"]->Set_Button(false);
        static_cast<CUI_Inventory_Button_Weapons*>(m_ChildUIs[L"Button_Weapon"])->Set_Selected(false);
        static_cast<CUI_Inventory_Button_Hero*>(m_ChildUIs[L"Button_Hero"])->Set_Selected(true);
       
    }
    if (m_ChildUIs[L"Button_Weapon"]->Get_Button())
    {
       
        m_ShowSelection = STATE_WEAPON;
        
        m_ChildUIs[L"Button_Weapon"]->Set_Button(false);
        static_cast<CUI_Inventory_Button_Weapons*>(m_ChildUIs[L"Button_Weapon"])->Set_Selected(true);
        static_cast<CUI_Inventory_Button_Hero*>(m_ChildUIs[L"Button_Hero"])->Set_Selected(false);
    }

    if (STATE_WEAPON & m_ShowSelection)
    {
        m_ChildUIs[L"Inventory_Weapon"]->Late_Update(fTimeDelta);
    }

}

HRESULT CUI_Inventory::Render()
{

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();

    m_pVIBufferCom->Render();


    m_ChildUIs[L"Button_Weapon"]->Render();
    m_ChildUIs[L"Button_Hero"]->Render();
    
    /*Seperator 렌더*/
    /*0이면 플레이어 무기 렌더*/
    if (STATE_WEAPON &m_ShowSelection)
    {
        m_ChildUIs[L"Inventory_Weapon"]->Render();
    }
    /* 1이면 영웅 상태 렌더*/
    else if (STATE_HERO & m_ShowSelection)
    {
      
    }

    return S_OK;
}

void CUI_Inventory::Set_Active()
{
    m_bActive = true;

    CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
    if (nullptr == pTargetCamera)
        return;
    pTargetCamera->Set_Camera_Still();
}

void CUI_Inventory::Set_InActive()
{
    m_bActive = false;
    CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
    if (nullptr == pTargetCamera)
        return;
    pTargetCamera->Set_Camera_Player();
}

HRESULT CUI_Inventory::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_BackGround"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_BackGround))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Inventory::Ready_ChildUIs()
{
    CUI_Inventory_Button_Weapons::UI_BUTTTON_INVENTORY_WEAPONS_DESC WeaponButtonDesc = {};
    WeaponButtonDesc.fRotationPerSec = XMConvertToRadians(90.f);
    WeaponButtonDesc.fSpeedPerSec = 10.f;
    WeaponButtonDesc.fSizeX = 180;
    WeaponButtonDesc.fSizeY = 60;
    WeaponButtonDesc.fX = (m_fX - m_fSizeX*0.5F)+ 400;
    WeaponButtonDesc.fY = (m_fY - m_fSizeY*0.5F) + 30;
    WeaponButtonDesc.iData = 0;

    CUI_Inventory_Button_Hero::UI_BUTTTON_INVENTORY_HERO_DESC HeroButtonDesc = {};
    HeroButtonDesc.fRotationPerSec = XMConvertToRadians(90.f);
    HeroButtonDesc.fSpeedPerSec = 10.f;
    HeroButtonDesc.fSizeX = 180;
    HeroButtonDesc.fSizeY = 60;
    HeroButtonDesc.fX =(m_fX - m_fSizeX*0.5F)+ 650;
    HeroButtonDesc.fY =(m_fY - m_fSizeY*0.5F)+ 30;
    HeroButtonDesc.iData = 0;

    CUI_Inventory_Weapons::UI_INVENTORY_WEAPON_DESC WeaponsDesc = {};
    WeaponsDesc.fRotationPerSec = XMConvertToRadians(90.f);
    WeaponsDesc.fSpeedPerSec = 10.f;
    WeaponsDesc.fSizeX =  m_fSizeX;
    WeaponsDesc.fSizeY =  m_fSizeY-120;
    WeaponsDesc.fX = (m_fX - m_fSizeX * 0.5F);
    WeaponsDesc.fY = (m_fY - m_fSizeY * 0.5F) - 120;
    WeaponsDesc.iData = 0;


    CUIObject* pObject = { nullptr };
    pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Button_Weapons"), &WeaponButtonDesc));
    m_ChildUIs.emplace(L"Button_Weapon", pObject);
    pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Hero"), &HeroButtonDesc));
    m_ChildUIs.emplace(L"Button_Hero", pObject);
    pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapons"), &WeaponsDesc));
    m_ChildUIs.emplace(L"Inventory_Weapon", pObject);

    return S_OK;
}

HRESULT CUI_Inventory::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom_BackGround->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    return S_OK;
}

void CUI_Inventory::Update_HandsInfo(_uint iIndex, HandsInfo eHandsInfo)
{
        if (iIndex < 0 || iIndex>4)
            return;
        m_HandsInfo[iIndex] = eHandsInfo;   
}

CUI_Inventory* CUI_Inventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Inventory* pInstance = new  CUI_Inventory(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created :  CUI_Inventory");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Inventory::Clone(void* pArg)
{
    CUI_Inventory* pInstance = new  CUI_Inventory(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned :  CUI_Inventory");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Inventory::Free()
{
    __super::Free();
    Safe_Release(m_pTextureCom_BackGround);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
}
