#include"stdafx.h"
#include "Hands04_Rasor.h"
#include"GameInstance.h"
CHands04_Rasor::CHands04_Rasor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice,pContext}
{
}

CHands04_Rasor::CHands04_Rasor(const CHands04_Rasor& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CHands04_Rasor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHands04_Rasor::Initialize(void* pArg)
{
	HANDS04_RASOR_DESC* pDesc = static_cast<HANDS04_RASOR_DESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Components()))
		return E_FAIL;
	return S_OK;
}

void CHands04_Rasor::Priority_Update(_float fTimeDelta)
{
	
}

void CHands04_Rasor::Update(_float fTimeDelta)
{
}

void CHands04_Rasor::Late_Update(_float fTimeDelta)
{
	if (false == m_isActive)
		return;

	m_fAccTime += fTimeDelta;
	if (m_fAccTime > m_fTimeLimit)
		m_fAccTime = 0.f;

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
}

HRESULT CHands04_Rasor::Render()
{
	if (m_bGlow)
	{
		if (FAILED(Render_Glow()))
			return E_FAIL;
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

HRESULT CHands04_Rasor::Render_Glow()
{
	if (FAILED(Bind_ShaderResources_Glow()))
		return E_FAIL;

	
	
	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CHands04_Rasor::SetPos(_fvector SrcPos, _fvector DescPos)
{
	_float fMyLength = XMVectorGetX(XMVector3Length(DescPos - SrcPos));
	m_fUVLength = fMyLength;
	_matrix PreMatrix = XMMatrixScaling(1.f, fMyLength, 1.f) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f))
		* XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(90.f));

	
	_vector vMyPos = 0.5f * (DescPos + SrcPos);

	_vector vLook = XMVector3Normalize(DescPos - SrcPos);

	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	_matrix matWorld ;
	matWorld.r[0]=vRight ;
	matWorld.r[1]= vUp*0.2f;
	matWorld.r[2]=vLook;
	matWorld.r[3] =vMyPos;

	matWorld = PreMatrix * matWorld;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, matWorld.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, matWorld.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, matWorld.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matWorld.r[3]);





}

HRESULT CHands04_Rasor::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_Hands04_Rasor"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player04_Rasor"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player04_Rasor_Glow_Trail"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Glow))))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CHands04_Rasor::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Glow->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom_Glow->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture_2", 1)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTime", &m_fAccTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fUVLength", &m_fUVLength, sizeof(_float))))
		return E_FAIL;
	return S_OK;
}

HRESULT CHands04_Rasor::Bind_ShaderResources_Glow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Glow->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom_Glow->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture_2",1)))
		return E_FAIL;
	

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	_float3 vThreshold = { 0.0f,0.5f,0.9f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vThreshold", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float vIntensity = 0.3f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", &vIntensity, sizeof(_float))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTime", &m_fAccTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fUVLength", &m_fUVLength, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CHands04_Rasor* CHands04_Rasor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands04_Rasor* pInstance = new CHands04_Rasor(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CHands04_Rasor");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CHands04_Rasor::Clone(void* pArg)
{
	CHands04_Rasor* pInstance = new CHands04_Rasor(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CHands04_Rasor");
		return nullptr;
	}

	return pInstance;
}

void CHands04_Rasor::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom_Glow);


}
