#include"stdafx.h"
#include"UI_Talk.h"
#include"GameInstance.h"
CUI_Talk::CUI_Talk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{pDevice, pContext}
{
}

CUI_Talk::CUI_Talk(const CUI_Talk& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CUI_Talk::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Talk::Initialize(void* pArg)
{
	UI_TALK_DESC* pDesc = static_cast<UI_TALK_DESC*>(pArg);
	m_vFontOffset = pDesc->vFontOffset;
	m_vSize = { pDesc->fSizeX, pDesc->fSizeY };

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;
	return S_OK;
}

void CUI_Talk::Priority_Update(_float fTimeDelta)
{
	

	if (false == m_bActive)
		return;
}

void CUI_Talk::Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
}

void CUI_Talk::Late_Update(_float fTimeDelta)
{
	
	if (false == m_bActive)
		return;
	if(m_pGameInstance->isIn_Frustum_InWorldSpace(m_pTransformCom->Get_WorldMatrix().r[3],2.f))
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Talk::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2); //postex 2번째 패스 데미지 폰트

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();


	m_pGameInstance->Render_Font(TEXT("Font32"), TEXT("Talk"), { m_vFontPos.x +m_vFontOffset.x ,m_vFontPos.y +m_vFontOffset.y }, XMVectorSet(1.f, 1.f, 0.f, 1.f));


	return S_OK;
}

void CUI_Talk::Set_WorldMatrix(_fmatrix TalkMatrix)
{
	//m_pMonsterHpFloat4x4->_41 =500.f;
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, TalkMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, TalkMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, TalkMatrix.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, TalkMatrix.r[3]);
}

void CUI_Talk::Find_XY_ForText()
{
	/*WorldPos*/
	_matrix vPosMatrix = m_pTransformCom->Get_WorldMatrix();

	_matrix  vPosWV_InWorld = XMMatrixMultiply(vPosMatrix, XMLoadFloat4x4(&m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))); 
	_matrix  vPosWVP_InWorld = XMMatrixMultiply(vPosWV_InWorld, XMLoadFloat4x4(&m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)));
	
	_float2 NDCPos = { XMVectorGetX(vPosWVP_InWorld.r[3])/ XMVectorGetW(vPosWVP_InWorld.r[3]) ,XMVectorGetY(vPosWVP_InWorld.r[3]) /XMVectorGetW(vPosWVP_InWorld.r[3]) };
	_float2 ScreenPos = { (NDCPos.x + 1.f) * g_iWinSizeX * 0.5f , -(NDCPos.y - 1.f) * g_iWinSizeY * 0.5f };

	_vector vWorldPosForOrtho = { ScreenPos.x - g_iWinSizeX * 0.5f, -ScreenPos.y + g_iWinSizeY * 0.5f,0.f,1.f };

	_matrix WorldMatrixForOrtho = {
		{m_vSize.x,0.f,0.f,0.f},
		{0.f, m_vSize.y,0.f,0.f},
		{0.f,0.f,1.f,0.f},
		vWorldPosForOrtho
	};

	_matrix  vP5osWV_Ortho = XMMatrixMultiply(WorldMatrixForOrtho, XMLoadFloat4x4(&m_ViewMatrix));
	_matrix  vPosWVP_Ortho = XMMatrixMultiply(vP5osWV_Ortho, XMLoadFloat4x4(&m_ProjMatrix));
	_float  ScreenX = { (XMVectorGetX(vPosWVP_Ortho.r[3]) + 1.f) * 0.5f * g_iWinSizeX };
	_float  ScreenY = { ( 1.f - XMVectorGetY(vPosWVP_Ortho.r[3]) ) * 0.5f * g_iWinSizeY };

	m_vFontPos = {ScreenX, ScreenY};

	return;

}

HRESULT CUI_Talk::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Talk"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Talk::Bind_ShaderResources()
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
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture",0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Size_For_WorldUI", &m_vSize, sizeof(_float2))))
		return E_FAIL;

	_float fOffsetIndex = 0;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_OffsetToPlace", &fOffsetIndex, sizeof(_float))))
		return E_FAIL;

	_float2 vWinSize = { ViewportDesc.Width ,ViewportDesc.Height };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WinSize", &vWinSize, sizeof(_float2))))
		return E_FAIL;
}

CUI_Talk* CUI_Talk::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Talk* pInstance = new  CUI_Talk(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Talk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Talk::Clone(void* pArg)
{
	CUI_Talk* pInstance = new  CUI_Talk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Talk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Talk::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
