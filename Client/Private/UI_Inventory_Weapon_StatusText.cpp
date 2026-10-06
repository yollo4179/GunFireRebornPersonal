#include"stdafx.h"
#include"UI_Inventory_Weapon_StatusText.h"
#include"GameInstance.h"
CUI_Inventory_Weapon_StatusText::CUI_Inventory_Weapon_StatusText(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{pDevice, pContext}
{
}

CUI_Inventory_Weapon_StatusText::CUI_Inventory_Weapon_StatusText(const CUI_Inventory_Weapon_StatusText& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CUI_Inventory_Weapon_StatusText::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Inventory_Weapon_StatusText::Initialize(void* pArg)
{
	UI_INVENTORY_WEAPON_STATUSTEXT* pDesc = static_cast<UI_INVENTORY_WEAPON_STATUSTEXT*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;


	return S_OK;
}

void CUI_Inventory_Weapon_StatusText::Priority_Update(_float fTimeDelta)
{
}

void CUI_Inventory_Weapon_StatusText::Update(_float fTimeDelta)
{
}

void CUI_Inventory_Weapon_StatusText::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Inventory_Weapon_StatusText::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	m_pGameInstance->Render_Font(TEXT("Font32"), m_szStatusText, _float2(m_fX - 0.5f * m_fSizeX+10.f, m_fY-10.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Inventory_Weapon_StatusText::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Status_Explain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}



HRESULT CUI_Inventory_Weapon_StatusText::Bind_ShaderResources()
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

CUI_Inventory_Weapon_StatusText* CUI_Inventory_Weapon_StatusText::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Inventory_Weapon_StatusText* pInstance = new  CUI_Inventory_Weapon_StatusText(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Inventory_Weapon_StatusText");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Inventory_Weapon_StatusText::Clone(void* pArg)
{
	CUI_Inventory_Weapon_StatusText* pInstance = new  CUI_Inventory_Weapon_StatusText(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Inventory_Weapon_StatusText");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_Weapon_StatusText::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
