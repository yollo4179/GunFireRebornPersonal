#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"

#include "Camera_Free.h"
#include"Camera_Player.h"
#include"UI_Arms.h"
#include"UI_DemageFont.h"
#include"UI_MiniMapMain.h"
#include"UI_Player_HP.h"
#include"UI_Inventory.h"
#include"UI_Shop.h"
#include"UI_Coin_And_Bullets.h"
#include"UI_Talk.h"
#include"UI_Dialogue.h"
#include"UI_Goblet.h"
#include"UI_Tab.h"
#include"UI_Bomb.h"
#include"UI_Dash.h"
#include"UI_Dash_Screen.h"
#include"UI_Victory.h"

//#include"CubeCollider.h"
#include"Terrain.h"
#include"Player.h"
/*NextStage*/
#include"Level_Loading.h"
/*Client Manager*/
#include"Item_Manager.h"

#include"DropItem_Money.h"
#include"DropItem_Grenade.h"
#include"DropItem_Bullet_Special.h"
#include"DropItem_Bullet_Standard.h"

#include"Npc_Smith.h"



CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
	, m_pItem_Manager{ CItem_Manager::GetInstance() }
{
	Safe_AddRef(m_pItem_Manager);
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;
	
	/*player가 UI 참조 -> 카메라가 플레이어 참조   -> UI -> 플레이어 -> 카메라 */  /* 업데이트 보장되어야 할 순서  플레이어 UPDATE-> 카메라 UPDATE*/
	if (FAILED(Ready_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Player(TEXT("Layer_APlayer"))))
		return E_FAIL;
	
	if (FAILED(Ready_ShadowCamera()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camara"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_GameObject(TEXT("Layer_GameObject"))))
		return E_FAIL;

	if (FAILED(Ready_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;
	
	

 	if (FAILED(Ready_Map(TEXT("Layer_Map"))))
		return E_FAIL;


	if (FAILED(Ready_Particle(TEXT("Layer_Particle"))))
		return E_FAIL;
	if(FAILED(Ready_DropItem(TEXT("Layer_DropItem"))))
		return E_FAIL;
	
	if (FAILED(Ready_Npc(TEXT("Layer_Npc"))))
		return E_FAIL;

	if(FAILED(Ready_Trail(TEXT("Layer_Trail"))))
		return E_FAIL;

	m_pGameInstance->PlayBGM(L"BGM_GamePlay.mp3", 0.1f);

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	Update_Check_Stage_Clear();
	if (GetKeyState(VK_RETURN) & 0x8000 || (M_SEMI_STAGE + 1 ==m_iNowStage) )
	{
		m_pGameInstance->StopBGM();
		m_pGameInstance->Clear_UIObjects();//ui는 static에 두면 안됨 아니면 다 static에 박아두고 이거 뺴면됨
 		m_pItem_Manager->Clear_All_Items();
		m_pGameInstance->Open_Level(LEVEL_LOADING,CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY2));
		return; //-> 오픈 레벨하고 키 입력하면 터짐 왜지? 
	}
	return ;
}

HRESULT CLevel_GamePlay::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
#endif
	
	
	

	return S_OK;
}

void CLevel_GamePlay::Update_Check_Stage_Clear()
{
	m_vecMonsters[M_SEMI_STAGE + 1];
	_bool b_NowStageMonster_AllDead = true;
	for (auto Monster: m_vecMonsters[m_iNowStage])
	{
		if (false == (b_NowStageMonster_AllDead = Monster->Get_Dead()))
			break;
	}
	if (false == m_bNpcActiveDone && true == b_NowStageMonster_AllDead)// npc 조건 추가
	{

		for (auto& Npc : m_vecNpcs[m_iNowStage])
		{
			Npc->Set_Active(true);
		}
		m_bNpcActiveDone = true;

	}

	if (true == m_bNpcActiveDone)
	{
		if (0 == m_iNowStage)
		{
			++m_iNowStage;
			for (auto Monster_NextStage : m_vecMonsters[m_iNowStage])
			{
				Monster_NextStage->Set_Active();
			}
			static_cast<CUI_MiniMapMain*>(m_pGameInstance->Get_UI_By_Tag(TEXT("MiniMap")))->Set_Vector(&m_vecMonsters[m_iNowStage]);
			m_bNpcActiveDone = false;
		}
		else if (true == static_cast<CNpc_Smith*>(m_vecNpcs[m_iNowStage][0])->GoToTheNextStage())
		{
			for (auto& Npc : m_vecNpcs[m_iNowStage])
			{
				Npc->Set_InActive();
			}

			++m_iNowStage;
			if (M_SEMI_STAGE + 1 <= m_iNowStage)
			{
				m_iNowStage = M_SEMI_STAGE + 1;
			}
			else
			{
				for (auto Monster_NextStage : m_vecMonsters[m_iNowStage])
				{
					Monster_NextStage->Set_Active();
				}
				static_cast<CUI_MiniMapMain*>(m_pGameInstance->Get_UI_By_Tag(TEXT("MiniMap")))->Set_Vector(&m_vecMonsters[m_iNowStage]);
			}
			m_bNpcActiveDone = false;
		}
		
	}



}

HRESULT CLevel_GamePlay::Ready_ShadowCamera()
{
	m_pGameInstance->SetUp_Shadow_TransformMatrix(CShadow::D3DTS_VIEW, XMMatrixLookAtLH(XMVectorSet(-20.f, 80.f, 20.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	m_pGameInstance->SetUp_Shadow_TransformMatrix(CShadow::D3DTS_PROJ, XMMatrixPerspectiveFovLH(XMConvertToRadians(160.f), static_cast<_float>(g_iWinSizeX) / g_iWinSizeY, 0.1f, 1000.f));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring & strLayerTag)
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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
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
	Desc_Player.pPlayerFloat4x4= m_pPlayerFloat4x4;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Player"),
		LEVEL_GAMEPLAY, strLayerTag, &Desc_Player)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring & strLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, strLayerTag, nullptr )))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_UI(const _wstring& strLayerTag)
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
			LEVEL_GAMEPLAY, strLayerTag, &UI_Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
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
			LEVEL_GAMEPLAY, strLayerTag, &Arms_Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
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
		for (int i = 0; i < 50; ++i)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_DemageFont"),
				LEVEL_GAMEPLAY, strLayerTag, &DemageFont_Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
				return E_FAIL;
			pUIObject->Set_InActive();
			pUIObject->Set_SortDepth(9);
			m_pGameInstance->Add_DemageFonts(pUIObject);
			
		}
	}

	/*ForMiniMap*/
	{
		CUI_MiniMapMain::UI_MAIN_MAP_DESC  MiniMapDesc = {};
		MiniMapDesc.eStageLevel = LEVEL_GAMEPLAY;
		MiniMapDesc.fRange = 50;
		MiniMapDesc.fSizeX = 200;
		MiniMapDesc.fSizeY = 200;
		MiniMapDesc.fX = 10;
		MiniMapDesc.fY = 10;
		MiniMapDesc.fRotationPerSec = XMConvertToRadians(90.f);
		MiniMapDesc.fSpeedPerSec = 10.f;
		MiniMapDesc.iData = 0.f;
		MiniMapDesc.eType = UI_TYPE::UI_FIXED;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MiniMap_Main"),
			LEVEL_GAMEPLAY, strLayerTag, &MiniMapDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		pUIObject->Set_SortDepth(1000);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("MiniMap"), pUIObject, UI_TYPE::UI_FIXED);


		CUI_MiniMapMain* pUIMiniMap = static_cast<CUI_MiniMapMain*>(m_pGameInstance->Get_UI_By_Tag( L"MiniMap"));
		pUIMiniMap->Set_szLevelName(L"Stage1");


		//static_cast<CUI_MiniMapMain*>(m_pGameInstance->Get_UI_By_Tag(TEXT("MiniMap"))) ->Set_Vector(&m_vecMonsters[m_iNowStage]);
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
		LEVEL_GAMEPLAY, strLayerTag, &PlayerHPDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
		return E_FAIL;
	pUIObject->Set_Active();
	pUIObject->Set_SortDepth(1000);
	m_pGameInstance->Add_UI_To_UIMap(TEXT("PlayerHP"), pUIObject, UI_TYPE::UI_FIXED);
	/*For Inventory */
	{//1280 720
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
			LEVEL_GAMEPLAY, strLayerTag, &Shop_Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(11);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Shop"), pUIObject, UI_TYPE::UI_FIXED);

	}

	/*For Properties */
	{//1280 720
		CUI_Coin_And_Bullets::UI_COIN_AND_BULLET_DESC PropertiesDesc = {};
		PropertiesDesc.eType = UI_TYPE::UI_FIXED;
		PropertiesDesc.fRotationPerSec = XMConvertToRadians(90.f);
		PropertiesDesc.fSizeX = 400;
		PropertiesDesc.fSizeY = 400;
		PropertiesDesc.fX = 900.f;
		PropertiesDesc.fY = 30.f;
		PropertiesDesc.fSpeedPerSec = 10.f;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Properties"),
			LEVEL_GAMEPLAY, strLayerTag, &PropertiesDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(10);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Properties"), pUIObject, UI_TYPE::UI_FIXED);

	}
	/*UI_Talk*/
	{
		CUI_Talk::UI_TALK_DESC talkDesc = {};
		talkDesc.eType = UI_TYPE::UI_FIXED;
		talkDesc.fRotationPerSec = XMConvertToRadians(90.f);
		talkDesc.fSpeedPerSec = 10.f;
		talkDesc.vFontOffset = { 20.f,-20.f };
		talkDesc.fSizeX = 32.f;
		talkDesc.fSizeY = 32.f;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Talk"),
			LEVEL_GAMEPLAY, strLayerTag, &talkDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(10);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Talk"), pUIObject, UI_TYPE::UI_FIXED);
	}

	/*UI_Dialogue*/
	{
		CUI_Dialogue::UI_DIALOGUE_DESC DialogueDesc = {};
		DialogueDesc.eType = UI_TYPE::UI_POP;
		DialogueDesc.fRotationPerSec = XMConvertToRadians(90.f);
		DialogueDesc.fSpeedPerSec = 10.f;
		DialogueDesc.fSizeX = g_iWinSizeX;
		DialogueDesc.fSizeY = 250.f;
		DialogueDesc.fX = 0.f;
		DialogueDesc.fY = g_iWinSizeY - 250.f;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Dialogue"),
			LEVEL_GAMEPLAY, strLayerTag, &DialogueDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(10);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Dialogue"), pUIObject, UI_TYPE::UI_POP);
	}
	/*Goblet*/
	{
		CUI_Goblet::UI_GOBLET_DESC GobletDesc = {};
		GobletDesc.eType = UI_TYPE::UI_POP;
		GobletDesc.fRotationPerSec = XMConvertToRadians(90.f);
		GobletDesc.fSpeedPerSec = 10.f;
		GobletDesc.fSizeX = g_iWinSizeX;
		GobletDesc.fSizeY = g_iWinSizeY;
		GobletDesc.fX = 0.f;
		GobletDesc.fY = 0.f;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Goblet"),
			LEVEL_GAMEPLAY, strLayerTag, &GobletDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(10);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Goblet"), pUIObject, UI_TYPE::UI_POP);
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
		DashScreenDesc.fX =0.f;
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

HRESULT CLevel_GamePlay::Ready_Layer_GameObject(const _wstring& strLayerTag)
{


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{

	m_pGameInstance->Set_LightsLevel(LEVEL_GAMEPLAY);
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(10.f, 3.f, 10.f, 0.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(20.f, 3.f, 10.f, 0.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Monster(const _wstring& strLayerTag)
{

	

	
	Read_Monsters_Desc_From_File();
	//MakeMonsters
	for (int i = 1; i <= M_SEMI_STAGE; ++i) {
		for (int j = 1; j <= M_MONSTER_TYPES; ++j) {
			int ivecSize = m_vecMonsterDescs[i][j].size();
			/*if (ivecSize)ivecSize = 1;*/
			for (int k = 0; k < ivecSize; ++k)
			{
				_tchar BaseMonsterProtoTag[MAX_PATH] = L"Prototype_GameObject_Monster0%d";
				_tchar NowMonsterProtoTag[MAX_PATH] = L"";
				wsprintf(NowMonsterProtoTag, BaseMonsterProtoTag, j);
				CMonster* pMonster = nullptr;

				if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, NowMonsterProtoTag, LEVEL_GAMEPLAY, strLayerTag,
					&m_vecMonsterDescs[i][j][k],
					reinterpret_cast<CGameObject**>(&pMonster)
				)))
					return E_FAIL;

				pMonster->Set_PlayerFloat4x4(m_pPlayerFloat4x4);
				m_vecMonsters[i].push_back(pMonster);
			}

		}
	}
	//CMonster::MONSTER_DESC desc = {};
	//desc.fRotationPerSec = 10;
	//desc.vPosition = { 2.f,0.f,0.f,1.f};
	//desc.iNaviIndex = 0;
	//desc.vScale = { 1.f,1.f,1.f };
	//desc.fSpeedPerSec = 10.f;
	//desc.fAngle = XMConvertToRadians(180);
	//desc.fRotationPerSec = XMConvertToRadians(90.f);
	//CMonster* pMonster = nullptr;
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, L"Prototype_GameObject_Monster05", LEVEL_GAMEPLAY, strLayerTag,
	//				&desc,
	//				reinterpret_cast<CGameObject**>(&pMonster)
	//			)))
	//				return E_FAIL;
	//			pMonster->Set_PlayerFloat4x4(m_pPlayerFloat4x4);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Map(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain_Stage01"), LEVEL_GAMEPLAY, strLayerTag, nullptr)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, strLayerTag, nullptr)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Player(const _wstring& strLayerTag)
{
	CPlayer::PLAYER_DESC Desc = {};
	Desc.fInitial_Height = 2.f;
	Desc.InitialPos = { 2.f,0.f,0.f,1.f };
	Desc.iNaviMeshIndex = 0;
	Desc.iNowMapStage = 1;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, strLayerTag, &Desc,reinterpret_cast<CGameObject**>(&m_pPlayer))))
		return E_FAIL;
	if (nullptr != m_pPlayer)
	{
		m_pPlayerFloat4x4 = static_cast<CPlayer*>(m_pPlayer)->Get_PlayerFloat4x4_ptr();
	}
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_DropItem(const _wstring& strLayerTag)
{
	CDropItem_Grenade::DROPITEM_GRENADE_DESC GrenadeDesc = {};
	GrenadeDesc.fRotationPerSec = XMConvertToRadians(90.f);
	GrenadeDesc.fSpeedPerSec = 10.f;
	GrenadeDesc.iNowMapStage = LEVEL_GAMEPLAY;
	CDropItem_Money::DROPITEM_MONEY_DESC MoneyDesc = {};
	MoneyDesc.fRotationPerSec = XMConvertToRadians(90.f);
	MoneyDesc.fSpeedPerSec = 10.f;
	MoneyDesc.iNowMapStage = LEVEL_GAMEPLAY;
	CDropItem_Bullet_Special::DROPITEM_BULLET_SPECIAL_DESC BulletSpecialDesc = {};
	BulletSpecialDesc.fRotationPerSec = XMConvertToRadians(90.f);
	BulletSpecialDesc.fSpeedPerSec = 10.f;
	BulletSpecialDesc.iNowMapStage = LEVEL_GAMEPLAY;
	CDropItem_Bullet_Standard::DROPITEM_BULLET_STANDARD_DESC BulletStandardDesc = {};
	BulletStandardDesc.fRotationPerSec = XMConvertToRadians(90.f);
	BulletStandardDesc.fSpeedPerSec = 10.f;
	BulletStandardDesc.iNowMapStage = LEVEL_GAMEPLAY;
	CDropItem* pTarget = {};

	for (size_t i= 0; i < g_iNum_DropItem_Grenade; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_DropItem_Grenade"), LEVEL_GAMEPLAY, strLayerTag, &GrenadeDesc, reinterpret_cast<CGameObject**>(&pTarget))))
			return E_FAIL;

		m_pItem_Manager->Set_Item(CItem_Manager::DROPITEM_TYPE::DROPITEM_GRENADE, pTarget);
		pTarget = nullptr;
	}

	for (size_t i = 0; i < g_iNum_DropItem_Bullets_Special; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_DropItem_Bullet_Special"), LEVEL_GAMEPLAY, strLayerTag, &BulletSpecialDesc, reinterpret_cast<CGameObject**>(&pTarget))))
			return E_FAIL;
		m_pItem_Manager->Set_Item(CItem_Manager::DROPITEM_TYPE::DROPITEM_BULLET_SPECIAL, pTarget);
		pTarget = nullptr;
	}

	for (size_t i = 0; i < g_iNum_DropItem_Bullets_Standard; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_DropItem_Bulet_Standard"), LEVEL_GAMEPLAY, strLayerTag, &BulletStandardDesc, reinterpret_cast<CGameObject**>(&pTarget))))
			return E_FAIL;
		m_pItem_Manager->Set_Item(CItem_Manager::DROPITEM_TYPE::DROPITEM_BULLET_STANDARD, pTarget);
		pTarget = nullptr;
	}

	for (size_t i = 0; i < g_iNum_DropItem_Coins; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_DropItem_Money"), LEVEL_GAMEPLAY, strLayerTag, &MoneyDesc, reinterpret_cast<CGameObject**>(&pTarget))))
			return E_FAIL;
		m_pItem_Manager->Set_Item(CItem_Manager::DROPITEM_TYPE::DROPITEM_COIN, pTarget);
		pTarget = nullptr;

	}


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Npc(const _wstring& strLayerTag)
{

	Read_Npc_Desc_From_File();
	//MakeMonsters
	for (int i = 1; i <= M_SEMI_STAGE; ++i) {
		for (int j = 1; j <= M_NPC_TYPES; ++j) {
			int ivecSize = m_vecNpcDescs[i][j].size();
			for (int k = 0; k < ivecSize; ++k)
			{
				_tchar BaseNpcProtoTag[MAX_PATH] = L"Prototype_GameObject_Npc_0%d";
				_tchar NowNpcProtoTag[MAX_PATH] = L"";
				wsprintf(NowNpcProtoTag, BaseNpcProtoTag, j);
				CNpc* pNpc = nullptr;

				if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, NowNpcProtoTag, LEVEL_GAMEPLAY, strLayerTag,
					&m_vecNpcDescs[i][j][k],
					reinterpret_cast<CGameObject**>(&pNpc)
				)))
					return E_FAIL;

				m_vecNpcs[i].push_back(pNpc);
			}

		}
	}
	 
	 vector<DialogueInfo> m_SmithDialogues;
     DialogueInfo eDialogueText1 = {};
	 memcpy(eDialogueText1.sz_text,TEXT("Are you going to the next stage?"),sizeof(eDialogueText1.sz_text));
	 DialogueInfo eDialogueText2 = {};
	 memcpy(eDialogueText2.sz_text, TEXT("Are you sure?"), sizeof(eDialogueText2.sz_text));
	 
	 
	 m_SmithDialogues.push_back(eDialogueText1);
	 m_SmithDialogues.push_back(eDialogueText2);

	 for (int i = 1; i < M_SEMI_STAGE; ++i)
	 {
		 CNpc_Smith* pSmith = static_cast<CNpc_Smith*>(m_vecNpcs[i][0]);
		 pSmith->Set_Dialogues(m_SmithDialogues);
	 }
	 DialogueInfo eDialogueText3 = {};
	 memcpy(eDialogueText3.sz_text, TEXT("The next stage is Boss stage. This one is gonna be quite hard. Good Luck."), sizeof(eDialogueText3.sz_text));
	 
	 m_SmithDialogues.push_back(eDialogueText3);
	 CNpc_Smith* pSmith = static_cast<CNpc_Smith*>(m_vecNpcs[M_SEMI_STAGE][0]);
	 pSmith->Set_Dialogues(m_SmithDialogues);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Trail(const _wstring strLayerTag)
{
	/*CGameObject::GAMEOBJECT_DESC Desc = {};
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.fSpeedPerSec = 10.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Trail_Test"), LEVEL_GAMEPLAY, strLayerTag, &Desc,nullptr)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Particle(const _wstring& strLayerTag)
{

	//for (size_t i = 0; i < 40; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect"),
	//		LEVEL_GAMEPLAY, strLayerTag)))
	//		return E_FAIL;
	//}
	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_BackGround"), LEVEL_GAMEPLAY, strLayerTag,nullptr)))
		return E_FAIL;*/
	return S_OK;
}

