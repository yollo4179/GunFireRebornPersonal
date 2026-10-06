#include"stdafx.h"
#include "Terrain_Base.h"
#include"GameInstance.h"

CTerrain_Base::CTerrain_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CTerrain_Base::CTerrain_Base(const CTerrain_Base& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CTerrain_Base::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	//오브젝트에서 트랜 스폼 생성
	if (nullptr == m_pTransformCom)
	{
		MSG_BOX("failed to create in Terrain Base transform com");
		return E_FAIL;
	}
	TERRAIN_BASE_DESC* pDesc = static_cast<TERRAIN_BASE_DESC*>(pArg);
	
	if (nullptr != pDesc)
	{
		m_Desc.vAxis = pDesc->vAxis;
		m_Desc.vPosition = pDesc->vPosition;
		m_Desc.vScale = pDesc->vScale;
		m_Desc.fRotation = pDesc->fRotation;
		m_Desc.fRotation = pDesc->fSpeedPerSec;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_Desc.vPosition));
		m_pTransformCom->Scaling(m_Desc.vScale.x, m_Desc.vScale.y, m_Desc.vScale.z);
		m_pTransformCom->Rotation(XMLoadFloat4(&m_Desc.vAxis), m_Desc.fRotation);
	}
	else
	{
		m_Desc.vAxis = { 0.f,1.f,0.f,0.f };
		m_Desc.vPosition = { 0.f,0.f,0.f,1.f };
		m_Desc.vScale = { 1.f,1.f,1.f };
		m_Desc.fRotation = 0.f;
		m_Desc.fRotationPerSec = 1;
		m_Desc.fRotationPerSec = 1.f;
	}
	
	return S_OK;
}

HRESULT CTerrain_Base::Initialize_Prototype()
{
	return S_OK;
}


void CTerrain_Base::Priority_Update(_float fTimeDelta)
{
}

void CTerrain_Base::Update(_float fTimeDelta)
{
}

void CTerrain_Base::Late_Update(_float fTimeDelta)
{
	/*m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);*/
}

HRESULT CTerrain_Base::Render()
{
	//if (FAILED(Bind_ShaderResources()))
	//	return E_FAIL;

	//_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	//for (size_t i = 0; i < iNumMeshes; i++)
	//{
	//	//(메시마다의 diffuse텍스처 던지기)
	//	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
	//		return E_FAIL;
	//	//셰이더 제출
	//	m_pShaderCom->Begin(0);

	//	//정점 그리기
	//	m_pModelCom->Render(i);
	//}
	return S_OK;
}

void CTerrain_Base::Update_Transform_By_Desc(TERRAIN_BASE_DESC Desc)
{
	_float3  vScale = Desc.vScale;
	m_pTransformCom->Scaling(vScale.x, vScale.y, vScale.z);
	_vector vPosition = XMLoadFloat4(&Desc.vPosition);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	_float fRot = Desc.fRotation;
	_vector vAxis =XMVector3Normalize( XMLoadFloat4(&Desc.vAxis));
	m_pTransformCom->Rotation(vAxis, fRot);
	m_Desc = Desc;

	return;
}

void CTerrain_Base::Update_Transform_Go_Straight(const _float fTimeDelta)
{
	if (fTimeDelta >= 0.1f)
		int a = 0;
	m_pTransformCom->Go_Straight(fTimeDelta);
	XMStoreFloat4(&m_Desc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

}

void CTerrain_Base::Update_Transform_Go_Backward(const _float fTimeDelta)
{
}

HRESULT CTerrain_Base::Ready_Components()
{
	return S_OK;
}

HRESULT CTerrain_Base::Bind_ShaderResources()
{
	/*if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
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
		return E_FAIL;*/

	return S_OK;
}



CGameObject* CTerrain_Base::Clone(void* pArg)
{
	return nullptr;
}

void CTerrain_Base::Free()
{

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	__super::Free();
}
