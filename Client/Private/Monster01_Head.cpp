#include"stdafx.h"
#include"Monster01_Head.h"
#include"GameInstance.h"
#include"Hands.h"
CMonster01_Head::CMonster01_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{ pDevice, pContext }
{
}

CMonster01_Head::CMonster01_Head(const CMonster01_Head& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CMonster01_Head::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster01_Head::Initialize(void* pArg)
{
	MONSTER01_HEAD_DESC* pDesc = static_cast<MONSTER01_HEAD_DESC*>(pArg);
	m_pState = pDesc->pState;
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pHeadShot = pDesc->pHeadShot;
	m_pisMonsterPicked = pDesc->pisMonsterPicked;
	m_pAccHitTime = pDesc->pAccHitTime;
	m_pHitTime = pDesc->pHitTime;
	m_pHPInfo = pDesc->pHPInfo;
	m_pHit_NormalMonster = pDesc->pHit_NormalMonster;
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Scaling(1.1f, 1.1f, 1.1f);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, -0.3f, -1.8f, 1.f));


	return S_OK;
}

void CMonster01_Head::Priority_Update(_float fTimeDelta)
{

}

void CMonster01_Head::Update(_float fTimeDelta)
{
	_matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
	for (size_t j = 0; j < 3; ++j)
	{
		SocketMatrix.r[j] = XMVector3Normalize(SocketMatrix.r[j]);
	}
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
	m_pColliderCom->Update(XMLoadFloat4x4(&m_WorldMatrix));


	m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER, this, m_pColliderCom);
}

void CMonster01_Head::Late_Update(_float fTimeDelta)
{

	CGameObject* pTarget = { nullptr };
	*m_pisMonsterPicked = false;
	if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, &pTarget))
	{
		if (nullptr != m_pHeadShot)
		{
			*m_pHeadShot = true;
		}

		*m_pHit_NormalMonster = true;
		CHands* pHands = static_cast<CHands*>(pTarget);
		HandsInfo eHandsInfo = pHands->Get_HandsInfo();
		_bool isCritical = false;

		_float fDemage = eHandsInfo.fDemage;
		_float fExtraDemage = m_pGameInstance->Compute_Random(0.f, fDemage * 0.1f);
		_float fCriticalDemage = eHandsInfo.fCriticalDemage;
		_float fCriticalRate = eHandsInfo.fCriticalRate;

		_float fRate = m_pGameInstance->Compute_Random(0.f, 100.f);

		if (fRate <= fCriticalRate)isCritical = true;

		if (isCritical)
		{
			fDemage += fDemage += fExtraDemage;
		}
		else
		{
			fDemage = fDemage * (1 + fCriticalDemage / 100.f);
		}


		if (*m_pHitTime <= *m_pAccHitTime)
		{
			 fDemage = 54.f;
			//Blue
			if ((*m_pHPInfo).x >= fDemage)
			{
				(*m_pHPInfo).x -= fDemage;
			}
			else
			{
				//Red
				if (0 != (*m_pHPInfo).x)
				{


					(*m_pHPInfo).z -= (fDemage - (*m_pHPInfo).x);
					(*m_pHPInfo).x = 0.f;
				}
				else
				{
					(*m_pHPInfo).z -= (fDemage);
					if ((*m_pHPInfo).z < 0)
					{
						(*m_pHPInfo).z = 0;
					}
				}

			}
			*m_pAccHitTime = 0.f;

		}
	}
	if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_CHECK_RAY, &pTarget))
	{
		*m_pisMonsterPicked = true;
	}

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CMonster01_Head::Render()
{
	
	return S_OK;
}

HRESULT CMonster01_Head::Ready_Components()
{
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster01_Head::Bind_ShaderResources()
{
	return S_OK;
}

CMonster01_Head* CMonster01_Head::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster01_Head* pInstance = new CMonster01_Head(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster01_Head");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster01_Head::Clone(void* pArg)
{
	CMonster01_Head* pInstance = new CMonster01_Head(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster01_Head");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster01_Head::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}
