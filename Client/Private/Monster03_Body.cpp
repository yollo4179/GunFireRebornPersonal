#include"stdafx.h"
#include"Monster03_Body.h"
#include"UI_DemageFont.h"
#include"GameInstance.h"
#include"Hands.h"
CMonster03_Body::CMonster03_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{ pDevice, pContext }
{
}

CMonster03_Body::CMonster03_Body(const CMonster03_Body& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CMonster03_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster03_Body::Initialize(void* pArg)
{
	MONSTER03_BODY_DESC* pDesc = static_cast<MONSTER03_BODY_DESC*>(pArg);
	m_pState = pDesc->pState;
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pisMonsterPicked = pDesc->pisMonsterPicked;

	m_pAccHitTime = pDesc->pAccHitTime;
	m_pHitTime = pDesc->pHitTime;
	m_pHPInfo = pDesc->pHPInfo;
	m_pHit_NormalMonster = pDesc->pHit_NormalMonster;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Scaling(1.2f, 1.2f, 1.2f);


	return S_OK;
}

void CMonster03_Body::Priority_Update(_float fTimeDelta)
{

}

void CMonster03_Body::Update(_float fTimeDelta)
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

void CMonster03_Body::Late_Update(_float fTimeDelta)
{
	/*Collision*/
	CGameObject* pTarget = { nullptr };
	if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, &pTarget))
	{
		if (*m_pHitTime <= *m_pAccHitTime)
		{
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

			fDemage += fExtraDemage;

			if (isCritical)
			{
				fDemage = fDemage * (1 + fCriticalDemage / 100.f);
			}
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
			/*Add Demage Font*/
			{
				CUI_DemageFont* pDemageFont = nullptr;
				pDemageFont = static_cast<CUI_DemageFont*>(m_pGameInstance->Get_Now_UI_DemageFont());
				if (nullptr == pDemageFont)
				{
					return;
				}
				_matrix matColliderWorld = m_pColliderCom->Get_WorldMatrix();
				_float fRandomOffsetX = m_pGameInstance->Compute_Random(0.f, XMVectorGetX(XMVector3Length(matColliderWorld.r[0])) * 0.5f);
				_float fRandomOffsetY = m_pGameInstance->Compute_Random(0.f, XMVectorGetX(XMVector3Length(matColliderWorld.r[1])) * 0.5f);
				matColliderWorld.r[3] = XMVectorSetY(matColliderWorld.r[3], XMVectorGetY(matColliderWorld.r[3]) + fRandomOffsetY);
				matColliderWorld.r[3] = XMVectorSetX(matColliderWorld.r[3], XMVectorGetX(matColliderWorld.r[3]) + fRandomOffsetX);

				pDemageFont->Set_Active(true);
				pDemageFont->Set_WorldMatrix(matColliderWorld);
				pDemageFont->Set_Demage(fDemage, isCritical);


			}
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

HRESULT CMonster03_Body::Render()
{

	return S_OK;
}

HRESULT CMonster03_Body::Ready_Components()
{
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster03_Body::Bind_ShaderResources()
{
	return S_OK;
}

CMonster03_Body* CMonster03_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster03_Body* pInstance = new CMonster03_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster03_Body");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster03_Body::Clone(void* pArg)
{
	CMonster03_Body* pInstance = new CMonster03_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster03_Body");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster03_Body::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}
