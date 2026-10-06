#include"stdafx.h"
#include"Monster09.h"
#include"Monster09_Head.h"
#include"Monster09_Shot.h"
#include"GameInstance.h"
#include"Particle_Splash.h"
#include"Trail_Monster09.h"
#include"Particle_Monster09_Shot_Charging.h"
CMonster09_Head::CMonster09_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{ pDevice, pContext }
{
}

CMonster09_Head::CMonster09_Head(const CMonster09_Head& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CMonster09_Head::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster09_Head::Initialize(void* pArg)
{
	MONSTER09_HEAD_DESC* pDesc = static_cast<MONSTER09_HEAD_DESC*>(pArg);
	m_pState = pDesc->pState;
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pNowShotTiming = pDesc->pNowShotTiming;
	
	for(int i=0;i<3;++i)
	m_pSocketMatrix_Eyes[i] = pDesc->pSocketMatrix_Eyes[i];

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Bullets()))
		return E_FAIL;
	if (FAILED(Ready_Particle()))
		return E_FAIL;
	if (FAILED(Ready_Trails()))
		return E_FAIL;
	
	m_pTransformCom->Scaling(1.1f, 1.1f, 1.1f);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f,-0.3f, -1.8f, 1.f));


	return S_OK;
}

void CMonster09_Head::Priority_Update(_float fTimeDelta)
{

	m_fAccAddTrailTiming += fTimeDelta;
	


	for (size_t i = 0; i < m_iNumShots; ++i)
	{
		/*if (m_pWaterShots[i]->Get_Active())*/
			m_pWaterShots[i]->Priority_Update(fTimeDelta);
	}


	if (false == m_bShotEnd && (*m_pNowShotTiming) >= 2.5f )
	{
		m_bShotSign = true;
	}
	if (0.f==(*m_pNowShotTiming))
	{
		m_bShotEnd = false; 
		m_bShotSign = false;
		m_pCharging->Reset_Vertices(fTimeDelta);

	}
	if ((0.8f <= *m_pNowShotTiming) && (*m_pNowShotTiming) <= 2.5f)
	{
		m_pCharging->Priority_Update(fTimeDelta);
	}

	
}

void CMonster09_Head::Update(_float fTimeDelta)
{
	//헤드의 최종 위치 행렬갱신
	if (m_pSocketMatrix) {
		_matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
		for (size_t j = 0; j < 3; ++j)
		{
			SocketMatrix.r[j] = XMVector3Normalize(SocketMatrix.r[j]);
		}
		XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
	}

	for (int i = 0; i < 3; ++i)
	{
		_matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix_Eyes[i]);
		for(size_t j = 0; j < 3; ++j)
		{
			SocketMatrix.r[j] = XMVector3Normalize(SocketMatrix.r[j]);
		}
		XMStoreFloat4x4(&m_EyeMatrix[i], m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
	}

	/*파츠 (총알 업데이트 */
	for (size_t i = 0; i < m_iNumShots; ++i)
	{
		/*if (m_pWaterShots[i]->Get_Active())*/
			m_pWaterShots[i]->Update(fTimeDelta);
	}
	
	/* 총알 발사 및 파티클 준비동작 로직 작성*/
	{
		_vector vShotPos;


		CTransform* pTargetTransform = static_cast <CTransform*> (m_pGameInstance->Get_Component(LEVEL_GAMEPLAY2, TEXT("Layer_APlayer"), TEXT("Com_Transform"), 0));
		if (nullptr == pTargetTransform)
			return;
		_vector vPlayerPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
		_vector vMyPos = XMLoadFloat4((_float4*)&m_WorldMatrix.m[3]) - XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vMyLook = XMLoadFloat4((_float4*)&m_WorldMatrix.m[2]);

		_vector vShotLook = XMVector3Normalize(vPlayerPos - vMyPos);
		_vector vShotRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vShotLook));
		_vector vShotUp =   XMVector3Normalize(XMVector3Cross(vShotLook, vShotRight));
		vShotPos = XMVector3Normalize(vMyLook) + vMyPos;
		_vector vParticleBunchPos = vShotPos;
		if (true == m_bShotSign && ((*m_pState) & M9_STATE::M9_STATE_ATTACK_SHOT))
		{

			m_bShotSign = false;
			m_bShotEnd = true;
			/*Shot logic*/
			m_pWaterShots[m_iNowShotIndex]->Set_Active(true);

			_matrix WaterShotMatrix;

			WaterShotMatrix.r[0] = vShotRight;
			WaterShotMatrix.r[1] = vShotUp;
			WaterShotMatrix.r[2] = vShotLook;
			WaterShotMatrix.r[3] = vShotPos;
			m_pWaterShots[m_iNowShotIndex]->Reset_Bullets(WaterShotMatrix);

			m_iNowShotIndex = (1 + m_iNowShotIndex) % (m_iNumShots);

		}


		if ((0.8f <= *m_pNowShotTiming) && (*m_pNowShotTiming) <= 2.5f)
		{
			m_pCharging->Late_Update(fTimeDelta);

			_float3 vParticlePos;
			memcpy(&vParticlePos, m_WorldMatrix.m[3], sizeof(_float3));
			m_pCharging->Set_Position(vParticleBunchPos);
			//m_pParticle->Set_Pivot(vParticlePos);

		}
	}

	if((0.8f<=*m_pNowShotTiming) && (*m_pNowShotTiming) <= 2.5f)
		m_pCharging->Update(fTimeDelta);

	if ((M9_STATE::M9_STATE_ATTACK_WAVE_RIGHT & *m_pState) || (M9_STATE::M9_STATE_ATTACK_WAVE_LEFT & *m_pState))
	{
		if (m_fAccAddTrailTiming >= m_fAddTrail_Timing) {
			for (size_t i = 0; i < 3; ++i)
			{
				m_pTrail_Eyes[i]->Set_BufferMatrix(XMLoadFloat4x4(&m_EyeMatrix[i]), XMLoadFloat4x4(&m_EyeMatrix[i]));
			}
		}
	}
	else
	{
		for (size_t i = 0; i < 3; ++i)
		{
			m_pTrail_Eyes[i]->Reset_VerticesCnt();
		}
	}
	if (m_fAccAddTrailTiming >= m_fAddTrail_Timing)
		m_fAccAddTrailTiming = 0.f;
	 
}

