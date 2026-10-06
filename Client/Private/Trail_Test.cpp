#include"stdafx.h"
#include"Trail_Test.h"
#include"GameInstance.h"
CTrail_Test::CTrail_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice,pContext}
{
}

CTrail_Test::CTrail_Test(const CTrail_Test& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CTrail_Test::Initialize_Prototype()
{

	
	return S_OK;
}

HRESULT CTrail_Test::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Scaling(1.f, 1.f, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.F, 3.F, 0.F, 1.F));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	return S_OK;
}

void CTrail_Test::Priority_Update(_float fTimeDelta)
{


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fCenter) + XMVectorSet(3 * cos(-m_fAccAngle), 0.f, 3 * sin(-m_fAccAngle),0.f));
	
	m_fAccBufferTiming += fTimeDelta;
	if (m_fAccBufferTiming >= 0.02f)
	{
		/*로컬 설정*/
		_matrix myMatrix = XMMatrixIdentity();
		//_vector vNowPosition = XMLoadFloat4(&m_fCenter) + XMVectorSet(5.f * cos(m_fAccAngle), 0.f/* 0.2f*(sin(m_fAccAngle * 50.f)+cos(m_fAccAngle*50.f))*/, 5.f* sin(m_fAccAngle), 0.f);
		
		_vector vNowPosition = XMLoadFloat4(&m_fCenter) + XMVectorSet(m_fSize ,  0.5f*(sin(m_fAccAngle)+cos(m_fAccAngle)),  m_fSize, 0.f);

		//_vector vNowPosition = XMLoadFloat4(&m_fCenter) + XMVectorSet(0.f, 0.f/* 0.2f*(sin(m_fAccAngle * 50.f)+cos(m_fAccAngle*50.f))*/, 5.f * m_fSize,0.f);
		//myMatrix.r[3] =  {-0.5f,0.f,1.f,1.f};
		myMatrix.r[3] = vNowPosition;
		//m_pVIBufferCom->Add_Points(myMatrix, myMatrix);
	
		if (m_fSize > 3.f)
		{
			m_pVIBufferCom->Reset_VerticesCnt();;
			m_fSize = 0.f;
		}
		else 
		m_pVIBufferCom->Add_Points_CatMullRom(myMatrix, myMatrix);

		
	}
	 if(m_fAccBufferTiming > 0.02f)
		m_fAccBufferTiming = 0.f;

	m_fAccBufferSubTiming += fTimeDelta*10.f;

	m_fAccAngle += fTimeDelta;
	m_fSize += fTimeDelta;

	
	
	


}

void CTrail_Test::Update(_float fTimeDelta)
{
}

void CTrail_Test::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONLIGHT, this);
}

HRESULT CTrail_Test::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(5);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTrail_Test::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	CVIBuffer_Trail::TRAIL_DESC TrailDesc = {};
	TrailDesc.fTrailLength = 10;
	TrailDesc.iNumMaxVertices = 244;
	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &TrailDesc)))
		return E_FAIL;

	m_pVIBufferCom->Resize_Vertices();

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail_Test"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CTrail_Test::Bind_ShaderResources()
{

	/*if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;*/

	_float4x4 WorldFloat4x4;
	XMStoreFloat4x4(&WorldFloat4x4, XMMatrixIdentity());
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &WorldFloat4x4)))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CTrail_Test* CTrail_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrail_Test* pInstance = new CTrail_Test(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrail_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrail_Test::Clone(void* pArg)
{
	CTrail_Test* pInstance = new CTrail_Test(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTrail_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail_Test::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
