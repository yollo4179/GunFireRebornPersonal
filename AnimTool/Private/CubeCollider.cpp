#include "stdafx.h"
#include "..\Public\CubeCollider.h"
#include "GameInstance.h"
#include <Effects.h>

CCubeCollider::CCubeCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CCubeCollider::CCubeCollider(const CCubeCollider& Prototype)
	: CGameObject( Prototype )
{
}

HRESULT CCubeCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCubeCollider::Initialize(void* pArg)
{
	CUBE_DESC* pDesc = static_cast<CUBE_DESC*>(pArg);

	m_Desc.vPos = pDesc->vPos;
	m_Desc.vRotAxis = pDesc->vRotAxis;
	m_Desc.vScale = pDesc->vScale;
	m_Desc.index = pDesc->index;
	m_Desc.fAngle = pDesc->fAngle;
	m_Desc.vPosY_From_Base = m_Desc.vScale.y*0.5;

	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (nullptr != m_pTransformCom)
	{
		m_pTransformCom->Scaling(m_Desc.vScale.x, m_Desc.vScale.y, m_Desc.vScale.z);
		//m_pTransformCom->Rotation(XMLoadFloat4(&m_vRotAxis),XMConvertToRadians(60));
		_float4 FinalPos = m_Desc.vPos;
		FinalPos.y += m_Desc.vPosY_From_Base;
		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,XMLoadFloat4(&FinalPos));

		m_Box.Center = { 0.f,0.f,0.f };
		m_Box.Extents = { 0.5f,0.5f,0.5f };
		m_Box.Transform(m_Box, m_pTransformCom->Get_WorldMatrix());

	}
	if (FAILED(Ready_Components()))
		return E_FAIL;


	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_NoneCullMode));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_CullMode));
	return S_OK;
}

void CCubeCollider::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CCubeCollider::Update(_float fTimeDelta)
{

}

void CCubeCollider::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CCubeCollider::Render()
{
	if (FAILED(Bind_ShaderResources()))
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

_float3 CCubeCollider::Get_Transform_Scale()
{

	return _float3();
}

_float4 CCubeCollider::Get_Transform_Move_Position()
{
	_float4 ret;
	XMStoreFloat4(&ret,m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	return ret;
}

_matrix CCubeCollider::Get_WorldMatrix_Inverse()
{
	if (nullptr == m_pTransformCom)
		return _matrix();

	return m_pTransformCom->Get_WorldMatrix_Inverse();
}

_matrix CCubeCollider::Get_WorldMatrix()
{

	if (nullptr == m_pTransformCom)
		return _matrix();

	return m_pTransformCom->Get_WorldMatrix();
}



HRESULT CCubeCollider::Ready_Components()
{
	///* Com_Shader */
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeTex"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;

	/*Com_Shader_For_Collider*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeCol"),
		TEXT("Com_Shader_For_Collider"), reinterpret_cast<CComponent**>(&m_pShader_For_ColliderCom))))
		return E_FAIL;
	/*Com_VIBuffer_Collider*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Collider"),
		TEXT("Com_VIBuffer_Collider"), reinterpret_cast<CComponent**>(&m_pVIBuffer_ColliderCom))))
		return E_FAIL;

	///* Com_VIBuffer */
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
	//	TEXT("Com_VIBuffer_Cube"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	//	return E_FAIL;

	///* Com_Texture */
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkyBox"),
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CCubeCollider::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader_For_ColliderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShader_For_ColliderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader_For_ColliderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	/*if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;*/

	return S_OK;
}

CCubeCollider* CCubeCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCubeCollider* pInstance = new CCubeCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCubeCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCubeCollider::Clone(void* pArg)
{
	CCubeCollider* pInstance = new CCubeCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCubeCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCubeCollider::Free()
{
	__super::Free();
	Safe_Release(m_CullMode);
	Safe_Release(m_NoneCullMode);
	Safe_Release(m_pShader_For_ColliderCom);
	Safe_Release(m_pVIBuffer_ColliderCom);

	/*Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);*/
}
