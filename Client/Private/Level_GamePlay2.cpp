#include"stdafx.h"
#include "Level_GamePlay2.h"

#include"GameInstance.h"
#include"Camera_Player.h"
#include"Camera_Free.h"

#include"UI_Arms.h"
#include"UI_DemageFont.h"
#include"UI_Player_HP.h"
#include"UI_MiniMapMain.h"
#include"UI_Inventory.h"
#include"UI_Shop.h"
#include"UI_Coin_And_Bullets.h"
#include"UI_Tab.h"
#include"UI_Dash.h"
#include"UI_Dash_Screen.h"
#include"UI_Bomb.h"
#include"UI_Victory.h"

#include"Player.h"
#include "Terrain.h"
#include"Monster.h"
#include"Monster09_Main.h"

CLevel_GamePlay2::CLevel_GamePlay2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CLevel{pDevice, pContext}
{
}

HRESULT CLevel_GamePlay2::Initialize()
{

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	/*player가 UI 참조 -> 카메라가 플레이어 참조   -> UI -> 플레이어 -> 카메라 */  /* 업데이트 보장되어야 할 순서  플레이어 UPDATE-> 카메라 UPDATE*/
	if (FAILED(Ready_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Player(TEXT("Layer_APlayer"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camara"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_GameObject(TEXT("Layer_GameObject"))))
		return E_FAIL;

	if (FAILED(Ready_Map(TEXT("Layer_Map"))))
		return E_FAIL;
	
	if (FAILED(Ready_Monster(TEXT("Layer_Monster"))))
		return E_FAIL; 


	m_pGameInstance->PlayBGM(L"Bgm_BeforeBoss.mp3",0.1f);
	


	/*if (FAILED(Ready_Particle(TEXT("Layer_Particle"))))
		return E_FAIL;*/

    return S_OK;
}

void CLevel_GamePlay2::Update(_float fTimeDelta)
{
	return ;
}

HRESULT CLevel_GamePlay2::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("Level_GamePlay2"));
#endif

    return S_OK;
}

HRESULT CLevel_GamePlay2::Ready_ShadowCamera()
{
	m_pGameInstance->SetUp_Shadow_TransformMatrix(CShadow::D3DTS_VIEW, XMMatrixLookAtLH(XMVectorSet(-10.f, 10.f, -10.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	m_pGameInstance->SetUp_Shadow_TransformMatrix(CShadow::D3DTS_PROJ, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.f), static_cast<_float>(g_iWinSizeX) / g_iWinSizeY, 0.1f, 1000.f));

	return S_OK;
}

HRESULT CLevel_GamePlay2::Ready_Layer_Camera(const _wstring& strLayerTag)
{

	/*CCamera_Free::CAMERA_FREE_DESC		Desc{};

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(180.f);
	Desc.fMouseSensor = 0.1f;
	Desc.fFovy = XMConvertToRadians(60.f);
	Desc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.vEye = _float3(0.f, 10.f, -7.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_GAMEPLAY, strLayerTag, &Desc)))
		return E_FAIL;*/

	CCamera_Player::CAMERA_PLAYER_DESC Desc_Player{};
	Desc_Player.fSpeedPerSec = 10.f;
	Desc_Player.fRotationPerSec = XMConvertToRadians(90.f);
	Desc_Player.fMouseSensor = 0.1f;
	Desc_Player.fFovy = XMConvertToRadians(60.f);
	Desc_Player.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	Desc_Player.fNear = 0.1f;
	Desc_Player.fFar = 1000.f;
	Desc_Player.vEye = _float3(0.f, 0.f, -7.f);
	Desc_Player.vAt = _float3(0.f, 0.f, 0.f);
	Desc_Player.pPlayerFloat4x4 = m_pPlayerFloat4x4;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Camera_Player"),
		LEVEL_GAMEPLAY2, strLayerTag, &Desc_Player)))
		return E_FAIL;


	return S_OK;

    return S_OK;
}

HRESULT CLevel_GamePlay2::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    return S_OK;
}

