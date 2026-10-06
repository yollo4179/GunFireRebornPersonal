#include"stdafx.h"
#include"UI_Victory_Circle.h"
#include"GameInstance.h"

CUI_Victory_Circle::CUI_Victory_Circle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Victory_Circle::CUI_Victory_Circle(const CUI_Victory_Circle& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Victory_Circle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Victory_Circle::Initialize(void* pArg)
{
	UI_VICTORY_CIRCLE_DESC* pDesc = static_cast<UI_VICTORY_CIRCLE_DESC*>(pArg);


	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;

}

void CUI_Victory_Circle::Priority_Update(_float fTimeDelta)
{
	
	if (true == m_bBigger)
		m_fAccBiggerTime += fTimeDelta;

	if (m_fAccBiggerTime >= m_fBiggerTime)
	{
		m_fAccBiggerTime = 0.F;
		m_bBigger = false;
	}
}

void CUI_Victory_Circle::Update(_float fTimeDelta)
{
}

void CUI_Victory_Circle::Late_Update(_float fTimeDelta)
{
	m_pTransformCom->Scaling(m_fSizeX*2.f*(1.f+m_fAccBiggerTime*4.f), m_fSizeY * 2.f*(1.f + m_fAccBiggerTime*4.f), 200.f);

}

HRESULT CUI_Victory_Circle::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Victory_Circle::Set_Bigger(_bool _bBigger)
{
		m_bBigger = _bBigger;
		m_fAccBiggerTime = 0.f;
}

void CUI_Victory_Circle::Set_Active()
{
	m_bActive = true;
	m_bBigger = true;
	m_fAccBiggerTime = 0.f;
}

HRESULT CUI_Victory_Circle::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Victory_Circle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CUI_Victory_Circle::Bind_ShaderResources()
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

CUI_Victory_Circle* CUI_Victory_Circle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Victory_Circle* pInstance = new  CUI_Victory_Circle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Victory_Circle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Victory_Circle::Clone(void* pArg)
{
	CUI_Victory_Circle* pInstance = new  CUI_Victory_Circle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Victory_Circle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Victory_Circle::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
