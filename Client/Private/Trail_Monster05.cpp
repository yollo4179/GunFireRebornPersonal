#include"stdafx.h"
#include"Trail_Monster05.h"
#include"GameInstance.h"
CTrail_Monster05::CTrail_Monster05(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice, pContext}
{
}

CTrail_Monster05::CTrail_Monster05(const CTrail_Monster05& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CTrail_Monster05::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrail_Monster05::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CTrail_Monster05::Priority_Update(_float fTimeDelta)
{
	
}

void CTrail_Monster05::Update(_float fTimeDelta)
{
}

void CTrail_Monster05::Late_Update(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta*5.f;
	if (m_fAccTime > m_fTime)
		m_fAccTime = 0.f;

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
}

HRESULT CTrail_Monster05::Render()
{
	if (true == m_bGlow)
	{
		if (FAILED(Bind_ShaderResources_Glow()))
			return E_FAIL;

		m_pShaderCom->Begin(3);

		m_pVIBufferCom->Bind_BufferDesc();

		m_pVIBufferCom->Render();
	}
	else {
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(2);

		m_pVIBufferCom->Bind_BufferDesc();

		m_pVIBufferCom->Render();
	}
	return S_OK;
}

void CTrail_Monster05::Set_BufferMatrix(_fmatrix matHigh, _fmatrix matLow)
{
	
	
	m_pVIBufferCom->Add_Points_CatMullRom(matHigh, matLow);
}

void CTrail_Monster05::Reset_VerticesCnt()
{
	m_pVIBufferCom->Reset_VerticesCnt();
}

void CTrail_Monster05::Resize_Vertices()
{
	m_pVIBufferCom->Resize_Vertices();
}

HRESULT CTrail_Monster05::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_Glow"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	CVIBuffer_Trail::TRAIL_DESC TrailDesc = {};
	TrailDesc.fTrailLength = 1.f;
	TrailDesc.iNumMaxVertices = 300;
	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &TrailDesc)))
		return E_FAIL;

	m_pVIBufferCom->Resize_Vertices();

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster05_Trail"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CTrail_Monster05::Bind_ShaderResources()
{
	_float4x4 WorldFloat4x4;
	XMStoreFloat4x4(&WorldFloat4x4, XMMatrixIdentity());
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &WorldFloat4x4)))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrail_Monster05::Bind_ShaderResources_Glow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture", 0)))
		return E_FAIL;

	_float3 vThreshold = { 0.f,0.f,0.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vThreshord", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float fIntensity = 0.1f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", &fIntensity, sizeof(_float))))
		return E_FAIL;
	return S_OK;
}

CTrail_Monster05* CTrail_Monster05::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrail_Monster05* pInstance = new CTrail_Monster05(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrail_Monster05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrail_Monster05::Clone(void* pArg)
{
	CTrail_Monster05* pInstance = new CTrail_Monster05(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTrail_Monster05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail_Monster05::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