HRESULT CLevel_GamePlay2::Ready_UI(const _wstring& strLayerTag)
{

	CUIObject* pUIObject = { nullptr };
	CUIObject::UIOBJECT_DESC UI_Desc = {};

	/*For Mouse*/
	{
		UI_Desc.eType = UI_TYPE::UI_END;
		UI_Desc.fRotationPerSec = 10.f;
		UI_Desc.fSpeedPerSec = XMConvertToRadians(90.f);
		UI_Desc.fSizeX = { 20 };
		UI_Desc.fSizeY = { 20 };
		UI_Desc.fX = { g_iWinSizeX << 1 };
		UI_Desc.fY = { g_iWinSizeY << 1 };
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Mouse"),
			LEVEL_GAMEPLAY2, strLayerTag, &UI_Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		pUIObject->Set_SortDepth(1000);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Mouse"), pUIObject, UI_TYPE::UI_END);
	}
	/*For Arms*/
	{
		CUI_Arms::UI_ARMS_DESC Arms_Desc = {};
		Arms_Desc.eType = UI_TYPE::UI_END;
		Arms_Desc.fRotationPerSec = 10.f;
		Arms_Desc.fSpeedPerSec = XMConvertToRadians(90.f);
		Arms_Desc.fSizeX = { 400 };
		Arms_Desc.fSizeY = { 300 };
		Arms_Desc.fX = { 760 };
		Arms_Desc.fY = { 430 };
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arms"),
			LEVEL_GAMEPLAY2, strLayerTag, &Arms_Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		pUIObject->Set_SortDepth(1000);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Arms"), pUIObject, UI_TYPE::UI_FIXED);
	}

	//Prototype_GameObject_UI_DemageFont;
	{
		CUI_DemageFont::UI_DEMAGE_FONT_DESC DemageFont_Desc = {};
		DemageFont_Desc.fSizeX = 10.f;
		DemageFont_Desc.fSizeY = 10.f;
		DemageFont_Desc.eType = UI_TYPE::UI_MONSTER;
		DemageFont_Desc.fX = 0.f;
		DemageFont_Desc.fY = 0.f;
		DemageFont_Desc.fRotationPerSec = XMConvertToRadians(90.f);
		DemageFont_Desc.fSpeedPerSec = 10.f;
		DemageFont_Desc.iData = 0.f;
		m_pGameInstance->Reset_NumDemageFonts();
		for (int i = 0; i < 50; ++i)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_DemageFont"),
				LEVEL_GAMEPLAY2, strLayerTag, &DemageFont_Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
				return E_FAIL;
			pUIObject->Set_SortDepth(9);
			m_pGameInstance->Add_DemageFonts(pUIObject);

		}
	}
	/*For PlayerHpBar*/

	CUI_Player_HP::UI_PLAYER_HP_DESC PlayerHPDesc = {};
	PlayerHPDesc.eType = UI_TYPE::UI_FIXED;
	PlayerHPDesc.fRotationPerSec = XMConvertToRadians(90.f);
	PlayerHPDesc.fSizeX = 1;
	PlayerHPDesc.fSizeY = 1;
	PlayerHPDesc.fX = 200.f;
	PlayerHPDesc.fY = 650.f;
	PlayerHPDesc.fSpeedPerSec = 10.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Player_HP"),
		LEVEL_GAMEPLAY2, strLayerTag, &PlayerHPDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
		return E_FAIL;
	pUIObject->Set_Active();
	pUIObject->Set_SortDepth(1000);
	m_pGameInstance->Add_UI_To_UIMap(TEXT("PlayerHP"), pUIObject, UI_TYPE::UI_FIXED);


	CUI_MiniMapMain::UI_MAIN_MAP_DESC  MiniMapDesc = {};
	MiniMapDesc.eStageLevel = LEVEL_GAMEPLAY2;
	MiniMapDesc.fRange = 100;
	MiniMapDesc.fSizeX = 200;
	MiniMapDesc.fSizeY = 200;
	MiniMapDesc.fX = 10;
	MiniMapDesc.fY = 10;
	MiniMapDesc.fRotationPerSec = XMConvertToRadians(90.f);
	MiniMapDesc.fSpeedPerSec = 10.f;
	MiniMapDesc.iData = 0.f;
	MiniMapDesc.eType = UI_TYPE::UI_FIXED;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MiniMap_Main"),
		LEVEL_GAMEPLAY2, strLayerTag, &MiniMapDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
		return E_FAIL;
	pUIObject->Set_Active();
	pUIObject->Set_SortDepth(1000);
	m_pGameInstance->Add_UI_To_UIMap(TEXT("MiniMap"), pUIObject, UI_TYPE::UI_FIXED);
	static_cast<CUI_MiniMapMain*>(m_pGameInstance->Get_UI_By_Tag(TEXT("MiniMap"))) ->Set_Vector(nullptr);

	CUI_MiniMapMain* pUIMiniMap = static_cast<CUI_MiniMapMain*>(m_pGameInstance->Get_UI_By_Tag(L"MiniMap"));
	pUIMiniMap->Set_szLevelName(L"Boss Stage");
    
	/*For Inventory */
	{
		//1280 720
			CUI_Inventory::UI_INVENTORY_DESC Inventory_Desc = {};
			Inventory_Desc.eType = UI_TYPE::UI_POP;
			Inventory_Desc.fRotationPerSec = XMConvertToRadians(90.f);
			Inventory_Desc.fSizeX = 1270;
			Inventory_Desc.fSizeY = 710;
			Inventory_Desc.fX = 5.f;
			Inventory_Desc.fY = 5.f;
			Inventory_Desc.fSpeedPerSec = 10.f;
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory"),
				LEVEL_GAMEPLAY, strLayerTag, &Inventory_Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
				return E_FAIL;
			pUIObject->Set_InActive();
			pUIObject->Set_SortDepth(10);
			m_pGameInstance->Add_UI_To_UIMap(TEXT("Inventory"), pUIObject, UI_TYPE::UI_FIXED);

		

	}
	
	/*For Shop */
	{//1280 720
		CUI_Shop::UI_SHOP_DESC Shop_Desc = {};
		Shop_Desc.eType = UI_TYPE::UI_POP;
		Shop_Desc.fRotationPerSec = XMConvertToRadians(90.f);
		Shop_Desc.fSizeX = 1270;
		Shop_Desc.fSizeY = 710;
		Shop_Desc.fX = 5.f;
		Shop_Desc.fY = 5.f;
		Shop_Desc.fSpeedPerSec = 10.f;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop"),
			LEVEL_GAMEPLAY2, strLayerTag, &Shop_Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(11);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Shop"), pUIObject, UI_TYPE::UI_FIXED);

	}

	/*For Properties */
	{//1280 720
		CUI_Coin_And_Bullets::UI_COIN_AND_BULLET_DESC PropertiesDesc = {};
		PropertiesDesc.eType = UI_TYPE::UI_POP;
		PropertiesDesc.fRotationPerSec = XMConvertToRadians(90.f);
		PropertiesDesc.fSizeX = 400;
		PropertiesDesc.fSizeY = 400;
		PropertiesDesc.fX = 900;
		PropertiesDesc.fY = 30;
		PropertiesDesc.fSpeedPerSec = 10.f;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Properties"),
			LEVEL_GAMEPLAY2, strLayerTag, &PropertiesDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(10);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Properties"), pUIObject, UI_TYPE::UI_FIXED);

	}

	/*Tab*/
	{
		CUI_Tab::UI_TAB_DESC  TabDesc = {};
		TabDesc.eType = UI_TYPE::UI_FIXED;
		TabDesc.fRotationPerSec = XMConvertToRadians(90.f);
		TabDesc.fSizeX = 1;
		TabDesc.fSizeY = 1;
		TabDesc.fX = 150.f;
		TabDesc.fY = 560.f;
		TabDesc.fSpeedPerSec = 10.f;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Tab"),
			LEVEL_GAMEPLAY, strLayerTag, &TabDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		pUIObject->Set_SortDepth(1000);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Tab"), pUIObject, UI_TYPE::UI_FIXED);
	}
	/*Bomb*/
	{
		CUI_Bomb::UI_BOMB_DESC  BombDesc = {};
		BombDesc.eType = UI_TYPE::UI_FIXED;
		BombDesc.fRotationPerSec = XMConvertToRadians(90.f);
		BombDesc.fSizeX = 1;
		BombDesc.fSizeY = 1;
		BombDesc.fX = 1000.f;
		BombDesc.fY = 680.f;
		BombDesc.fSpeedPerSec = 10.f;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Bomb"),
			LEVEL_GAMEPLAY, strLayerTag, &BombDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		pUIObject->Set_SortDepth(1000);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Bomb"), pUIObject, UI_TYPE::UI_FIXED);
	}
	/*Dash*/
	{
		CUI_Dash::UI_DASH_DESC  DashDesc = {};
		DashDesc.eType = UI_TYPE::UI_FIXED;
		DashDesc.fRotationPerSec = XMConvertToRadians(90.f);
		DashDesc.fSizeX = 1;
		DashDesc.fSizeY = 1;
		DashDesc.fX = 910.f;
		DashDesc.fY = 680.f;
		DashDesc.fSpeedPerSec = 10.f;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Dash"),
			LEVEL_GAMEPLAY, strLayerTag, &DashDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		pUIObject->Set_SortDepth(1000);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Dash"), pUIObject, UI_TYPE::UI_FIXED);
	}
	/*Dash_Screen*/
	{
		CUI_Dash_Screen::UI_DASH_SCREEN_DESC  DashScreenDesc = {};
		DashScreenDesc.eType = UI_TYPE::UI_FIXED;
		DashScreenDesc.fRotationPerSec = XMConvertToRadians(90.f);
		DashScreenDesc.fSizeX = g_iWinSizeX;
		DashScreenDesc.fSizeY = g_iWinSizeY;
		DashScreenDesc.fX = 0.f;
		DashScreenDesc.fY = 0.f;
		DashScreenDesc.fSpeedPerSec = 10.f;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Dash_Screen"),
			LEVEL_GAMEPLAY, strLayerTag, &DashScreenDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(9);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Dash_Screen"), pUIObject, UI_TYPE::UI_POP);
	}
	/*Victory*/
	{
		CUI_Victory::UI_VICTORY_DESC VictoryDesc = {};
		VictoryDesc.eType = UI_TYPE::UI_POP;
		VictoryDesc.fRotationPerSec = XMConvertToRadians(90.f);
		VictoryDesc.fSpeedPerSec = 10.f;
		VictoryDesc.fSizeX = g_iWinSizeX;
		VictoryDesc.fSizeY = 300;
		VictoryDesc.fX = 0.f;
		VictoryDesc.fY = 200.f;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Victory"),
			LEVEL_GAMEPLAY, strLayerTag, &VictoryDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(10);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Victory"), pUIObject, UI_TYPE::UI_POP);
	}
	return S_OK;
}

