#include"stdafx.h"
#include"Effect_Monster09_Moving.h"
#include"GameInstance.h"
CEffect_Monster09_Moving::CEffect_Monster09_Moving(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAlphaObject{ pDevice, pContext }
{
}

CEffect_Monster09_Moving::CEffect_Monster09_Moving(const CEffect_Monster09_Moving& Prototype)
	: CAlphaObject(Prototype)
{
}

HRESULT CEffect_Monster09_Moving::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Monster09_Moving::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pGameInstance->Compute_Random(0.f, 20.f), 2.f, m_pGameInstance->Compute_Random(0.f, 20.f), 1.f));

	return S_OK;
}

void CEffect_Monster09_Moving::Priority_Update(_float fTimeDelta)
{

	//m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	
}

void CEffect_Monster09_Moving::Update(_float fTimeDelta)
{
	m_fFrame += 5.f * fTimeDelta;

	if (m_fFrame >= 5.f)
		m_fFrame = 0.f;

	Compute_ViewZ();
}

void CEffect_Monster09_Moving::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_Monster09_Moving::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(4);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}


void CEffect_Monster09_Moving::Set_Matrix(_fmatrix PosMatrix,_bool bTurn)
{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, 20.F*PosMatrix.r[0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, 10.F * PosMatrix.r[1]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, 1.F * PosMatrix.r[2]);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, PosMatrix.r[3]+XMVectorSet(0.f,-1.f,0.f,0.f));
		if(true ==bTurn)
			m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(180.f));
		
}


HRESULT CEffect_Monster09_Moving::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Move"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Monster09_Moving::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", static_cast<_uint>(m_fFrame))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShaderCom, "g_DepthTexture", TEXT("Target_Depth"))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CEffect_Monster09_Moving* CEffect_Monster09_Moving::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Monster09_Moving* pInstance = new CEffect_Monster09_Moving(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Monster09_Moving");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Monster09_Moving::Clone(void* pArg)
{
	CEffect_Monster09_Moving* pInstance = new CEffect_Monster09_Moving(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Monster09_Moving");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Monster09_Moving::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
