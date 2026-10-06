
#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
/*For GameLogo*/
#include "BackGround.h"
#include"UI_Button_GameStart.h"
#include"UI_Button_Status.h"
#include"UI_Button_GameEnd.h"
/*Test*/
#include"SoftEffect.h"
#include"Mouse.h"
#include"UI_Numbers.h"


/*For UI */
//*UI_TOOL
#include"UI_Arm1.h"
#include"UI_Arm2.h"
#include"UI_Arm3.h"
#include"UI_Arm_Paint.h"
#include"UI_Arms.h""
//*UI_MiniMap
#include"UI_MiniMapMain.h"
#include"UI_Player_HP.h"

/*UI_Boss*/
#include"UI_BossHP.h"
/*UI_Inventory*/
#include"UI_Inventory.h"
#include"UI_Inventory_Button_Hero.h"
#include"UI_Inventory_Button_Weapons.h"
#include"UI_Inventory_Weapons.h"
#include"UI_Inventory_Weapon_Move_Button.h"
#include"UI_Inventory_Weapon_Explain.h"
#include"UI_Weapon_Equip_Button.h"
#include"UI_Inventory_WeaponPicture.h"
#include"UI_Inventory_Weapon_StatusText.h"
/*UI_Shop*/
#include"UI_Shop.h"
#include"UI_Shop_Button_Item.h"
#include"UI_Shop_ShowCard.h"

#include"UI_Coin_And_Bullets.h"

/*UI_Talk*/
#include"UI_Talk.h"
/*UI_Dialgoue*/
#include"UI_Dialogue.h"
#include"UI_Dialogue_Cancle_Button.h"
#include"UI_Dialogue_OK_Button.h"
/*Goblet*/
#include"UI_Goblet.h"
#include"UI_Goblet_RandomOptionButton.h"

/*UI_Victory*/
#include"UI_Victory.h"
#include"UI_Victory_Circle.h"

/*Tab*/
#include"UI_Tab.h"
/*UIBomb*/
#include"UI_Bomb.h"
#include"UI_Dash.h"
#include "UI_Dash_Screen.h"
//UI_IN_World
//*UI_MonsterHp
#include"UI_MonsterHP_BG.h"
#include"UI_MonsterHP_Red.h"
#include"UI_MonsterHP_Blue.h"
#include"UI_MonsterHP_Gray_BG.h"
#include"UI_MonsterHP.h"
/*DemageFont*/
#include"UI_DemageFont.h"



/*NPC*/
#include"Npc_Smith.h"
#include"Npc_Shop.h"
#include"Npc_ItemBox.h"
/*for GamePlay Stage01*/
#include "Camera_Free.h"
#include"Camera_Player.h"
#include "Terrain.h"
#include "CubeCollider.h"
#include"Sky.h"

/*Monster01*/
#include"Monster01.h"
#include"Monster01_Body.h"
#include"Monster01_Head.h"
/*For Monster02*/
#include"Monster02.h"
#include"Monster02_Sword.h"
#include"Monster02_Body.h"
#include"Monster02_Head.h"
#include"Trail_Monster02.h"
/*Monster03*/
#include"Monster03.h"
#include"Monster03_Body.h"
#include"Monster03_Head.h"
#include"Monster03_Gun.h"
#include"Monster03_Bullet.h"
#include"Monster03_Bullet_Multishot.h"

#include"Trail_Monster03.h"
#include"Effect_Monster03_Shot.h"
#include"Particle_Monster03_Shot.h"
#include"Effect_Monster03_Multishot.h"

/*Monster04*/
#include"Monster04.h"
#include"Monster04_Body.h"
#include"Monster04_Head.h"
#include"Monster04_Sword.h"
#include"Trail_Monster04.h"

/*For Monster05*/
#include"Monster05.h"
#include"Monster05_Gun.h"
#include"Monster05_Bullet.h"
#include"Monster05_Body.h"
#include"Monster05_Head.h"

#include"Effect_Monster05_Shot.h"
#include"Effect_Monster05_Ready.h"
#include"Trail_Monster05.h"
/*For Monster06*/
#include"Monster06.h"
#include"Monster07.h"
/*ForMonster 09*/
#include"Monster09_Main.h"
#include"Monster09.h"
#include"Monster09_Body.h"
#include"Monster09_Head.h"
#include"Monster09_Shot.h"
#include"Monster09_Wave.h"
#include"Monster09_Rush.h"
#include"Particle_Splash.h"
#include"Monster09_Mini.h"
#include"Monster09_Mini_Shot.h"
#include"Trail_Monster09.h"

//Monster09_Particle 
#include"Particle_Monster09_Attack.h"
#include"Particle_Monster09_Moving.h"
#include"Effect_Monster09_Moving.h"
#include"Particle_Monster09_Shot.h"
#include"Effect_Monster09_ShotDown.h"
#include"Particle_Monster09_Shot_Charging.h"

#include"Particle_Monster09_Mini.h"

#include"Monster09_WarningSign.h"
#include"Effect_Monster09_AngelRing.h"

/*DropItem*/
#include"DropItem_Money.h"
#include"DropItem_Grenade.h"
#include"DropItem_Bullet_Special.h"
#include"DropItem_Bullet_Standard.h"

/*For Stage*/
#include"Terrain_Stage1.h"
#include"Terrain_Stage02.h"
#include"Water_Stage02.h"
#include"Sky02.h"
/*For Player*/
/*Player01*/
#include"Player.h"
#include"Hands01.h"
#include"Hands01_Sword.h"
#include"Particle_Player01_On.h"
#include"Particle_Player01_Hit.h"
/*Player02*/
#include"Hands02.h"
#include"Hands02_Bullet.h"
#include"Particle_Player02_Bullet.h"
#include"Particle_Player02_Hit.h"

/*For Player03*/
#include"Hands03.h"
#include"Particle_Player03_Hit.h"
#include <time.h>
/*For Player04*/
#include"Hands04.h"
#include"Particle_Player04_Rasor.h"
#include"Hands04_Rasor.h"
/*For Player05*/
#include"Hands05.h"
#include"Hands05_Bullet.h"
#include"Particle_Player05_Bullet_Chasing.h"
#include"Particle_Player05_Bomb.h"

/*For Player Skill*/
#include"Hands_Skill.h"
#include"Hands_Skill_Granade.h"
#include"Hands_Skill_Granade_Bomb.h"
#include"Particle_Player_Skill_Poison.h"
/*Particle*/
#include"Particle_BackGround.h"

/*Trail...*/
#include"Trail_Test.h"

