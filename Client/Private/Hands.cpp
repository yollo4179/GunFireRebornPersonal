#include"stdafx.h"
#include "Hands.h"
#include "GameInstance.h"
#include"UI_Arms.h"

CHands::CHands(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{ pDevice, pContext }
{
}

CHands::CHands(const CHands& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CHands::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CHands::Initialize(void* pArg)
{

	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pArms = static_cast<CUI_Arms*> (m_pGameInstance->Get_UI_By_Tag(L"Arms"));
	Safe_AddRef(m_pArms);
	if (nullptr == m_pArms) 
	{
		MSG_BOX("Couldn't  Get UI Arms In Hands");
		return E_FAIL;
	}



	return S_OK;
}

void CHands::Priority_Update(_float fTimeDelta)
{
	
}

void CHands::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);
}

void CHands::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CHands::Render()
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


	return S_OK;

}


void CHands::Set_Matrix_From_Parent(CGameObject* pGameObject)
{
	
	


}

void CHands::Set_CurrentTransform_Matrix()
{
}

_bool CHands::Fire_A_Bullet()
{

	if (0 < m_iNumBullets_ICanFireNow)
	{
		m_iNumBullets_ICanFireNow -= 1;
		return true;
	}
	else
		return false;
}

_bool CHands::Load_Bullets()
{
	if ((m_iNumBullets_YouHave >= m_iNumChargingBullets) ||m_isInfinity)
	{
		m_iNumBullets_ICanFireNow = m_iNumChargingBullets;
		m_iNumBullets_YouHave -= m_iNumChargingBullets;

		return true;
	}
	else
	{
		return false;
	}

}



HRESULT CHands::Ready_Components()
{
	///* Com_Shader */
	//if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;

	///* Com_Model */
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player01"),
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;


	return S_OK;


}

HRESULT CHands::Bind_ShaderResources()
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

HRESULT CHands::Bind_ShaderResources_For_Collider()
{
	


	return S_OK;
}

CHands* CHands::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands* pInstance = new CHands(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CHands::Clone(void* pArg)
{
	CHands* pInstance = new CHands(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster01 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CHands::Free()
{
	__super::Free();

	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pArms);
}

