#include"stdafx.h"
#include "UI_Shop_ShowCard.h"
#include"GameInstance.h"

CUI_Shop_ShowCard::CUI_Shop_ShowCard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
 :CUI_Parent{pDevice, pContext}
{
}

CUI_Shop_ShowCard::CUI_Shop_ShowCard(const CUI_Shop_ShowCard& Prototype)
    :CUI_Parent(Prototype)
{
}

HRESULT CUI_Shop_ShowCard::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Shop_ShowCard::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;



    if (FAILED(Ready_Components()))
        return E_FAIL;


    for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
        m_EventFunction[i] = nullptr;
    return S_OK;
}

void CUI_Shop_ShowCard::Priority_Update(_float fTimeDelta)
{
    
}

void CUI_Shop_ShowCard::Update(_float fTimeDelta)
{
}

void CUI_Shop_ShowCard::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Shop_ShowCard::Render()
{
    if (FAILED(Render_ShaderResources_ShowCard()))
        return E_FAIL;
    if (FAILED(Render_ShaderResources_Frame()))
        return E_FAIL;
    if (FAILED(Render_ShaderResources_Glow()))
        return E_FAIL;
    if (FAILED(Render_ShaderResources_Item()))
        return E_FAIL;
    if (FAILED(Render_ShaderResources_Click()))
        return E_FAIL;
    if (1== m_eItemInfo.iIsRare)
    {
        m_pGameInstance->Render_Font(TEXT("Font32"), m_eItemInfo.sz_Name, _float2(m_fX - 0.5f * m_fSizeX + 30.f, m_fY - 0.5 * m_fSizeY + 20.f), XMVectorSet(0.f, 1.f, 0.f, 1.f));

        m_pGameInstance->Render_Font(TEXT("Font32"), m_eItemInfo.sz_Explain_Text, _float2(m_fX - 0.5f * m_fSizeX + 250.f, m_fY - 70.f), XMVectorSet(1.f, 0.f, 0.f, 1.f));
    }
    else
    {
        m_pGameInstance->Render_Font(TEXT("Font32"), m_eItemInfo.sz_Name, _float2(m_fX - 0.5f * m_fSizeX + 30.f, m_fY - 0.5 * m_fSizeY + 20.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

        m_pGameInstance->Render_Font(TEXT("Font32"), m_eItemInfo.sz_Explain_Text, _float2(m_fX - 0.5f * m_fSizeX + 250.f, m_fY - 70.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
    }
 

    
    return S_OK;
}

void CUI_Shop_ShowCard::Set_ItemInfo(CUI_Shop::ItemInfo eItemInfo)
{
    m_eItemInfo = eItemInfo;
}

HRESULT CUI_Shop_ShowCard::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /*Com_Texture*/
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Item_ShowCard"),
        TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_ShowCard))))
        return E_FAIL;

    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Item"),
        TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Item))))
        return E_FAIL;
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_WeaponBackground"),
        TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Glow))))
        return E_FAIL;

    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Item_Slot"),
        TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom_Frame))))
        return E_FAIL;

    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Click"),
        TEXT("Com_Texture5"), reinterpret_cast<CComponent**>(&m_pTextureCom_Click))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Shop_ShowCard::Render_ShaderResources_ShowCard()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom_ShowCard->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_eItemInfo.iIsRare)))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();

    m_pVIBufferCom->Render();

    return S_OK;

}

HRESULT CUI_Shop_ShowCard::Render_ShaderResources_Frame()
{

    _matrix  matWorldOfFrame = m_pTransformCom->Get_WorldMatrix();
    matWorldOfFrame.r[0] *= 0.3f;
    matWorldOfFrame.r[1] *= 0.6f;
    matWorldOfFrame.r[3] = XMVectorSetX(matWorldOfFrame.r[3], XMVectorGetX(matWorldOfFrame.r[3]) - m_fSizeX * 0.5f + 150.f);
    matWorldOfFrame.r[3] = XMVectorSetY(matWorldOfFrame.r[3], XMVectorGetY(matWorldOfFrame.r[3]));

    _float4x4 FrameFloat4x4;
    XMStoreFloat4x4(&FrameFloat4x4, matWorldOfFrame);
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &FrameFloat4x4)))

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom_Frame->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_Shop_ShowCard::Render_ShaderResources_Item()
{
    _matrix  matWorldOfItem = m_pTransformCom->Get_WorldMatrix();
    matWorldOfItem.r[0] *= 0.3f;
    matWorldOfItem.r[1] *= 0.3f;
    matWorldOfItem.r[3] = XMVectorSetX(matWorldOfItem.r[3], XMVectorGetX(matWorldOfItem.r[3]) - m_fSizeX * 0.5f + 150.f);
    matWorldOfItem.r[3] = XMVectorSetY(matWorldOfItem.r[3], XMVectorGetY(matWorldOfItem.r[3]));

    _float4x4 ItemFloat4x4;
    XMStoreFloat4x4(&ItemFloat4x4, matWorldOfItem);
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &ItemFloat4x4)))

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom_Item->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_eItemInfo.iItemNo)))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_Shop_ShowCard::Render_ShaderResources_Glow()
{
    _matrix  matWorldOfGlow = m_pTransformCom->Get_WorldMatrix();
    matWorldOfGlow.r[0] *= 0.3f;
    matWorldOfGlow.r[1] *= 0.3f;
    matWorldOfGlow.r[3] = XMVectorSetX(matWorldOfGlow.r[3], XMVectorGetX(matWorldOfGlow.r[3]) - m_fSizeX * 0.5f + 150.f);
    matWorldOfGlow.r[3] = XMVectorSetY(matWorldOfGlow.r[3], XMVectorGetY(matWorldOfGlow.r[3]));

    _float4x4 _GlowFloat4x4;
    XMStoreFloat4x4(&_GlowFloat4x4, matWorldOfGlow);
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &_GlowFloat4x4)))

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom_Glow->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_Shop_ShowCard::Render_ShaderResources_Click()
{

    _matrix  matWorldOfClick = m_pTransformCom->Get_WorldMatrix();
    matWorldOfClick.r[0] *= 0.1f;
    matWorldOfClick.r[1] *= 0.1f;
    matWorldOfClick.r[3] = XMVectorSetX(matWorldOfClick.r[3], XMVectorGetX(matWorldOfClick.r[3]) - m_fSizeX * 0.5f + 40.f);
    matWorldOfClick.r[3] = XMVectorSetY(matWorldOfClick.r[3], XMVectorGetY(matWorldOfClick.r[3]) - m_fSizeY * 0.5f + 20.f);

    _float4x4 ClickFloat4x4;
    XMStoreFloat4x4(&ClickFloat4x4, matWorldOfClick);
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &ClickFloat4x4)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom_Click->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();

    m_pVIBufferCom->Render();

    return S_OK;
}

CUI_Shop_ShowCard* CUI_Shop_ShowCard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Shop_ShowCard* pInstance = new  CUI_Shop_ShowCard(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created :  CUI_Shop_ShowCard");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Shop_ShowCard::Clone(void* pArg)
{
    CUI_Shop_ShowCard* pInstance = new  CUI_Shop_ShowCard(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned :  CUI_Shop_ShowCard");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Shop_ShowCard::Free()
{
    __super::Free();
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom_ShowCard);
    Safe_Release(m_pTextureCom_Item);
    Safe_Release(m_pTextureCom_Glow);
    Safe_Release(m_pTextureCom_Frame);
    Safe_Release(m_pTextureCom_Click);
    Safe_Release(m_pVIBufferCom);

}
