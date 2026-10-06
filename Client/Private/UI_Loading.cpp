#include"stdafx.h"
#include"UI_Loading.h"
#include"GameInstance.h"

CUI_Loading::CUI_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent{ pDevice, pContext }
{
}

CUI_Loading::CUI_Loading(const CUI_Loading& Prototype)
	:CUI_Parent(Prototype)
{
}

HRESULT CUI_Loading::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Loading::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_ChildUIs()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Loading::Priority_Update(_float fTimeDelta)
{


}

void CUI_Loading::Update(_float fTimeDelta)
{
}

void CUI_Loading::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Loading::Render()
{

	if (true == m_isStatic) {

		
		Bind_ShaderResources_Bar_Static();
		Bind_ShaderResources_Fill_Static();

		int m_iRatio = (_uint)(m_fRatio * 100);
		_tchar szPercent[MAX_PATH] = L"";
		wsprintf(szPercent, L"%d %%", m_iRatio);
		m_pGameInstance->Render_Font(TEXT("Font32"), szPercent, _float2(580.f, 610.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		Bind_ShaderResources_BG();
		Bind_ShaderResources_Fill();

		Bind_ShaderResources_Begin();
		if (0.25f <= m_fRatio)
			Bind_ShaderResources_Middle1();
		if (0.5f <= m_fRatio)
			Bind_ShaderResources_Middle2();
		if (0.75f <= m_fRatio)
			Bind_ShaderResources_Middle3();
		if (1.f <= m_fRatio)
			Bind_ShaderResources_Boss();

		int m_iRatio = (_uint)(m_fRatio * 100);
		_tchar szPercent[MAX_PATH] = L"";
		wsprintf(szPercent, L"%d %%", m_iRatio);
		m_pGameInstance->Render_Font(TEXT("Font32"), szPercent, _float2(600.f, 650.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
	}
	return S_OK;
}

HRESULT CUI_Loading::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Begin"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Begin))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Middle"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Middle))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loding_Boss"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Boss))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Fill"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom_Fill))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loding_Bar"),
		TEXT("Com_Texture5"), reinterpret_cast<CComponent**>(&m_pTextureCom_BG))))
		return E_FAIL;

	/*For Loading Static*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Fill_Static"),
		TEXT("Com_Texture6"), reinterpret_cast<CComponent**>(&m_pTextureCom_Static_Loading_Fill))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loding_Bar_Static"),
		TEXT("Com_Texture7"), reinterpret_cast<CComponent**>(&m_pTextureCom_Static_Loading_Bar))))
		return E_FAIL;

}

HRESULT CUI_Loading::Ready_ChildUIs()
{
	return S_OK;
}

HRESULT CUI_Loading::Bind_ShaderResources_BG()
{


	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_BG->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
	
}

HRESULT CUI_Loading::Bind_ShaderResources_Fill()
{

	_matrix  matWorldOfMoney = m_pTransformCom->Get_WorldMatrix();
	matWorldOfMoney.r[0] = { g_iWinSizeX-150.f,0.f,0.f,0.f };
	matWorldOfMoney.r[1] = { 0.f,3.f,0.f,0.f };
	matWorldOfMoney.r[3] = XMVectorSetX(matWorldOfMoney.r[3], XMVectorGetX(matWorldOfMoney.r[3])-5.f);
	
	_float4x4 MoneyFloat4x4;
	XMStoreFloat4x4(&MoneyFloat4x4, matWorldOfMoney);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MoneyFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	
	if(FAILED(m_pShaderCom->Bind_RawValue("g_fLoadingBarRatio",reinterpret_cast<void*>(&m_fRatio),sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Fill->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(8); //패스 하나 새로 파자 ..( 가로 discard)

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_Loading::Bind_ShaderResources_Boss()
{
	_matrix  matWorldOfMoney = m_pTransformCom->Get_WorldMatrix();
	matWorldOfMoney.r[0] = { 100.f,0.f,0.f,0.f };
	matWorldOfMoney.r[1] = { 0.f,75.f,0.f,0.f };
	matWorldOfMoney.r[3] = XMVectorSetX(matWorldOfMoney.r[3], XMVectorGetX(matWorldOfMoney.r[3]) + 600.f);
	//matWorldOfMoney.r[3] = XMVectorSetY(matWorldOfMoney.r[3], XMVectorGetY(matWorldOfMoney.r[3]) + m_fSizeY * 0.5f - 50.f);

	_float4x4 MoneyFloat4x4;
	XMStoreFloat4x4(&MoneyFloat4x4, matWorldOfMoney);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MoneyFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Boss->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Loading::Bind_ShaderResources_Middle1()
{
	_matrix  matWorldOfMoney = m_pTransformCom->Get_WorldMatrix();
	matWorldOfMoney.r[0] = { 75.f,0.f,0.f,0.f };
	matWorldOfMoney.r[1] = { 0.f,60.f,0.f,0.f };
	matWorldOfMoney.r[3] = XMVectorSetX(matWorldOfMoney.r[3], XMVectorGetX(matWorldOfMoney.r[3]) - 305.f);
	//matWorldOfMoney.r[3] = XMVectorSetY(matWorldOfMoney.r[3], XMVectorGetY(matWorldOfMoney.r[3]) );

	_float4x4 MoneyFloat4x4;
	XMStoreFloat4x4(&MoneyFloat4x4, matWorldOfMoney);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MoneyFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Middle->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();



	return S_OK;
}


HRESULT CUI_Loading::Bind_ShaderResources_Middle2()
{
	_matrix  matWorldOfMoney = m_pTransformCom->Get_WorldMatrix();
	matWorldOfMoney.r[0] = { 75.f,0.f,0.f,0.f };
	matWorldOfMoney.r[1] = { 0.f,60.f,0.f,0.f };
	matWorldOfMoney.r[3] = XMVectorSetX(matWorldOfMoney.r[3], XMVectorGetX(matWorldOfMoney.r[3])-5.f);
	//matWorldOfMoney.r[3] = XMVectorSetY(matWorldOfMoney.r[3], XMVectorGetY(matWorldOfMoney.r[3]) + m_fSizeY * 0.5f - 50.f);

	_float4x4 MoneyFloat4x4;
	XMStoreFloat4x4(&MoneyFloat4x4, matWorldOfMoney);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MoneyFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Middle->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();



	return S_OK;
}


HRESULT CUI_Loading::Bind_ShaderResources_Middle3()
{
	_matrix  matWorldOfMoney = m_pTransformCom->Get_WorldMatrix();
	matWorldOfMoney.r[0] = { 75.f,0.f,0.f,0.f };
	matWorldOfMoney.r[1] = { 0.f,60.f,0.f,0.f };
	matWorldOfMoney.r[3] = XMVectorSetX(matWorldOfMoney.r[3], XMVectorGetX(matWorldOfMoney.r[3]) + 290.f);
	//matWorldOfMoney.r[3] = XMVectorSetY(matWorldOfMoney.r[3], XMVectorGetY(matWorldOfMoney.r[3]) + m_fSizeY * 0.5f - 50.f);

	_float4x4 MoneyFloat4x4;
	XMStoreFloat4x4(&MoneyFloat4x4, matWorldOfMoney);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MoneyFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Middle->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();



	return S_OK;
}

HRESULT CUI_Loading::Bind_ShaderResources_Begin()
{
	_matrix  matWorldOfMoney = m_pTransformCom->Get_WorldMatrix();
	matWorldOfMoney.r[0] = { 110.f,0.f,0.f,0.f };
	matWorldOfMoney.r[1] = { 0.f,75.f,0.f,0.f };
	matWorldOfMoney.r[3] = XMVectorSetX(matWorldOfMoney.r[3], XMVectorGetX(matWorldOfMoney.r[3]) - 610.f);
	
	_float4x4 MoneyFloat4x4;
	XMStoreFloat4x4(&MoneyFloat4x4, matWorldOfMoney);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MoneyFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Begin->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CUI_Loading::Bind_ShaderResources_Bar_Static()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Static_Loading_Bar->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_Loading::Bind_ShaderResources_Fill_Static()
{
	_matrix  matWorldOfMoney = m_pTransformCom->Get_WorldMatrix();
	matWorldOfMoney.r[0] = { g_iWinSizeX - 110.f,0.f,0.f,0.f };
	matWorldOfMoney.r[1] = { 0.f,32.f,0.f,0.f };
	matWorldOfMoney.r[3] = XMVectorSetX(matWorldOfMoney.r[3], XMVectorGetX(matWorldOfMoney.r[3])-5.f );

	_float4x4 MoneyFloat4x4;
	XMStoreFloat4x4(&MoneyFloat4x4, matWorldOfMoney);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MoneyFloat4x4)))
		return E_FAIL;

	

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fLoadingBarRatio", reinterpret_cast<void*>(&m_fRatio), sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Static_Loading_Fill->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(8); //패스 하나 새로 파자 ..( 가로 discard)

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

CUI_Loading* CUI_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Loading* pInstance = new  CUI_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Shop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Loading::Clone(void* pArg)
{
	CUI_Loading* pInstance = new  CUI_Loading(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom_Begin);
	Safe_Release(m_pTextureCom_Middle);
	Safe_Release(m_pTextureCom_Boss);
	Safe_Release(m_pTextureCom_Fill);
	Safe_Release(m_pTextureCom_BG);

	Safe_Release(m_pTextureCom_Static_Loading_Bar);
	Safe_Release(m_pTextureCom_Static_Loading_Fill);
}
