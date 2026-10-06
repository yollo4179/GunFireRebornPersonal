#include"stdafx.h"
#include"DropItem_Bullet_Standard.h"
#include"GameInstance.h"
#include"Player.h"
CDropItem_Bullet_Standard::CDropItem_Bullet_Standard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CDropItem{ pDevice,pContext }
{
}

CDropItem_Bullet_Standard::CDropItem_Bullet_Standard(const CDropItem_Bullet_Standard& Prototype)
	: CDropItem(Prototype)
{
}

HRESULT CDropItem_Bullet_Standard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDropItem_Bullet_Standard::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(0, true);
	return S_OK;
}

void CDropItem_Bullet_Standard::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
	__super::Priority_Update(fTimeDelta);
}

void CDropItem_Bullet_Standard::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	__super::Update(fTimeDelta);

}

void CDropItem_Bullet_Standard::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CDropItem_Bullet_Standard::Render()
{

	__super::Render();
	return S_OK;
}

void CDropItem_Bullet_Standard::Activate_DropItem()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
	if (nullptr == pPlayer)
		return;
	PlayerItemInfo eInfo = pPlayer->Get_PlayerItemInfo();

	_int iNumExtraBullts = static_cast<_int>(m_pGameInstance->Compute_Random(3.f, 6.f));
	eInfo.iNumBullets += iNumExtraBullts;
	pPlayer->Set_PlayerItemInfo(eInfo);
	m_isActive = false;
}



HRESULT CDropItem_Bullet_Standard::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_DropItem_Bullet_Standard"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	/*Com_Navigagtion*/
	CNavigation::NAVIGATION_DESC		NaviDesc{};

	NaviDesc.iStartIndex = 0;

	if (LEVEL_GAMEPLAY == m_iNowMapStage) {
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
			TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
			return E_FAIL;
	}
	else if (LEVEL_GAMEPLAY2 == m_iNowMapStage)
	{
		if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Navigation"),
			TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
			return E_FAIL;
	}
	return S_OK;
	
}


HRESULT CDropItem_Bullet_Standard::Ready_Particle()
{
	return S_OK;
}

CDropItem_Bullet_Standard* CDropItem_Bullet_Standard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDropItem_Bullet_Standard* pInstance = new CDropItem_Bullet_Standard(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CDropItem_Bullet_Standard");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CDropItem_Bullet_Standard::Clone(void* pArg)
{
	CDropItem_Bullet_Standard* pInstance = new CDropItem_Bullet_Standard(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CDropItem_Bullet_Standard");
		return nullptr;
	}

	return pInstance;
}

void CDropItem_Bullet_Standard::Free()
{
	__super::Free();
}
