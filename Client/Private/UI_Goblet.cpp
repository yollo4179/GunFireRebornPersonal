#include"stdafx.h"
#include"UI_Goblet.h"
#include"UI_Goblet_RandomOptionButton.h"
#include"GameInstance.h"
#include"Camera_Player.h"
CUI_Goblet::CUI_Goblet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent{ pDevice, pContext }
{
}

CUI_Goblet::CUI_Goblet(const CUI_Goblet& Prototype)
	:CUI_Parent(Prototype)
{
}

HRESULT CUI_Goblet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Goblet::Initialize(void* pArg)
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

void CUI_Goblet::Priority_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
	/*CUI_Goblet_RandomOptionButton* pButton = {};
	pButton = static_cast<CUI_Goblet_RandomOptionButton*>(m_ChildUIs[L"Button1"]);*/
	if(m_ChildUIs[L"Button1"]->Get_Button())
	{
		m_ChildUIs[L"Button1"]->Set_Button(false);
		Set_InActive();
	}
	else if (m_ChildUIs[L"Button2"]->Get_Button())
	{
		m_ChildUIs[L"Button2"]->Set_Button(false);
		Set_InActive();
	}


	for (auto& Child : m_ChildUIs)
		Child.second->Priority_Update(fTimeDelta);
	
}

void CUI_Goblet::Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	for (auto& Child : m_ChildUIs)
		Child.second->Update(fTimeDelta);
}

void CUI_Goblet::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Goblet::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	for (auto& Child : m_ChildUIs)
		Child.second ->Render();


	return S_OK;
}

void CUI_Goblet::Reset_RandomGoblets()
{
	vector<_uint> GobletIndexes;
	for (int i = 0; i < g_iNumGoblets; ++i)
	{
		GobletIndexes.push_back(i);
	}

	for(int i=0;i<10;++i)
		for (int j = 0; j < 10; ++j)
		{
			int iIndex1 = rand() % (g_iNumGoblets);
			int iIndex2 = rand() % (g_iNumGoblets);

			_uint temp = GobletIndexes[iIndex1];
			GobletIndexes[iIndex1] = GobletIndexes[iIndex2];
			GobletIndexes[iIndex2] = temp;

		}
	m_iGobletIndex[0] = GobletIndexes[0];
	m_iGobletIndex[1] = GobletIndexes[1];

}

void CUI_Goblet::Set_Active()
{
	m_bActive = true;
	Reset_RandomGoblets();

	static_cast<CUI_Goblet_RandomOptionButton*> (m_ChildUIs[L"Button1"])->Set_GobletType(m_iGobletIndex[0]);
	static_cast<CUI_Goblet_RandomOptionButton*> (m_ChildUIs[L"Button2"])->Set_GobletType(m_iGobletIndex[1]);

	


}

void CUI_Goblet::Set_InActive()
{
	m_bActive = false;
	CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
	if (nullptr == pTargetCamera)
		return;
	pTargetCamera->Set_Camera_Player();
}

HRESULT CUI_Goblet::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Title"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_BackGround))))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Goblet::Ready_ChildUIs()
{
	CUI_Goblet_RandomOptionButton::UI_GOBLET_RANDOM_OPTION_BUTTON_DESC ButtonDesc1 = {};

	ButtonDesc1.fRotationPerSec = XMConvertToRadians(90.f);
	ButtonDesc1.fSpeedPerSec = 10.f;
	ButtonDesc1.fSizeX =m_fSizeX*0.5f-200.f;
	ButtonDesc1.fSizeY = m_fSizeY*0.5f+150.f;
	ButtonDesc1.fX = m_fX - 0.5 * m_fSizeX +50;
	ButtonDesc1.fY = m_fY - 0.5 * m_fSizeY + 100.f;


	CUI_Goblet_RandomOptionButton::UI_GOBLET_RANDOM_OPTION_BUTTON_DESC ButtonDesc2 = {};

	ButtonDesc2.fRotationPerSec = XMConvertToRadians(90.f);
	ButtonDesc2.fSpeedPerSec = 10.f;
	ButtonDesc2.fSizeX = m_fSizeX * 0.5f -200.f;
	ButtonDesc2.fSizeY = m_fSizeY * 0.5f+150.f;
	ButtonDesc2.fX = m_fX + 0.5 * m_fSizeX - 550.f;
	ButtonDesc2.fY = m_fY - 0.5 * m_fSizeY + 100.f;

		CUIObject* pUIObject = { nullptr };
	pUIObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Goblet_Button"), &ButtonDesc1));
	m_ChildUIs.emplace(L"Button1", pUIObject);

	pUIObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Goblet_Button"), &ButtonDesc2));
	m_ChildUIs.emplace(L"Button2", pUIObject);
	return S_OK;


	return S_OK;
}

HRESULT CUI_Goblet::Bind_ShaderResources()
{

	_matrix  matWorldOfTitle = m_pTransformCom->Get_WorldMatrix();
	matWorldOfTitle.r[0] = { 200.f,0.f,0.f,0.f };
	matWorldOfTitle.r[1] = { 0.f,100.f,0.f,0.f };
	matWorldOfTitle.r[3] = XMVectorSetX(matWorldOfTitle.r[3], XMVectorGetX(matWorldOfTitle.r[3])-30.f);
	matWorldOfTitle.r[3] = XMVectorSetY(matWorldOfTitle.r[3], XMVectorGetY(matWorldOfTitle.r[3]) + m_fSizeY * 0.5f - 80.f); 

	_float4x4 TitleFloat4x4;
	XMStoreFloat4x4(&TitleFloat4x4, matWorldOfTitle);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &TitleFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_BackGround->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_Goblet* CUI_Goblet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Goblet* pInstance = new  CUI_Goblet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Goblet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Goblet::Clone(void* pArg)
{
	CUI_Goblet* pInstance = new  CUI_Goblet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Goblet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Goblet::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom_BackGround);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