HRESULT CLevel_GamePlay2::Ready_Layer_GameObject(const _wstring& strLayerTag)
{
    return S_OK;
}

HRESULT CLevel_GamePlay2::Ready_Lights()
{

	m_pGameInstance->Set_LightsLevel(LEVEL_GAMEPLAY2);
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(10.f, 3.f, 10.f, 0.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(0.f, 0.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(20.f, 3.f, 10.f, 0.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(0.f, 0.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.5f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay2::Ready_Monster(const _wstring& strLayerTag)
{
	CMonster09_Main::MONSTER09_MAIN_DESC desc = {};
	desc.fRotationPerSec = 10;
	desc.vPosition = { 2.f,0.f,200.f,1.f };
	desc.iNaviIndex = 0.f;
	desc.vScale = { 1.f,1.f,1.f };
	desc.fSpeedPerSec = 10.f;
	desc.fAngle = XMConvertToRadians(180);
	desc.fRotationPerSec = XMConvertToRadians(90.f);
	desc.fDomainX = 40.f;
	desc.fDomainZ = 40.f;
	desc.fHPInfo = {280000, 280000 };
	desc.pMonsters = &m_pMonsters;
	CMonster* pMonster = nullptr;
	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY2, L"Prototype_GameObject_Monster09", LEVEL_GAMEPLAY2, strLayerTag,
		&desc,
		reinterpret_cast<CGameObject**>(&pMonster))
	))
		return E_FAIL;
	pMonster->Set_PlayerFloat4x4(m_pPlayerFloat4x4);
 
	m_pBoss = pMonster;*/
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY2, L"Prototype_GameObject_Monster09_Main", LEVEL_GAMEPLAY2, strLayerTag,
		&desc,reinterpret_cast<CGameObject**>(&pMonster))
	))
		return E_FAIL;
	pMonster->Set_PlayerFloat4x4(m_pPlayerFloat4x4);

	m_pBoss = pMonster;
	static_cast<CUI_MiniMapMain*>(m_pGameInstance->Get_UI_By_Tag(TEXT("MiniMap")))->Set_Vector(&m_pMonsters);
	return S_OK;


  
}

