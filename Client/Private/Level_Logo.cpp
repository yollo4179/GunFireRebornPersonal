#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include"UI_Button_GameStart.h"
/*forTest */
#include"UI_Numbers.h"
CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;	
	if (FAILED(Ready_Layer_UI(TEXT("LAYER_UI"))))
		return E_FAIL;

	 m_pGameInstance->PlayBGM(L"Bgm_Logo.mp3", 0.1F);
	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		m_pGameInstance->StopBGM();
		m_pGameInstance->Clear_UIObjects();//ui는 static에 두면 안됨 아니면 다 static에 박아두고 이거 뺴면됨
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY2));
		return; //-> 오픈 레벨하고 키 입력하면 터짐 왜지? 
	}

	else if (
		 nullptr !=m_pGameInstance->Get_UI_By_Tag(TEXT("Button_GameStart"))&&
		static_cast<CUI_Button_GameStart*>(m_pGameInstance->Get_UI_By_Tag(TEXT("Button_GameStart")))->Get_NextLevel())
	{
		m_pGameInstance->StopBGM();
		m_pGameInstance->Clear_UIObjects();//ui는 static에 두면 안됨 아니면 다 static에 박아두고 이거 뺴면됨
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY));
		return;
	}

	return;

}

HRESULT CLevel_Logo::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	CBackGround::BACKGROUND_DESC		Desc{};


	CUIObject* pUIObject = { nullptr };
	{
		Desc.iData = 0;
		Desc.fRotationPerSec = XMConvertToRadians(90.f);
		Desc.fSpeedPerSec = 5.f;
		Desc.fSizeX = g_iWinSizeX;
		Desc.fSizeY = g_iWinSizeY;
		Desc.fX =0;
		Desc.fY = 0;
		Desc.eType = UI_TYPE::UI_FIXED;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_LogoBackGround"),
			LEVEL_LOGO, strLayerTag, &Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		m_pGameInstance->Add_UI_To_UIMap(TEXT("BackGround"), pUIObject, UI_TYPE::UI_FIXED);//-> 제일 먼저 깔리므로 자동으로 depth order는 10이된다

	}
	{
		Desc.iData = 0;
		Desc.fRotationPerSec = XMConvertToRadians(90.f);
		Desc.fSpeedPerSec = 5.f;
		Desc.fSizeX = 200;
		Desc.fSizeY = 100;
		Desc.fX = 200;
		Desc.fY = 100;
		Desc.eType = UI_TYPE::UI_FIXED;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, TEXT("Prototype_GameObject_UI_Button_GameStart"),
			LEVEL_LOGO, strLayerTag, &Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Button_GameStart"), pUIObject, UI_TYPE::UI_FIXED);//-> 제일 먼저 깔리므로 자동으로 depth order는 10이된다
		pUIObject->Set_SortDepth(11);
	}

	{
		Desc.iData = 0;
		Desc.fRotationPerSec = XMConvertToRadians(90.f);
		Desc.fSpeedPerSec = 5.f;
		Desc.fSizeX = 120;
		Desc.fSizeY = 60;
		Desc.fX = 200;
		Desc.fY = 300;
		Desc.eType = UI_TYPE::UI_FIXED;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, TEXT("Prototype_GameObject_UI_Button_Status"),
			LEVEL_LOGO, strLayerTag, &Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Button_Status"), pUIObject, UI_TYPE::UI_FIXED);//-> 제일 먼저 깔리므로 자동으로 depth order는 10이된다
		pUIObject->Set_SortDepth(11);
	}

	{
		Desc.iData = 0;
		Desc.fRotationPerSec = XMConvertToRadians(90.f);
		Desc.fSpeedPerSec = 5.f;
		Desc.fSizeX = 180;
		Desc.fSizeY = 90;
		Desc.fX = 200;
		Desc.fY = 500;
		Desc.eType = UI_TYPE::UI_FIXED;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, TEXT("Prototype_GameObject_UI_Button_GameEnd"),
			LEVEL_LOGO, strLayerTag, &Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Button_GameEnd"), pUIObject, UI_TYPE::UI_FIXED);//-> 제일 먼저 깔리므로 자동으로 depth order는 10이된다
		pUIObject->Set_SortDepth(11);
	}

	//{/*For Numbers*/

	//	CUI_Numbers::UI_NUMBERS_DESC NumberDesc = {};
	//	NumberDesc.iData = 0;
	//	NumberDesc.fRotationPerSec = XMConvertToRadians(90.f);
	//	NumberDesc.fSpeedPerSec = 5.f;
	//	NumberDesc.fSizeX = 20;
	//	NumberDesc.fSizeY = 20;
	//	NumberDesc.fX =200;
	//	NumberDesc.fY = 500;
	//	NumberDesc.eType = UI_TYPE::UI_FIXED;
	//	NumberDesc.fRadian = 0.f;
	//	NumberDesc.iNumerator = 135464797;
	//	NumberDesc.iDenominator=3022251516;
	//	NumberDesc.isDenominatorInfinity = true;
	//	NumberDesc.isFraction = true;
	//	NumberDesc.vColor = { 1.f,1.f,0.f };

	//	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"),
	//		LEVEL_LOGO, strLayerTag, &NumberDesc, reinterpret_cast<CGameObject**>(&pUIObject))))
	//		return E_FAIL;
	//	pUIObject->Set_Active();
	//	m_pGameInstance->Add_UI_To_UIMap(TEXT("Numbers"), pUIObject, UI_TYPE::UI_FIXED);//-> 제일 먼저 깔리므로 자동으로 depth order는 10이된다
	//	pUIObject->Set_SortDepth(11);
	//}
	/*CUIObject* pUIObject = { nullptr };
	{
		Desc.iData = 10;
		Desc.fSpeedPerSec = 5.f;
		Desc.fRotationPerSec = XMConvertToRadians(90.f);
		Desc.fSizeX = 700;
		Desc.fSizeY = 700;
		Desc.fX = 0;
		Desc.fY = 0;
		Desc.eType = UI_TYPE::UI_POP;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
			LEVEL_LOGO, strLayerTag, &Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;


		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(101);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Inventory1"), pUIObject, UI_TYPE::UI_POP);
	}
	{
		Desc.fSizeX = 800;
		Desc.fSizeY = 100;
		Desc.fX = 0;
		Desc.fY = 0;
		Desc.eType = UI_TYPE::UI_POP;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
			LEVEL_LOGO, strLayerTag, &Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;

		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(100);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Inventory2"), pUIObject, UI_TYPE::UI_POP);
	}

	{
		Desc.fSizeX = 400;
		Desc.fSizeY = 300;
		Desc.fX = 100;
		Desc.fY = 200;
		Desc.eType = UI_TYPE::UI_POP;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
			LEVEL_LOGO, strLayerTag, &Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;

		pUIObject->Set_InActive();
		pUIObject->Set_SortDepth(103);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Inventory3"), pUIObject, UI_TYPE::UI_POP);
	}*/
	
	pUIObject = nullptr;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UI(const _wstring strLayerTag)
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
			LEVEL_LOGO, strLayerTag, &UI_Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		pUIObject->Set_SortDepth(1000);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("Mouse"), pUIObject, UI_TYPE::UI_END);
	}

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
