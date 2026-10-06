#include "stdafx.h"
#include "..\Public\UI_Arm_Paint.h"
#include "GameInstance.h"


CUI_Arm_Paint::CUI_Arm_Paint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Arm_Paint::CUI_Arm_Paint(const   CUI_Arm_Paint& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT   CUI_Arm_Paint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT   CUI_Arm_Paint::Initialize(void* pArg)
{
	UI_ARM_PAINT_DESC* pDesc = static_cast<UI_ARM_PAINT_DESC*>(pArg);

	m_pNowArmNumber = pDesc->pNowArmNumber;

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

	return S_OK;
}

void   CUI_Arm_Paint::Priority_Update(_float fTimeDelta)
{
}

void   CUI_Arm_Paint::Update(_float fTimeDelta)
{
	
}

void   CUI_Arm_Paint::Late_Update(_float fTimeDelta)
{

		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT   CUI_Arm_Paint::Render()
{

	

	if (FAILED(Render_BG()))
		return E_FAIL;

	if (FAILED(Render_CurrentArm()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Arm_Paint::Render_BG()
{
	if (FAILED(Bind_ShaderResources(0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_Arm_Paint::Render_CurrentArm()
{
	if (nullptr == m_pNowArmNumber)
	{
		MSG_BOX("COUDNT GET ARMOR NUMBER FROM PARENT UI");
		return E_FAIL;
	}
	if (FAILED(Bind_ShaderResources(1, *m_pNowArmNumber)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}



HRESULT   CUI_Arm_Paint::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponBG"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[0]))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Arm"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[1]))))
		return E_FAIL;

	return S_OK;


}

HRESULT   CUI_Arm_Paint::Bind_ShaderResources(_uint iTextureIndex,_uint iNowArmTexture)
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[iTextureIndex]->Bind_ShaderResource(m_pShaderCom, "g_Texture", iNowArmTexture)))
		return E_FAIL;

	return S_OK;
}

CUI_Arm_Paint* CUI_Arm_Paint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Arm_Paint* pInstance = new   CUI_Arm_Paint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :   CUI_Arm_Paint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Arm_Paint::Clone(void* pArg)
{
	CUI_Arm_Paint* pInstance = new   CUI_Arm_Paint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :   CUI_Arm_Paint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void   CUI_Arm_Paint::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom[0]);
	Safe_Release(m_pTextureCom[1]);
	Safe_Release(m_pVIBufferCom);
}
