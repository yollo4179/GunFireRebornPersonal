#include"stdafx.h"
#include"Effect_Monster09_ShotDown.h"
#include "GameInstance.h"


CEffect_Monster09_ShotDown::CEffect_Monster09_ShotDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAlphaObject{ pDevice, pContext }
{
}

CEffect_Monster09_ShotDown::CEffect_Monster09_ShotDown(const CEffect_Monster09_ShotDown& Prototype)
	: CAlphaObject(Prototype)
{
}

HRESULT CEffect_Monster09_ShotDown::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Monster09_ShotDown::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pGameInstance->Compute_Random(0.f, 20.f), 2.f, m_pGameInstance->Compute_Random(0.f, 20.f), 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));


	return S_OK;
}

void CEffect_Monster09_ShotDown::Priority_Update(_float fTimeDelta)
{
	
	int a = 10;
}

void CEffect_Monster09_ShotDown::Update(_float fTimeDelta)
{


	Compute_ViewZ();
}

void CEffect_Monster09_ShotDown::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_Monster09_ShotDown::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(4);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CEffect_Monster09_ShotDown::Set_Matrix(_fmatrix PosMatrix)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVectorSet(20.F, 0.F, 0.F, 0.F));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVectorSet(0.F, 20.F, 0.F, 0.F));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSet(0.F, 0.F, 1.F, 0.F));



	m_pTransformCom->Set_State(CTransform::STATE_POSITION, PosMatrix.r[3]);
}

HRESULT CEffect_Monster09_ShotDown::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Effect_Monster09_ShotDown"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Monster09_ShotDown::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture",0)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShaderCom, "g_DepthTexture", TEXT("Target_Depth"))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CEffect_Monster09_ShotDown* CEffect_Monster09_ShotDown::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Monster09_ShotDown* pInstance = new CEffect_Monster09_ShotDown(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Monster09_ShotDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Monster09_ShotDown::Clone(void* pArg)
{
	CEffect_Monster09_ShotDown* pInstance = new CEffect_Monster09_ShotDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Monster09_ShotDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Monster09_ShotDown::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}