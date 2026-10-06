#include"stdafx.h"
#include"UI_Inventory_Weapons.h"
#include"UI_Inventory_Weapon_Move_Button.h"
#include"UI_Inventory_Weapon_Explain.h"
#include"Player.h"
#include"GameInstance.h"
CUI_Inventory_Weapons::CUI_Inventory_Weapons(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent{ pDevice, pContext }
{
}

CUI_Inventory_Weapons::CUI_Inventory_Weapons(const CUI_Inventory_Weapons& Prototype)
	:CUI_Parent(Prototype)
{
}

HRESULT CUI_Inventory_Weapons::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Inventory_Weapons::Initialize(void* pArg)
{
	UI_INVENTORY_WEAPON_DESC* pDesc = static_cast<UI_INVENTORY_WEAPON_DESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_ChildUIs()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;


	return S_OK;
}

void CUI_Inventory_Weapons::Priority_Update(_float fTimeDelta)
{
		for (auto& Child : m_ChildUIs)
			Child.second->Priority_Update(fTimeDelta);
	
}

void CUI_Inventory_Weapons::Update(_float fTimeDelta)
{
	
		for (auto& Child : m_ChildUIs)
			Child.second->Update(fTimeDelta);
	
}

void CUI_Inventory_Weapons::Late_Update(_float fTimeDelta)
{
	if (m_ChildUIs[L"Button_Right_Weapon"]->Get_Button())
	{
		if (m_iMaxWeaponBoxIndex < g_iNumTools-2 )
		{
			m_iMinWeaponBoxIndex += 1;
			m_iMaxWeaponBoxIndex += 1;

			_uint* pToolArray = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0))->Get_ToolInfo();
		
			static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_1"])->Set_PlayerToolNo(pToolArray[m_iMinWeaponBoxIndex], m_iMinWeaponBoxIndex);
			static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_2"])->Set_PlayerToolNo(pToolArray[m_iMinWeaponBoxIndex + 1], (m_iMinWeaponBoxIndex + 1));
			static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_3"])->Set_PlayerToolNo(pToolArray[m_iMaxWeaponBoxIndex], m_iMaxWeaponBoxIndex);
			
		}
		m_ChildUIs[L"Button_Right_Weapon"]->Set_Button(false);
	}
	else if (m_ChildUIs[L"Button_Left_Weapon"]->Get_Button())
	{
		if (m_iMinWeaponBoxIndex > 0)
		{
			m_iMinWeaponBoxIndex -= 1;
			m_iMaxWeaponBoxIndex -= 1;
		}
		_uint* pToolArray = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0))->Get_ToolInfo();

		static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_1"])->Set_PlayerToolNo(pToolArray[m_iMinWeaponBoxIndex], m_iMinWeaponBoxIndex);
		static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_2"])->Set_PlayerToolNo(pToolArray[m_iMinWeaponBoxIndex +1], (m_iMinWeaponBoxIndex + 1));
		static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_3"])->Set_PlayerToolNo(pToolArray[m_iMaxWeaponBoxIndex], m_iMaxWeaponBoxIndex);
		m_ChildUIs[L"Button_Left_Weapon"]->Set_Button(false);
	}


	for (auto& Child : m_ChildUIs)
		Child.second->Late_Update(fTimeDelta);

	_uint iSwappedCard = -1;
	CUI_Inventory_Weapon_Explain* pTemp = nullptr;
	_uint* pToolArray = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0))->Get_ToolInfo();

	if (-1!=(iSwappedCard=static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_1"])->Get_SwappedCard()))
	{
		pTemp = static_cast<CUI_Inventory_Weapon_Explain*> (m_ChildUIs[L"Weapon_Explain_1"]);
		pTemp->Reset_SwappedCard();
		

		if (iSwappedCard == m_iMinWeaponBoxIndex + 1)
		{
			static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_2"])->Set_PlayerToolNo(pToolArray[m_iMinWeaponBoxIndex + 1], m_iMinWeaponBoxIndex + 1);
		}
		if (iSwappedCard == m_iMaxWeaponBoxIndex)
		{
			static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_3"])->Set_PlayerToolNo(pToolArray[m_iMaxWeaponBoxIndex], m_iMaxWeaponBoxIndex);
		}
	}
	else if (-1!=(iSwappedCard=static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_2"])->Get_SwappedCard()))
	{
		pTemp = static_cast<CUI_Inventory_Weapon_Explain*> (m_ChildUIs[L"Weapon_Explain_2"]);
		pTemp->Reset_SwappedCard();
		
		if (iSwappedCard == m_iMinWeaponBoxIndex)
		{
			static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_1"])->Set_PlayerToolNo(pToolArray[m_iMinWeaponBoxIndex], m_iMinWeaponBoxIndex);
		}
		if (iSwappedCard == m_iMaxWeaponBoxIndex)
		{
			static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_3"])->Set_PlayerToolNo(pToolArray[m_iMaxWeaponBoxIndex], m_iMaxWeaponBoxIndex);
		}

	}
	else if (-1!=(iSwappedCard=static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_3"])->Get_SwappedCard()))
	{
		pTemp = static_cast<CUI_Inventory_Weapon_Explain*> (m_ChildUIs[L"Weapon_Explain_3"]);
		pTemp->Reset_SwappedCard();
	
		if (iSwappedCard == m_iMinWeaponBoxIndex)
		{
			static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_1"])->Set_PlayerToolNo(pToolArray[m_iMinWeaponBoxIndex], m_iMinWeaponBoxIndex);
		}
		if (iSwappedCard == m_iMinWeaponBoxIndex + 1)
		{
			static_cast<CUI_Inventory_Weapon_Explain*>(m_ChildUIs[L"Weapon_Explain_2"])->Set_PlayerToolNo(pToolArray[m_iMinWeaponBoxIndex + 1], m_iMinWeaponBoxIndex + 1);
		}
	}
	
	

	


}

HRESULT CUI_Inventory_Weapons::Render()
{


	//m_ChildUIs[L"Button_Right_Weapon"]->Render();
	//m_ChildUIs[L"Button_Left_Weapon"]->Render();
	for (auto& Child : m_ChildUIs)
	{
		(Child).second->Render();
	}
	return S_OK;
	
}

HRESULT CUI_Inventory_Weapons::Ready_Components()
{
	return S_OK;;
}

HRESULT CUI_Inventory_Weapons::Ready_ChildUIs()
{

	CUI_Inventory_Weapon_Move_Button::UI_BUTTTON_INVENTORY_WEAPON_MOVE_BUTTON_DESC WeaponMoveButtonRightDesc = {};
	WeaponMoveButtonRightDesc.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponMoveButtonRightDesc.fSpeedPerSec = 10.f;
	WeaponMoveButtonRightDesc.fSizeX = 100;
	WeaponMoveButtonRightDesc.fSizeY = 100;
	WeaponMoveButtonRightDesc.fX = m_fX  + 500;
	WeaponMoveButtonRightDesc.fY = (m_fY + m_fSizeY)-170.f ;
	WeaponMoveButtonRightDesc.iData = 0;
	WeaponMoveButtonRightDesc.iButton_Dir = 0;

	CUI_Inventory_Weapon_Move_Button::UI_BUTTTON_INVENTORY_WEAPON_MOVE_BUTTON_DESC WeaponMoveButtonLeftDesc = {};
	WeaponMoveButtonLeftDesc.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponMoveButtonLeftDesc.fSpeedPerSec = 10.f;
	WeaponMoveButtonLeftDesc.fSizeX = 100;
	WeaponMoveButtonLeftDesc.fSizeY = 100;
	WeaponMoveButtonLeftDesc.fX = m_fX  - 600;
	WeaponMoveButtonLeftDesc.fY = (m_fY + m_fSizeY)-170.f;
	WeaponMoveButtonLeftDesc.iData = 0;
	WeaponMoveButtonLeftDesc.iButton_Dir = 1;


	CUI_Inventory_Weapon_Explain::UI_INVENTORY_WEAPON_EXPLAIN_DESC WeaponExplainDesc1 = {};
	WeaponExplainDesc1.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponExplainDesc1.fSpeedPerSec = 10.f;
	WeaponExplainDesc1.fSizeX = 400;
	WeaponExplainDesc1.fSizeY = 500;
	WeaponExplainDesc1.fX = m_fX - 600-20.f;
	WeaponExplainDesc1.fY = (m_fY -80.f);
	WeaponExplainDesc1.iData = 0;
	

	CUI_Inventory_Weapon_Explain::UI_INVENTORY_WEAPON_EXPLAIN_DESC WeaponExplainDesc2 = {};
	WeaponExplainDesc2.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponExplainDesc2.fSpeedPerSec = 10.f;
	WeaponExplainDesc2.fSizeX = 400;
	WeaponExplainDesc2.fSizeY = 500;
	WeaponExplainDesc2.fX = m_fX - 150.f-30.f-20.f;
	WeaponExplainDesc2.fY = m_fY -80.f;
	WeaponExplainDesc2.iData = 0;
	

	CUI_Inventory_Weapon_Explain::UI_INVENTORY_WEAPON_EXPLAIN_DESC WeaponExplainDesc3 = {};
	WeaponExplainDesc3.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponExplainDesc3.fSpeedPerSec = 10.f;
	WeaponExplainDesc3.fSizeX = 400;
	WeaponExplainDesc3.fSizeY = 500;
	WeaponExplainDesc3.fX = m_fX +300.f-60.f-20.f;
	WeaponExplainDesc3.fY = (m_fY -80.f);
	WeaponExplainDesc3.iData = 0;

	CUIObject* pObject = { nullptr };
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapon_Move_Button"), &WeaponMoveButtonRightDesc));
	m_ChildUIs.emplace(L"Button_Right_Weapon", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapon_Move_Button"), &WeaponMoveButtonLeftDesc));
	m_ChildUIs.emplace(L"Button_Left_Weapon", pObject);

	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapon_Explain"), &WeaponExplainDesc1));
	m_ChildUIs.emplace(L"Weapon_Explain_1", pObject);

	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapon_Explain"), &WeaponExplainDesc2));
	m_ChildUIs.emplace(L"Weapon_Explain_2", pObject);

	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapon_Explain"), &WeaponExplainDesc3));
	m_ChildUIs.emplace(L"Weapon_Explain_3", pObject);





	return S_OK;
}

HRESULT CUI_Inventory_Weapons::Bind_ShaderResources()
{
	return S_OK;
}

CUI_Inventory_Weapons* CUI_Inventory_Weapons::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Inventory_Weapons* pInstance = new  CUI_Inventory_Weapons(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Inventory_Weapons");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Inventory_Weapons::Clone(void* pArg)
{
	CUI_Inventory_Weapons* pInstance = new  CUI_Inventory_Weapons(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Inventory_Weapons");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_Weapons::Free()
{
	__super::Free();
}
