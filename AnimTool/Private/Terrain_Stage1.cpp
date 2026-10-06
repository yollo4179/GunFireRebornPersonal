#include"stdafx.h"
#include "Terrain_Stage1.h"
#include"GameInstance.h"
CTerrain_Stage01::CTerrain_Stage01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTerrain_Base{pDevice, pContext}
{
}

CTerrain_Stage01::CTerrain_Stage01(const CTerrain_Stage01& Prototype)
	:CTerrain_Base(Prototype)
{
}

HRESULT CTerrain_Stage01::Initialize(void* pArg)
{
	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (nullptr == m_pTransformCom)
	{
		MSG_BOX("failed to create stage01 transform com");
		return E_FAIL;
	}


	if (Ready_Components())
		return E_FAIL;



	return S_OK;
}

HRESULT CTerrain_Stage01::Initialize_Prototype()
{
	return S_OK;
}


void CTerrain_Stage01::Priority_Update(_float fTimeDelta)
{
}

void CTerrain_Stage01::Update(_float fTimeDelta)
{
}

void CTerrain_Stage01::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CTerrain_Stage01::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		//(메시마다의 diffuse텍스처 던지기)
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		//셰이더 제출
		m_pShaderCom->Begin(0);

		//정점 그리기
		m_pModelCom->Render(i);
	}
	return S_OK;
}



HRESULT CTerrain_Stage01::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Stage01"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CTerrain_Stage01::Bind_ShaderResources()
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

	return S_OK;
}

CTerrain_Stage01* CTerrain_Stage01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain_Stage01* pInstance = new CTerrain_Stage01(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CTerrain_Stage01 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance; 
}

CGameObject* CTerrain_Stage01::Clone(void* pArg)
{
	CTerrain_Stage01* pInstance = new CTerrain_Stage01(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CTerrain_Stage01: Failed");
		Safe_Release(pInstance);
		return nullptr; 
	}

	return pInstance;
}

void CTerrain_Stage01::Free()
{
	__super::Free();
}

