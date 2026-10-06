#include"stdafx.h"
#include "UI_Shop_Button_Item.h"
#include"GameInstance.h"
#include"UI_Numbers.h"
CUI_Shop_Button_Item::CUI_Shop_Button_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Parent{ pDevice, pContext }
{
}

CUI_Shop_Button_Item::CUI_Shop_Button_Item(const CUI_Shop_Button_Item& Prototype)
	: CUI_Parent(Prototype)
{
}

HRESULT CUI_Shop_Button_Item::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Shop_Button_Item::Initialize(void* pArg)
{
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Number()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;

	m_EventFunction[(_uint)UI_STATE::ON] = std::bind(&CUI_Shop_Button_Item::Event_MouseOn, this);
	m_EventFunction[(_uint)UI_STATE::UP] = std::bind(&CUI_Shop_Button_Item::Event_MouseUp, this);
	m_EventFunction[(_uint)UI_STATE::AWAY] = std::bind(&CUI_Shop_Button_Item::Event_MouseAway, this);
	return S_OK;
}

void CUI_Shop_Button_Item::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_Shop_Button_Item::Update(_float fTimeDelta)
{
	__super::Update_Event();
}


void CUI_Shop_Button_Item::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Shop_Button_Item::Render()
{
	if (FAILED(Render_ShaderResources_Frame()))
		return E_FAIL;
	if (FAILED(Render_ShaderResources_Glow()))
		return E_FAIL;
	if (FAILED(Render_ShaderResources_Item()))
		return E_FAIL;

	if (FAILED(Render_ShaderResources_Money()))
		return E_FAIL;
	m_ChildUIs[L"Numbers_Money"]->Render();

	if(m_bSoldOut)
	if (FAILED(Render_ShaderResources_SoldOut()))
		return E_FAIL;

	

	return S_OK;
}

void CUI_Shop_Button_Item::Set_ItemInfo(CUI_Shop::ItemInfo eItemInfo)
{
	m_eItemInfo = eItemInfo; 
	static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Money"])->Set_Numerator(m_eItemInfo.iValue);
}

void CUI_Shop_Button_Item::Event_MouseOn()
{
	m_bMouseOn = true;
}

void CUI_Shop_Button_Item::Event_MouseAway()
{
	m_bMouseOn= false;
}

void CUI_Shop_Button_Item::Event_MouseUp()
{
	m_bSoldOutButton = true;
	m_pGameInstance->PlayEffect(L"UI_Button.ogg", 0.1f);
}

HRESULT CUI_Shop_Button_Item::Ready_Components()
{
	/* Com_Shader */
	if(FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if(FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/*Com_Texture*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Item_Slot"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_WeaponBackground"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Glow))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Item"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Item))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Sold_Out"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom_SoldOut))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Coin"),
		TEXT("Com_Texture5"), reinterpret_cast<CComponent**>(&m_pTextureCom_Money))))
		return E_FAIL;
	

	///*Com_Texture_Glow*/
	//if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Item_Slot"),
	//	TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CUI_Shop_Button_Item::Ready_Number()
{
	CUIObject* pUIObject = { nullptr };
	CUI_Numbers::UI_NUMBERS_DESC NumberDesc = {};
	NumberDesc.fRadian = 0.f;
	NumberDesc.fRotationPerSec = XMConvertToRadians(90.f);
	NumberDesc.fSpeedPerSec = 10.f;
	NumberDesc.fSizeX = 25.f;
	NumberDesc.fSizeY = 25.f;
	NumberDesc.fX = m_fX -0.5*m_fSizeX+50;
	NumberDesc.fY = m_fY - 0.5 * m_fSizeY + 20.f;
	NumberDesc.iNumerator = 0;
	NumberDesc.iDenominator = 0;
	NumberDesc.isFraction = false;
	NumberDesc.vColor = { 1.f,1.f,1.f };
	NumberDesc.isDenominatorInfinity = false;

	pUIObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"), &NumberDesc));
	m_ChildUIs.emplace(L"Numbers_Money", pUIObject);
	return S_OK;
}

HRESULT CUI_Shop_Button_Item::Render_ShaderResources_Frame()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Shop_Button_Item::Render_ShaderResources_Glow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

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

HRESULT CUI_Shop_Button_Item::Render_ShaderResources_Item()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Item->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_ItemNo)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Shop_Button_Item::Render_ShaderResources_SoldOut()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_SoldOut->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Shop_Button_Item::Render_ShaderResources_Money()
{
	_uint iMoney = 0;
	_float fScale = 0.2f;
	if (1 == m_eItemInfo.iIsRare)
	{
		fScale = 0.3f;
		iMoney = 1;
	}
	_matrix  matWorldOfMoney = m_pTransformCom->Get_WorldMatrix();
	matWorldOfMoney.r[0] *= fScale;
	matWorldOfMoney.r[1] *= fScale;
	matWorldOfMoney.r[3] = XMVectorSetX(matWorldOfMoney.r[3], XMVectorGetX(matWorldOfMoney.r[3]) - m_fSizeX * 0.5f + 40.f);
	matWorldOfMoney.r[3] = XMVectorSetY(matWorldOfMoney.r[3], XMVectorGetY(matWorldOfMoney.r[3]) + m_fSizeY * 0.5f -40.f);

	_float4x4 MoneyFloat4x4;
	XMStoreFloat4x4(&MoneyFloat4x4, matWorldOfMoney);
	if (FAILED(m_pShaderCom->Bind_Matrix( "g_WorldMatrix",&MoneyFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	
	if (FAILED(m_pTextureCom_Money->Bind_ShaderResource(m_pShaderCom, "g_Texture", iMoney)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;

}

CUI_Shop_Button_Item* CUI_Shop_Button_Item::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Shop_Button_Item* pInstance = new  CUI_Shop_Button_Item(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Shop_Button_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Shop_Button_Item::Clone(void* pArg)
{
	CUI_Shop_Button_Item* pInstance = new  CUI_Shop_Button_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Shop_Button_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Shop_Button_Item::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_Glow);
	Safe_Release(m_pTextureCom_Item);
	Safe_Release(m_pTextureCom_SoldOut);
	Safe_Release(m_pTextureCom_Money);

	Safe_Release(m_pVIBufferCom);
}