#include"AsyncLoader.h"

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
	switch (eNextLevelID) {
	case LEVEL_LOGO:
		m_AsyncLoaders_Logo.resize(18);
		for (int i = 0; i < 18; ++i)
			m_AsyncLoaders_Logo[i] = CAsyncLoader::Create();
		break;
	case LEVEL_GAMEPLAY:
		m_AsyncLoaders_GamePlay1.resize(11);
		for (int i = 0; i < 11; ++i)
			m_AsyncLoaders_GamePlay1[i] = CAsyncLoader::Create();
		break;
	}

	


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
	case LEVEL_GAMEPLAY2:
		hr = Loading_Level_GamePlay2();
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
static HRESULT annyung() {  return S_OK;  };
HRESULT CLoader::Loading_Level_Logo()
{
	
	m_AsyncLoaders_Logo[0]->Begin_Thread(
	[&]()->HRESULT {

		m_pGameInstance->PlayBGM(L"45-main-theme-e-101soundboards.mp3", 0.05f);
		/*For UI*/
		{
			CBackGround* pMyBackGround = static_cast<CBackGround*>(m_pGameInstance->Get_UI_By_Tag(TEXT("BackGround")));
			if (nullptr == pMyBackGround)
				return E_FAIL;
			pMyBackGround->Set_Static();
		}



		lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

		/*_matrix matOK = XMMatrixIdentity();
		CModel::Make_Binary_NonAnim(CModel::TYPE_NONANIM, "../Bin/resources/Models/Monster/Monster09/Monster09_Shot/Monster09_Shot.fbx"
			,TEXT("../Bin/resources/Models/Monster/Monster09/Monster09_Shot/Monster09_Shot_Anim.dat"), matOK);*/


			/*Test Trail*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail_Test"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Trail/Trail_Test/Trail_Test.png"), 1))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Logo */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Default%d.jpg"), 2))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GlowPurple"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/GlowPurple.png")))))
			return E_FAIL;
		/*For UI*/
		Load_UI();
		AddUpdateCnt_Static();
		return S_OK;
		}
	);
	/*For UI*/
	

	
	m_AsyncLoaders_Logo[1]->Begin_Thread(
		[&]()->HRESULT {
	/*For Button GameStart*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_UI_Button_GameStart"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UI_Logo/UI_Button_GameStart.png"), 1))))
		return E_FAIL;
	/*For Button Status*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_UI_Button_Status"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UI_Logo/UI_Button_Status.png"), 1))))
		return E_FAIL;
	/*For Button GameEnd*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_UI_Button_GameEnd"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UI_Logo/UI_Button_GameEnd.png"), 1))))
		return E_FAIL;
	/* <--- Texture For_Particle------------------------>*/
	/*Arm Particle Textiure*/
	/*Arm03_Texture Fire Particle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player03_Hit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm03_Fire/Arm03_Hit/Arm3_Hit%d.png"), 15))))
		return E_FAIL;

	/*Arm02_Texture Bullet Particle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player02_Bullet"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm02/Arm02_Bullet.png"),1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player02_Hit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm02/Arm02_Hit/Arm02_Hit%d.png"), 7))))
		return E_FAIL;

	/*Arm01_Texture Particle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player01_1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm01/Arm01_Particle1/Arm01_Particle1_%d.png"), 16))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player01_2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm01/Arm01_Particle2/Arm01_Particle2_%d.png"), 16))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player01_3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm01/Arm01_Particle3/Arm01_Particle3_%d.png"), 16))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player01_4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm01/Arm01_Particle4/Arm01_Particle4_%d.png"), 16))))
		return E_FAIL;

	/*Arm05_Texture_Particle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player05_Bullet_Chasing"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm05/Particle_Chasing/ChasingWater_%d.png"),4))))
		return E_FAIL;
	/*Arm04_Texture_Particle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player04_Rasor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm04/Arm04_Bullet.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player04_Rasor_Glow_Trail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm04/Arm04_ArmTrail_%d.png"), 2))))
		return E_FAIL;

	/*Arm_Skill_Texture_Particle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Player_Skill_Poison"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm_Skill/images/Particle_Poison_%d.png"), 16))))
		return E_FAIL;
	/* Arm_Skill_Texture_Poison_Plane*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Skill_Poison_Plane"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Particle/Arm_Skill/Poison_Plane.png"), 1))))
		return E_FAIL;


	AddUpdateCnt_Static();
	return S_OK;
		}
	);

	m_AsyncLoaders_Logo[2]->Begin_Thread(
		[&]()->HRESULT {
	/*Texture_Arm_Glow*/
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Glow"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Player/Player02/hero_fpp_101_A_s.png"), 1))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player01_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Player/Player01/1603_c.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player02_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Player/Player02/weapon_1410_s.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player03_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Player/Player03/weapon_1107_s.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player04_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Player/Player04/1409_s.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player05_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Player/Player05/weapon_1302_s.png"), 1))))
		return E_FAIL;


	/*Glow ItemBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ItemBox_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/NPC/ItemBox/prop_chest001_s.png"), 1))))
		return E_FAIL;


	/*Monster Desolve*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Desolve"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Monster/Monster_Desolve.png"), 1))))
		return E_FAIL;

	AddUpdateCnt_Static();
	return S_OK;
		}
	);

	//lstrcpy(m_szLoadingText, TEXT("사운드를 로딩중입니다."));
	m_AsyncLoaders_Logo[3]->Begin_Thread(
		[&]()->HRESULT {
	lstrcpy(m_szLoadingText, TEXT("콜라이더를 로딩중입니다."));
	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;
	/*For.Prototype_Component_Collider_Ray*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Ray"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_RAY))))
		return E_FAIL;
	AddUpdateCnt_Static();
	return S_OK;
		}
	);

	m_AsyncLoaders_Logo[4]->Begin_Thread(
		[&]()->HRESULT {
	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	/* For.Prototype_Component_Shader_VtxPosTex */
	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
	/*ForSkybox*/
 
	

	/*For Cube Collider*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeCol"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCubeCol.hlsl"), VTXCUBECOL::Elements, VTXCUBECOL::iNumElements))))
		return E_FAIL;

	/*For NonAnim Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	/*For Anim Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/*For VTXCube*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxRectInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxRectInstance.hlsl"), VTXRECTINSTANCE::Elements, VTXRECTINSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxPointInstance.hlsl"), VTXPOINTINSTANCE::Elements, VTXPOINTINSTANCE::iNumElements))))
		return E_FAIL;

	AddUpdateCnt_Static();
	return S_OK;
		}
	);

	LOGO_LOADING_BEGIN(5)
	/*For TrailShader Glow*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_TrailGlow"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxPosTex_TrailGlow.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_Glow"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxPosTex_Glow.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
	AddUpdateCnt_Static();
	LOADING_END
	
	LOGO_LOADING_BEGIN(6)
	/*Player Glow */
	/*For Player01*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh_Player01_Glow"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh_Player01_Glow.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	/*ForPlayer02*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh_Player02_Glow"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh_Player02_Glow.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	/*For Player03*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh_Player03_Glow"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh_Player03_Glow.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	AddUpdateCnt_Static();
	LOADING_END


	m_AsyncLoaders_Logo[7]->Begin_Thread(
		[&]()->HRESULT {
	/*For Player04*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh_Player04_Glow"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh_Player04_Glow.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	
	/*For Player04_Rasor*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_Hands04_Rasor"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxPosTex_Hands04_Rasor.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
	AddUpdateCnt_Static();
	return S_OK;
		}
	);
	m_AsyncLoaders_Logo[8]->Begin_Thread(
		[&]()->HRESULT {
	/*ForPlayer05*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh_Player05_Glow"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh_Player05_Glow.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	AddUpdateCnt_Static();
	/*ForNpcGlow_ItemBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh_Npc_ItemBox"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh_Npc_ItemBox.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	AddUpdateCnt_Static();
	return S_OK;
		}
	);

	m_AsyncLoaders_Logo[9]->Begin_Thread(
		[&]()->HRESULT {
	lstrcpy(m_szLoadingText, TEXT("모델(을)를 로딩중입니다."));
	

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Flat"),
		CVIBuffer_Flat::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*For Orototype_Component_VIBuffer_Cube*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For Prototype collider For Test*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Collider"),
		CVIBuffer_Collider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"),
		CVIBuffer_Trail::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	AddUpdateCnt_Static();
	return S_OK;
		}
	);
	m_AsyncLoaders_Logo[10]->Begin_Thread(
		[&]()->HRESULT {
	_matrix PreTransformMatrix = XMMatrixIdentity();
	
	
	/*"../Bin/resources/Models/Player/Player_Skill/PlayerSkill_Granade/Monster09.fbx", PreTransformMatrix
		, L"../Bin/resources/Models/Player/Player_Skill/PlayerSkill_Granade/Player_Skill_Anim.dat"*///0.7
  	// CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/DropItem/Money/DropItem_Money.fbx", TEXT("../Bin/resources/Models/DropItem/Money/DropItem_Money_Anim.dat"), PreTransformMatrix);
	

	 //../Bin/resources/Models/DropItem/Bullet_Standard/DropItem_Bullet_Standard.fbx
	//CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/DropItem/Bullet_Standard/DropItem_Bullet_Standard.fbx", TEXT("../Bin/resources/Models/DropItem/Bullet_Standard/DropItem_Bullet_Standard_Anim.dat"), PreTransformMatrix);
	//CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/DropItem/Bullet_Special/DropItem_Bullet_Special.fbx", TEXT("../Bin/resources/Models/DropItem/Bullet_Special/DropItem_Bullet_Special_Anim.dat"), PreTransformMatrix);
	
	/* For Player01 ok 칼*/
	PreTransformMatrix = XMMatrixTranslation(-1.3f, -1.0f, -1.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player01/Player01.dat", false
		))))
		return E_FAIL;


	/* For Player02  0k 파란장갑*/
	PreTransformMatrix = XMMatrixTranslation(0.f, 0.0f, 0.5f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player02/Player02.dat", false
		))))
		return E_FAIL;


	/* For Player03  OK호랑이총*/
	PreTransformMatrix = XMMatrixTranslation(-0.5f, -0.8f, -0.8f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player03/Player03.dat", false
		))))
		return E_FAIL;


	/* For Player04 0k  캐논*/
	PreTransformMatrix = XMMatrixTranslation(0.f, -0.2f, -0.0f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player04/Player04.dat", false
		))))
		return E_FAIL;

	/* For Player05 ok 리빙건*/
	PreTransformMatrix = XMMatrixTranslation(0.f, -0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player05/Player05.dat", false
		))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixTranslation(0.15F, 0, 0.2F) * XMMatrixRotationX(XMConvertToRadians(110.F)) * XMMatrixRotationZ(XMConvertToRadians(180.F));
  	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player05_Bullet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player05/Player05_Bullet/Player05_Bullet_Anim.dat", false
		))))
		return E_FAIL;

	AddUpdateCnt_Static();
	return S_OK;
		}
	);
	m_AsyncLoaders_Logo[11]->Begin_Thread(
		[&]()->HRESULT {
	_matrix PreTransformMatrix = XMMatrixIdentity();
	
	PreTransformMatrix = XMMatrixTranslation(17.6F, 5.6f, 56.7f);

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player_Skill"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player_Skill/PlayerSkill_Granade/Player_Skill_Anim.dat", false
		))))
		return E_FAIL;

	 PreTransformMatrix = XMMatrixIdentity();
	 /*DropItem+ Skill*/
	  if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player_Skill_Granade"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Player/Player_Skill/Dropltem_Granade/Monster09.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Player/Player_Skill/Dropltem_Granade/Granade_Anim.dat", false
		))))
		return E_FAIL;
	  
	 AddUpdateCnt_Static();
	 return S_OK;
		}
	);
	/*DropItem*/
	m_AsyncLoaders_Logo[12]->Begin_Thread(
		[&]()->HRESULT {
			_matrix PreTransformMatrix = XMMatrixIdentity();
	/*Bullet Special*/
	  if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_DropItem_Bullet_Special"),
		  CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM,nullptr, PreTransformMatrix
			  , L"../Bin/resources/Models/DropItem/Bullet_Special/DropItem_Bullet_Special_Anim.dat", false
		  ))))
		  return E_FAIL;
	  /*Bullet Standard*/
	  if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_DropItem_Bullet_Standard"),
		  CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			  , L"../Bin/resources/Models/DropItem/Bullet_Standard/DropItem_Bullet_Standard_Anim.dat", false
		  ))))
		  return E_FAIL;
	//Money
	  PreTransformMatrix = XMMatrixTranslation(0.f, 0.f, 0.7f);
	  if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_DropItem_Money"),
		  CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			  , TEXT("../Bin/resources/Models/DropItem/Money/DropItem_Money_Anim.dat"), false
		  ))))
		  return E_FAIL;
	  /*NPC*/
	 /* CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/NPC/ItemBox/ItemBox.fbx", TEXT("../Bin/resources/Models/NPC/ItemBox/ItemBox_Anim.dat"), PreTransformMatrix);
	  CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/NPC/Shop/Shop.fbx", TEXT("../Bin/resources/Models/NPC/Shop/Shop_Anim.dat"), PreTransformMatrix);
	  CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/NPC/Smith/Smith.fbx", TEXT("../Bin/resources/Models/NPC/Smith/Smith_Anim.dat"), PreTransformMatrix);
	 */
	  PreTransformMatrix = XMMatrixTranslation(38.7F, 1.f, 20.f);
	  if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NPC_ItemBox"),
		  CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			  , TEXT("../Bin/resources/Models/NPC/ItemBox/ItemBox_Anim.dat"), false
		  ))))
		  return E_FAIL;
	  PreTransformMatrix = XMMatrixIdentity();
	  if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NPC_Shop"),
		  CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			  , TEXT("../Bin/resources/Models/NPC/Shop/Shop_Anim.dat"), false
		  ))))
		  return E_FAIL;
	  PreTransformMatrix = XMMatrixTranslation(66.5F, -12.f, 90.5f);
	  if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NPC_Smith"),
		  CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, nullptr, PreTransformMatrix
			  , TEXT("../Bin/resources/Models/NPC/Smith/Smith_Anim.dat"), false
		  ))))
		  return E_FAIL;
	 AddUpdateCnt_Static();
	 return S_OK;
		}
	);
	m_AsyncLoaders_Logo[13]->Begin_Thread(
		[&]()->HRESULT {
	/*For ParticleInstance Splash*/
	CVIBuffer_Particle_Instance::PARTICLE_DESC			SplashDesc{};

	SplashDesc.iNumInstances = 700;
	SplashDesc.vCenter = _float3(0.f, 0.f, 0.f);
	SplashDesc.vRange = _float3(10.f, 5.f, 0.5f);
	SplashDesc.vSpeed = _float2(4.f, 20.f);
	SplashDesc.vLifeTime = _float2(0.3f, 1.f);
	SplashDesc.vSize = _float2(1.f, 1.f);
	SplashDesc.vPivot = _float3(0.f, 0.f, 0.f);
	SplashDesc.isLoop = false;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Splash"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, SplashDesc))))
		return E_FAIL;
	/*ForParticle_ Arm01*/
	CVIBuffer_Particle_Instance::PARTICLE_DESC    Player01_Particle_On_Desc{};
	Player01_Particle_On_Desc.iNumInstances = 15;
	Player01_Particle_On_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Player01_Particle_On_Desc.vRange = _float3(.7f, .7f, 0.7f);
	Player01_Particle_On_Desc.vSpeed = _float2(1.f, 1.5f);
	Player01_Particle_On_Desc.vLifeTime = _float2(1.f, 2.f);
	Player01_Particle_On_Desc.vSize = _float2(0.1f, 0.1f);
	Player01_Particle_On_Desc.vPivot = _float3(1000.f, 0.f, 0.f);
	Player01_Particle_On_Desc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player01_On"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Player01_Particle_On_Desc))))
		return E_FAIL;

	CVIBuffer_Particle_Instance::PARTICLE_DESC    Player01_Particle01_Hit_Desc{};
	Player01_Particle01_Hit_Desc.iNumInstances = 6;
	Player01_Particle01_Hit_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Player01_Particle01_Hit_Desc.vRange = _float3(1.f, 1.f, 1.f);
	Player01_Particle01_Hit_Desc.vSpeed = _float2(1.f, 2.f);
	Player01_Particle01_Hit_Desc.vLifeTime = _float2(0.5f, 0.9f);
	Player01_Particle01_Hit_Desc.vSize = _float2(0.3f, 0.3f);
	Player01_Particle01_Hit_Desc.vPivot = _float3(-10.f,0.f , 0.f);
	Player01_Particle01_Hit_Desc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player01_Hit"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Player01_Particle01_Hit_Desc))))
		return E_FAIL;

	/*For Particle Instance Arm02 Bullet Shoot*/
	CVIBuffer_Particle_Instance::PARTICLE_DESC    Player02_Particle_Bullet_Desc{};
	Player02_Particle_Bullet_Desc.iNumInstances = 1;
	Player02_Particle_Bullet_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Player02_Particle_Bullet_Desc.vRange = _float3(0.f, 0.f, 0.f);
	Player02_Particle_Bullet_Desc.vSpeed = _float2(5.f, 8.f);
	Player02_Particle_Bullet_Desc.vLifeTime = _float2(10.f, 10.f);
	Player02_Particle_Bullet_Desc.vSize = _float2(1.f, 1.f);
	Player02_Particle_Bullet_Desc.vPivot = _float3(0.f, 0.f, 0.f);
	Player02_Particle_Bullet_Desc.isLoop = true;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player02_Bullet"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Player02_Particle_Bullet_Desc))))
		return E_FAIL;
	
	CVIBuffer_Particle_Instance::PARTICLE_DESC    Player02_Particle_Hit_Desc{};
	Player02_Particle_Hit_Desc.iNumInstances = 4;
	Player02_Particle_Hit_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Player02_Particle_Hit_Desc.vRange = _float3(1.f, 1.f, 1.f);
	Player02_Particle_Hit_Desc.vSpeed = _float2(0.5f, 1.f);
	Player02_Particle_Hit_Desc.vLifeTime = _float2(0.5f, 1.f);
	Player02_Particle_Hit_Desc.vSize = _float2(0.3f, 0.3f);
	Player02_Particle_Hit_Desc.vPivot = _float3(-10.f, 0.f, 0.f);
	Player02_Particle_Hit_Desc.isLoop = true;
   	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player02_Hit"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Player02_Particle_Hit_Desc))))
		return E_FAIL;



	/*For Particle Instance Arm_03 Hit-go down*/
	CVIBuffer_Particle_Instance::PARTICLE_DESC    Player03_HitDesc{};

	Player03_HitDesc.iNumInstances = 15;
	Player03_HitDesc.vCenter = _float3(0.f, 0.f, 0.f);
	Player03_HitDesc.vRange = _float3(1.0f, 1.0f, 1.0f);
	Player03_HitDesc.vSpeed = _float2(3.f, 5.f);
	Player03_HitDesc.vLifeTime = _float2(0.2f, 0.8f);
	Player03_HitDesc.vSize = _float2(0.3f, 0.3f);
	Player03_HitDesc.vPivot = _float3(0.f, -0.5f, 0.f);
	Player03_HitDesc.isLoop = false;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player03_Hit"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Player03_HitDesc))))
		return E_FAIL;

	/*For Particle Instance Arm_05 Chasing*/
	CVIBuffer_Particle_Instance::PARTICLE_DESC    Player05_ChasingDesc{};

	Player05_ChasingDesc.iNumInstances = 6;
	Player05_ChasingDesc.vCenter = _float3(0.f, 0.f, 1.f);
	Player05_ChasingDesc.vRange = _float3(0.2f, 0.2f, 0.2f);
	Player05_ChasingDesc.vSpeed = _float2(3.f, 5.f);
	Player05_ChasingDesc.vLifeTime = _float2(0.2f, 0.8f);
	Player05_ChasingDesc.vSize = _float2(0.3f, 0.3f);
	Player05_ChasingDesc.vPivot = _float3(0.f, 0.f, 1.f);
	Player05_ChasingDesc.isLoop = true;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player05_Bullet_Chasing"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Player05_ChasingDesc))))
		return E_FAIL;

	CVIBuffer_Particle_Instance::PARTICLE_DESC    Player05_BombDesc{};

	Player05_BombDesc.iNumInstances = 60;
	Player05_BombDesc.vCenter = _float3(0.f, 0.f, 0.f);
	Player05_BombDesc.vRange = _float3(0.3f, 0.3f, 0.3f);
	Player05_BombDesc.vSpeed = _float2(6.f, 8.f);
	Player05_BombDesc.vLifeTime = _float2(0.2f, 0.5f);
	Player05_BombDesc.vSize = _float2(0.05f, 0.05f);
	Player05_BombDesc.vPivot = _float3(0.f, 0.f, 0.f);
	Player05_BombDesc.isLoop = false;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player05_Bullet_Bomb"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Player05_BombDesc))))
		return E_FAIL;
	/*For Particle Instance Arm04 Bullet Shoot*/
	CVIBuffer_Particle_Instance::PARTICLE_DESC    Player04_Particle_Rasor_Desc{};
	Player04_Particle_Rasor_Desc.iNumInstances = 1;
	Player04_Particle_Rasor_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Player04_Particle_Rasor_Desc.vRange = _float3(0.f, 0.f, 0.f);
	Player04_Particle_Rasor_Desc.vSpeed = _float2(5.f, 8.f);
	Player04_Particle_Rasor_Desc.vLifeTime = _float2(100.f, 100.f);
	Player04_Particle_Rasor_Desc.vSize = _float2(0.2f, 1.f);
	Player04_Particle_Rasor_Desc.vPivot = _float3(0.f, 0.f, 0.f);
	Player04_Particle_Rasor_Desc.isLoop = true;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player04_Rasor"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Player04_Particle_Rasor_Desc))))
		return E_FAIL;

	/*For Particle Instance Arm_Skill_Granade Bomb*/
	CVIBuffer_Particle_Instance::PARTICLE_DESC    Player_Skill_Poison_Particle_Desc{};
	Player_Skill_Poison_Particle_Desc.iNumInstances = 50;
	Player_Skill_Poison_Particle_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Player_Skill_Poison_Particle_Desc.vRange = _float3(1.f, 1.f, 1.f);
	Player_Skill_Poison_Particle_Desc.vSpeed = _float2(0.1f, 0.3f);
	Player_Skill_Poison_Particle_Desc.vLifeTime = _float2(0.2f, 0.6f);
	Player_Skill_Poison_Particle_Desc.vSize = _float2(0.1f, 0.5f);
	Player_Skill_Poison_Particle_Desc.vPivot = _float3(0.f, -10.f, 0.f);
	Player_Skill_Poison_Particle_Desc.isLoop = true;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Player_Skill_Granade_Bomb"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Player_Skill_Poison_Particle_Desc))))
		return E_FAIL;


	AddUpdateCnt_Static();
	return S_OK;
		}
	);

	m_AsyncLoaders_Logo[14]->Begin_Thread(
		[&]()->HRESULT {
	lstrcpy(m_szLoadingText, TEXT("객체원형(을)를 로딩중입니다."));
	/* For.Prototype_GameObject_BackGround */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_UI_LogoBackGround"),
	//	CBackGround::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_UI_Button_GameStart"),
		CUI_Button_GameStart::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_UI_Button_Status"),
		CUI_Button_Status::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_UI_Button_GameEnd"),
		CUI_Button_GameEnd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*<--------------------Particle------------------------->*/
	/* For.Prototype_GameObject_Particle_Explosion */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Splash"),
		CParticle_Splash::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*Player_Particle*/
	/*For Prototype_GameObject_Particle_Arm03_Hit*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player03_Hit"),
		CParticle_Player03_Hit::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*For Prototype_GameObject_Particle_Arm02_Bullet*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player02_Bullet"),
		CParticle_Player02_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player02_Hit"),
		CParticle_Player02_Hit::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For Prototype_GameObject_Particle_Arm01_On*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player01_On"),
		CParticle_Player01_On::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player01_Hit"),
		CParticle_Player01_Hit::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	AddUpdateCnt_Static();
	return S_OK;
		}
	);
	m_AsyncLoaders_Logo[15]->Begin_Thread(
		[&]()->HRESULT {
	/*Player*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands01"),
		CHands01::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands01_Sword"),
		CHands01_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands02"),
		CHands02::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands02_Bullet"),
		CHands02_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands03"),
		CHands03::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands04"),
		CHands04::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands04_Rasor"),
		CHands04_Rasor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands04_Particle_Player04_Rasor"),
		CParticle_Player04_Rasor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands05"),
		CHands05::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands05_Bullet"),
		CHands05_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player05_Bullet_Chasing"),
		CParticle_Player05_Bullet_Chasing::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player05_Bomb"),
		CParticle_Player05_Bomb::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands_Skill"),
		CHands_Skill::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands_Skill_Granade"),
		CHands_Skill_Granade::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Hands_Skill_Granade_Bomb"),
		CHands_Skill_Granade_Bomb::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player_Skill_Poison"),
		CParticle_Player_Skill_Poison::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	AddUpdateCnt_Static();
	return S_OK;
		}
	);
	m_AsyncLoaders_Logo[16]->Begin_Thread(
		[&]()->HRESULT {
	/*Drop Item*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_DropItem_Money"),
		CDropItem_Money::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_DropItem_Bulet_Standard"),
		CDropItem_Bullet_Standard::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_DropItem_Bullet_Special"),
		CDropItem_Bullet_Special::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_DropItem_Grenade"),
		CDropItem_Grenade::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*Npc*/
	AddUpdateCnt_Static();
	return S_OK;
		}
	);
	m_AsyncLoaders_Logo[17]->Begin_Thread(
		[&]()->HRESULT {
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Npc_02"),
		CNpc_Shop::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Npc_03"),
		CNpc_ItemBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Npc_01"),
		CNpc_Smith::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*Traill_Test*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Trail_Test"),
		CTrail_Test::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	return S_OK;
		}
	);
	while (1)
	{
		_bool isFinished = true;
		for (auto& el : m_AsyncLoaders_Logo)
			if (false == el->isFinished())isFinished = false;
		if (true == isFinished) {
			m_isFinished = true;
			for (auto& el : m_AsyncLoaders_Logo)
				Safe_Release(el);
			m_AsyncLoaders_Logo.clear();
			break;
		}
	}
	
	
	//m_isFinished = true;

	

	return S_OK;
}

