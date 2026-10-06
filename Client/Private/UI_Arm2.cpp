#include "stdafx.h"
#include "..\Public\UI_Arm2.h"
#include "GameInstance.h"


CUI_Arm2::CUI_Arm2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Arm2::CUI_Arm2(const   CUI_Arm2& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT   CUI_Arm2::Initialize_Prototype()
{
	return S_OK;
}

HRESULT   CUI_Arm2::Initialize(void* pArg)
{
	UI_ARM2_DESC* pDesc = static_cast<UI_ARM2_DESC*>(pArg);

	m_pNowEquipedArmIndex = pDesc->pNowEquipedArmIndex;

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
}

void   CUI_Arm2::Priority_Update(_float fTimeDelta)
{
	//__super::Priority_Update(fTimeDelta);
}

void   CUI_Arm2::Update(_float fTimeDelta)
{
	//__super::Update_Event();
}

void   CUI_Arm2::Late_Update(_float fTimeDelta)
{
	
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT   CUI_Arm2::Render()
{

	

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT   CUI_Arm2::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Equip"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;


}

HRESULT   CUI_Arm2::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 1)))
		return E_FAIL;

	return S_OK;
}

CUI_Arm2* CUI_Arm2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Arm2* pInstance = new   CUI_Arm2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :   CUI_Arm2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Arm2::Clone(void* pArg)
{
	CUI_Arm2* pInstance = new   CUI_Arm2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :   CUI_Arm2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void   CUI_Arm2::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
