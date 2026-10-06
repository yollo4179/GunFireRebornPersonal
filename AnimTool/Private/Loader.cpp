#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
/*For GameLogo*/
#include "BackGround.h"
/*for GamePlay*/
#include "Camera_Free.h"
#include "Terrain.h"
#include "CubeCollider.h"
#include"Monster01.h"
#include"Monster02.h"
#include"Monster03.h"
#include"Monster04.h"
#include"Monster05.h"
#include"Monster06.h"
#include"Monster07.h"
#include"TestMonster.h"
#include"Terrain_Stage1.h"
#include"Flat_Virtual_Terrain.h"
#include <time.h>

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);

}

//typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
_uint APIENTRY LoadingMain(void* pArg)
{	
	CLoader*	pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1; 

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	/* 내 코드를 읽어나갈수 있는 하나의 스레드를 생성한다. */
	/* 생성한 스레드가 호출해야할 진입점함수를 지정해준다. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	CoInitializeEx( nullptr, 0);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_Level_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_Level_GamePlay();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}

#ifdef _DEBUG

void CLoader::Show_Debug()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

#endif

HRESULT CLoader::Loading_Level_Logo()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For.Prototype_Component_Texture_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드를 로딩중입니다."));
	

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;	

	lstrcpy(m_szLoadingText, TEXT("모델(을)를 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	

	

	lstrcpy(m_szLoadingText, TEXT("객체원형(을)를 로딩중입니다."));
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Level_GamePlay()
{
	///*model binary*/
	//CModel::Make_Binary_Anim(CModel::TYPE::TYPE_NONANIM, "../Bin/resources/Models/Fiona/Fiona.fbx", TEXT("../Bin/resources/Models/Fiona/Fiona.dat"));
	
   	//CModel::Make_Binary_Anim(CModel::TYPE::TYPE_NONANIM, "../Bin/resources/Models/Monster/Monster01/Monster01.fbx", TEXT("../Bin/resources/Models/Monster/Monster01/Monster01_Anim.dat"));

	_matrix PreTransformMatrix = XMMatrixIdentity();
	

 /*	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster01/Monster01.fbx", TEXT("../Bin/resources/Models/Monster/Monster01/Monster01_Anim.dat"), PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster02/Monster02.fbx", TEXT("../Bin/resources/Models/Monster/Monster02/Monster02_Anim.dat"),PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster04/Monster04.fbx", TEXT("../Bin/resources/Models/Monster/Monster04/Monster04_Anim.dat"),PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster05/Monster05.fbx", TEXT("../Bin/resources/Models/Monster/Monster05/Monster05_Anim.dat"), PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster06/Monster06.fbx", TEXT("../Bin/resources/Models/Monster/Monster06/Monster06_Anim.dat"), PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster07/Monster07.fbx", TEXT("../Bin/resources/Models/Monster/Monster07/Monster07_Anim.dat"), PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster08/Monster08.fbx", TEXT("../Bin/resources/Models/Monster/Monster08/Monster08_Anim.dat"), PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster09/Monster09.fbx", TEXT("../Bin/resources/Models/Monster/Monster09/Monster09_Anim.dat"), PreTransformMatrix);
    CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/1302_Right_ok_LivingGun/Anim.fbx", TEXT("../Bin/resources/Models/1302_Right_ok_LivingGun/Anim.dat"), PreTransformMatrix);	
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster03/Monster03.fbx", TEXT("../Bin/resources/Models/Monster/Monster03/Monster03_Anim.dat"),PreTransformMatrix);
	
	 lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	


  	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkyBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/SkyBox/Sky_%d.dds"), 5))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드를 로딩중입니다."));
		

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex*/
	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::Elements, VTXCUBETEX::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;*/


	/*For AnimModel*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델(을)를 로딩중입니다."));
	///* For.Prototype_Component_VIBuffer_Terrain */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Terrain/Height.bmp")))))
	//	return E_FAIL;
	///*For Orototype_Component_VIBuffer_Cube*/
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
	//	CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	///*for.Prototype_Component_VIBuffer_Collider */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Collider"),
	//	CVIBuffer_Collider::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	///*For Prototype_Component_VIBuffer_Flat*/
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Flat"),
	//	CVIBuffer_Flat::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	
	
	
	
	PreTransformMatrix = XMMatrixIdentity();
	/*For TestMonster*/
	lstrcpy(m_szLoadingText, TEXT(" Fiona Model"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/1302_Right_ok_LivingGun/Anim.fbx", PreTransformMatrix
			, TEXT("../Bin/resources/Models/1302_Right_ok_LivingGun/Anim.dat"),false
		))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	clock_t start = clock();
	
	/*For Monster01 Model*/
	lstrcpy(m_szLoadingText, TEXT(" Monster01 Model"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster01/Monster01.fbx", PreTransformMatrix
			,L"../Bin/resources/Models/Monster/Monster01/Monster01_Anim.dat",false
		))))
		return E_FAIL;
	 lstrcpy(m_szLoadingText, TEXT(" Monster02 Model"));
	/*For Monster02 Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster02/Monster02.fbx", PreTransformMatrix
			,L"../Bin/resources/Models/Monster/Monster02/Monster02_Anim.dat",false
		))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT(" Monster04 Model"));
	/*For Monster04 Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster04/Monster04.fbx", PreTransformMatrix
			,L"../Bin/resources/Models/Monster/Monster04/Monster04_Anim.dat",false
		))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT(" Monster05 Model"));
	/*For Monster05 Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster05/Monster05.fbx", PreTransformMatrix
			,L"../Bin/resources/Models/Monster/Monster05/Monster05_Anim.dat",false
		))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT(" Monster06 Model"));
	/*For Monster06 Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster06"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/resources/Models/Monster/Monster06/Monster06.fbx", PreTransformMatrix
			,L"../Bin/resources/Models/Monster/Monster06/Monster06.dat"
		))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT(" Monster07 Model"));
	
	/*For Monster07 Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster07"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/resources/Models/Monster/Monster07/Monster07.fbx", PreTransformMatrix
			,L"../Bin/resources/Models/Monster/Monster07/Monster07.dat"
		))))
		return E_FAIL;

	/*PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixTranslation(16.8, 0, 68);
 	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster09"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster09/Monster09.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster09/Monster09_Anim.dat",false
		))))
		return E_FAIL;*/

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixTranslation(5, 0, 10) *XMMatrixRotationY(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster09"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster09/3921_mini/3921_mini.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster09/3921_mini/3921_mini.dat", false
		))))
		return E_FAIL;

	/*Monster09 Mini*/

	/*PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixTranslation(5, 0, 10);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster09_Mini"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster09/3921_mini/3921_mini.fbx", PreTransformMatrix
		))))
		return E_FAIL;*/
	
	lstrcpy(m_szLoadingText, TEXT(" Monster03 Model"));
	/*For Monster03 Model*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixTranslation(23.5, 0, 28) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster03/Monster03.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster03/Monster03_Anim.dat", false
		))))
		return E_FAIL;
	

	/* For Player01 ok 칼*/
	PreTransformMatrix = XMMatrixTranslation(-1.f, -1.0f, -1.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player01/Player01.dat"
		))))
		return E_FAIL;


	/* For Player02  0k 파란장갑*/
	PreTransformMatrix = XMMatrixTranslation(0.f, 0.0f, 0.5f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player02/Player02.dat"
		))))
		return E_FAIL;


	/* For Player03  */
	PreTransformMatrix = XMMatrixTranslation(0.f, 0.0f, -0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player03/Player03.dat"
		))))
		return E_FAIL;


	/* For Player04 0k  캐논*/
	PreTransformMatrix = XMMatrixTranslation(0.f, -0.2f, -0.0f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player04/Player04.dat"
		))))
		return E_FAIL;

	/* For Player05 ok 리빙건*/
	PreTransformMatrix = XMMatrixTranslation(0.f, 0.0f, -0.1f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player05/Player05.dat"
		))))
		return E_FAIL;
	clock_t end = clock();
 
	double a = (double)(end - start) / CLOCKS_PER_SEC;

	lstrcpy(m_szLoadingText, TEXT("Model Completed"));



	lstrcpy(m_szLoadingText, TEXT("객체원형(을)를 로딩중입니다."));
	
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/*for monster01*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster01"),
		CMonster01::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*for monster02*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster02"),
		CMonster02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*for monster03*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03"),
		CMonster03::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*for monster04*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster04"),
		CMonster04::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*for monster05*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster05"),
		CMonster05::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*for monster06*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster06"),
		CMonster06::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*for monster07*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster07"),
		CMonster07::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TestMonster"),
		CTestMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);


}
