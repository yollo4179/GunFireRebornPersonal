#include"stdafx.h"
#include"UI_MonsterHP_Red.h"

#include"GameInstance.h"

CUI_MonsterHP_Red::CUI_MonsterHP_Red(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice,pContext }
{
}

CUI_MonsterHP_Red::CUI_MonsterHP_Red(const CUI_MonsterHP_Red& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CUI_MonsterHP_Red::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_MonsterHP_Red::Initialize(void* pArg)
{

	UI_MONSTER_HP_RED_DESC* pDesc = static_cast<UI_MONSTER_HP_RED_DESC*>(pArg);

	/*For World UI*/
	m_pMonsterHpFloat4x4 = pDesc->pMonsterHpFloat4x4;
	m_pViewZDistance = pDesc->pViewZDistance;
	m_vSize = { pDesc->fSizeX , pDesc->fSizeY };
	m_pMonsterHPInfo = pDesc->pMonsterHPInfo;
	m_fPrevRatio = (*m_pMonsterHPInfo).x / (*m_pMonsterHPInfo).y;
	/* 백그라우늗의 ㅁ크기아ㅘ 위치를 잡아주기위한 용오 .*/
	if (nullptr == pDesc) {
		pDesc->fX = 100;//g_iWinSizeX >> 1;
		pDesc->fY = 0;//g_iWinSizeY >> 1;
		pDesc->fSizeX = 200;//g_iWinSizeX;
		pDesc->fSizeY = 100;//g_iWinSizeY;
	}

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;




	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(10.f));
	if (FAILED(Ready_Components()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;

	return S_OK;



	return S_OK;
}

void CUI_MonsterHP_Red::Priority_Update(_float fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	_float fBarRatio = (*m_pMonsterHPInfo).z / (*m_pMonsterHPInfo).w;
	if (m_bHitSign)
	{
		m_fPrevRatio -= m_fTimeDelta*1.f;
		if (m_fPrevRatio <= fBarRatio)
		{
			m_fPrevRatio = fBarRatio;
			m_bHitSign = false;
		}
	}
	if (m_fPrevRatio != fBarRatio)
	{
		m_bHitSign = true;
	}
}

void CUI_MonsterHP_Red::Update(_float fTimeDelta)
{
	Update_Set_Transform();
}

void CUI_MonsterHP_Red::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_MonsterHP_Red::Render()
{
	if (nullptr == m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Monster"), 0))
		return S_OK;

	{
		/*For_GrayBG*/
		if (FAILED(Bind_ShaderResources_ForBG()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Bind_BufferDesc();

		m_pVIBufferCom->Render();

	}


	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
}

void CUI_MonsterHP_Red::Update_Set_Transform()
{


	//m_pMonsterHpFloat4x4->_41 =500.f;
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4((_float4*)m_pMonsterHpFloat4x4->m[0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMLoadFloat4((_float4*)m_pMonsterHpFloat4x4->m[1]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMLoadFloat4((_float4*)m_pMonsterHpFloat4x4->m[2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4((_float4*)m_pMonsterHpFloat4x4->m[3]));
}

HRESULT CUI_MonsterHP_Red::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Red"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Gray"),
		TEXT("Com_Texture_Gray"), reinterpret_cast<CComponent**>(&m_pTextureCom_BG))))
		return E_FAIL;

	return S_OK;





	return S_OK;
}

HRESULT CUI_MonsterHP_Red::Bind_ShaderResources()
{
	if (nullptr == m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Monster"), 0))
		return S_OK;

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

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;


	_float fBarRatio = (*m_pMonsterHPInfo).z / (*m_pMonsterHPInfo).w;

	/*if (m_fPrevRatio != fBarRatio)
	{
		m_bHitSign = true;
	}*/
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_BarRatio", &fBarRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Size_For_WorldUI", &m_vSize, sizeof(_float2))))
		return E_FAIL;

	_float fHeight = *m_pViewZDistance;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewZDistance", &fHeight, sizeof(_float))))
		return E_FAIL;

	_float fAdjustingHeight = -12.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_AdjustingHeight", &fAdjustingHeight, sizeof(_float))))
		return E_FAIL;


	_float2 vWinSize = { ViewportDesc.Width ,ViewportDesc.Height };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WinSize", &vWinSize, sizeof(_float2))))
		return E_FAIL;



	return S_OK;

}

HRESULT CUI_MonsterHP_Red::Bind_ShaderResources_ForBG()
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

	if (FAILED(m_pTextureCom_BG->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;


	/*_float fBarRatio = (*m_pMonsterHPInfo).z / (*m_pMonsterHPInfo).w;

	if (m_bHitSign)
	{
		m_fPrevRatio -= m_fTimeDelta*10.f;
		if (m_fPrevRatio <= fBarRatio)
		{
			m_fPrevRatio = fBarRatio;
			m_bHitSign = false;
		}
	}*/

	if (FAILED(m_pShaderCom->Bind_RawValue("g_BarRatio", &m_fPrevRatio, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_Size_For_WorldUI", &m_vSize, sizeof(_float2))))
		return E_FAIL;

	_float fHeight = *m_pViewZDistance;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewZDistance", &fHeight, sizeof(_float))))
		return E_FAIL;
	_float fAdjustingHeight = -12.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_AdjustingHeight", &fAdjustingHeight, sizeof(_float))))
		return E_FAIL;



	_float2 vWinSize = { ViewportDesc.Width ,ViewportDesc.Height };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WinSize", &vWinSize, sizeof(_float2))))
		return E_FAIL;



	return S_OK;
}

CUI_MonsterHP_Red* CUI_MonsterHP_Red::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MonsterHP_Red* pInstance = new   CUI_MonsterHP_Red(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :   CUI_MonsterHP_Red");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MonsterHP_Red::Clone(void* pArg)
{
	CUI_MonsterHP_Red* pInstance = new   CUI_MonsterHP_Red(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :   CUI_MonsterHP_Red");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MonsterHP_Red::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_BG);
	Safe_Release(m_pVIBufferCom);
}