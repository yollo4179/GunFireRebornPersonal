#include"stdafx.h"
#include"UI_Inventory_Button_Weapons.h"
#include"GameInstance.h"
CUI_Inventory_Button_Weapons::CUI_Inventory_Button_Weapons(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Inventory_Button_Weapons::CUI_Inventory_Button_Weapons(const CUI_Inventory_Button_Weapons& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Inventory_Button_Weapons::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Inventory_Button_Weapons::Initialize(void* pArg)
{
	UI_BUTTTON_INVENTORY_WEAPONS_DESC* pDesc = static_cast<UI_BUTTTON_INVENTORY_WEAPONS_DESC*>(pArg);


	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	

	if (FAILED(Ready_Components()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;

	m_EventFunction[(_uint)UI_STATE::ON] = std::bind(&CUI_Inventory_Button_Weapons::Event_MouseOn, this);
	m_EventFunction[(_uint)UI_STATE::UP] = std::bind(&CUI_Inventory_Button_Weapons::Event_MouseUp, this);
	m_EventFunction[(_uint)UI_STATE::AWAY] = std::bind(&CUI_Inventory_Button_Weapons::Event_MouseAway, this);


	return S_OK;
}

void CUI_Inventory_Button_Weapons::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_Inventory_Button_Weapons::Update(_float fTimeDelta)
{
	__super::Update_Event();
}

void CUI_Inventory_Button_Weapons::Late_Update(_float fTimeDelta)
{
	/*부모UI에서 처리*/
}

HRESULT CUI_Inventory_Button_Weapons::Render()
{

	if (true ==m_isSelected)
	{
		if (FAILED(Bind_ShaderResources_Glow()))
			return E_FAIL;
		m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	}


	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}
/* 스크린 안에 들오언것까지 고려가 된것임*/
void CUI_Inventory_Button_Weapons::Event_MouseOn()
{
}

void CUI_Inventory_Button_Weapons::Event_MouseAway()
{
	
}

void CUI_Inventory_Button_Weapons::Event_MouseUp()
{
	m_bActiveWeapons = true;
	m_pGameInstance->PlayEffect(L"UI_Button.ogg", 0.1f);
}

HRESULT CUI_Inventory_Button_Weapons::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Button_Weapons"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_SelectedGlow"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_CheckGlow))))
		return E_FAIL;
}

HRESULT CUI_Inventory_Button_Weapons::Bind_ShaderResources()
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

HRESULT CUI_Inventory_Button_Weapons::Bind_ShaderResources_Glow()
{
	_matrix GlowMatrix = m_pTransformCom->Get_WorldMatrix();
	GlowMatrix.r[0] *= 2.f;
	GlowMatrix.r[1] *= 2.f;
	GlowMatrix.r[3] = XMVectorSetX(GlowMatrix.r[3], XMVectorGetX(GlowMatrix.r[3]) + 50.f);
	_float4x4 matGlowFloat4x4;
	XMStoreFloat4x4(&matGlowFloat4x4, GlowMatrix);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix",&matGlowFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_CheckGlow->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	return S_OK;
}

CUI_Inventory_Button_Weapons* CUI_Inventory_Button_Weapons::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Inventory_Button_Weapons* pInstance = new  CUI_Inventory_Button_Weapons(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Inventory_Button_Weapons");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Inventory_Button_Weapons::Clone(void* pArg)
{
	CUI_Inventory_Button_Weapons* pInstance = new  CUI_Inventory_Button_Weapons(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Inventory_Button_Weapons");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_Button_Weapons::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_CheckGlow);
	Safe_Release(m_pVIBufferCom);
}
