#include"stdafx.h"
#include"Particle_Player04_Rasor.h"

#include"GameInstance.h"
CParticle_Player04_Rasor::CParticle_Player04_Rasor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CParticle_Player04_Rasor::CParticle_Player04_Rasor(const CParticle_Player04_Rasor& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CParticle_Player04_Rasor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Player04_Rasor::Initialize(void* pArg)
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

void CParticle_Player04_Rasor::Priority_Update(_float fTimeDelta)
{
}

void CParticle_Player04_Rasor::Update(_float fTimeDelta)
{
	_matrix BufferMatrix= XMMatrixIdentity();
	BufferMatrix.r[0] *= 1.f;
	BufferMatrix.r[1] *= m_fRayDist;;
	//BufferMatrix *= XMMatrixRotationX(XMConvertToRadians(90.f));
	m_pVIBufferCom->Reset_Particle_Transform(XMLoadFloat4x4(&m_ParticleMatrix));
}

void CParticle_Player04_Rasor::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONLIGHT, this);
}

HRESULT CParticle_Player04_Rasor::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(3);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

void CParticle_Player04_Rasor::Set_Pivot(_float3 fPivot)
{
	m_pVIBufferCom->Set_Pivot(fPivot);
}

void CParticle_Player04_Rasor::Set_Position(_fvector vPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CParticle_Player04_Rasor::Reset_Vertices(_float fTimeDelta)
{
	//m_pVIBufferCom->Reset_Particle_Transform(XMLoadFloat4x4(& m_ParticleMatrix));
}

void CParticle_Player04_Rasor::Set_Matrix(_fmatrix PosMatrix)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, PosMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, PosMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, PosMatrix.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, PosMatrix.r[3]);

}

void CParticle_Player04_Rasor::Set_ParticleMatrix(_fmatrix ParticleMatrix)
{
	_matrix matParticle = XMLoadFloat4x4(&m_ParticleMatrix);

	matParticle.r[0] = ParticleMatrix.r[0];
	matParticle.r[1] = ParticleMatrix.r[1];
	matParticle.r[2] = ParticleMatrix.r[2];
	matParticle.r[3] = ParticleMatrix.r[3];
	XMStoreFloat4x4(&m_ParticleMatrix, matParticle);
}

HRESULT CParticle_Player04_Rasor::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player04_Rasor"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player04_Rasor"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

HRESULT CParticle_Player04_Rasor::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CParticle_Player04_Rasor* CParticle_Player04_Rasor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Player04_Rasor* pInstance = new   CParticle_Player04_Rasor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :   CParticle_Player04_Rasor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Player04_Rasor::Clone(void* pArg)
{
	CParticle_Player04_Rasor* pInstance = new   CParticle_Player04_Rasor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :   CParticle_Player04_Rasor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Player04_Rasor::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
