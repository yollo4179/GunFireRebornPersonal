#include "stdafx.h"
#include "..\Public\UI_Button_Status.h"

#include "GameInstance.h"


 CUI_Button_Status:: CUI_Button_Status(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

 CUI_Button_Status:: CUI_Button_Status(const   CUI_Button_Status& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT   CUI_Button_Status::Initialize_Prototype()
{
	return S_OK;
}

HRESULT   CUI_Button_Status::Initialize(void* pArg)
{
	UI_BUTTTON_STATUS_DESC* pDesc = static_cast<UI_BUTTTON_STATUS_DESC*>(pArg);

	/* 백그라우늗의 ㅁ크기아ㅘ 위치를 잡아주기위한 용오 .*/
	if (nullptr == pDesc) {
		pDesc->fX = 100;//g_iWinSizeX >> 1;
		pDesc->fY = 0;//g_iWinSizeY >> 1;
		pDesc->fSizeX = 200;//g_iWinSizeX;
		pDesc->fSizeY = 100;//g_iWinSizeY;
	}

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;

	m_EventFunction[(_uint)UI_STATE::ON] = std::bind(&CUI_Button_Status::Event_MouseOn, this);
	m_EventFunction[(_uint)UI_STATE::UP] = std::bind(&CUI_Button_Status::Event_MouseUp, this);
	m_EventFunction[(_uint)UI_STATE::AWAY] = std::bind(&CUI_Button_Status::Event_MouseAway, this);

	return S_OK;
}

void   CUI_Button_Status::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void   CUI_Button_Status::Update(_float fTimeDelta)
{
	__super::Update_Event();
}

void   CUI_Button_Status::Late_Update(_float fTimeDelta)
{
	if (m_bActive)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT   CUI_Button_Status::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}
void CUI_Button_Status::Event_MouseOn()
{


	m_pTransformCom->Scaling(160.f, 80.f, 1.2f);
}

void CUI_Button_Status::Event_MouseAway()
{
	m_pTransformCom->Scaling(120.f, 60.f, 1.f);
}

void CUI_Button_Status::Event_MouseUp()
{
	
}
HRESULT   CUI_Button_Status::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_UI_Button_Status"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;






	return S_OK;


}

HRESULT   CUI_Button_Status::Bind_ShaderResources()
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

 CUI_Button_Status*  CUI_Button_Status::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	 CUI_Button_Status* pInstance = new   CUI_Button_Status(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :   CUI_Button_Status");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject*  CUI_Button_Status::Clone(void* pArg)
{
	 CUI_Button_Status* pInstance = new   CUI_Button_Status(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :   CUI_Button_Status");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void   CUI_Button_Status::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