HRESULT CLevel_GamePlay2::Ready_Map(const _wstring& strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Terrain_Stage02"), LEVEL_GAMEPLAY2, strLayerTag, nullptr)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY2, strLayerTag, nullptr)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Water_Stage02"), LEVEL_GAMEPLAY2, strLayerTag, nullptr)))
		return E_FAIL;

 	

	return S_OK;
}

HRESULT CLevel_GamePlay2::Ready_Player(const _wstring& strLayerTag)
{
    
	CPlayer::PLAYER_DESC Desc = {};
	Desc.fInitial_Height = 2.f;
	Desc.InitialPos = { 4.551f,3.1286f,-0.199f,1.f };
	Desc.iNaviMeshIndex = 127;
	Desc.iNowMapStage = 2;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY2, strLayerTag, &Desc, reinterpret_cast<CGameObject**>(&m_pPlayer))))
		return E_FAIL;
	if (nullptr != m_pPlayer)
	{
		m_pPlayerFloat4x4 = static_cast<CPlayer*>(m_pPlayer)->Get_PlayerFloat4x4_ptr();
	}

	return S_OK;
	
}

HRESULT CLevel_GamePlay2::Ready_Particle(const _wstring& strLayerTag)
{
    return S_OK;
}

HRESULT CLevel_GamePlay2::Read_Monsters_Desc_From_File()
{
    return S_OK;
}

HRESULT CLevel_GamePlay2::Read_Map_Desc_From_File()
{
    return S_OK;
}

HRESULT CLevel_GamePlay2::Load_Objects_From_File()
{
    return S_OK;
}

CLevel_GamePlay2* CLevel_GamePlay2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay2* pInstance = new CLevel_GamePlay2(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay2");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLevel_GamePlay2::Free()
{
	/*for (auto& Monster : m_vecMonsters)
	{
		Safe_Release(Monster);
	}
	m_vecMonsters.clear();*/
	Safe_Release(m_pBoss);

	

	Safe_Release(m_pPlayer);

	__super::Free();
}
