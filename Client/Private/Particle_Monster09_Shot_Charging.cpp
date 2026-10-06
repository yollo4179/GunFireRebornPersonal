#include"stdafx.h"
#include"Particle_Monster09_Shot_Charging.h"
#include"GameInstance.h"
CParticle_Monster09_Shot_Charging::CParticle_Monster09_Shot_Charging(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CParticle_Monster09_Shot_Charging::CParticle_Monster09_Shot_Charging(const CParticle_Monster09_Shot_Charging& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CParticle_Monster09_Shot_Charging::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Monster09_Shot_Charging::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVectorSet(1.f, 0.f, 0.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Monster09_Shot_Charging::Priority_Update(_float fTimeDelta)
{
	m_fAccTimeDelta += fTimeDelta * 10.f;
	if (m_fAccTimeDelta >= 5.f)
		m_fAccTimeDelta = 0.f;
}

void CParticle_Monster09_Shot_Charging::Update(_float fTimeDelta)
{
	m_pVIBufferCom->Absorb(fTimeDelta);
}

void CParticle_Monster09_Shot_Charging::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONLIGHT, this);
}

HRESULT CParticle_Monster09_Shot_Charging::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

void CParticle_Monster09_Shot_Charging::Set_Position(_fvector vPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CParticle_Monster09_Shot_Charging::Reset_Vertices(_float fTimeDelta)
{
	m_pVIBufferCom->Reset_Vertices_Random(fTimeDelta);
}

void CParticle_Monster09_Shot_Charging::Set_Matrix(_fmatrix PosMatrix)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, PosMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, PosMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, PosMatrix.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, PosMatrix.r[3]);
}

HRESULT CParticle_Monster09_Shot_Charging::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_VIBuffer_Particle_Monster09_Shot_Charging"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Particle_Attack"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Monster09_Shot_Charging::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", ((int)m_fAccTimeDelta) % 4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CParticle_Monster09_Shot_Charging* CParticle_Monster09_Shot_Charging::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Monster09_Shot_Charging* pInstance = new   CParticle_Monster09_Shot_Charging(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :   CParticle_Monster09_Shot_Charging");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Monster09_Shot_Charging::Clone(void* pArg)
{
	CParticle_Monster09_Shot_Charging* pInstance = new   CParticle_Monster09_Shot_Charging(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :   CParticle_Monster09_Shot_Charging");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Monster09_Shot_Charging::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
