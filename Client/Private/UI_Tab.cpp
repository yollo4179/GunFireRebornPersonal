#include"stdafx.h"
#include"GameInstance.h"
#include"UI_Tab.h"

CUI_Tab::CUI_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent(pDevice, pContext)
{
}

CUI_Tab::CUI_Tab(const CUI_Tab& Prototype)
	:CUI_Parent(Prototype)
{
}

HRESULT CUI_Tab::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Tab::Initialize(void* pArg)
{
	UI_TAB_DESC* pDesc = static_cast<UI_TAB_DESC*>(pArg);
	

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;
	return S_OK;
}

void CUI_Tab::Priority_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
}

void CUI_Tab::Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return ;

}

void CUI_Tab::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Tab::Render()
{

 	BindAndRender_Icon();
	BindAndRender_Tab();


	return S_OK;
}

HRESULT CUI_Tab::Ready_Components()
{

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Key_Tab"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Tab))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Icon_Inven"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Icon))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Tab::BindAndRender_Icon()
{
	_matrix matIconPos = XMMatrixIdentity();
	matIconPos.r[0] *= 50;
	matIconPos.r[1] *= 50;


	matIconPos.r[3] = m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matIconPos);

	XMStoreFloat4x4(&Float4x4World, matIconPos);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Icon->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
	
}

HRESULT CUI_Tab::BindAndRender_Tab()
{

	_matrix matTabPos = XMMatrixIdentity();
	matTabPos.r[0] *= 45;
	matTabPos.r[1] *= 20;

	_vector vPos = { 30,-30,0 };
	matTabPos.r[3] = vPos + m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matTabPos);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Tab->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
;
}

CUI_Tab* CUI_Tab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Tab* pInstance = new  CUI_Tab(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Tab");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Tab::Clone(void* pArg)
{
	CUI_Tab* pInstance = new  CUI_Tab(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Tab");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Tab::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom_Icon);
	Safe_Release(m_pTextureCom_Tab);
	Safe_Release(m_pVIBufferCom);

}
