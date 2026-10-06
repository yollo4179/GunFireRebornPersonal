#include"stdafx.h"
#include"Trail_Monster03.h"
#include"GameInstance.h"
CTrail_Monster03::CTrail_Monster03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CTrail_Monster03::CTrail_Monster03(const CTrail_Monster03& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CTrail_Monster03::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrail_Monster03::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	if (FAILED(Ready_MyTransform()))
		return E_FAIL;
	return S_OK;
}

void CTrail_Monster03::Priority_Update(_float fTimeDelta)
{

	if (m_fAccTime < m_fTime)
		m_fAccTime += 10*fTimeDelta;
	else
		m_fAccTime = 0;


	if (m_fAccAddVeticesTime < m_fAddVerticesTime)
		m_fAccAddVeticesTime += fTimeDelta;


	m_vMyPosInLocal.x = m_fRange * cos(/*XMConvertToRadians*/(m_fAccTime));
	m_vMyPosInLocal.y = m_fRange * sin(/*XMConvertToRadians*/(m_fAccTime));


	if (m_fAccAddVeticesTime >= m_fAddVerticesTime)
	{
		_matrix matHigh = m_pLocalTransform->Get_WorldMatrix();
		_matrix matLow = m_pLocalTransform->Get_WorldMatrix();


		{
			_vector vMyPos;
			vMyPos = XMLoadFloat3(&m_vMyPosInLocal);
			vMyPos = XMVectorSetW(vMyPos, 1.f);

			_vector vOrigin = { 0.f,0.f,0.f,1.f };

			_vector vMyLook = XMVector3Normalize(vOrigin - vMyPos);
			matHigh.r[3] = (matHigh.r[3]+ 0.3f * vMyLook +XMVectorSet(0.f, 0.f, -0.3f, 0.f));
		}
		
		ResetMyMatrixByPos();
		
		Set_BufferMatrix(matHigh, matLow);
		m_fAccAddVeticesTime = 0.f;
	}


	



}

void CTrail_Monster03::Update(_float fTimeDelta)
{
}

void CTrail_Monster03::Late_Update(_float fTimeDelta)
{


	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
}

HRESULT CTrail_Monster03::Render()
{
	if (true == m_bGlow)
	{
		if (FAILED(Bind_ShaderResources_Glow()))
			return E_FAIL;

		m_pShaderCom->Begin(3);

		m_pVIBufferCom->Bind_BufferDesc();

		m_pVIBufferCom->Render();
	}
	else {
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(2);

		m_pVIBufferCom->Bind_BufferDesc();

		m_pVIBufferCom->Render();
	}
	return S_OK;
}

void CTrail_Monster03::Set_BufferMatrix(_fmatrix matHigh, _fmatrix matLow)
{


	m_pVIBufferCom->Add_Points_CatMullRom(matHigh, matLow);
}

void CTrail_Monster03::Set_Matrix(_fmatrix matWorld,_float3 Size)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT,Size.x* matWorld.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, Size.y * matWorld.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, Size.z * 0.8f*matWorld.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matWorld.r[3]);
}

void CTrail_Monster03::ResetMyMatrixByPos()
{
	
	//x,y
	_vector vMyPos;
	vMyPos = XMLoadFloat3(&m_vMyPosInLocal);
	vMyPos = XMVectorSetW(vMyPos, 1.f);

	_vector vOrigin = { 0.f,0.f,0.f,1.f };

	_vector vMyLook = XMVector3Normalize(vOrigin - vMyPos);

	_vector vMyRight = XMVector3Cross(XMVectorSet(0.f, 0.f, -1.f, 0.f), vMyLook);

	_vector vMyUp = XMVector3Cross(vMyLook, vMyRight);

	_matrix vMyRotation = XMMatrixRotationAxis(vMyRight, XMConvertToRadians(120.f));

	m_pLocalTransform->Set_State(CTransform::STATE_RIGHT, vMyRight);
	m_pLocalTransform->Set_State(CTransform::STATE_UP, vMyUp);
	m_pLocalTransform->Set_State(CTransform::STATE_LOOK, vMyLook);
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, vMyPos);


}

void CTrail_Monster03::Reset_VerticesCnt()
{
	m_pVIBufferCom->Reset_VerticesCnt();
}

void CTrail_Monster03::Resize_Vertices()
{
	m_pVIBufferCom->Resize_Vertices();
}

HRESULT CTrail_Monster03::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_Glow"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	CVIBuffer_Trail::TRAIL_DESC TrailDesc = {};
	TrailDesc.fTrailLength = 1.f;
	TrailDesc.iNumMaxVertices = 150;
	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &TrailDesc)))
		return E_FAIL;

	m_pVIBufferCom->Resize_Vertices();

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster03_Trail_Circle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CTrail_Monster03::Ready_MyTransform()
{
	m_pLocalTransform  = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC Desc = {};
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.fSpeedPerSec = 10.f;
	if (FAILED(m_pLocalTransform->Initialize(&Desc)))
		return E_FAIL;


	m_pLocalTransform->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	return S_OK;
}

HRESULT CTrail_Monster03::Bind_ShaderResources()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrail_Monster03::Bind_ShaderResources_Glow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture", 0)))
		return E_FAIL;

	_float3 vThreshold = { 0.f,0.f,0.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vThreshord", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float fIntensity = 0.1f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", &fIntensity, sizeof(_float))))
		return E_FAIL;
	return S_OK;
}

CTrail_Monster03* CTrail_Monster03::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrail_Monster03* pInstance = new CTrail_Monster03(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrail_Monster03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrail_Monster03::Clone(void* pArg)
{
	CTrail_Monster03* pInstance = new CTrail_Monster03(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTrail_Monster03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail_Monster03::Free()
{
	__super::Free();
	Safe_Release(m_pLocalTransform);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
