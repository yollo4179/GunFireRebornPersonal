#include"stdafx.h"
#include "Flat_Virtual_Terrain.h"
#include"GameInstance.h"

CFlat_Virtual_Terrain::CFlat_Virtual_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTerrain_Base{pDevice, pContext}
{
}

CFlat_Virtual_Terrain::CFlat_Virtual_Terrain(const CFlat_Virtual_Terrain& Prototype)
	:CTerrain_Base(Prototype)
{
}

HRESULT CFlat_Virtual_Terrain::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CFlat_Virtual_Terrain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	if (nullptr != pArg)
	{
		FLAT_VIRTUAL_TERRAIN_DESC* pDesc = static_cast<FLAT_VIRTUAL_TERRAIN_DESC*>(pArg);
		m_Flat_Desc = *pDesc;
	}

	else
	{
		m_Flat_Desc.fSizeX = 1.f;
		m_Flat_Desc.fSizeZ = 1.f;
		m_Flat_Desc.iNumX = 10;
		m_Flat_Desc.iNumZ = 10;

	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	{
		m_vecVertices_Origin = *(m_pVIBufferCom->Get_VerticesList());
		m_vecVertices_World = m_vecVertices_Origin;

		int iIndex = 0;
		for (const auto& vertice : m_vecVertices_Origin)
		{
			XMStoreFloat4(&m_vecVertices_World[iIndex++], XMVector3TransformCoord(XMLoadFloat4(&vertice), m_pTransformCom->Get_WorldMatrix()));
		}
	}

	//D3D11_RASTERIZER_DESC rasterizerDesc;
	//ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	//rasterizerDesc.CullMode = D3D11_CULL_BACK;
	//rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_NoneCullMode));
	//rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	//rasterizerDesc.CullMode = D3D11_CULL_NONE;
	//(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_CullMode));



	return S_OK;
}

void CFlat_Virtual_Terrain::Priority_Update(_float fTimeDelta)
{

}

void CFlat_Virtual_Terrain::Update(_float fTimeDelta)
{

	int iIndex = 0;
	for (const auto& vertice : m_vecVertices_Origin)
	{
		XMStoreFloat4(&m_vecVertices_World[iIndex++], XMVector3TransformCoord(XMLoadFloat4( &vertice ), m_pTransformCom->Get_WorldMatrix()));
	}
}

void CFlat_Virtual_Terrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CFlat_Virtual_Terrain::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_BufferDesc();

	/*m_pContext->RSSetState(m_CullMode);
	m_pVIBufferCom->Render();
	m_pContext->RSSetState(m_NoneCullMode);*/
	return S_OK;
}

void CFlat_Virtual_Terrain::Update_Transform_By_Desc(TERRAIN_BASE_DESC Desc)
{

		_float3  vScale = Desc.vScale;
		m_pTransformCom->Scaling(vScale.x, vScale.y, vScale.z);
		_vector vPosition = XMLoadFloat4(&Desc.vPosition);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

		_float fRot = Desc.fRotation;
		_vector vAxis = XMVector3Normalize(XMLoadFloat4(&Desc.vAxis));
		m_pTransformCom->Rotation(vAxis, fRot);
		m_Desc = Desc;

		int iIndex = 0;
		for (const auto& vertice : m_vecVertices_Origin)
		{
			XMStoreFloat4(&m_vecVertices_World[iIndex++], XMVector3TransformCoord(XMLoadFloat4(&vertice), m_pTransformCom->Get_WorldMatrix()));
		}

		return;

}

void CFlat_Virtual_Terrain::Update_Transform_Go_Straight(const _float fTimeDelta)
{

	m_pTransformCom->Go_Straight(fTimeDelta);
	XMStoreFloat4(&m_Desc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	int iIndex = 0;
	for (const auto& vertice : m_vecVertices_Origin)
	{
		XMStoreFloat4(&m_vecVertices_World[iIndex++], XMVector3TransformCoord(XMLoadFloat4(&vertice), m_pTransformCom->Get_WorldMatrix()));
	}

}



HRESULT CFlat_Virtual_Terrain::Ready_Components()
{

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	
	CVIBuffer_Flat::VIBUFFER_FLAT_DESC Desc = {};
	
	Desc.fSizeX = m_Flat_Desc.fSizeX;
	Desc.fSizeZ = m_Flat_Desc.fSizeZ;
	Desc.iNumX_Vertices = m_Flat_Desc.iNumX;
	Desc.iNumZ_Vertices = m_Flat_Desc.iNumZ;
	
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Flat"),
		TEXT("Com_VIBuffer_Collider"), reinterpret_cast<CComponent**>(&m_pVIBufferCom),&Desc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CFlat_Virtual_Terrain::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float fColG = 0.5F; _float fColR = 1.F; _float fColB = 0.5f;
	m_pShaderCom->Bind_RawValue("g_ColorR", &fColR, sizeof(float));
	m_pShaderCom->Bind_RawValue("g_ColorG", &fColG, sizeof(float));
	m_pShaderCom->Bind_RawValue("g_ColorB", &fColB, sizeof(float));
	
	return S_OK;
}

CFlat_Virtual_Terrain* CFlat_Virtual_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFlat_Virtual_Terrain* pInstance = new CFlat_Virtual_Terrain(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create CFlat_Virtual_Terrain Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CFlat_Virtual_Terrain::Clone(void* pArg)
{
	CGameObject* pInstance = new CFlat_Virtual_Terrain(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CFlat_Virtual_Terrain Failed");
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

void CFlat_Virtual_Terrain::Free()
{
	__super::Free();
	Safe_Release(m_CullMode);
	Safe_Release(m_NoneCullMode);
	Safe_Release(m_pVIBufferCom);
}