void CMonster09_Head::Late_Update(_float fTimeDelta)
{
	if ((0.8f <= *m_pNowShotTiming) && (*m_pNowShotTiming) <= 2.5f)
		m_pCharging->Late_Update(fTimeDelta);

	for (size_t i = 0; i < m_iNumShots; ++i)
	{
		/*if (m_pWaterShots[i]->Get_Active())*/
			m_pWaterShots[i]->Late_Update(fTimeDelta);
	}

	if ((M9_STATE::M9_STATE_ATTACK_WAVE_RIGHT & *m_pState) ||(M9_STATE::M9_STATE_ATTACK_WAVE_LEFT & *m_pState))
	{
		for (size_t i = 0; i < 3; ++i)
		{
			m_pTrail_Eyes[i]->Late_Update(fTimeDelta);
		}
	}
	
}

HRESULT CMonster09_Head::Render()
{
	//if ((0.8f <= *m_pNowShotTiming) && (*m_pNowShotTiming) <= 2.5f)
		//m_pParticle->Render();
	
	//m_pColliderCom->Render();
	return S_OK;
}

HRESULT CMonster09_Head::Ready_Components()
{
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster09_Head::Ready_Bullets()
{
	m_pWaterShots.resize(m_iNumShots);
	CMonster09_Shot::MONSTER09_SHOT_DESC Desc = {};
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.fSpeedPerSec = 10.f;
	Desc.fSpeed = 3.f;

	for (size_t i = 0; i < m_iNumShots; ++i)
	{
		m_pWaterShots[i] = static_cast<CMonster09_Shot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Shot"), &Desc));
	}

	return S_OK;
}

HRESULT CMonster09_Head::Ready_Particle()
{
	//m_pParticle = static_cast<CParticle_Splash*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Splash"), nullptr));;
	m_pCharging= static_cast<CParticle_Monster09_Shot_Charging*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Shot_Charging"), nullptr));;

	
	return S_OK;
}

HRESULT CMonster09_Head::Bind_ShaderResources()
{
	return S_OK;
}

HRESULT CMonster09_Head::Ready_Trails()
{


	for(size_t i=0;i<3;++i)
		m_pTrail_Eyes[i] = static_cast<CTrail_Monster09*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Trail_Monster09"), nullptr));



	return S_OK;
}

CMonster09_Head* CMonster09_Head::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster09_Head* pInstance = new CMonster09_Head(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster09_Head");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster09_Head::Clone(void* pArg)
{
	CMonster09_Head* pInstance = new CMonster09_Head(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster09_Head");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster09_Head::Free()
{
	__super::Free();

	for (auto Shot : m_pWaterShots)
	{
		Safe_Release(Shot);
	}
	m_pWaterShots.clear();
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pParticle);
	Safe_Release(m_pCharging);
	for (int i = 0; i < 3; ++i)
	{
		Safe_Release(m_pTrail_Eyes[i]);
	}
}
