#include"stdafx.h"
#include"UI_Weapon_Equip_Button.h"
#include"UI_Inventory_WeaponPicture.h"
#include "UI_Inventory_Weapon_Explain.h"
#include"UI_Inventory_Weapon_StatusText.h"

#include"GameInstance.h"
#include"Hands.h"
#include"Player.h"
CUI_Inventory_Weapon_Explain::CUI_Inventory_Weapon_Explain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent{ pDevice, pContext }
	
{
}

CUI_Inventory_Weapon_Explain::CUI_Inventory_Weapon_Explain(const CUI_Inventory_Weapon_Explain& Prototype)
	:CUI_Parent(Prototype)
	, m_mapArmInfo(Prototype.m_mapArmInfo)
{
}

HRESULT CUI_Inventory_Weapon_Explain::Initialize_Prototype()
{
	
	Arm_InfoText Info = { TEXT("Poisonous\n Ghost"),TEXT("The soul of Poisnous Bird, One of the Legend animals,\n is in it. The Legend says those who use it\n get a good job real soon.\nIt has high Demage and Critical Rate \n but low Speed.\n It requires high cost to reinforce.") };
	m_mapArmInfo.emplace(make_pair(CPlayer::PART_HANDS01, Info));

	Info = { TEXT("Thunder\n Gloves"),TEXT("It makes energy balls made of electricity,\n leaving Thunder particles around the monsters.\n It has quite good status")};
	m_mapArmInfo.emplace(make_pair(CPlayer::PART_HANDS02, Info));

	Info = { TEXT("Concealed\n Ammo"),TEXT("A Gun which looks like tiger.\n It makes cloud Particles when it hit monsters.\n make sure to reinforce it.\n it shouldn't cost that much ") };
	m_mapArmInfo.emplace(make_pair(CPlayer::PART_HANDS03, Info));

	Info = { TEXT("Deafening\n Mortar"),TEXT("It makes Rasor giving monsters consistant Demages .\n it has high Critical rate ,but low demage") };
	m_mapArmInfo.emplace(make_pair(CPlayer::PART_HANDS04, Info));

	Info = { TEXT("Hell Fish\n Shooter"),TEXT("A living Gun.It throws up flying Fishes.\n it has high Critical rate and very high demage.\n one of the amazing arms") };
	m_mapArmInfo.emplace(make_pair(CPlayer::PART_HANDS05, Info));

	return S_OK;
}

HRESULT CUI_Inventory_Weapon_Explain::Initialize(void* pArg)
{
	UI_INVENTORY_WEAPON_EXPLAIN_DESC* pDesc = static_cast<UI_INVENTORY_WEAPON_EXPLAIN_DESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_ChildUIs()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;
	mapPlayerArmID.emplace(make_pair( CPlayer::PART_HANDS01,0));
	mapPlayerArmID.emplace(make_pair(CPlayer::PART_HANDS02, 1));
	mapPlayerArmID.emplace(make_pair(CPlayer::PART_HANDS03, 2));
	mapPlayerArmID.emplace(make_pair(CPlayer::PART_HANDS04, 3));
	mapPlayerArmID.emplace(make_pair(CPlayer::PART_HANDS05, 4));

	return S_OK;
}

void CUI_Inventory_Weapon_Explain::Priority_Update(_float fTimeDelta)
{
	
		for (auto& Child : m_ChildUIs)
			Child.second->Priority_Update(fTimeDelta);
	
}

void CUI_Inventory_Weapon_Explain::Update(_float fTimeDelta)
{
	for (auto& Child : m_ChildUIs)
		Child.second->Update(fTimeDelta);



}

