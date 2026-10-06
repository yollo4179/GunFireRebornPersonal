#include"stdafx.h"
#include"Effect_Monster05_Ready.h"
#include"GameInstance.h"

CEffect_Monster05_Ready::CEffect_Monster05_Ready(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CEffect_Monster05_Ready::CEffect_Monster05_Ready(const CEffect_Monster05_Ready& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CEffect_Monster05_Ready::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Monster05_Ready::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Monster05_Ready::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Monster05_Ready::Update(_float fTimeDelta)
{
}

void CEffect_Monster05_Ready::Late_Update(_float fTimeDelta)
{
	if (false == m_isActive)
		return;
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
}

HRESULT CEffect_Monster05_Ready::Render()
{

	if (true == m_bGlow)
	{
		if (FAILED(Bind_ShaderResources_Glow()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Bind_BufferDesc();

		m_pVIBufferCom->Render();
	}
	else {
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Bind_BufferDesc();

		m_pVIBufferCom->Render();
	}
	return S_OK;
}

void CEffect_Monster05_Ready::Set_Matrix(_fmatrix PosMatrix)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVectorSet(4.f, 0.f, 0.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, PosMatrix.r[3] - 1.5F*PosMatrix.r[2] );

}

HRESULT CEffect_Monster05_Ready::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_Glow"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster05_Ready"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Monster05_Ready::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;



	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Monster05_Ready::Bind_ShaderResources_Glow()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;



	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture", 0)))
		return E_FAIL;

	_float3 vThreshold = { 0.f,0.f,1.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vThreshord", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float fIntensity = 0.3f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", &fIntensity, sizeof(_float))))
		return E_FAIL;
	return S_OK;
}

CEffect_Monster05_Ready* CEffect_Monster05_Ready::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Monster05_Ready* pInstance = new CEffect_Monster05_Ready(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Monster05_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Monster05_Ready::Clone(void* pArg)
{
	CEffect_Monster05_Ready* pInstance = new CEffect_Monster05_Ready(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Monster05_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Monster05_Ready::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
