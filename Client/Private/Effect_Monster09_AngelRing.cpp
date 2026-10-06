#include"stdafx.h"
#include "Effect_Monster09_AngelRing.h"
#include"GameInstance.h"
Effect_Monster09_AngelRing::Effect_Monster09_AngelRing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :
    CAlphaObject(pDevice, pContext)
{
}

Effect_Monster09_AngelRing::Effect_Monster09_AngelRing(const Effect_Monster09_AngelRing& Prototype)
    :
    CAlphaObject(Prototype)
{
}

HRESULT Effect_Monster09_AngelRing::Initialize_Prototype()
{
    return S_OK;
}

HRESULT Effect_Monster09_AngelRing::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;



    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pGameInstance->Compute_Random(0.f, 20.f), 2.f, m_pGameInstance->Compute_Random(0.f, 20.f), 1.f));

    return S_OK;
}

void Effect_Monster09_AngelRing::Priority_Update(_float fTimeDelta)
{
    Compute_ViewZ();
}

void Effect_Monster09_AngelRing::Update(_float fTimeDelta)
{
}

void Effect_Monster09_AngelRing::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
}


HRESULT Effect_Monster09_AngelRing::Render()
{
    if (true == m_bGlow)
    {
        if (FAILED(Bind_ShaderResources_Glow()))
            return E_FAIL;

        m_pShaderCom->Begin(2);

        m_pVIBufferCom->Bind_BufferDesc();

        m_pVIBufferCom->Render();
    }
    else {
        if (FAILED(Bind_ShaderResources()))
            return E_FAIL;

        m_pShaderCom->Begin(3);

        m_pVIBufferCom->Bind_BufferDesc();

        m_pVIBufferCom->Render();
    }
    return S_OK;
}

void Effect_Monster09_AngelRing::Set_Matrix(_matrix mat)
{
	mat.r[3] -= XMVector3Normalize(mat.r[2]) * 3.f;

	_matrix PreMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	_matrix MyMatrix;
	
	MyMatrix = /*PreMatrix**/mat;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, MyMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, MyMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, MyMatrix.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyMatrix.r[3]);

}


HRESULT Effect_Monster09_AngelRing::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_Glow"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Effect_Monster09_A_Circle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT Effect_Monster09_AngelRing::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;



	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

HRESULT Effect_Monster09_AngelRing::Bind_ShaderResources_Glow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;



	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture", 0)))
		return E_FAIL;

	_float3 vThreshold = { 0.f,0.f,0.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vThreshord", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float fIntensity = 0.3f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", &fIntensity, sizeof(_float))))
		return E_FAIL;
	return S_OK;
}

Effect_Monster09_AngelRing* Effect_Monster09_AngelRing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Effect_Monster09_AngelRing* pInstance = new Effect_Monster09_AngelRing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Effect_Monster09_AngelRing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* Effect_Monster09_AngelRing::Clone(void* pArg)
{
	Effect_Monster09_AngelRing* pInstance = new Effect_Monster09_AngelRing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Effect_Monster09_AngelRing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Effect_Monster09_AngelRing::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	
};

