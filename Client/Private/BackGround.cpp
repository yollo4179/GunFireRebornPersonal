#include "stdafx.h"
#include "..\Public\BackGround.h"

#include "GameInstance.h"
#include"UI_Loading.h"

CBackGround::CBackGround(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Parent{ pDevice, pContext }
{
}

CBackGround::CBackGround(const CBackGround & Prototype)
	: CUI_Parent( Prototype )
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void * pArg)
{
	BACKGROUND_DESC*		pDesc = static_cast<BACKGROUND_DESC*>(pArg);

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
	if (FAILED(Ready_ChildUIs()))
		return E_FAIL;
	

	return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
	
	static_cast<CUI_Loading*>(m_ChildUIs[L"Loading"])->Set_Ratio(m_fRatio);

	int a = 10;

	m_fAccTime += fTimeDelta;
}

void CBackGround::Update(_float fTimeDelta)
{	

}

void CBackGround::Late_Update(_float fTimeDelta)
{
	if(m_bActive)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CBackGround::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	

	if(false == m_isLogoBackGround )
		m_ChildUIs[L"Loading"]->Render();

	return S_OK;
}

void CBackGround::Set_Static()
{
	static_cast <CUI_Loading*>(m_ChildUIs[L"Loading"])->Set_Static_Loading();
}

HRESULT CBackGround::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LogoBackGround"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	



	return S_OK;

	
}

HRESULT CBackGround::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (m_isLogoBackGround) {
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;
	}
	else
	{
		if (m_fAccTime >= 4.f) {
			int iPrevPaint = m_iCurPaint;
			for (int i = 0; i < 100; ++i)
			{
				int iNowPaint = 1 + rand() % 4;
				if (iNowPaint != iPrevPaint) {
					m_iCurPaint = iNowPaint;
					break;
				}
			}
			m_fAccTime = 0.f;
		}
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurPaint)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CBackGround::Ready_ChildUIs()
{
	//Loading
	{
		CUIObject* pUIObject = { nullptr };

		CUI_Loading::UI_LOADING_DESC LoadingDesc = {};
		LoadingDesc.eType = UI_TYPE::UI_POP;
		LoadingDesc.fRotationPerSec = XMConvertToRadians(90.f);
		LoadingDesc.fSpeedPerSec = 10.f;
		LoadingDesc.fSizeX = g_iWinSizeX;
		LoadingDesc.fSizeY = 50;
		LoadingDesc.fX = 0.f;
		LoadingDesc.fY = 600.f;


		pUIObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Loading"), &LoadingDesc));
		m_ChildUIs.emplace(L"Loading", pUIObject);
		

	}
	return S_OK;
}

CBackGround * CBackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBackGround*	pInstance = new CBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround::Clone(void * pArg)
{
	CBackGround*	pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
