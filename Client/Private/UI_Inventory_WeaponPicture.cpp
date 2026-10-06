#include "stdafx.h"
#include "UI_Inventory_WeaponPicture.h"
#include"Player.h"
#include "GameInstance.h"

CUI_Inventory_WeaponPicture::CUI_Inventory_WeaponPicture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Inventory_WeaponPicture::CUI_Inventory_WeaponPicture(const CUI_Inventory_WeaponPicture& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Inventory_WeaponPicture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Inventory_WeaponPicture::Initialize(void* pArg)
{
	UI_INVENTORY_WEAPON_PICTURE_DESC* pDesc = static_cast<UI_INVENTORY_WEAPON_PICTURE_DESC*>(pArg);

	mapPlayerArmID.emplace(make_pair( CPlayer::PART_HANDS01,0));
	mapPlayerArmID.emplace(make_pair(CPlayer::PART_HANDS02, 1));
	mapPlayerArmID.emplace(make_pair(CPlayer::PART_HANDS03, 2));
	mapPlayerArmID.emplace(make_pair(CPlayer::PART_HANDS04, 3));
	mapPlayerArmID.emplace(make_pair(CPlayer::PART_HANDS05, 4));

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;

	return S_OK;
}

void CUI_Inventory_WeaponPicture::Priority_Update(_float fTimeDelta)
{
}

void CUI_Inventory_WeaponPicture::Update(_float fTimeDelta)
{
}

void CUI_Inventory_WeaponPicture::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Inventory_WeaponPicture::Render()
{
	
	{
		if (FAILED(Bind_ShaderResources_BG()))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Bind_BufferDesc();

		m_pVIBufferCom->Render();
	}


	if (FAILED(Bind_ShaderResources(mapPlayerArmID[m_iPlayerToolNo])))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Inventory_WeaponPicture::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Arm"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_WeaponBackground"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureComBG))))
		return E_FAIL;

	
}

HRESULT CUI_Inventory_WeaponPicture::Bind_ShaderResources(_uint iNowArmTexture)
{


	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", iNowArmTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Inventory_WeaponPicture::Bind_ShaderResources_BG()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureComBG->Bind_ShaderResource(m_pShaderCom, "g_Texture",0)))
		return E_FAIL;

	return S_OK;
}

CUI_Inventory_WeaponPicture* CUI_Inventory_WeaponPicture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Inventory_WeaponPicture* pInstance = new   CUI_Inventory_WeaponPicture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :   CUI_Arm_Paint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Inventory_WeaponPicture::Clone(void* pArg)
{
	CUI_Inventory_WeaponPicture* pInstance = new   CUI_Inventory_WeaponPicture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :   CUI_Inventory_WeaponPicture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_WeaponPicture::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureComBG);
	Safe_Release(m_pVIBufferCom);
}