void CUI_Inventory_Weapon_Explain::Late_Update(_float fTimeDelta)
{
	
	for (auto& Child : m_ChildUIs)
	{
		Child.second->Late_Update(fTimeDelta);
	}
	
	




	if (m_ChildUIs[L"Button_Equip_Weapon"]->Get_Button())
	{
		CPlayer* pPlayer =static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
		int iCurrentToolIndex = pPlayer->Get_CurrentTool();
		
		_uint iCurToolNo = pPlayer->Get_ToolNoByIndex(iCurrentToolIndex);
		_uint iMyToolNo = pPlayer->Get_ToolNoByIndex(m_iPlayerToolIndex);

		
		pPlayer->Set_ToolNoByIndex(iCurrentToolIndex, iMyToolNo);
		pPlayer->Set_ToolNoByIndex(m_iPlayerToolIndex, iCurToolNo);

		m_eHandsInfo = static_cast<CHands*> (pPlayer->Find_GameObject(pPlayer->Get_ToolNoByIndex(m_iPlayerToolIndex)))->Get_HandsInfo();
		m_iMyArmNo = (pPlayer->Get_ToolNoByIndex(m_iPlayerToolIndex));

		_tchar sz_FireRate[MAX_PATH] = L"";
		_tchar sz_BulletSpeed[MAX_PATH] = L"";
		_tchar sz_Demage[MAX_PATH] = L"";
		_tchar sz_Critical_Rate[MAX_PATH] = L"";
		_tchar sz_Critical_Demage[MAX_PATH] = L"";
		_tchar sz_Load_Capicity[MAX_PATH] = L"";


		wsprintf(sz_FireRate, L"Rate of Fire   :%d", m_eHandsInfo.fFireSpeed);
		wsprintf(sz_BulletSpeed, L"Bullet Speed   :%d", m_eHandsInfo.fBulletSpeed);
		wsprintf(sz_Demage, L"Demage         :%d", m_eHandsInfo.fDemage);
		wsprintf(sz_Critical_Rate, L"Citical Rate   :%d", m_eHandsInfo.fCriticalRate);
		wsprintf(sz_Critical_Demage, L"Critical Demage:%d", m_eHandsInfo.fCriticalDemage);
		wsprintf(sz_Load_Capicity, L"LoadCapicity:%d", m_eHandsInfo.fMaxLoadBullets);


		static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_1"])->Set_Status(sz_FireRate);
		static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_2"])->Set_Status(sz_BulletSpeed);
		static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_3"])->Set_Status(sz_Demage);
		static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_4"])->Set_Status(sz_Critical_Rate);
		static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_5"])->Set_Status(sz_Critical_Demage);
		static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_6"])->Set_Status(sz_Load_Capicity);
		
		pPlayer->Set_State_PopOut();
		static_cast<CUI_Inventory_WeaponPicture*>(m_ChildUIs[L"Picture_Weapon"])->Set_PlayerToolNo(iCurToolNo);

		m_ChildUIs[L"Button_Equip_Weapon"]->Set_Button(false);


		m_iSwappedCard = iCurrentToolIndex;
	}
}

