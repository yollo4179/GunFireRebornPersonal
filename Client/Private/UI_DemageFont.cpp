#include"stdafx.h"
#include "UI_DemageFont.h"
#include"GameInstance.h"
CUI_DemageFont::CUI_DemageFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject(pDevice, pContext)
{
}

CUI_DemageFont::CUI_DemageFont(const CUI_DemageFont& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CUI_DemageFont::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_DemageFont::Initialize(void* pArg)
{
	UI_DEMAGE_FONT_DESC* pDesc = static_cast<UI_DEMAGE_FONT_DESC*>(pArg);
	m_vSize = {pDesc->fSizeX, pDesc->fSizeY};

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;
	return S_OK;
}

void CUI_DemageFont::Priority_Update(_float fTimeDelta)
{

	m_fAccLifeTime += fTimeDelta;
	if (m_fAccLifeTime >= m_fLifeTime)
	{
		m_bActive = false;
	}

}

void CUI_DemageFont::Update(_float fTimeDelta)
{

}

void CUI_DemageFont::Late_Update(_float fTimeDelta)
{
	_matrix matCamera =(m_pGameInstance->Get_TransformInversedMatrix(CPipeLine::D3DTS_VIEW));


	//원래 카메라 룩이 맞음
	_vector vPlayerLook = matCamera.r[2];
	_vector vPlayerPos = matCamera.r[3];
	_vector vMonsterPos = m_pTransformCom->Get_WorldMatrix().r[3];

	_vector vTargetDir = vMonsterPos - vPlayerPos;
	_float fDot = XMVectorGetX(XMVector3Dot(vPlayerLook, vTargetDir));
	_bool isCounterDir = (fDot > 0) ? true : false;

	if (isCounterDir&&m_bActive)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_DemageFont::Render()
{
	if (!m_bActive)
		return S_OK;

	string  strNumber = to_string(m_iDemage);
	_uint iNumberCount = 0;
	{

		_bool bChangeColor = true;
		m_pShaderCom->Bind_RawValue("g_bColorChange", &bChangeColor, sizeof(_bool));
		m_pShaderCom->Bind_RawValue("g_ColorR", &m_vColor.x, sizeof(_float));
		m_pShaderCom->Bind_RawValue("g_ColorG", &m_vColor.y, sizeof(_float));
		m_pShaderCom->Bind_RawValue("g_ColorB", &m_vColor.z, sizeof(_float));
	}
	for (auto& Number_OfDemage : strNumber)
	{
		
		NumberToRender(Number_OfDemage - '0',iNumberCount);
		++iNumberCount;
	}

	{
		_bool bChangeColor = false;
		int iClearColor = 1.f;
		m_pShaderCom->Bind_RawValue("g_bColorChange", &bChangeColor, sizeof(_bool));
		m_pShaderCom->Bind_RawValue("g_ColorR", &iClearColor, sizeof(_float));
		m_pShaderCom->Bind_RawValue("g_ColorG", &iClearColor, sizeof(_float));
		m_pShaderCom->Bind_RawValue("g_ColorB", &iClearColor, sizeof(_float));
	}

	return S_OK;
}

HRESULT CUI_DemageFont::NumberToRender(_uint iSymbol, _int iNumberCount)
{

	if (FAILED(Bind_ShaderResources(iSymbol, iNumberCount)))
		return E_FAIL;

	m_pShaderCom->Begin(2); //postex 2번째 패스 데미지 폰트

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_DemageFont::Set_Demage(_uint iNumber, _bool isCritical)
{
	if (isCritical)
	{
		m_vColor = { 1.f,1.f,0.f };
	}
	else
	{
		m_vColor = { 1.f,1.f,1.f };
	}

	m_iDemage = iNumber;

}

void CUI_DemageFont::Set_WorldMatrix(_fmatrix DemageMatrix)
{
	//m_pMonsterHpFloat4x4->_41 =500.f;
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, DemageMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP,DemageMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, DemageMatrix.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, DemageMatrix.r[3]);
}

HRESULT CUI_DemageFont::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Numbers"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_DemageFont::Bind_ShaderResources(_uint iSymbol, _uint iNumberIndex)
{
	/*월드에 있는 ui에 던져줄 요소들 -> 월드 뷰 투영 -> 직교 뷰 투영 -> 유아이 크기, 뷰포트 크기*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_UIWorldMatrix_In_World")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_UIViewMatrix_In_World", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_UIProjMatrix_In_World", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	/* 직교튀영을 위한 뷰ㅡ, 투영행르을 만들었다. */
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", iSymbol)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Size_For_WorldUI", &m_vSize, sizeof(_float2))))
		return E_FAIL;

	_float fOffsetIndex =(_float)iNumberIndex;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_OffsetToPlace", &fOffsetIndex, sizeof(_float))))
		return E_FAIL;

	_float2 vWinSize = { ViewportDesc.Width ,ViewportDesc.Height };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WinSize", &vWinSize, sizeof(_float2))))
		return E_FAIL;

}

CUI_DemageFont* CUI_DemageFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_DemageFont* pInstance = new  CUI_DemageFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_DemageFont");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_DemageFont::Clone(void* pArg)
{
	CUI_DemageFont* pInstance = new  CUI_DemageFont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_DemageFont");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_DemageFont::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
