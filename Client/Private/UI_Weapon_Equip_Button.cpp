#include"stdafx.h"
#include"UI_Weapon_Equip_Button.h"
#include "GameInstance.h"
CUI_Weapon_Equip_Button::CUI_Weapon_Equip_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Weapon_Equip_Button::CUI_Weapon_Equip_Button(const CUI_Weapon_Equip_Button& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Weapon_Equip_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Weapon_Equip_Button::Initialize(void* pArg)
{
	UI_BUTTTON_INVENTORY_EQUIP_BUTTON_DESC* pDesc = static_cast<UI_BUTTTON_INVENTORY_EQUIP_BUTTON_DESC*>(pArg);


	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;

	m_EventFunction[(_uint)UI_STATE::ON] = std::bind(&CUI_Weapon_Equip_Button::Event_MouseOn, this);
	m_EventFunction[(_uint)UI_STATE::UP] = std::bind(&CUI_Weapon_Equip_Button::Event_MouseUp, this);
	m_EventFunction[(_uint)UI_STATE::AWAY] = std::bind(&CUI_Weapon_Equip_Button::Event_MouseAway, this);


	return S_OK;
}

void CUI_Weapon_Equip_Button::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_Weapon_Equip_Button::Update(_float fTimeDelta)
{
	__super::Update_Event();
}

void CUI_Weapon_Equip_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Weapon_Equip_Button::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Weapon_Equip_Button::Event_MouseOn()
{
}

void CUI_Weapon_Equip_Button::Event_MouseAway()
{
}

void CUI_Weapon_Equip_Button::Event_MouseUp()
{
	m_bEquipActive = true;
	m_pGameInstance->PlayEffect(L"UI_Button.ogg", 0.1f);
}

HRESULT CUI_Weapon_Equip_Button::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Button_Select"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

HRESULT CUI_Weapon_Equip_Button::Bind_ShaderResources()
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

HRESULT CUI_Weapon_Equip_Button::Ready_ChildUIs()
{
	return S_OK;
}


CUI_Weapon_Equip_Button* CUI_Weapon_Equip_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Weapon_Equip_Button* pInstance = new  CUI_Weapon_Equip_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Weapon_Equip_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Weapon_Equip_Button::Clone(void* pArg)
{
	CUI_Weapon_Equip_Button* pInstance = new  CUI_Weapon_Equip_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Inventory_Button_Hero");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Weapon_Equip_Button::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
