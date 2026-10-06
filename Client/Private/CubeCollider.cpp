#include "stdafx.h"
#include "..\Public\CubeCollider.h"
#include "GameInstance.h"


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

	m_vPos = pDesc->vPos;
	//m_vRotAxis = pDesc->vRotAxis;
	m_vScale = pDesc->vScale;

	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (nullptr != m_pTransformCom)
	{
		m_pTransformCom->Scaling(m_vScale.x, m_vScale.y, m_vScale.z);
		//m_pTransformCom->Rotation(XMLoadFloat4(&m_vRotAxis),XMConvertToRadians(60));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_vPos), 1.f));
	}


	if (FAILED(Ready_Components()))
		return E_FAIL;



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

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}
/*for Maptool Scaling*/
void CCubeCollider::Update_Transform_Scale(_float3 vDeltaScale)
{
	m_pTransformCom->Scaling(vDeltaScale.x, vDeltaScale.y, vDeltaScale.z);
}
/*for Maptool Rotation*/
void CCubeCollider::Update_Transform_Rotaion(_float4 vAxis, _float fRadian)
{
	m_pTransformCom->Rotation(XMLoadFloat4(&vAxis),fRadian);
}
/*for Maptool Change Position*/
void CCubeCollider::Update_Transform__Move_Position(_float4 _vDeltaMove)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&_vDeltaMove));

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

HRESULT CCubeCollider::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCube"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer_Cube"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkyBox"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeCollider::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

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


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
