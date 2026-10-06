#include"stdafx.h"
#include "Monster05.h"
#include "GameInstance.h"


CMonster05::CMonster05(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster05::CMonster05(const CMonster05& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster05::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster05::Initialize(void* pArg)
{
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(rand() % 5, true);

	return S_OK;
}

void CMonster05::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMonster05::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta, m_isChangeing);
}

void CMonster05::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMonster05::Render()
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


	if (FAILED(Bind_ShaderResources_For_Collider()))
		return E_FAIL;

	{

		m_pShader_For_ColliderCom->Bind_RawValue("g_ColorR", &m_arrColiderColor[0], sizeof(_float));
		m_pShader_For_ColliderCom->Bind_RawValue("g_ColorG", &m_arrColiderColor[1], sizeof(_float));
		m_pShader_For_ColliderCom->Bind_RawValue("g_ColorB", &m_arrColiderColor[2], sizeof(_float));
	}
	m_pShader_For_ColliderCom->Begin(0);
	m_pVIBuffer_ColliderCom->Bind_BufferDesc();
	m_pContext->RSSetState(m_CullMode);
	m_pVIBuffer_ColliderCom->Render();
	m_pContext->RSSetState(m_NoneCullMode);
	return S_OK;

}

HRESULT CMonster05::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster05"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	/*Com_Shader_For_Collider*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeCol"),
		TEXT("Com_Shader_For_Collider"), reinterpret_cast<CComponent**>(&m_pShader_For_ColliderCom))))
		return E_FAIL;
	/*Com_VIBuffer_Collider*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Collider"),
		TEXT("Com_VIBuffer_Collider"), reinterpret_cast<CComponent**>(&m_pVIBuffer_ColliderCom))))
		return E_FAIL;
	return S_OK;


}

HRESULT CMonster05::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CMonster05* CMonster05::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster05* pInstance = new CMonster05(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster05_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster05::Clone(void* pArg)
{
	CMonster05* pInstance = new CMonster05(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster05 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster05::Free()
{
	__super::Free();


	
}