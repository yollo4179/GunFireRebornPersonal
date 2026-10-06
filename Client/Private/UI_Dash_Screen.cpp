#include"stdafx.h"
#include "UI_Dash_Screen.h"
#include"GameInstance.h"



CUI_Dash_Screen::CUI_Dash_Screen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Dash_Screen::CUI_Dash_Screen(const CUI_Dash_Screen& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Dash_Screen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Dash_Screen::Initialize(void* pArg)
{
	UI_DASH_SCREEN_DESC* pDesc = static_cast<UI_DASH_SCREEN_DESC*>(pArg);

	/* 백그라우늗의 ㅁ크기아ㅘ 위치를 잡아주기위한 용오 .*/
	if (nullptr == pDesc) {
		pDesc->fX = 100;//g_iWinSizeX >> 1;
		pDesc->fY = 0;//g_iWinSizeY >> 1;
		pDesc->fSizeX = 200;//g_iWinSizeX;
		pDesc->fSizeY = 100;//g_iWinSizeY;
	}

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;



	return S_OK;
}

void CUI_Dash_Screen::Priority_Update(_float fTimeDelta)
{
	
}

void CUI_Dash_Screen::Update(_float fTimeDelta)
{

}

void CUI_Dash_Screen::Late_Update(_float fTimeDelta)
{
	if (m_bActive)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Dash_Screen::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Dash_Screen::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dash"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;


}

HRESULT CUI_Dash_Screen::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	
	return S_OK;
}

CUI_Dash_Screen* CUI_Dash_Screen::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Dash_Screen* pInstance = new CUI_Dash_Screen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Dash_Screen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dash_Screen::Clone(void* pArg)
{
	CUI_Dash_Screen* pInstance = new CUI_Dash_Screen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Dash_Screen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dash_Screen::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
