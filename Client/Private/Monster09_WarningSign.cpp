#include"stdafx.h"
#include"Monster09_WarningSign.h"
#include"GameInstance.h"

CMonster09_WarningSign::CMonster09_WarningSign(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAlphaObject{ pDevice, pContext }
{
}

CMonster09_WarningSign::CMonster09_WarningSign(const CMonster09_WarningSign& Prototype)
	: CAlphaObject(Prototype)
{
}

HRESULT CMonster09_WarningSign::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster09_WarningSign::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;



	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pGameInstance->Compute_Random(0.f, 20.f), 2.f, m_pGameInstance->Compute_Random(0.f, 20.f), 1.f));

	return S_OK;
}


void CMonster09_WarningSign::Priority_Update(_float fTimeDelta)
{
	Compute_ViewZ();
}

void CMonster09_WarningSign::Update(_float fTimeDelta)
{
	
}

void CMonster09_WarningSign::Late_Update(_float fTimeDelta)
{
	
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
}

HRESULT CMonster09_WarningSign::Render()
{
	if (true == m_bGlow)
	{
		if (FAILED(Bind_ShaderResources_Glow()))
			return E_FAIL;

		m_pShaderCom->Begin(4);

		m_pVIBufferCom->Bind_BufferDesc();

		m_pVIBufferCom->Render();
	}
	else {
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(5);

		m_pVIBufferCom->Bind_BufferDesc();

		m_pVIBufferCom->Render();
	}
	return S_OK;
}


void CMonster09_WarningSign::Set_Vector(_fvector vPos)
{
	_matrix PreMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f),XMConvertToRadians(90.f));
	

	_vector vRight =XMVectorSet(8.f, 0.f, 0.f, 0.f);
	_vector vUp =XMVectorSet(0.f,8.f,0.f,0.f);
	_vector vLook =XMVectorSet(0.f,0.f,1.f,0.f);
	
	_matrix MyMatrix = XMMatrixIdentity();
	MyMatrix.r[0] = vRight;
	MyMatrix.r[1] = vUp;
	MyMatrix.r[2] = vLook;

	MyMatrix =   MyMatrix* PreMatrix;

	MyMatrix.r[3] = vPos;

	
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, MyMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, MyMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, MyMatrix.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyMatrix.r[3]);
}

HRESULT CMonster09_WarningSign::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Effect_Monster09_ShotWarningCircle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster09_WarningSign::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;



	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture" )))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster09_WarningSign::Bind_ShaderResources_Glow()
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

CMonster09_WarningSign* CMonster09_WarningSign::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster09_WarningSign* pInstance = new CMonster09_WarningSign(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster09_WarningSign");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster09_WarningSign::Clone(void* pArg)
{
	CMonster09_WarningSign* pInstance = new CMonster09_WarningSign(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster09_WarningSign");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster09_WarningSign::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
