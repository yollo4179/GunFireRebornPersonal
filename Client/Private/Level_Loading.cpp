#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "Loader.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include"Level_GamePlay2.h"

/*For UI*/
#include"BackGround.h"

CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevelID)
{
	
	if (FAILED(Ready_Layer_UI(TEXT("UI_BackGround"))))
		return E_FAIL;

	
	m_eNextLevelID = eNextLevelID;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;
	


	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (true == m_pLoader->isFinished() &&
		GetKeyState(VK_SPACE) & 0x8000)
	{
		
		m_pGameInstance->StopBGM();
		m_pGameInstance->Clear_UIObjects();
		CLevel*			pNewLevel = { nullptr };


		switch (m_eNextLevelID)
		{
		case LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_GAMEPLAY:
			pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_GAMEPLAY2:
			pNewLevel = CLevel_GamePlay2::Create(m_pDevice, m_pContext);
		}

		
		if (nullptr == pNewLevel)
			return;
		
		//여기서ui clear x 
		if (FAILED(m_pGameInstance->Open_Level(m_eNextLevelID, pNewLevel)))
			return ;
	}
}

HRESULT CLevel_Loading::Render()
{

#ifdef _DEBUG
	m_pLoader->Show_Debug();
#endif

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_UI(const _wstring strLayerTag)
{
	CBackGround::BACKGROUND_DESC		Desc{};


	CUIObject* pUIObject = { nullptr };
	{
		Desc.iData = 0;
		Desc.fRotationPerSec = XMConvertToRadians(90.f);
		Desc.fSpeedPerSec = 5.f;
		Desc.fSizeX = g_iWinSizeX;
		Desc.fSizeY = g_iWinSizeY;
		Desc.fX = 0;
		Desc.fY = 0;
		Desc.eType = UI_TYPE::UI_FIXED;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_LogoBackGround"),
			LEVEL_LOADING, strLayerTag, &Desc, reinterpret_cast<CGameObject**>(&pUIObject))))
			return E_FAIL;
		pUIObject->Set_Active();
		static_cast<CBackGround*>(pUIObject)->Set_BackGroundType(false);
		m_pGameInstance->Add_UI_To_UIMap(TEXT("BackGround"), pUIObject, UI_TYPE::UI_FIXED);//-> 제일 먼저 깔리므로 자동으로 depth order는 10이된다

	}
}


CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
