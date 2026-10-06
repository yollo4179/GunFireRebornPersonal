#include"stdafx.h"
#include"GameInstance.h"
#include"UI_Dash.h"
#include"UI_Numbers.h"
#include"Player.h"

CUI_Dash::CUI_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent(pDevice, pContext)
{
}

CUI_Dash::CUI_Dash(const CUI_Dash& Prototype)
	:CUI_Parent(Prototype)
{
}

HRESULT CUI_Dash::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Dash::Initialize(void* pArg)
{
	UI_DASH_DESC* pDesc = static_cast<UI_DASH_DESC*>(pArg);


	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	
	return S_OK;
}

void CUI_Dash::Priority_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
	CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
	if (nullptr == pPlayer)
		return ;

	m_fBombCoolTimeRatio = pPlayer->Get_DashCoolTimeRatio();

}

void CUI_Dash::Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

}

void CUI_Dash::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);

}

HRESULT CUI_Dash::Render()
{

	if (FAILED(BindAndRender_BG()))
		return E_FAIL;

	if (FAILED(BindAndRender_Icon()))
		return E_FAIL;
	if (FAILED(BindAndRender_Icon_Back()))
		return E_FAIL;

	if (FAILED(BindAndRender_Shift()))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CUI_Dash::Ready_Components()
{

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Key_Shift"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Shift))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Icon_Dash"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Icon))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BG_Skill"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_BG))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Icon_Bomb"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom_Icon_Back))))
		return E_FAIL;




	return S_OK;
}


HRESULT CUI_Dash::BindAndRender_Icon()
{
	_matrix matIconPos = XMMatrixIdentity();
	matIconPos.r[0] *= 50;
	matIconPos.r[1] *= 50;


	matIconPos.r[3] = m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
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

HRESULT CUI_Dash::BindAndRender_Icon_Back()
{
	_matrix matIconPos = XMMatrixIdentity();
	matIconPos.r[0] *= 50;
	matIconPos.r[1] *= 50;


	matIconPos.r[3] = m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matIconPos);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Icon->Bind_ShaderResource(m_pShaderCom, "g_Texture", 1)))
		return E_FAIL;


	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_SkillCoolTimeRatio", reinterpret_cast<void*>(&m_fBombCoolTimeRatio), sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(7);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_Dash::BindAndRender_BG()
{

	_matrix matBGPos = XMMatrixIdentity();
	matBGPos.r[0] *= 60;
	matBGPos.r[1] *= 60;


	matBGPos.r[3] = m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matBGPos);


	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_BG->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
	return S_OK;
}

HRESULT CUI_Dash::BindAndRender_Shift()
{

	_matrix matShiftPos = XMMatrixIdentity();
	matShiftPos.r[0] *= 20;
	matShiftPos.r[1] *= 20;

	_vector vPos = { 30,-30,0 };
	matShiftPos.r[3] = vPos + m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matShiftPos);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Shift->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
	;
}

CUI_Dash* CUI_Dash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Dash* pInstance = new  CUI_Dash(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dash::Clone(void* pArg)
{
	CUI_Dash* pInstance = new  CUI_Dash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dash::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom_Icon);
	Safe_Release(m_pTextureCom_Icon_Back);
	Safe_Release(m_pTextureCom_Shift);
	Safe_Release(m_pTextureCom_BG);
	Safe_Release(m_pVIBufferCom);

}