HRESULT CUI_Inventory_Weapon_Explain::Render()
{
	/*WeaponBackGround Rendering*/
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	{
		for (auto& Child : m_ChildUIs)
			Child.second->Render();
	}
	
	if (FAILED(Bind_And_Render_ToolNumber()))
		return E_FAIL;

	{
		/*Text*/
		m_pGameInstance->Render_Font(TEXT("Font32"), m_mapArmInfo[m_iMyArmNo].sz_Name, _float2(m_fX - 0.5f * m_fSizeX + 30.f, m_fY - 0.5 * m_fSizeY + 50.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

		 m_pGameInstance->Render_Font(TEXT("Font10"),m_mapArmInfo[m_iMyArmNo].sz_Feature, _float2(m_fX - 0.5f * m_fSizeX + 30.f, m_fY + 100.f), XMVectorSet(0.f, 1.f, 0.f, 1.f));


	}
	return S_OK;
}

void CUI_Inventory_Weapon_Explain::Set_PlayerToolNo(_uint iPlayerToolNo, _uint iPlayerToolIndex)
{
	m_iPlayerToolIndex = iPlayerToolIndex;
	static_cast<CUI_Inventory_WeaponPicture*>(m_ChildUIs[L"Picture_Weapon"])->Set_PlayerToolNo(iPlayerToolNo);
	
	CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
	CHands* pHands = static_cast<CHands*> (pPlayer->Find_GameObject(pPlayer->Get_ToolNoByIndex(m_iPlayerToolIndex)));
	
	m_eHandsInfo =pHands->Get_HandsInfo();
	m_iMyArmNo = (pPlayer->Get_ToolNoByIndex(m_iPlayerToolIndex));

 	_tchar sz_FireRate[MAX_PATH] = L"";
	_tchar sz_BulletSpeed[MAX_PATH] = L"";
	_tchar sz_Demage[MAX_PATH] = L"";
	_tchar sz_Critical_Rate[MAX_PATH] = L"";
	_tchar sz_Critical_Demage[MAX_PATH] = L"";
	_tchar sz_Load_Capicity[MAX_PATH] = L"";
	
	
	wsprintf(sz_FireRate, L"Rate of Fire   :%d", m_eHandsInfo.fFireSpeed);
	wsprintf(sz_BulletSpeed, L"Bullet Speed   :%d", m_eHandsInfo.fBulletSpeed);
	wsprintf(sz_Demage, L"Demage         :%d", m_eHandsInfo.fDemage);
	wsprintf(sz_Critical_Rate, L"Citical Rate   :%d", m_eHandsInfo.fCriticalRate);
	wsprintf(sz_Critical_Demage, L"Critical Demage:%d", m_eHandsInfo.fCriticalDemage);
	wsprintf(sz_Load_Capicity, L"LoadCapicity :%d", m_eHandsInfo.fMaxLoadBullets);


	static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_1"])->Set_Status(sz_FireRate);
	static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_2"])->Set_Status(sz_BulletSpeed);
	static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_3"])->Set_Status(sz_Demage);
	static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_4"])->Set_Status(sz_Critical_Rate);
	static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_5"])->Set_Status(sz_Critical_Demage);
	static_cast<CUI_Inventory_Weapon_StatusText*>(m_ChildUIs[L"Weapon_Status_6"])->Set_Status(sz_Load_Capicity);




}

HRESULT CUI_Inventory_Weapon_Explain::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_WeaponBox"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Equip"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_ToolNumber))))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Inventory_Weapon_Explain::Ready_ChildUIs()
{
	CUI_Weapon_Equip_Button::UI_BUTTTON_INVENTORY_EQUIP_BUTTON_DESC EquipButtonDesc = {};
	EquipButtonDesc.fRotationPerSec = XMConvertToRadians(90.f);
	EquipButtonDesc.fSpeedPerSec = 10.f;
	EquipButtonDesc.fSizeX = 80;
	EquipButtonDesc.fSizeY = 80;
	EquipButtonDesc.fX = (m_fX + m_fSizeX * 0.5F) -100;
	EquipButtonDesc.fY = (m_fY + m_fSizeY * 0.5F) -100 ;
	EquipButtonDesc.iData = 0;
	
	CUI_Inventory_WeaponPicture::UI_INVENTORY_WEAPON_PICTURE_DESC WeaponPictureDesc = {};
	WeaponPictureDesc.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponPictureDesc.fSpeedPerSec = 10.f;
	WeaponPictureDesc.fSizeX = 160;
	WeaponPictureDesc.fSizeY = 100;
	WeaponPictureDesc.fX = (m_fX - m_fSizeX * 0.5F);
	WeaponPictureDesc.fY = (m_fY - m_fSizeY * 0.5F) +150;
	WeaponPictureDesc.iData = 0;

	CUI_Inventory_Weapon_StatusText::UI_INVENTORY_WEAPON_STATUSTEXT WeaponTextDesc1 = {};
	WeaponTextDesc1.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponTextDesc1.fSpeedPerSec = 10.f;
	WeaponTextDesc1.fSizeX =230 ;
	WeaponTextDesc1.fSizeY =50 ;
	WeaponTextDesc1.fX = (m_fX - m_fSizeX * 0.5F)+150; 
	WeaponTextDesc1.fY = (m_fY - m_fSizeY * 0.5F)+10;

	CUI_Inventory_Weapon_StatusText::UI_INVENTORY_WEAPON_STATUSTEXT WeaponTextDesc2 = {};
	WeaponTextDesc2.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponTextDesc2.fSpeedPerSec = 10.f;
	WeaponTextDesc2.fSizeX = 230;
	WeaponTextDesc2.fSizeY = 50;
	WeaponTextDesc2.fX = (m_fX - m_fSizeX * 0.5F) + 150;
	WeaponTextDesc2.fY = (m_fY - m_fSizeY * 0.5F) + 60;

	CUI_Inventory_Weapon_StatusText::UI_INVENTORY_WEAPON_STATUSTEXT WeaponTextDesc3 = {};
	WeaponTextDesc3.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponTextDesc3.fSpeedPerSec = 10.f;
	WeaponTextDesc3.fSizeX = 230;
	WeaponTextDesc3.fSizeY = 50;
	WeaponTextDesc3.fX = (m_fX - m_fSizeX * 0.5F) + 150;
	WeaponTextDesc3.fY = (m_fY - m_fSizeY * 0.5F) + 110;

	CUI_Inventory_Weapon_StatusText::UI_INVENTORY_WEAPON_STATUSTEXT WeaponTextDesc4 = {};
	WeaponTextDesc4.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponTextDesc4.fSpeedPerSec = 10.f;
	WeaponTextDesc4.fSizeX = 230;
	WeaponTextDesc4.fSizeY = 50;
	WeaponTextDesc4.fX = (m_fX - m_fSizeX * 0.5F) + 150;
	WeaponTextDesc4.fY = (m_fY - m_fSizeY * 0.5F) + 160;

	CUI_Inventory_Weapon_StatusText::UI_INVENTORY_WEAPON_STATUSTEXT WeaponTextDesc5 = {};
	WeaponTextDesc5.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponTextDesc5.fSpeedPerSec = 10.f;
	WeaponTextDesc5.fSizeX = 230;
	WeaponTextDesc5.fSizeY = 50;
	WeaponTextDesc5.fX = (m_fX - m_fSizeX * 0.5F) + 150;
	WeaponTextDesc5.fY = (m_fY - m_fSizeY * 0.5F) + 210; 
	CUI_Inventory_Weapon_StatusText::UI_INVENTORY_WEAPON_STATUSTEXT WeaponTextDesc6 = {};
	WeaponTextDesc6.fRotationPerSec = XMConvertToRadians(90.f);
	WeaponTextDesc6.fSpeedPerSec = 10.f;
	WeaponTextDesc6.fSizeX = 230;
	WeaponTextDesc6.fSizeY = 50;
	WeaponTextDesc6.fX = (m_fX - m_fSizeX * 0.5F) + 150;
	WeaponTextDesc6.fY = (m_fY - m_fSizeY * 0.5F) + 260;



	//Prototype_GameObject_UI_Inventory_StatusText


	CUIObject* pObject = { nullptr };
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapon_Equip_Button"), &EquipButtonDesc));
	m_ChildUIs.emplace(L"Button_Equip_Weapon", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_WeaponPicture"), &WeaponPictureDesc));
	m_ChildUIs.emplace(L"Picture_Weapon", pObject);

	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_StatusText"), &WeaponTextDesc1));
	m_ChildUIs.emplace(L"Weapon_Status_1", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_StatusText"), &WeaponTextDesc2));
	m_ChildUIs.emplace(L"Weapon_Status_2", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_StatusText"), &WeaponTextDesc3));
	m_ChildUIs.emplace(L"Weapon_Status_3", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_StatusText"), &WeaponTextDesc4));
	m_ChildUIs.emplace(L"Weapon_Status_4", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_StatusText"), &WeaponTextDesc5));
	m_ChildUIs.emplace(L"Weapon_Status_5", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_StatusText"), &WeaponTextDesc6));
	m_ChildUIs.emplace(L"Weapon_Status_6", pObject);



	return S_OK;
}

HRESULT CUI_Inventory_Weapon_Explain::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Inventory_Weapon_Explain::Bind_And_Render_ToolNumber()
{
	if (m_iPlayerToolIndex >= 3)
		return S_OK;

	_matrix  matWorldOfToolNumber = m_pTransformCom->Get_WorldMatrix();
	matWorldOfToolNumber.r[0] *= 0.1f;
	matWorldOfToolNumber.r[1] *= 0.1;
	matWorldOfToolNumber.r[3] = XMVectorSetX(matWorldOfToolNumber.r[3], XMVectorGetX(matWorldOfToolNumber.r[3]) - m_fSizeX * 0.5f + 30.f);
	matWorldOfToolNumber.r[3] = XMVectorSetY(matWorldOfToolNumber.r[3], XMVectorGetY(matWorldOfToolNumber.r[3]));

	_float4x4 ToolNumberFloat4x4;
	XMStoreFloat4x4(&ToolNumberFloat4x4, matWorldOfToolNumber);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &ToolNumberFloat4x4)))
		return E_FAIL;

	

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_ToolNumber->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iPlayerToolIndex)))
		return E_FAIL;


	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
	
}

CUI_Inventory_Weapon_Explain* CUI_Inventory_Weapon_Explain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Inventory_Weapon_Explain* pInstance = new  CUI_Inventory_Weapon_Explain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Inventory_Weapon_Explain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Inventory_Weapon_Explain::Clone(void* pArg)
{
	CUI_Inventory_Weapon_Explain* pInstance = new  CUI_Inventory_Weapon_Explain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Inventory_Weapon_Explain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_Weapon_Explain::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom_ToolNumber);
}
