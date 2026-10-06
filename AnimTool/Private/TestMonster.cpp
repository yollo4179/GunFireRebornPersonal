#include"stdafx.h"
#include "TestMonster.h"
#include"GameInstance.h"
CTestMonster::CTestMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
:CMonster(pDevice, pContext)
{
}

CTestMonster::CTestMonster(const CTestMonster& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CTestMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTestMonster::Initialize(void* pArg)
{
	
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(rand()&10, true);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
	XMVectorSet(m_pGameInstance->Compute_Random(0.f, 10.f), 0.f, m_pGameInstance->Compute_Random(0.f, 10.f), 1.f));

	return S_OK;

}

void CTestMonster::Priority_Update(_float fTimeDelta)
{
}

void CTestMonster::Update(_float fTimeDelta)
{

	m_pModelCom->Play_Animation(fTimeDelta, m_isChangeing);
}

void CTestMonster::Late_Update(_float fTimeDelta)
{
	//m_fAccTime += fTimeDelta;
	//m_pModelCom->Set_AnimIndex((_int)m_fAccTime);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	//if (m_fAccTime > 8) m_fAccTime = 0.f;
}

HRESULT CTestMonster::Render()
{


	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	
	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	//for (size_t i = 0; i < iNumMeshes; i++)
	//{
	//	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
	//		return E_FAIL;
	///*	if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
	//		return E_FAIL;*/

	//	m_pShaderCom->Begin(0);

	//	m_pModelCom->Render(i);
	//}

	//__super::Render();

	return S_OK;
}

HRESULT CTestMonster::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTestMonster::Bind_ShaderResources()
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

CTestMonster* CTestMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTestMonster* pInstance = new CTestMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTestMonster::Clone(void* pArg)
{
	CTestMonster* pInstance = new CTestMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTestMonster::Free()
{
	__super::Free();


}
