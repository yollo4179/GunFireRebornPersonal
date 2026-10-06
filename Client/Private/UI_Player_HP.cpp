#include"stdafx.h"
#include "UI_Player_HP.h"
#include"UI_Numbers.h"
#include"GameInstance.h"

CUI_Player_HP::CUI_Player_HP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI_Parent(pDevice, pContext)
{
}

CUI_Player_HP::CUI_Player_HP(const CUI_Player_HP& Prototype)
    :CUI_Parent(Prototype)
{
}

HRESULT CUI_Player_HP::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Player_HP::Initialize(void* pArg)
{
    UI_PLAYER_HP_DESC* pDesc = static_cast<UI_PLAYER_HP_DESC*>(pArg);
    m_pPlayerHPInfo = pDesc->pPlayerHPInfo;

    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;
    if (FAILED(Ready_Components()))
        return E_FAIL;
    if (FAILED(Ready_ChildUIs()))
        return E_FAIL;
    return S_OK;
}

void CUI_Player_HP::Priority_Update(_float fTimeDelta)
{
    if (m_bActive)
    {
        for (auto& Child : m_ChildUIs)
            Child.second->Priority_Update(fTimeDelta);

        static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Green"])->Set_Numerator((*m_pPlayerHPInfo).x);
        static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Green"])->Set_MaxNumber((*m_pPlayerHPInfo).y);
        static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Red"])->Set_Numerator((*m_pPlayerHPInfo).z);
        static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Red"])->Set_MaxNumber((*m_pPlayerHPInfo).w);

    }
}

void CUI_Player_HP::Update(_float fTimeDelta)
{
    if (m_bActive)
    {
        for (auto& Child : m_ChildUIs)
            Child.second->Update(fTimeDelta);
    }
}

void CUI_Player_HP::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
    
    if (m_bActive)
    {
        for (auto& Child : m_ChildUIs)
            Child.second->Late_Update(fTimeDelta);
    }
}

HRESULT CUI_Player_HP::Render()
{
    if (nullptr == m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0))
        return S_OK;

    BindAndRender_ShaderResources_BG();
    BindAndRender_ShaderResources_Player();
    BindAndRender_ShaderResources_HP_Red();
    Bind_ShaderResources_HP_Green();


    if (m_bActive)
    {
        for (auto& Child : m_ChildUIs)
            Child.second->Render();
    }
    return S_OK;

}

HRESULT CUI_Player_HP::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;


    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerHP_BG"),
        TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_BG))))
        return E_FAIL;
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI__UI_PlayerHP_Player"),
        TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Player))))
        return E_FAIL;
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerHP_Red"),
        TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Red))))
        return E_FAIL;
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerHP_Green"),
        TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom_Green))))
        return E_FAIL;
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerHP_BackBar"),
        TEXT("Com_Texture5"), reinterpret_cast<CComponent**>(&m_pTextureCom_Back))))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_Player_HP::BindAndRender_ShaderResources_BG()
{
    _matrix matPlayerPos = XMMatrixIdentity();
    matPlayerPos.r[0] *= 450;
    matPlayerPos.r[1] *= 100;
    
    matPlayerPos.r[3] = m_pTransformCom->Get_WorldMatrix().r[3];
    _float4x4 Float4x4World;
    XMStoreFloat4x4(&Float4x4World, matPlayerPos);

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom_BG->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
        return E_FAIL;
    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();

    m_pVIBufferCom->Render();
    return S_OK;
}

HRESULT CUI_Player_HP::BindAndRender_ShaderResources_Player()
{
    if (nullptr == m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0))
        return S_OK;

    _matrix matPlayerPos = XMMatrixIdentity();
    matPlayerPos.r[0] *= 180;
    matPlayerPos.r[1] *= 180;
    _vector vPos = { -130,50,0 };
    matPlayerPos.r[3] = vPos + m_pTransformCom->Get_WorldMatrix().r[3];
    _float4x4 Float4x4World;
    XMStoreFloat4x4(&Float4x4World, matPlayerPos);

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom_Player->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
        return E_FAIL;
    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();

    m_pVIBufferCom->Render();
    return S_OK;
}

