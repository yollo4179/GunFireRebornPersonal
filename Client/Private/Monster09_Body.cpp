#include"stdafx.h"
#include"Monster09.h"
#include"Monster09_Body.h"
#include"GameInstance.h"
#include"UI_DemageFont.h"
#include"Hands.h"
#include"Effect_Monster09_AngelRing.h"

CMonster09_Body::CMonster09_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{ pDevice, pContext }
{
}

CMonster09_Body::CMonster09_Body(const CMonster09_Body& Prototype)
:CPartObject(Prototype)
{
}

HRESULT CMonster09_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster09_Body::Initialize(void* pArg)
{
	MONSTER09_BODY_DESC* pDesc = static_cast<MONSTER09_BODY_DESC*>(pArg);
	m_pState = pDesc->pState;
	m_pBossHPInfo = pDesc->pBossHPInfo;
	m_pHitTime = pDesc->pHitTime;
	m_pAccHitTime = pDesc->pAccHitTime;
	m_pSocketMatrixFor_AngelRing = pDesc->pSocketMatrixFor_AngelRing;

	for (int i = 0; i < 10; ++i)
	{
		m_pSocketMatrix[i] = pDesc->pSocketMatrix[i];
	}




	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;
	if (FAILED(Ready_AngelRing()))
		return E_FAIL;

	m_pTransformCom->Scaling(6.f, 6.f, 6.f);


	return S_OK;
}

void CMonster09_Body::Priority_Update(_float fTimeDelta)
{
	
}

void CMonster09_Body::Update(_float fTimeDelta)
{

	/*rush and  body collider*/
	for (int i = 0; i < 10; ++i)
	{
		_matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix[i]);
		for (size_t j = 0; j < 3; ++j)
		{
			SocketMatrix.r[j] = XMVector3Normalize(SocketMatrix.r[j]);
		}
		XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
		m_pColliderCom[i]->Update(XMLoadFloat4x4(&m_WorldMatrix));
		m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER, this, m_pColliderCom[i]);
		
		if (*m_pState & M9_STATE::M9_STATE_ATTACK_RUSH)
		{
			m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER_ATTACK, this, m_pColliderCom[i]);
		}

	}

	_matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrixFor_AngelRing);
	for (size_t j = 0; j < 3; ++j)
	{
		
		SocketMatrix.r[j] = XMVector3Normalize(SocketMatrix.r[j]);
	}
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

 	m_pAngelRing->Set_Matrix(XMLoadFloat4x4(&m_WorldMatrix));




}

void CMonster09_Body::Late_Update(_float fTimeDelta)
{
	if (*m_pState & M9_STATE::M9_STATE_SUBMERGE)
		return;

	/*Collision*/
	CGameObject* pTarget = { nullptr };
	for (int i = 0; i < 10; ++i)
	{
		/*플레이어한테 맞았다.*/
		if (m_pGameInstance->Check_Collision(m_pColliderCom[i], CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, &pTarget))
		{

			_float fDemage = 100.f;
			if (*m_pHitTime <= *m_pAccHitTime)
			{
				
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
				
				(*m_pBossHPInfo).x -= fDemage;
					
				if ((*m_pBossHPInfo).x < 0)
				{
					(*m_pBossHPInfo).x = 0;
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
					_matrix matColliderWorld = m_pColliderCom[i]->Get_WorldMatrix();
					_float fRandomOffsetX = m_pGameInstance->Compute_Random(0.f, XMVectorGetX(XMVector3Length(matColliderWorld.r[0])) * 0.5f);
					_float fRandomOffsetY = m_pGameInstance->Compute_Random(0.f, XMVectorGetX(XMVector3Length(matColliderWorld.r[1])) * 0.5f);
					matColliderWorld.r[3] = XMVectorSetY(matColliderWorld.r[3], XMVectorGetY(matColliderWorld.r[3]) + fRandomOffsetY);
					matColliderWorld.r[3] = XMVectorSetX(matColliderWorld.r[3], XMVectorGetX(matColliderWorld.r[3]) + fRandomOffsetX);

					pDemageFont->Set_Active(true);
					pDemageFont->Set_WorldMatrix(matColliderWorld);
					pDemageFont->Set_Demage(fDemage, isCritical);


				}
			}
			/* 데미지*/
			
			



			int a = 0;
		}
		/*RUSH_ATTACK일때 닿으면 플레이어를 밀어낸다.*/
		if (*m_pState & M9_STATE::M9_STATE_ATTACK_RUSH)
		{
			if (m_pGameInstance->Check_Collision(m_pColliderCom[i], CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER, &pTarget))
			{
				_matrix matColliderPart = m_pColliderCom[i]->Get_WorldMatrix();
				_matrix matTarget = pTarget->Get_WorldMatrix();
				_vector vPartColliderPos = matColliderPart.r[3];
				_vector vTargetPos = matTarget.r[3];
				_vector vDir = vTargetPos - vPartColliderPos;
				vDir = XMVector3Normalize(vDir);
				_float fPower = 3.f;
				CTransform* pTargetTransformCom = static_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY2, L"Layer_APlayer", L"Com_Transform", 0));
				CNavigation* pTargetNavigation = static_cast<CNavigation*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY2, L"Layer_APlayer", L"Com_Navigation", 0));
				pTargetTransformCom->Go_Pushed(fTimeDelta*fPower, vDir, pTargetNavigation);

			}
		}
	}
	
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_pAngelRing->Late_Update(fTimeDelta);
#ifdef _DEBUG
	for(int i=0;i<10;++i)
	m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);
#endif
}

HRESULT CMonster09_Body::Render()
{
	
	return S_OK;
}

HRESULT CMonster09_Body::Ready_Components()
{
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;

	TCHAR szBaseComponentTag[MAX_PATH] = L"Com_Collider%d";
	for (int i = 0; i < 10; ++i)
	{
		TCHAR szNowColliderTag[MAX_PATH] = L"";
		wsprintf(szNowColliderTag, szBaseComponentTag, i);

		if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			szNowColliderTag, reinterpret_cast<CComponent**>(&m_pColliderCom[i]), &ColliderDesc)))
			return E_FAIL;
	}


	/*if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CMonster09_Body::Bind_ShaderResources()
{
	return S_OK;
}

HRESULT CMonster09_Body::Ready_AngelRing()
{
	m_pAngelRing = static_cast<Effect_Monster09_AngelRing*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Angelring"), nullptr));
 	return S_OK;
}

CMonster09_Body* CMonster09_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster09_Body* pInstance = new CMonster09_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster09_Body");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster09_Body::Clone(void* pArg)
{
	CMonster09_Body* pInstance = new CMonster09_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster09_Body");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster09_Body::Free()
{
	__super::Free();
	for (int i = 0; i < 10; ++i)
	{
		Safe_Release(m_pColliderCom[i]);
	}
	Safe_Release(m_pAngelRing);
}
