#include"stdafx.h"
#include"UI_Dialogue_OK_Button.h"
#include"GameInstance.h"
CUI_Dialogue_OK_Button::CUI_Dialogue_OK_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
:CUIObject{pDevice, pContext}
{
}

CUI_Dialogue_OK_Button::CUI_Dialogue_OK_Button(const CUI_Dialogue_OK_Button& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CUI_Dialogue_OK_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Dialogue_OK_Button::Initialize(void* pArg)
{


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;

	m_EventFunction[(_uint)UI_STATE::ON] = std::bind(&CUI_Dialogue_OK_Button::Event_MouseOn, this);
	m_EventFunction[(_uint)UI_STATE::UP] = std::bind(&CUI_Dialogue_OK_Button::Event_MouseUp, this);
	m_EventFunction[(_uint)UI_STATE::AWAY] = std::bind(&CUI_Dialogue_OK_Button::Event_MouseAway, this);
	return S_OK;
	return S_OK;
}

void CUI_Dialogue_OK_Button::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}


void CUI_Dialogue_OK_Button::Update(_float fTimeDelta)
{
	__super::Update_Event();
}

void CUI_Dialogue_OK_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Dialogue_OK_Button::Render()
{
	if (FAILED(Render_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

void CUI_Dialogue_OK_Button::Event_MouseOn()
{
}

void CUI_Dialogue_OK_Button::Event_MouseAway()
{
}

void CUI_Dialogue_OK_Button::Event_MouseUp()
{
	m_isClicked = true;
	m_pGameInstance->PlayEffect(L"UI_Button.ogg", 0.1f);
}

HRESULT CUI_Dialogue_OK_Button::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/*Com_Texture*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_OK"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;



	return S_OK;
}

HRESULT CUI_Dialogue_OK_Button::Render_ShaderResources()
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

CUI_Dialogue_OK_Button* CUI_Dialogue_OK_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Dialogue_OK_Button* pInstance = new  CUI_Dialogue_OK_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Dialogue_OK_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dialogue_OK_Button::Clone(void* pArg)
{
	CUI_Dialogue_OK_Button* pInstance = new  CUI_Dialogue_OK_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Dialogue_OK_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dialogue_OK_Button::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
