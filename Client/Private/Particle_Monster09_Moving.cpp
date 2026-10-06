#include"stdafx.h"
#include"Particle_Monster09_Moving.h"
#include "GameInstance.h"

CParticle_Monster09_Moving::CParticle_Monster09_Moving(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CParticle_Monster09_Moving::CParticle_Monster09_Moving(const CParticle_Monster09_Moving& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CParticle_Monster09_Moving::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Monster09_Moving::Initialize(void* pArg)
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

void CParticle_Monster09_Moving::Priority_Update(_float fTimeDelta)
{
	
	if (false == m_bActive)
		return;
	m_fAccTimeDelta += fTimeDelta * 10.f;
	if (m_fAccTimeDelta >= 15.f)
		m_fAccTimeDelta = 0.f;

}
	
 

void CParticle_Monster09_Moving::Update(_float fTimeDelta)
{
	
	m_pVIBufferCom->Spread_Gravity(fTimeDelta);//중력으로 바꾸기
}

void CParticle_Monster09_Moving::Late_Update(_float fTimeDelta)
{
	
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONLIGHT, this);
}

HRESULT CParticle_Monster09_Moving::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

void CParticle_Monster09_Moving::Set_Pivot(_float3 fPivot)
{
	m_pVIBufferCom->Set_Pivot(fPivot);
}
void CParticle_Monster09_Moving::Set_Matrix(_fmatrix PosMatrix, _uint iLeftRight)
{

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT,XMVectorSet(1.f,0.f,0.f,0.f) );
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, PosMatrix.r[3]+XMVectorSet(0.f,-3.f,0.f,0.f));

	if (0==iLeftRight)
	{
		m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-60.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, PosMatrix.r[3] + XMVectorSet(-3.f, 0.f, 0.f, 0.f));
	}
	else
	{
		m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(60.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, PosMatrix.r[3] + XMVectorSet(3.f, 0.f, 0.f, 0.f));
	}
	
}


void CParticle_Monster09_Moving::Reset_Vertices(_float fTimeDelta)
{
	m_pVIBufferCom->Reset_Vertices_Random(fTimeDelta);
}

HRESULT CParticle_Monster09_Moving::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_VIBuffer_Particle_Monster09_Moving"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;
	m_pVIBufferCom->Reset_Vertices_Random(1);
	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Particle_Rush_Wave"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CParticle_Monster09_Moving::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", ((int)m_fAccTimeDelta) % 15)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;;
}

CParticle_Monster09_Moving* CParticle_Monster09_Moving::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Monster09_Moving* pInstance = new   CParticle_Monster09_Moving(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :   CParticle_Monster09_Moving");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CParticle_Monster09_Moving::Clone(void* pArg)
{
	CParticle_Monster09_Moving* pInstance = new   CParticle_Monster09_Moving(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :   CParticle_Monster09_Moving");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Monster09_Moving::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