HRESULT CLevel_GamePlay::Read_Monsters_Desc_From_File()
{

	for (int i = 1; i <= M_SEMI_STAGE; ++i)
	{
		HANDLE hFile = CreateFile(m_DescPathes[i], GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			MSG_BOX("Failed To Read Monster Desc Checkk Pathes Again");
			return E_FAIL;
		}
		_ulong dwByte = { 0 };
		_uint iNumMonsterTypes = { 0 };
		ReadFile(hFile, &iNumMonsterTypes, sizeof(_uint), &dwByte, nullptr);

		vector<_uint>m_NumMonstersPerTypes;

		m_NumMonstersPerTypes.resize(iNumMonsterTypes + 1);
		for (int j = 1; j <= iNumMonsterTypes; ++j)
			ReadFile(hFile, &m_NumMonstersPerTypes[j], sizeof(_uint), &dwByte, nullptr);

		for(int j=1;j<=iNumMonsterTypes;++j)
			for (int k = 0; k < m_NumMonstersPerTypes[j]; ++k)
			{
				CMonster::MONSTER_DESC desc = {};
			
				ReadFile(hFile, &desc.vScale, sizeof(_float3), &dwByte, nullptr);
				ReadFile(hFile, &desc.fAngle, sizeof(_float), &dwByte, nullptr);
				ReadFile(hFile, &desc.vPosition, sizeof(_float4), &dwByte, nullptr);
				ReadFile(hFile, &desc.iNaviIndex, sizeof(_int), &dwByte, nullptr);
				desc.fRotationPerSec = XMConvertToRadians(90.f);
				desc.fSpeedPerSec = 10.f;
				m_vecMonsterDescs[i][j].push_back(desc);
			}
		CloseHandle(hFile);
	}
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Read_Map_Desc_From_File()
{
	return S_OK;
}

HRESULT CLevel_GamePlay::Read_Npc_Desc_From_File()
{
	_int iNpcNo = {-1};
	for (int i = 1; i <= M_SEMI_STAGE; ++i)
	{
		HANDLE hFile = CreateFile(m_DescPathes_Npc[i], GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			MSG_BOX("Failed To Read Npc Desc Checkk Pathes Again");
			return E_FAIL;
		}
		_ulong dwByte = { 0 };
		_uint iNumNpcTypes = { 0 };
		ReadFile(hFile, &iNumNpcTypes, sizeof(_uint), &dwByte, nullptr);

		vector<_uint>vecNpcsPerTypes;

		vecNpcsPerTypes.resize(iNumNpcTypes + 1);
		for (int j = 1; j <= iNumNpcTypes; ++j)
			ReadFile(hFile, &vecNpcsPerTypes[j], sizeof(_uint), &dwByte, nullptr);

		for (int j = 1; j <= iNumNpcTypes; ++j)
			for (int k = 0; k < vecNpcsPerTypes[j]; ++k)
			{
				CNpc::NPC_DESC desc = {};

				ReadFile(hFile, &desc.vScale, sizeof(_float3), &dwByte, nullptr);
				ReadFile(hFile, &desc.fAngle, sizeof(_float), &dwByte, nullptr);
				ReadFile(hFile, &desc.vPosition, sizeof(_float4), &dwByte, nullptr);
				ReadFile(hFile, &desc.iNaviIndex, sizeof(_int), &dwByte, nullptr);
				desc.fRotationPerSec = XMConvertToRadians(90.f);
				desc.fSpeedPerSec = 10.f;
				desc.NPCNo = ++iNpcNo;
				m_vecNpcDescs[i][j].push_back(desc);
			}
		CloseHandle(hFile);
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Load_Objects_From_File()
{
	return S_OK;
}






CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{

	
	for (auto& MonstersInStage : m_vecMonsters)
	{
		for (auto& Monster : MonstersInStage)
		{
			Safe_Release(Monster);
		}
		MonstersInStage.clear();
	}

	for (auto& NpcsInStage : m_vecNpcs)
	{
		for (auto& Npc : NpcsInStage)
		{
			Safe_Release(Npc);
		}
		NpcsInStage.clear();
	}


	m_vecMonsters->clear();
	Safe_Release(m_pPlayer);
	Safe_Release(m_pItem_Manager);
	
	__super::Free();
}