HRESULT CLoader::Loading_Level_GamePlay()
{
	GAMEPLAY_LOADING_BEGIN(0)
	m_pGameInstance->PlayBGM(L"Bgm_Loading.mp3", 0.1f);
	_matrix PreTransformMatrix = XMMatrixIdentity();
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Explosion/Explosion%d.png"), 90))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect"),
		CSoftEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/SkyBox/Sky_%d.dds"), 6))))
		return E_FAIL;
	/*Objects*/
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Terrain/Tile0.dds"), 1))))
		return E_FAIL;
  	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkyBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/SkyBox/Sky_%d.dds"), 5))))
		return E_FAIL;
	Add_UpdateCnt_Stage_1();
	LOADING_END;

	GAMEPLAY_LOADING_BEGIN(1)
	/*Monster05 Bullets*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster05_Shot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster05/Monster05_Bullet/Monster05_Shot.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster05_Trail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster05/Monster05_Bullet/Monster05_Trail.png"), 5))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster05_Ready"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster05/Monster05_Bullet/Monster05_Ready.png"), 5))))
		return E_FAIL;
	/*mosnter03_Bullet*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster03_Shot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster03/Monster03_Bullet/Monster03_Shot.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster03_Trail_Circle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster03/Monster03_Bullet/Monster03_Trail_Circle.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster03_Particle_Shot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster03/Monster03_Bullet/Particle/Particle_Monster03_%d.png"), 16))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster03_Multishot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster03/Monster03_Bullet/Monster03_Multishot/Monster03_Multishot_0%d.png"), 4))))
		return E_FAIL;
	/*Monster02_SwordTrail*/
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Trail_Monster02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Trail/Trail_Monster02/Trail_Monster02.png"),1))))
		return E_FAIL;

	Add_UpdateCnt_Stage_1();

	LOADING_END;
	

	
	/*For Anim Model*/

	GAMEPLAY_LOADING_BEGIN(2)
	_matrix PreTransformMatrix = XMMatrixIdentity();


	lstrcpy(m_szLoadingText, TEXT("모델(을)를 로딩중입니다."));
	
	
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	/*For Monster01 Model*/
	lstrcpy(m_szLoadingText, TEXT(" Monster01 Model"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster01/Monster01.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster01/Monster01_Anim.dat", false
		))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT(" Monster02 Model"));
	/*For Monster02 Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster02/Monster02.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster02/Monster02_Anim.dat", false
		))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT(" Monster04 Model"));
	/*For Monster04 Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster04/Monster04.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster04/Monster04_Anim.dat", false
		))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT(" Monster05 Model"));
	/*For Monster05 Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster05/Monster05.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster05/Monster05_Anim.dat", false
		))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT(" Monster06 Model"));

	Add_UpdateCnt_Stage_1();
	LOADING_END;
	//////////////////////////////////////////////////////
	GAMEPLAY_LOADING_BEGIN(3)
	_matrix PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	/*For Monster06 Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster06"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster06/Monster06.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster06/Monster06_Anim.dat",false
		))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT(" Monster07 Model"));
	/*For Monster07 Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster07"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster07/Monster07.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster07/Monster07_Anim.dat"
		))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT(" Monster03 Model"));
	/*For Monster03 Model*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixTranslation(23.5, 0, 28) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster03/Monster03.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster03/Monster03_Anim.dat", false
		))))
		return E_FAIL;
	Add_UpdateCnt_Stage_1();
	LOADING_END;

	GAMEPLAY_LOADING_BEGIN(4)
	_matrix PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	lstrcpy(m_szLoadingText, TEXT("Prototype_Component_Model_Terrain_Stage01"));
	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Stage01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/resources/Models/Map/Map01_Stage01/Map01_Stage01.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Map/Map01_Stage01/Map01_Stage01.dat"
		))))
		return E_FAIL;
	Add_UpdateCnt_Stage_1();
	LOADING_END;

	GAMEPLAY_LOADING_BEGIN(5)
	/*For Particle Instance Monster_03 Shot*/
	CVIBuffer_Particle_Instance::PARTICLE_DESC    Particle_Monster03_ShotDesc{};
	Particle_Monster03_ShotDesc.iNumInstances = 30;
	Particle_Monster03_ShotDesc.vCenter = _float3(0.f, 1.f, 0.f);
	Particle_Monster03_ShotDesc.vRange = _float3(1.f, 1.f, 1.f);
	Particle_Monster03_ShotDesc.vSpeed = _float2(2.f, 3.f);
	Particle_Monster03_ShotDesc.vLifeTime = _float2(0.2f, 0.6f);
	Particle_Monster03_ShotDesc.vSize = _float2(0.1f, 0.3f);
	Particle_Monster03_ShotDesc.vPivot = _float3(0.f, -10.f, 0.f);
	Particle_Monster03_ShotDesc.isLoop = true;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Particle_Monster03_Shot"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Particle_Monster03_ShotDesc))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("네비게이션원형(을)를 로딩중입니다."));
	/* For.Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation.dat")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Model Completed"));

	lstrcpy(m_szLoadingText, TEXT("객체원형(을)를 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;*/

	/*For. Prototype_GameObject_CubeCollider*/
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CubeCollider"),
		CCubeCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*For Prototype_GameObject_Camera_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/*for monster01*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster01"),
		CMonster01::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster01_Head"),
		CMonster01_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster01_Body"),
		CMonster01_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Add_UpdateCnt_Stage_1();
	LOADING_END;
	GAMEPLAY_LOADING_BEGIN(6)
	/*for monster02*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster02"),
		CMonster02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster02_Sword"),
		CMonster02_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster02_Head"),
		CMonster02_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster02_Body"),
		CMonster02_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Trail_Monster02"),
		CTrail_Monster02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Add_UpdateCnt_Stage_1();
	LOADING_END;
	GAMEPLAY_LOADING_BEGIN(7)
	/*for monster03*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03"),
		CMonster03::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03_Head"),
		CMonster03_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03_Body"),
		CMonster03_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03_Gun"),
		CMonster03_Gun::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03_Bullet"),
		CMonster03_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03_Bullet_Multishot"),
		CMonster03_Bullet_Multishot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Add_UpdateCnt_Stage_1();
	LOADING_END;
	GAMEPLAY_LOADING_BEGIN(8)
	/*for monster04*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster04"),
		CMonster04::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster04_Head"),
		CMonster04_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster04_Body"),
		CMonster04_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster04_Sword"),
		CMonster04_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Add_UpdateCnt_Stage_1();
	LOADING_END;
	GAMEPLAY_LOADING_BEGIN(9)
	/*for monster05*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster05"),
		CMonster05::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster05_Gun"),
		CMonster05_Gun::Create(m_pDevice, m_pContext))))
			return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster05_Bullet"),
		CMonster05_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster05_Head"),
		CMonster05_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster05_Body"),
		CMonster05_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*for monster06*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster06"),
		CMonster06::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*for monster07*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster07"),
		CMonster07::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	

	Add_UpdateCnt_Stage_1();
	LOADING_END;

	GAMEPLAY_LOADING_BEGIN(10)
	/*ForStage 01*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain_Stage01"),
		CTerrain_Stage01::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	

	/*For Payer*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;






	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*fORpARTICLE_BackGround*/
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_BackGround"),
	//	CParticle_BackGround::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Splash"),
		CParticle_Splash::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	
	/*Effect*/

	/*Monster04*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Trail_Monster04"),
		CTrail_Monster04::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*Monster05*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Trail_Monster05"),
		CTrail_Monster05::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Monster05_Shot"),
		CEffect_Monster05_Shot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Monster05_Ready"),
		CEffect_Monster05_Ready::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*Monster03*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Trail_Monster03"),
		CTrail_Monster03::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Monster03_Shot"),
		CEffect_Monster03_Shot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Monster03_Shot"),
		CParticle_Monster03_Shot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Monster03_MultiShot"),
		CEffect_Monster03_MultiShot::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	Add_UpdateCnt_Stage_1();
	LOADING_END;
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));


	while (1)
	{
		_bool isFinished = true;
		for (auto& el : m_AsyncLoaders_GamePlay1)
			if (false == el->isFinished())isFinished = false;
		if (true == isFinished) {
			m_isFinished = true; break;
		}
	}

	for (auto& el : m_AsyncLoaders_GamePlay1)
		Safe_Release(el);
	m_AsyncLoaders_GamePlay1.clear();
	//m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Level_GamePlay2()
{

	m_pGameInstance->PlayBGM(L"Bgm_Loading.mp3", 0.1f);

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Water"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Water/Water.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Water_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Water/Water_Normal.png"), 1))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/SkyBox/Sky_%d.dds"), 6))))
		return E_FAIL;

	/*Texture For Boss*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Move"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Particle/Monster09/Move/Monster09_Move_%d.png"), 4))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Particle_Attack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Particle/Monster09/Particle_Attack/Monster09_Attack0_%d.png"), 4))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Particle_Moving"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Particle/Monster09/Particle_Moving/Monster09_Moving_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Particle_Rush_Wave"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Particle/Monster09/Rush_Wave/Monster09_Wave_%d.png"), 16))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Particle_Drop"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Particle/Monster09/Drop/Monster09_Drop_%d.png"), 4))))
		return E_FAIL;
	

	/*Monster09_Wave UV Noise Texture*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Wave_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster09/Monster09_Wave/UV_noise_wave_12.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Wave_Diffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster09/Monster09_Wave/Monster09_Wave.png"), 1))))
		return E_FAIL;


	/*Monter09_SHot UV_Noise_Teexture*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Shot_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster09/Monster09_Shot/Monster09_Shot_Noise.png"), 1))))
		return E_FAIL;


	/*Monster09_Wave_Glow*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Wave_Glow_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster09/Monster09_Wave/Wave_Glow_Mask.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_WaveEvent_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster09/Monster09_Wave/Final_Event_Noise.png"), 1))))
		return E_FAIL;

	/*Monster09_GlowMask*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Glow_Emissive_Eye"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster09/3921_mask.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Glow_Emissive_Body"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster09/3921_s.png"), 1))))
		return E_FAIL;

	/*Monster09_Trail_Texture*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Trail_Eyes"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Trail/Trail_Monster09/trail_red.png"), 1))))
		return E_FAIL;

	/*Texture Sign Before Rush*/



	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_SignBeforeRush_Moving"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/SignBeforeAttack/Monster09/SignBeforeRush_Moving.png"),1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_SignBeforeRush_Range"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/SignBeforeAttack/Monster09/SignBeforeRush_Range.png"), 1))))
		return E_FAIL;


	/*Monster09_SHOT*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Effect_Monster09_ShotDown"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Particle/Monster09/Particle_Attack/Monster09_Shot_Down.png"), 1))))
		return E_FAIL;



	/*WarningSign*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Effect_Monster09_ShotWarningCircle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster09/ring.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Effect_Monster09_A_Circle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Models/Monster/Monster09/Angel_Ring.png"), 1))))
		return E_FAIL;

	Add_UpdateCnt_Stage_2();
	lstrcpy(m_szLoadingText, TEXT("사운드를 로딩중입니다."));

 
	 
	 
	 
	 
	 
	  
	 
	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	
	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Shader_VtxWater"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxWater.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Shader_VtxAnim_Emissive_Monster09"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh_Monster09.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/*For Monster09_Rush*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Shader_VtxWater_Monster09_Rush"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxWater_Monster09_Rush.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	Add_UpdateCnt_Stage_2();
	/*For Anim Model*/

	lstrcpy(m_szLoadingText, TEXT("모델(을)를 로딩중입니다."));

	_matrix PreTransformMatrix = XMMatrixIdentity();
	

	lstrcpy(m_szLoadingText, TEXT("Prototype_Component_Model_Terrain_Stage02"));
	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Model_Terrain_Stage02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/resources/Models/Map/Map01_Stage02/Map01_Stage02fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Map/Map01_Stage02/Map01_Stage02.dat"
		))))
		return E_FAIL; 
	Add_UpdateCnt_Stage_2();

	/*For Monster09*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixTranslation(16.8, 0, 68);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Model_Monster09"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster09/Monster09.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster09/Monster09_Anim.dat", false
		))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixTranslation(5, 0, 10) * XMMatrixRotationY(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Model_Monster09_Mini"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster09/3921_mini/3921_mini.fbx", PreTransformMatrix
			, L"../Bin/resources/Models/Monster/Monster09/3921_mini/3921_mini.dat", false
		))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Model_Monster09_Shot"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/resources/Models/Monster/Monster09/Monster09_Shot/Monster09_Shot.fbx", PreTransformMatrix
			,TEXT("../Bin/resources/Models/Monster/Monster09/Monster09_Shot/Monster09_Shot_Anim.dat"), false
		))))
		return E_FAIL;
	
	Add_UpdateCnt_Stage_2();
	/*_matrix matOK = XMMatrixIdentity();
	CModel::Make_Binary_NonAnim(CModel::TYPE_NONANIM, "../Bin/resources/Models/Monster/Monster09/Monster09_Shot/Monster09_Shot.fbx"
		, TEXT("../Bin/resources/Models/Monster/Monster09/Monster09_Shot/Monster09_Shot_Anim.dat"), matOK);*/




	lstrcpy(m_szLoadingText, TEXT("네비게이션원형(을)를 로딩중입니다."));
	/* For.Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Client_NaviMesh_Stage02.dat")))))
		return E_FAIL;
	Add_UpdateCnt_Stage_2();
	lstrcpy(m_szLoadingText, TEXT("Model Completed"));

	lstrcpy(m_szLoadingText, TEXT("객체원형(을)를 로딩중입니다."));


	/*ForStage 01*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Terrain_Stage02"),
		CTerrain_Stage02::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Water_Stage02"),
		CWater_Stage02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Sky"),
		CSky02::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	Add_UpdateCnt_Stage_2();

	/*For Player*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*For Prototype_GameObject_Camera_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Main"),
		CMonster09_Main::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09"),
		CMonster09::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Body"),
		CMonster09_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Head"),
		CMonster09_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Shot"),
		CMonster09_Shot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Wave"),
		CMonster09_Wave::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Rush"),
		CMonster09_Rush::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Mini"),
		CMonster09_Mini::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Mini_Shot"),
		CMonster09_Mini_Shot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	Add_UpdateCnt_Stage_2();

	
	/*Particle ANDeFFECT*/
	/*Model*/
	CVIBuffer_Particle_Instance::PARTICLE_DESC    Monster09_Moving_Particle_Desc{};
	Monster09_Moving_Particle_Desc.iNumInstances = 40;
	Monster09_Moving_Particle_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Monster09_Moving_Particle_Desc.vRange = _float3(10.f, 1.f, 2.f);
	Monster09_Moving_Particle_Desc.vSpeed = _float2(15.f, 20.f);
	Monster09_Moving_Particle_Desc.vLifeTime = _float2(3.f, 4.f);
	Monster09_Moving_Particle_Desc.vSize = _float2(0.5f, 1.f);
	Monster09_Moving_Particle_Desc.vPivot = _float3(0.f, -0.1f, 0.f);
	Monster09_Moving_Particle_Desc.isLoop = true;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_VIBuffer_Particle_Monster09_Moving"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Monster09_Moving_Particle_Desc))))
		return E_FAIL;

	CVIBuffer_Particle_Instance::PARTICLE_DESC    Monster09_Attack_Particle_Desc{};
	Monster09_Attack_Particle_Desc.iNumInstances =40;
	Monster09_Attack_Particle_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Monster09_Attack_Particle_Desc.vRange = _float3(1.f, 1.f, 1.f);
	Monster09_Attack_Particle_Desc.vSpeed = _float2(1.f, 2.f);
	Monster09_Attack_Particle_Desc.vLifeTime = _float2(5.f, 5.f);
	Monster09_Attack_Particle_Desc.vSize = _float2(1.f, 1.f);
	Monster09_Attack_Particle_Desc.vPivot = _float3(0.f, -10.f, 0.f);
	Monster09_Attack_Particle_Desc.isLoop = true;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_VIBuffer_Particle_Monster09_Attack"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Monster09_Attack_Particle_Desc))))
		return E_FAIL;

	CVIBuffer_Particle_Instance::PARTICLE_DESC    Monster09_ShotDown_Particle_Desc{};
	Monster09_ShotDown_Particle_Desc.iNumInstances = 40;
	Monster09_ShotDown_Particle_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Monster09_ShotDown_Particle_Desc.vRange = _float3(1.f, 1.f, 1.f);
	Monster09_ShotDown_Particle_Desc.vSpeed = _float2(8.f, 10.f);
	Monster09_ShotDown_Particle_Desc.vLifeTime = _float2(3.f, 4.f);
	Monster09_ShotDown_Particle_Desc.vSize = _float2(2.f, 4.f);
	Monster09_ShotDown_Particle_Desc.vPivot = _float3(0.f, -100.f, 0.f);
	Monster09_ShotDown_Particle_Desc.isLoop = false;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_VIBuffer_Particle_Monster09_Shot_Down"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Monster09_ShotDown_Particle_Desc))))
		return E_FAIL;

	CVIBuffer_Particle_Instance::PARTICLE_DESC    Monster09_ShotCharging_Particle_Desc{};
	Monster09_ShotDown_Particle_Desc.iNumInstances = 40;
	Monster09_ShotDown_Particle_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Monster09_ShotDown_Particle_Desc.vRange = _float3(10.f, 10.f, 10.f);
	Monster09_ShotDown_Particle_Desc.vSpeed = _float2(10.f, 15.f);
	Monster09_ShotDown_Particle_Desc.vLifeTime = _float2(3.f, 4.f);
	Monster09_ShotDown_Particle_Desc.vSize = _float2(4.f, 5.f);
	Monster09_ShotDown_Particle_Desc.vPivot = _float3(0.f, 0.f, 0.f);
	Monster09_ShotDown_Particle_Desc.isLoop = false;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_VIBuffer_Particle_Monster09_Shot_Charging"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Monster09_ShotDown_Particle_Desc))))
		return E_FAIL;
	/*ParticleMonster09_Mini*/
	CVIBuffer_Particle_Instance::PARTICLE_DESC    Particle_Monster09_Mini_Desc{};
	Particle_Monster09_Mini_Desc.iNumInstances = 80;
	Particle_Monster09_Mini_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Particle_Monster09_Mini_Desc.vRange = _float3(1.f, 1.f, 1.f);
	Particle_Monster09_Mini_Desc.vSpeed = _float2(10.f, 15.f);
	Particle_Monster09_Mini_Desc.vLifeTime = _float2(3.f, 4.f);
	Particle_Monster09_Mini_Desc.vSize = _float2(1.f, 2.f);
	Particle_Monster09_Mini_Desc.vPivot = _float3(0.f,0.f, 0.f);
	Particle_Monster09_Mini_Desc.isLoop = false;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_VIBuffer_Particle_Monster09_Mini"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, Particle_Monster09_Mini_Desc))))
		return E_FAIL;



	/*원형*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Moving"),
		CParticle_Monster09_Moving::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Attack"),
		CParticle_Monster09_Attack::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Effect_Monster09_Moving"),
		CEffect_Monster09_Moving::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Shot_Down"),
		CParticle_Monster09_Shot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Effect_Monster09_Shot_Down"),
		CEffect_Monster09_ShotDown::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Shot_Charging"),
		CParticle_Monster09_Shot_Charging::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*Monster09_Trail*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Trail_Monster09"),
		CTrail_Monster09::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*Particle_Monster09_Mini*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Mini"),
		CParticle_Monster09_Mini::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_WarningSign_Shot"),
		CMonster09_WarningSign::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Angelring"),
		Effect_Monster09_AngelRing::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Add_UpdateCnt_Stage_2();

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Load_UI()
{
	/*For Mouse*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mouse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/Mouse.png"), 1))))
		return E_FAIL;
	/*For Mouse*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mouse_Target"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/TargetMouse.png"), 1))))
		return E_FAIL;
	/*For Numbers*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Numbers"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Numbers/Number_%d.png"), 12))))
		return E_FAIL;
	/* For Tools UI */
	Load_UI_Tool();
	Load_UI_Invectory();
	Load_UI_Shop();
	Load_UI_Money();
	Load_UI_Talk();
	Load_UI_Dialogue();
	Load_UI_Goblet();
	Load_UI_Keys();
	Load_UI_Dash();
	Load_UI_Victory();
	/*ForMonsterHP*/
	Load_UI_MonsterHP();
	/*객체 원형로딩*/
	Load_DemageFont();
	Load_UI_MiniMap();
	Load_UI_PlayerHP();
	Load_UI_BossHP();
	/*For Mouse*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Mouse"),
		CMouse::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*For Numbers -Part_UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"),
		CUI_Numbers::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoader::Load_UI_Tool()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Arm"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/Arms/Arm%d.png"), 6))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Equip"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/Arms/Equip%d.png"), 3))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponBG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/Arms/UI_WeaponBG.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BulletIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/Arms/UI_BulletIcon%d.png"), 3))))
		return E_FAIL;

	/*원형 객체*/
	/*새끼 UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arm_Paint"),
		CUI_Arm_Paint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arm1"),
		CUI_Arm1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arm2"),
		CUI_Arm2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arm3"),
		CUI_Arm3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* 부모 UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arms"),
		CUI_Arms::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CLoader::Load_UI_MonsterHP()
{
	/*For Texture*/
	//BG
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MonsterHp/MonsterHp_BG%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Blue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MonsterHp/MonsterHp_Blue.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MonsterHp/MonsterHp_Red.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Gray_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MonsterHp/MonsterHp_Gray_BG.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MonsterHp/bar.png"), 1))))
		return E_FAIL;


	/*For 원향 객체 */
	/*새끼 UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MonsterHP_BG"),
		CUI_MonsterHP_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MonsterHP_Red"),
		CUI_MonsterHP_Red::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MonsterHP_Blue"),
		CUI_MonsterHP_Blue::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterHP_Gray_BG"),
		CUI_MonsterHP_Gray_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	
		/*부모 UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MonsterHP"),
		CUI_MonsterHP::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_UI_BossHP()
{
	/*For Texture*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHP_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MonsterHp/Boss_Hp_Red.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHP_Gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MonsterHp/Boss_Hp_Gray.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHP_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MonsterHp/Boss_Hp_Back.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHP_Img"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MonsterHp/BossImg_0.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHP_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MonsterHp/BossHP_BG.png"), 1))))
		return E_FAIL;
	/*부모 UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_BossHP"),
		CUI_BossHP::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoader::Load_DemageFont()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_DemageFont"),
		CUI_DemageFont::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_UI_MiniMap()
{

	/*For Texture*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniMap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MiniMap/bg_minimap.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniMap_Level_Text"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MiniMap/bg_minimap_level.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniMap_Monster"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MiniMap/icon_monster.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniMap_Player"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MiniMap/icon_own.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniMap_Portal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/MiniMap/icon_door_nextlevel.png"), 1))))
		return E_FAIL;
	/*객체 원형*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MiniMap_Main"),
		CUI_MiniMapMain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_UI_PlayerHP()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerHP_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/PlayerBar/Bar_Character_HPBar_1.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerHP_Green"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/PlayerBar/Bar_Character_HPBar_2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerHP_BackBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/PlayerBar/Bar_Character_Bg_2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerHP_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/PlayerBar/bg_tips.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI__UI_PlayerHP_Player"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/PlayerBar/icon_hero2_3.png"), 1))))
		return E_FAIL;
	/*객체 원형*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Player_HP"),
		CUI_Player_HP::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoader::Load_UI_Invectory()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Inventory/Inventory_Background.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_SelectedGlow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Inventory/Selected_Glow.png"), 1))))
		return E_FAIL;
	


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Button_Weapons"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Inventory/Button_Weapons.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_WeaponBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Inventory/WeaponBox.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Status_Explain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Inventory/Status_Explain.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Button_Next"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Button/Button_Next.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Button_Close"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Button/Button_Close.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Button_Select"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Button/Button_Select.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_WeaponBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Inventory/WeaponBackground.png"), 1))))
		return E_FAIL;
	/*영웅*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Button_Hero"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Inventory/Button_Hero.png"), 1))))
		return E_FAIL;


	/*객체 원형*/

	/*Main*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory"),
		CUI_Inventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*button*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Button_Weapons"),
		CUI_Inventory_Button_Weapons::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Hero"),
		CUI_Inventory_Button_Hero::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapons"),
		CUI_Inventory_Weapons::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapon_Move_Button"),
		CUI_Inventory_Weapon_Move_Button::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapon_Explain"),
		CUI_Inventory_Weapon_Explain::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_Weapon_Equip_Button"),
		CUI_Weapon_Equip_Button::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_WeaponPicture"),
		CUI_Inventory_WeaponPicture::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Inventory_StatusText"),
		CUI_Inventory_Weapon_StatusText::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoader::Load_UI_Shop()
{

	/*Texture*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Item_Slot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Shop/Item_Slot.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Item"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Shop/Shop_Item_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Item_ShowCard"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Shop/Item_ShowCard_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Sold_Out"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Shop/SoldOut.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Coin"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Shop/Shop_Coin%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Shop_Click"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Shop/Shop_Click.png"), 1))))
		return E_FAIL;
	/* 객체 원형*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop"),
		CUI_Shop::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_Item_Button"),
		CUI_Shop_Button_Item::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_ShowCard"),
		CUI_Shop_ShowCard::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	return S_OK;
}

HRESULT CLoader::Load_UI_Money()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Icon_Coin"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Money/icon_Coin.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Icon_bullets"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Money/icon_Bullets.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Properties"),
		CUI_Coin_And_Bullets::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoader::Load_UI_Talk()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Talk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Talk/Talk_F.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Talk"),
		CUI_Talk::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoader::Load_UI_Dialogue()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Dialogue/Dialogue.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_Button_Close"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Dialogue/Dialogue_Close.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_OK"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Dialogue/Dialogue_OK.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Dialogue"),
		CUI_Dialogue::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Dialogue_OK_Button"),
		CUI_Dialogue_OK_Button::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Dialogue_Cancle_Button"),
		CUI_Dialogue_Cancle_Button::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	return S_OK;
}

HRESULT CLoader::Load_UI_Goblet()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_LoadPlus"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_ItemBox/Goblet_Grenade_LoadPlus.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_CirticalDemage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_ItemBox/Goblet_Grenade_CriticalDemage.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_Range"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_ItemBox/Goblet_Grenade_Range.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_CriticalChance"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_ItemBox/Goblet_Grenade_Critical.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_Demag"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_ItemBox/Goblet_Grenade_Demage.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_HPPlus"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_ItemBox/Goblet_HPPlus.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_BG_Grenade"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_ItemBox/Goblet_BG_Grenade.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_BG_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_ItemBox/Goblet_BG_HP.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Title"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_ItemBox/Golden_Goblet.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Goblet_Grenade_Reinforce"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_ItemBox/Grenade_Reinforce.png"), 1))))
		return E_FAIL;
	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Goblet"),
		CUI_Goblet::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Goblet_Button"),
		CUI_Goblet_RandomOptionButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	
	return S_OK;
}

HRESULT CLoader::Load_UI_Keys()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Key_Tab"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_SKILL/Key_Tab.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Key_Shift"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_SKILL/Key_Shift.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Key_Q"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_SKILL/Key_Q.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BG_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_SKILL/BG_Skill.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Icon_Bomb"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_SKILL/Icon_Bomb_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Icon_Dash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_SKILL/Icon_Dash_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Icon_Inven"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_SKILL/Icon_Inven.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Tab"),
		CUI_Tab::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Bomb"),
		CUI_Bomb::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Dash"),
		CUI_Dash::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_UI_Dash()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Dash/UI_Dash.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Dash_Screen"),
		CUI_Dash_Screen::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoader::Load_UI_Victory()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Victory"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Victory/UI_Victory.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Victory_Part"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Victory/UI_Victory_Part.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Victory_Circle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/UI_Victory/UI_Victory_Circle.png"), 1))))
		return E_FAIL;
	/*객체원형*/


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Victory"),
		CUI_Victory::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Victory_Circle"),
		CUI_Victory_Circle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

