#include"stdafx.h"
#include"Particle_Player05_Bomb.h"
#include"GameInstance.h"
CParticle_Player05_Bomb::CParticle_Player05_Bomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CParticle_Player05_Bomb::CParticle_Player05_Bomb(const CParticle_Player05_Bomb& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CParticle_Player05_Bomb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Player05_Bomb::Initialize(void* pArg)
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

void CParticle_Player05_Bomb::Priority_Update(_float fTimeDelta)
{
	m_fAccTimeDelta += fTimeDelta ;
	if (m_fAccTimeDelta >= 5.f)
		m_fAccTimeDelta = 0.f;
}

void CParticle_Player05_Bomb::Update(_float fTimeDelta)
{
	m_pVIBufferCom->Rotation_Horizon(fTimeDelta);
}

void CParticle_Player05_Bomb::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONLIGHT, this);
}

HRESULT CParticle_Player05_Bomb::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

void CParticle_Player05_Bomb::Set_Pivot(_float3 fPivot)
{
	m_pVIBufferCom->Set_Pivot(fPivot);
}

void CParticle_Player05_Bomb::Set_Position(_fvector vPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CParticle_Player05_Bomb::Reset_Vertices(_float fTimeDelta)
{
	m_pVIBufferCom->Reset_Vertices_Random(fTimeDelta);
}

void CParticle_Player05_Bomb::Set_Matrix(_fmatrix PosMatrix)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, PosMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, PosMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, PosMatrix.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, PosMatrix.r[3]);
}

HRESULT CParticle_Player05_Bomb::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player05_Bullet_Bomb"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player05_Bullet_Chasing"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

HRESULT CParticle_Player05_Bomb::Bind_ShaderResources()
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

CParticle_Player05_Bomb* CParticle_Player05_Bomb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Player05_Bomb* pInstance = new   CParticle_Player05_Bomb(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :   CParticle_Player05_Bomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Player05_Bomb::Clone(void* pArg)
{
	CParticle_Player05_Bomb* pInstance = new   CParticle_Player05_Bomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :   CParticle_Player05_Bomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Player05_Bomb::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
