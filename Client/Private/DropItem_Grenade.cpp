#include"stdafx.h"
#include"DropItem_Grenade.h"
#include"GameInstance.h"
#include"Player.h"
CDropItem_Grenade::CDropItem_Grenade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CDropItem{pDevice,pContext}
{
}

CDropItem_Grenade::CDropItem_Grenade(const CDropItem_Grenade& Prototype)
	: CDropItem(Prototype)
{
}

HRESULT CDropItem_Grenade::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDropItem_Grenade::Initialize(void* pArg)
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

void CDropItem_Grenade::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
	if (false == m_bOnFround) {
		m_fCurHeight += m_v0 * fTimeDelta;
		m_v0 -= m_fGravity * fTimeDelta * 5.f;
	}
	_vector vPos = m_pNavigationCom->SetUp_HeightOnNavigation(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	_float fBaseHeight = XMVectorGetY(vPos);
	

	if (0.1f >= m_fCurHeight)
	{
		m_fCurHeight = 0.1f;
		m_bOnFround = true;
		/*습득시 0으로*/
	}
	vPos = XMVectorSetY(vPos, m_fCurHeight+ fBaseHeight);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	

	
}

void CDropItem_Grenade::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	if (true == m_bOnFround) {
		m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_ITEM, this, m_pColliderCom);
		
		_matrix matCol = m_pTransformCom->Get_WorldMatrix();
		matCol.r[3] = XMVectorSetY(matCol.r[3],XMVectorGetY(matCol.r[3])+ 2.5f);
		m_pColliderCom->Update(matCol);
	}

	m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);

}

void CDropItem_Grenade::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;


	

	CGameObject* pTarget = { nullptr };
	/*Collision-Picking*/
	if (true == m_bOnFround) {
		if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER, &pTarget))
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
			if (nullptr == pPlayer)
				return;
			PlayerItemInfo eInfo = pPlayer->Get_PlayerItemInfo();

			_int iExtraGrenades = 1;

			if (eInfo.iNumMaxGrenades >= eInfo.iNumGrenades + iExtraGrenades)
			{
				eInfo.iNumGrenades += iExtraGrenades;
				pPlayer->Set_PlayerItemInfo(eInfo);
				m_isActive = false;
			}
		}
	}


	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

	//__super::Late_Update(fTimeDelta);
}

HRESULT CDropItem_Grenade::Render()
{

	__super::Render();
	return S_OK;
}

void CDropItem_Grenade::Activate_DropItem()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
	if (nullptr == pPlayer)
		return;
	PlayerItemInfo eInfo = pPlayer->Get_PlayerItemInfo();

	_int iExtraGrenades = 1;
	
	if (eInfo.iNumMaxGrenades >= eInfo.iNumGrenades + iExtraGrenades) {
		eInfo.iNumGrenades += iExtraGrenades;
		pPlayer->Set_PlayerItemInfo(eInfo);
		m_isActive = false;
	}
	else
		m_bOnFround = false;
	
}




HRESULT CDropItem_Grenade::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player_Skill_Granade"),
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


HRESULT CDropItem_Grenade::Ready_Particle()
{
	return S_OK;
}

CDropItem_Grenade* CDropItem_Grenade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDropItem_Grenade* pInstance = new CDropItem_Grenade(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CDropItem_Grenade");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CDropItem_Grenade::Clone(void* pArg)
{
	CDropItem_Grenade* pInstance = new CDropItem_Grenade(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CDropItem_Grenade");
		return nullptr;
	}

	return pInstance;
}

void CDropItem_Grenade::Free()
{
	__super::Free();
}
