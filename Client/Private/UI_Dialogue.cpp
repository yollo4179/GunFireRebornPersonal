#include"stdafx.h"
#include "UI_Dialogue.h"
#include"UI_Dialogue_Cancle_Button.h"
#include"UI_Dialogue_OK_Button.h"
#include"GameInstance.h"
#include"Camera_Player.h"

CUI_Dialogue::CUI_Dialogue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent{ pDevice, pContext }
{
}

CUI_Dialogue::CUI_Dialogue(const CUI_Dialogue& Prototype)
	:CUI_Parent(Prototype)
{
}

HRESULT CUI_Dialogue::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Dialogue::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_ChildUIs()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;


	return S_OK;
}

void CUI_Dialogue::Priority_Update(_float fTimeDelta)
{

	if (false == m_bActive)
		return;

	/*if (KEY_UP(KEY::ESC))
	{
		m_bActive = false;
		CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
		if (nullptr == pTargetCamera)
			return;
		pTargetCamera->Set_Camera_Player();
	}*/


		for (auto& Child : m_ChildUIs)
			Child.second->Priority_Update(fTimeDelta);
	





}

void CUI_Dialogue::Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
		for (auto& Child : m_ChildUIs)
			Child.second->Update(fTimeDelta);
	
}

void CUI_Dialogue::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
	

	for (auto& Child : m_ChildUIs)
		Child.second->Late_Update(fTimeDelta);


	if (true == m_ChildUIs[L"Button_Cancle_Dialogue"]->Get_Button())
	{
		m_bActive = false;
		CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
		if (nullptr == pTargetCamera)
			return;
		pTargetCamera->Set_Camera_Player();
		m_ChildUIs[L"Button_Cancle_Dialogue"]->Set_Button(false);
	}

	if (true == m_ChildUIs[L"Button_Ok_Dialogue"]->Get_Button())
	{
		++m_iDialogueIndex;

		m_ChildUIs[L"Button_Ok_Dialogue"]->Set_Button(false);
	}
	if (m_Dialogues.size() == m_iDialogueIndex)
	{
		m_isTextEnd = true;
		m_iDialogueIndex = 0;
		m_bActive = false;
		CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
		if (nullptr == pTargetCamera)
			return;
		pTargetCamera->Set_Camera_Player();


	}

}

HRESULT CUI_Dialogue::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	for (auto& Child : m_ChildUIs)
		Child.second->Render();
	
	if (!m_Dialogues.empty() && m_iDialogueIndex < m_Dialogues.size()) 
	{
		m_pGameInstance->Render_Font(TEXT("Font32"), m_Dialogues[m_iDialogueIndex].sz_text, _float2(m_fX - 0.5f * m_fSizeX + 50.f, m_fY - 60.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
	}

	m_pGameInstance->Render_Font(TEXT("Font32"), m_szNpcName, _float2(m_fX - 0.5f * m_fSizeX + 80.f, m_fY -m_fSizeY*0.5f+10.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
}


void  CUI_Dialogue::Set_Dialogues(vector<DialogueInfo>& Dialogues,_uint iNowNPCNo)
{
	m_Dialogues = Dialogues;
	m_iNowNpcNo = iNowNPCNo;
}

void CUI_Dialogue::Clear_Text()
{
	m_Dialogues.clear();
}

void CUI_Dialogue::Reset_Dialogue()
{
	m_isTextEnd = false;
	Clear_Text();
}


void CUI_Dialogue::Set_Active()
{

	m_isTextEnd = false;
	m_bActive = true;
	m_iDialogueIndex = 0;
	CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
	if (nullptr == pTargetCamera)
		return;
	pTargetCamera->Set_Camera_Still();
	

}

void CUI_Dialogue::Set_InActive()
{
	m_bActive = false;	
}

HRESULT CUI_Dialogue::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_BackGround))))
		return E_FAIL;

	return S_OK;
	return S_OK;
}

HRESULT CUI_Dialogue::Ready_ChildUIs()
{
	CUI_Dialogue_Cancle_Button::UI_DIALOGUE_CANCLE_BUTTON_DESC Cancle_Desc = {};
	Cancle_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Cancle_Desc.fSpeedPerSec = 10.f;
	Cancle_Desc.fSizeX = 50.f;
	Cancle_Desc.fSizeY = 50.f;
	Cancle_Desc.fX = m_fX - m_fSizeX * 0.5f + 70.f;
	Cancle_Desc.fY = m_fY + m_fSizeY * 0.5f - 80.f;


	CUI_Dialogue_OK_Button::UI_DIALOGUE_OK_BUTTON_DESC OK_Desc = {};
	OK_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	OK_Desc.fSpeedPerSec = 10.f;
	OK_Desc.fSizeX = 50.f;
	OK_Desc.fSizeY = 50.f;
	OK_Desc.fX = m_fX + m_fSizeX * 0.5f-100.f;
	OK_Desc.fY = m_fY + m_fSizeY * 0.5f -80.f;


	CUIObject* pObject = { nullptr };
	/* For Item Picture*/
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Dialogue_Cancle_Button"), &Cancle_Desc));
	m_ChildUIs.emplace(L"Button_Cancle_Dialogue", pObject);
	
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Dialogue_OK_Button"), &OK_Desc));
	m_ChildUIs.emplace(L"Button_Ok_Dialogue", pObject);
	

	return S_OK;
}

HRESULT CUI_Dialogue::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_BackGround->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;


	return S_OK;
	
}

CUI_Dialogue* CUI_Dialogue::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Dialogue* pInstance = new  CUI_Dialogue(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Dialogue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dialogue::Clone(void* pArg)
{
	CUI_Dialogue* pInstance = new  CUI_Dialogue(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Dialogue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dialogue::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom_BackGround);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
