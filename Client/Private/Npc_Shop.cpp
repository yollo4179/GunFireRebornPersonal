#include"stdafx.h"
#include "Npc_Shop.h"
#include"GameInstance.h"
#include"UI_Shop.h"
CNpc_Shop::CNpc_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CNpc{pDevice, pContext}
{
}

CNpc_Shop::CNpc_Shop(const CNpc_Shop& Prototype)
	:CNpc(Prototype)
{
}

HRESULT CNpc_Shop::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc_Shop::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pModelCom->SetUp_Animation(0, true);
	return S_OK;
	
}

void CNpc_Shop::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	



	__super::Priority_Update(fTimeDelta);
	
}

void CNpc_Shop::Update(_float fTimeDelta)
{
	
	if (!m_isActive)
		return;

	__super::Update(fTimeDelta);
}

void CNpc_Shop::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CNpc_Shop::Render()
{
	__super::Render();
	return S_OK;
}

void CNpc_Shop::Activate_Npc()
{
	CUI_Shop* pUIShop = static_cast<CUI_Shop*>(m_pGameInstance->Get_UI_By_Tag(TEXT("Shop")));
	pUIShop->Set_Active();


}

void CNpc_Shop::Set_Active(_bool isActive)
{
	m_isActive = isActive;
	CUI_Shop* pUIShop = static_cast<CUI_Shop*>(m_pGameInstance->Get_UI_By_Tag(TEXT("Shop")));
	if(nullptr != pUIShop)
		pUIShop->ResetItems();

}

HRESULT CNpc_Shop::Ready_Components()
{
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model *///(0번 애니메이션만 사용하자)
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_NPC_Shop"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CNpc_Shop::Ready_Particle()
{
	return E_NOTIMPL;
}

CNpc_Shop* CNpc_Shop::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNpc_Shop* pInstance = new CNpc_Shop(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CNpc_Shop");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CNpc_Shop::Clone(void* pArg)
{
	CNpc_Shop* pInstance = new CNpc_Shop(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CNpc_Shop");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Shop::Free()
{
	__super::Free();
}
