#include"stdafx.h"
#include"DropItem_Money.h"
#include"GameInstance.h"
#include"Player.h"
CDropItem_Money::CDropItem_Money(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CDropItem{ pDevice,pContext }
{
}

CDropItem_Money::CDropItem_Money(const CDropItem_Money& Prototype)
	: CDropItem(Prototype)
{
}

HRESULT CDropItem_Money::Initialize_Prototype()
{
	return S_OK;
}
HRESULT CDropItem_Money::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pModelCom->Set_AnimSpeed(0, 50.f);
	m_pModelCom->SetUp_Animation(0, true);
	return S_OK;
}

void CDropItem_Money::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
	__super::Priority_Update(fTimeDelta);
}

void CDropItem_Money::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	__super::Update(fTimeDelta);
}

void CDropItem_Money::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CDropItem_Money::Render()
{
	__super::Render();
	return S_OK;
}

void CDropItem_Money::Activate_DropItem()
{

	CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
	if (nullptr == pPlayer)
		return;
	PlayerItemInfo eInfo = pPlayer->Get_PlayerItemInfo();

	_int iNumExtraCoins = static_cast<_int>(m_pGameInstance->Compute_Random(20.f, 50.f));
	eInfo.iCoins += iNumExtraCoins;
	pPlayer->Set_PlayerItemInfo(eInfo);
	m_isActive = false;
}

void CDropItem_Money::Reset_Item(_fmatrix matInitialItemPos, CNavigation* pMonsterNavigation)
{
	m_isActive = true;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, 2.f * XMVector3Normalize(matInitialItemPos.r[0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, 2.f * XMVector3Normalize(matInitialItemPos.r[1]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, 2.f * XMVector3Normalize(matInitialItemPos.r[2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	_float fRandomRadians = m_pGameInstance->Compute_Random(0.f, XMConvertToRadians(360.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 2.f, 0.f, 0.f), fRandomRadians);

	_vector fMonsterHeight = { 0.f,1.f,0.f,0.f };

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialItemPos.r[3] + fMonsterHeight);

	m_pModelCom->SetUp_Animation(0, true);
	m_pNavigationCom->Set_Index(pMonsterNavigation->Get_Index());

	m_fGravity = 9.8f;
	m_fCurHeight = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_v0 = m_pGameInstance->Compute_Random(0.0000001f, 0.0000005f);
	m_bOnFround = false;
}

HRESULT CDropItem_Money::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_DropItem_Money"),
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

HRESULT CDropItem_Money::Ready_Particle()
{
	return E_NOTIMPL;
}

CDropItem_Money* CDropItem_Money::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDropItem_Money* pInstance = new CDropItem_Money(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CDropItem_Money");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CDropItem_Money::Clone(void* pArg)
{
	CDropItem_Money* pInstance = new CDropItem_Money(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CDropItem_Money");
		return nullptr;
	}

	return pInstance;
}

void CDropItem_Money::Free()
{
	__super::Free();
}
