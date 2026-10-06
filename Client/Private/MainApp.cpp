#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include"Item_Manager.h"
#include "Level_Loading.h"
#include"BackGround.h"
#include"UI_Loading.h"


CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::GetInstance() }
	, m_pItem_Manager{CItem_Manager::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
	/*Itme Manager는 여기서 처음 생성-AddRef하지 않는다.*/
	/*D3D11_SAMPLER_DESC*/
}

HRESULT CMainApp::Initialize()
{
	
	ENGINE_DESC			EngineDesc{};

	EngineDesc.hInstance = g_hInst;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.isWindowed = true;
	EngineDesc.iNumLevels = LEVEL_END;
	EngineDesc.iViewportWidth = g_iWinSizeX;
	EngineDesc.iViewportHeight = g_iWinSizeY;
	

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_UI_LOGO()))
		return E_FAIL;
	if (FAILED(Ready_LoadingBar()))
		return E_FAIL;

	if (FAILED(Ready_Test()))
		return E_FAIL;

	if (FAILED(SetUp_StartLevel(LEVEL_LOGO)))
		return E_FAIL;
	

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	

	if (fTimeDelta > 1.f / 60.0f)
		fTimeDelta = 1.f / 60.0f;

	if (nullptr != m_pGameInstance)
		m_pGameInstance->Update_Engine(fTimeDelta);

	m_fAccFrame += fTimeDelta;
	++m_iFrame;
	if (m_fAccFrame >= 1.f)
	{
		m_fAccFrame = 0.f;
		m_iNowFrame = m_iFrame;
		m_iFrame = 0;
	}
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Render_Begin()))
		return E_FAIL;

	m_pGameInstance->Draw();

	/*_tchar szFrame[MAX_PATH] = L"";
	wsprintf(szFrame, L"FPS:%d", m_iNowFrame);
	m_pGameInstance->Render_Font(TEXT("Font10"), szFrame, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));*/

	if (FAILED(m_pGameInstance->Render_End()))
		return E_FAIL;



	return S_OK;
}


HRESULT CMainApp::SetUp_StartLevel(LEVELID eLevelID)
{
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING,
		CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMainApp::Ready_Test()
{
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font32"), TEXT("../Bin/Resources/Fonts/myfile.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font10"), TEXT("../Bin/Resources/Fonts/myfile2.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font64"), TEXT("../Bin/Resources/Fonts/myfile3.spritefont"))))
		return E_FAIL;
	
	

	/*_ulong				dwByte = {};

	HANDLE				hFile = CreateFile(TEXT("../Bin/DataFiles/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3				vPoints[3] = {};

	vPoints[0] = _float3(0.f, 0.f, 10.f);
	vPoints[1] = _float3(10.f, 0.f, 0.f);
	vPoints[2] = _float3(0.f, 20.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.f, 0.f, 10.f);
	vPoints[1] = _float3(10.f, 0.f, 10.f);
	vPoints[2] = _float3(10.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.f, 0.f, 20.f);
	vPoints[1] = _float3(10.f, 0.f, 10.f);
	vPoints[2] = _float3(0.f, 0.f, 10.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(10.f, 0.f, 10.f);
	vPoints[1] = _float3(20.f, 0.f, 0.f);
	vPoints[2] = _float3(10.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);*/
	
	return S_OK;

}

HRESULT CMainApp::Ready_UI_LOGO()
{
	CUIObject* m_pUIObject = { nullptr };

	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LogoBackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UI_Logo/UI_BG_Login%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_LogoBackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_LoadingBar()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Begin"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Loading/icon_begin1.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Middle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Loading/icon_normal_loading3.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loding_Boss"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Loading/IconBoss.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Fill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Loading/bar_Fill.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loding_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Loading/bar_bg_loading_shenhe3_1.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Fill_Static"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Loading/bar_loading.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loding_Bar_Static"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Loading/bar_bg_loading.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Loading"),
		CUI_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	

	return S_OK;
}

CMainApp * CMainApp::Create()
{	
	CMainApp*	pInstance = new CMainApp();	

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{

	/* 부모 멤버를 정리한다. */
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pItem_Manager);
	
	CGameInstance::Release_Engine();


}