void CLoader::AddUpdateCnt_Static()
{
	m_fNowRatio +=(1.f / (_float)m_iMaxStatic_Cnt);
	if (m_fNowRatio >= 1.f)
		m_fNowRatio = 1.f;
	CBackGround* pMyBackGround = static_cast<CBackGround*>(m_pGameInstance->Get_UI_By_Tag(TEXT("BackGround")));
	if (nullptr == pMyBackGround)
		return;
	pMyBackGround->Set_Ratio(m_fNowRatio);

}

void CLoader::Add_UpdateCnt_Stage_1()
{

	m_fNowRatio += 0.5f * (1.f / (_float)m_iMaxStage1_Cnt);
	if (m_fNowRatio >= 0.5f)
		m_fNowRatio = 0.5f;
	CBackGround* pMyBackGround = static_cast<CBackGround*>(m_pGameInstance->Get_UI_By_Tag(TEXT("BackGround")));
	if (nullptr == pMyBackGround)
		return;
	pMyBackGround->Set_Ratio(m_fNowRatio);
	
}

void CLoader::Add_UpdateCnt_Stage_2()
{
	m_fNowRatio += 0.5f * (1.f / (_float)m_iMaxStage2_Cnt);

	if (m_fNowRatio >= 1.f)
		m_fNowRatio = 1.f;

	CBackGround* pMyBackGround = static_cast<CBackGround*>(m_pGameInstance->Get_UI_By_Tag(TEXT("BackGround")));
	if (nullptr == pMyBackGround)
		return;
	pMyBackGround->Set_Ratio(m_fNowRatio+0.5f);
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
