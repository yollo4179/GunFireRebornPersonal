#include"stdafx.h"
#include"UI_Goblet_RandomOptionButton.h"
#include"GameInstance.h"
#include"Player.h"
#include"Hands_Skill.h"

CUI_Goblet_RandomOptionButton::CUI_Goblet_RandomOptionButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Parent{ pDevice, pContext }
{
}

CUI_Goblet_RandomOptionButton::CUI_Goblet_RandomOptionButton(const CUI_Goblet_RandomOptionButton& Prototype)
	: CUI_Parent(Prototype)
{
}

HRESULT CUI_Goblet_RandomOptionButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Goblet_RandomOptionButton::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	if (FAILED(Ready_Components()))
		return E_FAIL;

	if(FAILED(Ready_Text()))
		return S_OK;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;

	m_EventFunction[(_uint)UI_STATE::ON] = std::bind(&CUI_Goblet_RandomOptionButton::Event_MouseOn, this);
	m_EventFunction[(_uint)UI_STATE::UP] = std::bind(&CUI_Goblet_RandomOptionButton::Event_MouseUp, this);
	m_EventFunction[(_uint)UI_STATE::AWAY] = std::bind(&CUI_Goblet_RandomOptionButton::Event_MouseAway, this);
	return S_OK;
}

void CUI_Goblet_RandomOptionButton::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);


	
}

void CUI_Goblet_RandomOptionButton::Update(_float fTimeDelta)
{
	__super::Update_Event();
}

void CUI_Goblet_RandomOptionButton::Late_Update(_float fTimeDelta)
{

	
}

HRESULT CUI_Goblet_RandomOptionButton::Render()
{
	if (FAILED(Render_ShaderResources_BG()))
		return E_FAIL;

	if (FAILED(Render_ShaderResources_Icon()))
		return E_FAIL;

	if (GOBLET_GRENADE) 
	{
		if (FAILED(Render_ShaderResources_Kind()))
			return E_FAIL;
	}
	m_pGameInstance->Render_Font(TEXT("Font32"), m_szText[m_iGobletType], _float2(m_fX - 0.5f * m_fSizeX + 30.f, m_fY /*+ 0.5 * m_fSizeY - 20.f*/), XMVectorSet(1.f, 1.f, 0.f, 1.f));


	return S_OK;
}

void CUI_Goblet_RandomOptionButton::Event_MouseOn()
{
	m_fSizefOnMouse = 1.1F;
}

void CUI_Goblet_RandomOptionButton::Event_MouseAway()
{
	m_fSizefOnMouse=1.F;
}

void CUI_Goblet_RandomOptionButton::Event_MouseUp()
{
	m_pGameInstance->PlayEffect(L"Goblet_Select.ogg", 0.1f);
	m_bOptionSelected = true;
	/*call hands event */

	CPlayer* pPlayer = static_cast<CPlayer*>( m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
	CHands* pHandsSkill = static_cast<CHands*> (m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS_SKILL, 0));

	HandsInfo eHandsInfo = pHandsSkill->Get_HandsInfo();
	switch(m_iGobletType)
	{
	case GRENADE_CRI_DEMAGE:
		eHandsInfo.fCriticalDemage = (_uint)(eHandsInfo.fCriticalDemage*1.1f);
		pHandsSkill->Set_HandsInfo(eHandsInfo);
		break;

	case GRENADE_CRI_CHANCE:
		eHandsInfo.fCriticalRate = (_uint)(eHandsInfo.fCriticalRate * 1.1f);
		pHandsSkill->Set_HandsInfo(eHandsInfo);
		break;
	case GRENADE_LOADPLUS:
		PlayerItemInfo pItemInfo =pPlayer->Get_PlayerItemInfo();
		pItemInfo.iNumMaxGrenades+=1;
		pPlayer->Set_PlayerItemInfo(pItemInfo);
		break;
	case GRENADE_RANGE:
		eHandsInfo.iRange =(_uint)eHandsInfo.iRange*1.3f;
		pHandsSkill->Set_HandsInfo(eHandsInfo);
		static_cast<CHands_Skill*>(pHandsSkill)->Set_BombRange(eHandsInfo.iRange * 1.3f);
		break;
	case GRENADE_DEMAGE:
		eHandsInfo.fDemage = (_uint) 1.3f * eHandsInfo.fDemage;
		pHandsSkill->Set_HandsInfo(eHandsInfo);
		break;
	case HP_PLUS:
		_float4 vHpInfo = pPlayer->Get_HPInfo();
		vHpInfo.w *= 1.3f;
		pPlayer->Set_HPInfo(vHpInfo);
		break;
	default:
		break;
	}


}

HRESULT CUI_Goblet_RandomOptionButton::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_CirticalDemage"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom[0]))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_CriticalChance"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[1]))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_Range"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom[2]))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_LoadPlus"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom[3]))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_Demag"),
		TEXT("Com_Texture5"), reinterpret_cast<CComponent**>(&m_pTextureCom[4]))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_HPPlus"),
		TEXT("Com_Texture6"), reinterpret_cast<CComponent**>(&m_pTextureCom[5]))))
		return E_FAIL;


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_BG_Grenade"),
		TEXT("Com_Texture10"), reinterpret_cast<CComponent**>(&m_pTextureCom_BG[0]))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_BG_HP"),
		TEXT("Com_Texture11"), reinterpret_cast<CComponent**>(&m_pTextureCom_BG[1]))))
		return E_FAIL;


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_Reinforce"),
		TEXT("Com_Texture15"), reinterpret_cast<CComponent**>(&m_pTextureCom_Kind))))
		return E_FAIL;


	CTexture* m_pTextureCom[10] = { nullptr };
	CTexture* m_pTextureCom_BG[2] = { nullptr };
	CTexture* m_pTextureCom_Kind = { nullptr };



	return S_OK;
}

