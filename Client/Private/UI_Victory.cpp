#include"stdafx.h""
#include"UI_Victory.h"
#include"GameInstance.h"
#include"UI_Victory_Circle.h"

CUI_Victory::CUI_Victory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent{ pDevice, pContext }
{
}

CUI_Victory::CUI_Victory(const CUI_Victory& Prototype)
	:CUI_Parent(Prototype) 
{
}

HRESULT CUI_Victory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Victory::Initialize(void* pArg)
{
	UI_VICTORY_DESC* pDesc = static_cast<UI_VICTORY_DESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_ChildUIs()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL; 
	return S_OK;
}

void CUI_Victory::Priority_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	if (STATE_SMALLER == m_iState)
	{
		m_fAccSmallerTime += fTimeDelta;
	}
	if (m_fAccSmallerTime >= m_fSmallerTime)
	{
		m_fAccSmallerTime = 0.f;
		m_iState = STATE_BIGGER;
	}
	if (STATE_BIGGER == m_iState)
	{
		
		if(false == m_bStart_Bigger)
			m_ChildUIs[L"Victory_Circle"]->Set_Active();
		m_fAccBiggerAgainTime += fTimeDelta;
		m_bStart_Bigger = true;
	}
	if (m_fAccBiggerAgainTime >= m_fBiggerTime)
	{
		m_fAccBiggerAgainTime = 0.f;
		m_iState = STATE_INACTIVE;
	}

	if (STATE_INACTIVE== m_iState)
	{
		m_bActive = false;
	}


	if (STATE_BIGGER == m_iState)
	{
		for (auto Child : m_ChildUIs)
			Child.second->Priority_Update(fTimeDelta);
	}
	
}

void CUI_Victory::Update(_float fTimeDelta)
{
}

void CUI_Victory::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	if (STATE_BIGGER == m_iState)
	{
		m_pTransformCom->Scaling(m_fSizeX * 0.35f*(1.f + 0.3f*m_fAccBiggerAgainTime), m_fSizeY * 0.35f*(1.f + 0.3f*m_fAccBiggerAgainTime), 1.F);
	}
	else if (STATE_SMALLER == m_iState)
	{
		m_pTransformCom->Scaling(m_fSizeX * (1 - m_fAccSmallerTime), m_fSizeY * (1 - m_fAccSmallerTime), 1.F);
	}

	if (STATE_BIGGER == m_iState)
	{
		for (auto Child : m_ChildUIs)
			Child.second->Late_Update(fTimeDelta);
	}


	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Victory::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	
	
	if (STATE_BIGGER == m_iState)
	{
		for (auto Child : m_ChildUIs)
			Child.second->Render();
	}
	return S_OK;

}

void CUI_Victory::Set_Active()
{
	m_bActive = true;
	m_iState = STATE_SMALLER;
	m_bStart_Bigger = false;
	m_pGameInstance->PlayEffect(L"Victory_Sound.ogg", 0.4f);
}

HRESULT CUI_Victory::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Victory"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Victory::Ready_ChildUIs()
{

	

	CUI_Victory_Circle::UI_VICTORY_CIRCLE_DESC  Victory_CircleDesc = {};
	Victory_CircleDesc.fRotationPerSec = XMConvertToRadians(90.f);
	Victory_CircleDesc.fSpeedPerSec = 10.f;
	Victory_CircleDesc.fSizeX = 300.f;
	Victory_CircleDesc.fSizeY = 300.f;
	Victory_CircleDesc.fX =g_iWinSizeX    * 0.5F -150.f;
	Victory_CircleDesc.fY = g_iWinSizeY   * 0.5F -150.f;
	Victory_CircleDesc.iData = 0;


	CUIObject* pObject = { nullptr };
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Victory_Circle"), &Victory_CircleDesc));
	m_ChildUIs.emplace(L"Victory_Circle", pObject);
	return S_OK;
}

HRESULT CUI_Victory::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_Victory* CUI_Victory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Victory* pInstance = new  CUI_Victory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Victory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Victory::Clone(void* pArg)
{
	CUI_Victory* pInstance = new  CUI_Victory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Victory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Victory::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
