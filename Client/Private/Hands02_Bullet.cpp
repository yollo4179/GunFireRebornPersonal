#include"stdafx.h"
#include"Hands02_Bullet.h"
#include"GameInstance.h"
#include"Particle_Player02_Bullet.h"
#include"Particle_Player02_Hit.h"
#include"Player.h"
CHands02_Bullet::CHands02_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice, pContext}
{
}

CHands02_Bullet::CHands02_Bullet(const CHands02_Bullet& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CHands02_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHands02_Bullet::Initialize(void* pArg)
{
	PARTICLE_HANDS03_BULLET_DESC* pDesc = static_cast<PARTICLE_HANDS03_BULLET_DESC*>(pArg);
	
	if (FAILED(__super::Initialize(pDesc)))
	{
		return E_FAIL;
	}

	_matrix matWorld = XMMatrixIdentity();
	Reset_Bullets(matWorld);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Particle()))
		return E_FAIL;
	for (int i = 0; i < g_iNumParticle_Player02_Hit; ++i)
		m_fAccHitParticle_Time[i] = 3.5f;
	return S_OK;
}

void CHands02_Bullet::Priority_Update(_float fTimeDelta)
{
	/*상대방 파티클*/
	for (int i = 0; i < g_iNumParticle_Player02_Hit; ++i)
	{
		if (m_fAccHitParticle_Time[i] < 1.f) {
			m_fAccHitParticle_Time[i] += fTimeDelta;
			m_pHitParticles[i]->Priority_Update(fTimeDelta);
		}
		else
		{
			m_pFloat4x4sOfTargetPart[i] = nullptr;
		}
	}




	if (!m_isActive)
		return;

	if (m_isActive && m_fAccNowLifeTime < m_fLifeTimeLimit)
	{
		m_fAccNowLifeTime += fTimeDelta;
	}
	else
	{
		m_isActive = false;
	}

	
	m_pTransformCom->Go_Straight( 3.F* fTimeDelta);
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	/*물릿 파티클도 같이 이동*/
	m_pParticle_Bullet->Set_Matrix(m_pTransformCom->Get_WorldMatrix());
	
}

void CHands02_Bullet::Update(_float fTimeDelta)
{

	/* 상대방 Particle Update*/
	for (int j = 0; j < g_iNumParticle_Player02_Hit; ++j)
	{
		if (m_fAccHitParticle_Time[j] < 1.f) {
			m_pHitParticles[j]->Update(fTimeDelta);
			if (nullptr != m_pFloat4x4sOfTargetPart[j])
				m_pHitParticles[j]->Set_Matrix(XMLoadFloat4x4(m_pFloat4x4sOfTargetPart[j]));
		}
	}


	if (!m_isActive)
		return;
	{
		CHands* pHands02 = static_cast<CHands*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS02, 0));
		m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, pHands02, m_pColliderCom);
	
		/*Particle_bullet*/
		m_pParticle_Bullet->Update(fTimeDelta);

	}
}

void CHands02_Bullet::Late_Update(_float fTimeDelta)
{
	/*상대방 파티클*/
	for (int j = 0; j < g_iNumParticle_Player02_Hit; ++j)
	{
		if (m_fAccHitParticle_Time[j] < 1.f)
			m_pHitParticles[j]->Late_Update(fTimeDelta);
	}


	if (!m_isActive)
		return;
	
	{
		CGameObject* pTarget = nullptr;
		if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER, &pTarget))
		{

			const _float4x4* pTargetFloat4x4 = static_cast<CPartObject*>(pTarget)->Get_Float4x4_Ptr_OfPartObject();
			m_pFloat4x4sOfTargetPart[m_iNowHitParticleIndex] = pTargetFloat4x4;
			_matrix matTargetMatrix = XMLoadFloat4x4(pTargetFloat4x4);
			_float3 mvRange = {
					  XMVectorGetX(XMVector3Length(matTargetMatrix.r[0]))
					, XMVectorGetX(XMVector3Length(matTargetMatrix.r[1]))
					, XMVectorGetX(XMVector3Length(matTargetMatrix.r[2]))
			};
			m_pHitParticles[m_iNowHitParticleIndex]->Set_Range(mvRange);
			m_pHitParticles[m_iNowHitParticleIndex]->Reset_Vertices(fTimeDelta);
			m_pHitParticles[m_iNowHitParticleIndex]->Set_Matrix(XMLoadFloat4x4(m_pFloat4x4sOfTargetPart[m_iNowHitParticleIndex]));
			m_fAccHitParticle_Time[m_iNowHitParticleIndex] = 0.f;
			m_iNowHitParticleIndex = (m_iNowHitParticleIndex + 1) % g_iNumParticle_Player02_Hit;

		}


		/*Particle_bullet*/
		m_pParticle_Bullet->Late_Update(fTimeDelta);

		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
#ifdef _DEBUG
	if(m_isActive)
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CHands02_Bullet::Render()
{

	
	
	return S_OK;
}

void CHands02_Bullet::Set_Pivot(_float3 fPivot)
{
	m_pParticle_Bullet->Set_Pivot(fPivot);
}

void CHands02_Bullet::Set_Particle_Matrix(_fmatrix vPosMatrix)
{
	m_pParticle_Bullet->Set_Matrix(vPosMatrix);
}

void CHands02_Bullet::Reset_Vertices(_float fTimeDelta)
{
	m_pParticle_Bullet->Reset_Vertices(fTimeDelta);
}

void CHands02_Bullet::Reset_Bullets(_fmatrix matInitialShootingPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, 1.f*XMVector3Normalize(matInitialShootingPos.r[0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, 1.f*XMVector3Normalize(matInitialShootingPos.r[1]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK,1.f* XMVector3Normalize(matInitialShootingPos.r[2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialShootingPos.r[3]);
	m_fAccNowLifeTime = 0.f;

}

HRESULT CHands02_Bullet::Ready_Components()
{
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;


	return S_OK;
	
}

HRESULT CHands02_Bullet::Bind_ShaderResources()
{
	return S_OK;
}

HRESULT CHands02_Bullet::Ready_Particle()
{

	m_pParticle_Bullet = static_cast<CParticle_Player02_Bullet*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player02_Bullet"), nullptr));

	for (size_t i = 0; i < g_iNumParticle_Player02_Hit; ++i)
	{
		m_pHitParticles[i]=static_cast<CParticle_Player02_Hit*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player02_Hit"), nullptr));
	}
	
	return S_OK;
}

CHands02_Bullet* CHands02_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands02_Bullet* pInstance = new CHands02_Bullet(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CHands02_Bullet");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CHands02_Bullet::Clone(void* pArg)
{
	CHands02_Bullet* pInstance = new CHands02_Bullet(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CHands02_Bullet");
		return nullptr;
	}

	return pInstance;
}

void CHands02_Bullet::Free()
{

	for (int i = 0; i < g_iNumParticle_Player02_Hit; ++i)
	{
		Safe_Release(m_pHitParticles[i]);
	}
	Safe_Release(m_pParticle_Bullet);
	Safe_Release(m_pColliderCom);
	
	__super::Free();
}