HRESULT CUI_Goblet_RandomOptionButton::Ready_Text()
{
	memcpy(m_szText[GRENADE_CRI_DEMAGE], L"Increase Grenade Critical DMG +10%", sizeof(m_szText[GRENADE_CRI_DEMAGE]));
	memcpy(m_szText[GRENADE_CRI_CHANCE], L"Increase Grenade Critical Chance +10%", sizeof(m_szText[GRENADE_CRI_CHANCE]));
	memcpy(m_szText[GRENADE_RANGE], L"Increase Grenade Range +30% ", sizeof(m_szText[GRENADE_RANGE]));
	memcpy(m_szText[GRENADE_LOADPLUS], L"Increase Grenade Capicity +1", sizeof(m_szText[GRENADE_LOADPLUS]));
	memcpy(m_szText[GRENADE_DEMAGE], L"Increase Grenade Demage +30%", sizeof(m_szText[GRENADE_DEMAGE]));
	memcpy(m_szText[HP_PLUS], L"Increase MaxHP +30%", sizeof(m_szText[HP_PLUS]));
	
	
	return S_OK;
}

HRESULT CUI_Goblet_RandomOptionButton::Render_ShaderResources_BG()
{

	_matrix  matWorldOfBG = m_pTransformCom->Get_WorldMatrix();
	matWorldOfBG.r[0] = matWorldOfBG.r[0]*m_fSizefOnMouse;
	matWorldOfBG.r[1] = matWorldOfBG.r[1] *m_fSizefOnMouse;
	matWorldOfBG.r[3] = XMVectorSetX(matWorldOfBG.r[3], XMVectorGetX(matWorldOfBG.r[3]));
	matWorldOfBG.r[3] = XMVectorSetY(matWorldOfBG.r[3], XMVectorGetY(matWorldOfBG.r[3]) + m_fSizeY * 0.5f - 250.f);

	_float4x4 BGFloat4x4;
	XMStoreFloat4x4(&BGFloat4x4, matWorldOfBG);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &BGFloat4x4)))
		return E_FAIL;

	/*if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;*/

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	int iBG_ID = {};
	if (GOBLET_GRENADE)
	{
		iBG_ID = 0;
	}
	else
	{
		iBG_ID = 1;
	}


	
	if (FAILED(m_pTextureCom_BG[iBG_ID]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Goblet_RandomOptionButton::Render_ShaderResources_Icon()
{
	_matrix  matWorldOfIcon = m_pTransformCom->Get_WorldMatrix();
	matWorldOfIcon.r[0] = {100.f,0.f,0.f,0.f};
	matWorldOfIcon.r[1] = { 0.f,100.f,0.f,0.f };
	matWorldOfIcon.r[3] = XMVectorSetX(matWorldOfIcon.r[3], XMVectorGetX(matWorldOfIcon.r[3]));
	matWorldOfIcon.r[3] = XMVectorSetY(matWorldOfIcon.r[3], XMVectorGetY(matWorldOfIcon.r[3]) + m_fSizeY * 0.5f - 150.f);

	_float4x4 IconFloat4x4;
	XMStoreFloat4x4(&IconFloat4x4, matWorldOfIcon);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &IconFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[m_iGobletType]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CUI_Goblet_RandomOptionButton::Render_ShaderResources_Kind()
{
	
	
	_matrix  matWorldOfKind = m_pTransformCom->Get_WorldMatrix();
	matWorldOfKind.r[0] = { 100.f,0.f,0.f,0.f };
	matWorldOfKind.r[1] = { 0.f,50.f,0.f,0.f };
	matWorldOfKind.r[3] = XMVectorSetX(matWorldOfKind.r[3], XMVectorGetX(matWorldOfKind.r[3]));
	matWorldOfKind.r[3] = XMVectorSetY(matWorldOfKind.r[3], XMVectorGetY(matWorldOfKind.r[3]) - m_fSizeY * 0.5f +50.f);

	_float4x4 KindFloat4x4;
	XMStoreFloat4x4(&KindFloat4x4, matWorldOfKind);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &KindFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Kind->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();


	return S_OK;
	
	
	return S_OK;
}

CUI_Goblet_RandomOptionButton* CUI_Goblet_RandomOptionButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Goblet_RandomOptionButton* pInstance = new  CUI_Goblet_RandomOptionButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Goblet_RandomOptionButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Goblet_RandomOptionButton::Clone(void* pArg)
{
	CUI_Goblet_RandomOptionButton* pInstance = new  CUI_Goblet_RandomOptionButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Goblet_RandomOptionButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Goblet_RandomOptionButton::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

	for (size_t i = 0; i < 10; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
	}
	for (size_t i = 0; i < 2; ++i)
	{
		Safe_Release(m_pTextureCom_BG[i]);
	}
	Safe_Release(m_pTextureCom_Kind);

}
