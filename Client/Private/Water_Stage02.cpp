#include "stdafx.h"
#include "..\Public\Water_Stage02.h"
#include "GameInstance.h"


CWater_Stage02::CWater_Stage02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CWater_Stage02::CWater_Stage02(const CWater_Stage02& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CWater_Stage02::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWater_Stage02::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	m_pTransformCom->Scaling(5.f, 5.f, 8.f);
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-100.f, 0.f, 40.f, 1.f));

	if (FAILED(Ready_Components()))
		return E_FAIL;




	return S_OK;
}

void CWater_Stage02::Priority_Update(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	int a = 10;
}

void CWater_Stage02::Update(_float fTimeDelta)
{

}

void CWater_Stage02::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CWater_Stage02::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	
	m_pVIBufferCom->Render();
	

	return S_OK;
}

HRESULT CWater_Stage02::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Shader_VtxWater"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Flat"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Water"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom[0]))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Water_Normal"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[1]))))
		return E_FAIL;


	return S_OK;
}

HRESULT CWater_Stage02::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;



	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("Time", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[0]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[1]->Bind_ShaderResource(m_pShaderCom, "g_NormalMap", 0)))
		return E_FAIL;

	CTexture* pSkyTexture =static_cast<CTexture*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY2, L"Layer_Map", L"Com_Texture",1));
	if (FAILED(pSkyTexture->Bind_ShaderResource(m_pShaderCom, "g_EnvironmentMap", 5)))
		return E_FAIL;

	return S_OK;
}

CWater_Stage02* CWater_Stage02::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWater_Stage02* pInstance = new CWater_Stage02(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWater_Stage02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWater_Stage02::Clone(void* pArg)
{
	CWater_Stage02* pInstance = new CWater_Stage02(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWater_Stage02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWater_Stage02::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom[0]);
	Safe_Release(m_pTextureCom[1]);
	Safe_Release(m_pVIBufferCom);
	__super::Free();



}