HRESULT CUI_Player_HP::BindAndRender_ShaderResources_HP_Red()
{
    if (nullptr == m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0))
        return S_OK;
    _float fBarRatio = (*m_pPlayerHPInfo).z / (*m_pPlayerHPInfo).w;
    _matrix matBarPos = XMMatrixIdentity();
   
    _float fSizeX = 250;
    _float fOffset = fSizeX* (1 - fBarRatio) * 0.5f;
   
    matBarPos.r[0] *= fSizeX * fBarRatio;
    matBarPos.r[1] *= 20;
    _vector vPos = {80.f - 1.f * fOffset , -20.f, 0 };
    matBarPos.r[3] = vPos + m_pTransformCom->Get_WorldMatrix().r[3];
    _float4x4 Float4x4World;
    XMStoreFloat4x4(&Float4x4World, matBarPos);

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom_Red->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
        return E_FAIL;
    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();
    m_pVIBufferCom->Render();
    return S_OK;
}

HRESULT CUI_Player_HP::Bind_ShaderResources_HP_Green()
{
    if (nullptr == m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0))
        return S_OK;
    float fBarRatio = (*m_pPlayerHPInfo).x / (*m_pPlayerHPInfo).y;
    _matrix matBarPos = XMMatrixIdentity();

    _float fSizeX = 250;
    _float fOffset = fSizeX * (1 - fBarRatio) * 0.5f;

    matBarPos.r[0] *= fSizeX * fBarRatio;
    matBarPos.r[1] *= 20;
    _vector vPos = {80.f -1.f * fOffset,20.f,0 };
    matBarPos.r[3] = vPos + m_pTransformCom->Get_WorldMatrix().r[3];
    _float4x4 Float4x4World;
    XMStoreFloat4x4(&Float4x4World, matBarPos);

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom_Green->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
        return E_FAIL;
    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();
    m_pVIBufferCom->Render();
    return S_OK;
}

HRESULT CUI_Player_HP::Ready_ChildUIs()
{
    CUIObject* pUIObject = { nullptr };
    
   
    
    CUI_Numbers::UI_NUMBERS_DESC NumberDesc = {};
    NumberDesc.fRadian = 0.f;
    NumberDesc.fRotationPerSec = XMConvertToRadians(90.f);
    NumberDesc.fSpeedPerSec = 10.f;
    NumberDesc.fSizeX = 15.f;
    NumberDesc.fSizeY = 15.f;
    NumberDesc.fX = m_fX+200;
    NumberDesc.fY =m_fY-20;
    NumberDesc.iNumerator = 0;
    NumberDesc.iDenominator = 0;
    NumberDesc.isFraction = true;
    NumberDesc.vColor = { 0.f,1.f,0.f };
    NumberDesc.isDenominatorInfinity = false;

    pUIObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"), &NumberDesc));
    m_ChildUIs.emplace(L"Numbers_Green", pUIObject);


    
    NumberDesc.fRadian = 0.f;
    NumberDesc.fRotationPerSec = XMConvertToRadians(90.f);
    NumberDesc.fSpeedPerSec = 10.f;
    NumberDesc.fSizeX = 15.f;
    NumberDesc.fSizeY = 15.f;
    NumberDesc.fX = m_fX + 200;
    NumberDesc.fY = m_fY+20;
    NumberDesc.iNumerator = 0;
    NumberDesc.iDenominator = 0;
    NumberDesc.isFraction = true;
    NumberDesc.vColor = { 1.f,0.f,0.f };
    NumberDesc.isDenominatorInfinity = false;

    pUIObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"), &NumberDesc));
    m_ChildUIs.emplace(L"Numbers_Red", pUIObject);

    return S_OK;
}

CUI_Player_HP* CUI_Player_HP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Player_HP* pInstance = new  CUI_Player_HP(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created :  CUI_Player_HP");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Player_HP::Clone(void* pArg)
{
    CUI_Player_HP* pInstance = new  CUI_Player_HP(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned :  CUI_Player_HP");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Player_HP::Free()
{

    __super::Free();
  
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom_BG);
    Safe_Release(m_pTextureCom_Player);
    Safe_Release(m_pTextureCom_Red);
    Safe_Release(m_pTextureCom_Green);
    Safe_Release(m_pTextureCom_Back);
    Safe_Release(m_pVIBufferCom);
}
