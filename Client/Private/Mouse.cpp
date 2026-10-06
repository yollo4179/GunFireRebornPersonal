#include"stdafx.h"
#include "Mouse.h"
#include"GameInstance.h"
#include"Camera_Player.h"
CMouse::CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject{pDevice, pContext}
{
}

CMouse::CMouse(const CMouse& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CMouse::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMouse::Initialize(void* pArg)
{
    CUIObject::UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

    if (nullptr == pDesc)
    {
        MSG_BOX("Give Mouse Info: nullptr");
        return E_FAIL;
    }
    
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    if (FAILED(Ready_Components()))
        return E_FAIL;


    return S_OK;
}

void CMouse::Priority_Update(_float fTimeDelta)
{
    Update_MousePos();

}

void CMouse::Update(_float fTimeDelta)
{
}

void CMouse::Late_Update(_float fTimeDelta)
{
    if (m_bActive)
        m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CMouse::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();

    m_pVIBufferCom->Render();

    return S_OK;
}

void CMouse::Update_MousePos()
{
    POINT pt = {};
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd,&pt);

    pt.x = pt.x + (int)(m_fSizeX * 0.5f);
    pt.y = pt.y +(int)(m_fSizeY * 0.5f);

    _vector vPos = { pt.x - g_iWinSizeX * 0.5f,-pt.y + g_iWinSizeY * 0.5f, 0.f , 1.f };
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
    
    m_vPos = { (_float)pt.x, (_float)pt.y };

}
HRESULT CMouse::Ready_Components()
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
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mouse"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mouse_Target"),
        TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Target))))
        return E_FAIL;

    

    return S_OK;
}

HRESULT CMouse::Bind_ShaderResources()
{
    _bool isCrossHair = false;

    CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
    if (nullptr == pTargetCamera)
    {
        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
            return E_FAIL;

    }
    
     _bool isCamearaStill = true;
     if(nullptr!= pTargetCamera)
     isCamearaStill = pTargetCamera->Get_Camera_Still();

    if (false == isCamearaStill)
    {
        if (FAILED(m_pTextureCom_Target->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
            return E_FAIL;
        isCrossHair = true;
    }
    else
    {
        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
            return E_FAIL;
    }

    if (true == isCrossHair)
    {

        m_pTransformCom->Scaling(50.f, 50.f, 1.f);
        if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
            return E_FAIL;
        m_pTransformCom->Scaling(20.f, 20.f, 1.f);
    }
    else
    {
        if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
            return E_FAIL;

        
    }
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

   

  
    return S_OK;
}

CMouse* CMouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMouse* pInstance = new CMouse(pDevice, pContext);

    if (pInstance->Initialize_Prototype())
    {
        MSG_BOX("Create Mouse Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

CGameObject* CMouse::Clone(void* pArg)
{
    CMouse* pInstance = new CMouse(*this);

    if (pInstance->Initialize(pArg))
    {
        MSG_BOX("Clone Mouse Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CMouse::Free()
{

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTextureCom_Target);
    Safe_Release(m_pVIBufferCom);
    __super::Free();
}
