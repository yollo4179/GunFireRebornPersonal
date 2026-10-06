#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include"CubeCollider.h"
#include"Terrain.h"
#include"Monster.h"
#include"Terrain_Base.h"
#include"Flat_Virtual_Terrain.h"
#include"Terrain_Stage1.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camara"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_GameObject(TEXT("Layer_GameObject"))))
		return E_FAIL;


	if (FAILED(Ready_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	m_pImguiMgr = CImguiWindow::Create(m_pDevice, m_pContext);



	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{

 	if (MOUSE_UP(MOUSE::DIM_LB) &&m_bCreateMode && CREATE_OBJECT_MODE::COLLIDER==m_eCreateMode) {
		Picking_And_Create_Collider();
	}

	if(MOUSE_UP(MOUSE::DIM_LB) && m_bCreateMode && CREATE_OBJECT_MODE::MONSTER == m_eCreateMode) {
		Creating_Monsters();
	}

	if (MOUSE_UP(MOUSE::DIM_LB) && m_bCreateMode && CREATE_OBJECT_MODE::NAVI_MESH_VTX == m_eCreateMode)
	{
		if (false == m_bMake_NaviMeshOnVirtualTerrain)
		{
			Creating_Vertex();
		}
		else if (true == m_bMake_NaviMeshOnVirtualTerrain)
		{
			Creating_Vertex_On_Virtual_Terrain();
		}
	}

	
	
}

HRESULT CLevel_GamePlay::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
#endif
	
	if (nullptr == m_pImguiMgr)return E_FAIL;
	if (nullptr == m_pGameInstance)return E_FAIL;


	if (FAILED(m_pImguiMgr->Start_Render()))return E_FAIL;

	IMGUI_SELCT_STAGE();
	IMGUI_SelectMode();
	
	if (FAILED(m_pImguiMgr->End_Render()))return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring & strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC		Desc{};

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(180.f);
	Desc.fMouseSensor = 0.1f;
	Desc.fFovy = XMConvertToRadians(60.f);
	Desc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.vEye = _float3(0.f, 4.f, 0.f);
	Desc.vAt = _float3(0.f, 4.f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_GAMEPLAY, strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring & strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, strLayerTag, nullptr ,reinterpret_cast<CGameObject**>(& m_pTerrain) )))
		return E_FAIL;
	if (nullptr != m_pTerrain)
	{
		m_pPos = m_pTerrain->Get_Vertices();
		m_VerticesX = m_pTerrain->Get_VerticesX();
		m_VerticesZ = m_pTerrain->Get_VerticesZ();
	}
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain_Stage01"), LEVEL_GAMEPLAY, strLayerTag, nullptr, reinterpret_cast<CGameObject**>(&m_pBaseTerrain))))
		return E_FAIL;
	
	m_Triangles = static_cast<CTerrain_Stage01*>(m_pBaseTerrain)->m_vecTriangles;

	CFlat_Virtual_Terrain::FLAT_VIRTUAL_TERRAIN_DESC FlatDesc = {};
	{
		FlatDesc.fSizeX = 1;
		FlatDesc.fRotation = 1;
		FlatDesc.iNumZ = 20;
		FlatDesc.iNumX = 20;
		FlatDesc.vScale = { 20.f,20.f,20.f };
		FlatDesc.vPosition = { 0.f,0.f,0.f,1.f };
		FlatDesc.vAxis = { 0.f,1.f,0.f,0.f };
		FlatDesc.fRotation = 3.f;
		FlatDesc.fRotationPerSec = 3.f;
	}
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Flat_Virtual_Terrain"), LEVEL_GAMEPLAY, strLayerTag,&FlatDesc, reinterpret_cast<CGameObject**>(&m_pVirtualTerrain))))
		return E_FAIL;


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

HRESULT CLevel_GamePlay::Ready_Layer_GameObject(const _wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Monster(const _wstring& strLayerTag)
{

	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TestMonster"), LEVEL_GAMEPLAY, strLayerTag)))
		return E_FAIL;*/

	/*for (size_t i = 0; i < 20; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TestMonster"),
			LEVEL_GAMEPLAY, strLayerTag)))
			return E_FAIL;
	}*/

	return S_OK;
}

void CLevel_GamePlay::IMGUI_SELCT_STAGE()
{
	
	ImGui::Begin("Select_Stage");

	ImGui::Text("Sellect Stage to load and save the stage data");
	if (ImGui::Button("Select Stage01", ImVec2(100, 20)))
	{
		m_eCurStage = STAGE::STAGE01;
	}
	ImGui::SameLine();
	if (ImGui::Button("Select Stage02", ImVec2(100, 20)))
	{
		m_eCurStage = STAGE::STAGE02;
	}
	ImGui::SameLine();
	if (ImGui::Button("Select Stage03", ImVec2(100, 20)))
	{
		m_eCurStage = STAGE::STAGE03;
	}

	ImGui::End();

}

pair<_vector, _vector> CLevel_GamePlay::Get_PickingPosInWorld()
{
	POINT		pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	

	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	_vector vMousePos;
	
	//마우스-> ndc 좌표
	vMousePos =
	{
			(_float)pt.x / (ViewportDesc.Width * 0.5f) - 1.f,
			(_float)pt.y / (ViewportDesc.Height * 0.5f) * -1 + 1.f,
			0.f,
			1.f 
	};
	//투영 ->뷰스페이스
	_matrix matProj = {};
	matProj = m_pGameInstance->Get_TransformInversedMatrix(CPipeLine::D3DTS_PROJ);
	vMousePos = XMVector3TransformCoord(vMousePos, matProj);

	_vector vRayPos, vRayDir;

	vRayPos = { 0.f,0.f,0.f,1.f };
	vRayDir = vMousePos - vRayPos;

	//뷰->월드
	_matrix matView = {};
	matView = m_pGameInstance->Get_TransformInversedMatrix(CPipeLine::D3DTS_VIEW);
	vRayPos = XMVector3TransformCoord(vRayPos, matView);
	vRayDir = XMVector3TransformNormal(vRayDir, matView);
	vRayDir = XMVector3Normalize(vRayDir);//필수적으로 노말라이즈 시켜준다.

	return { vRayPos, vRayDir };

}

HRESULT CLevel_GamePlay::IMGUI_SelectMode()
{

	ImGui::Begin("Select_Option");

	if (ImGui::Button("CreateCollider", ImVec2(100, 20)))
	{
		IMGUI_MODE |= IMGUI_ColliderMode;
		
	}

	if (ImGui::Button("CreateMonsters", ImVec2(100, 20)))
	{
		IMGUI_MODE |= IMGUI_MonsterMode;

	}

	if (ImGui::Button("Adjust_Map", ImVec2(100, 20)))
	{
		IMGUI_MODE |= IMGUI_MapAdjustingMode;

	}

	if (ImGui::Button("NaviMesh", ImVec2(100,20)))
	{
		IMGUI_MODE |= IMGUI_NaviMeshMode;
	}
	ImGui::End();





	if (IMGUI_ColliderMode & IMGUI_MODE)
		IMGUI_Creating_Collider();
	
	if (IMGUI_MonsterMode & IMGUI_MODE)
		IMGUI_Createing_Monsters();

	if (IMGUI_MapAdjustingMode & IMGUI_MODE)
		IMGUI_Moving_Map();

	if (IMGUI_NaviMeshMode & IMGUI_MODE)
		IMGUI_NaviMesh();

	return S_OK;
}

HRESULT CLevel_GamePlay::Picking_And_Create_Collider()
{

	
	pair<_vector, _vector> vRayInfo = Get_PickingPosInWorld();
	_vector vRayPos = vRayInfo.first;
	_vector vRayDir = vRayInfo.second;


	_int VerticesZ = m_pVirtualTerrain->m_Flat_Desc.iNumX;
	_int VerticesX = m_pVirtualTerrain->m_Flat_Desc.iNumZ;
	const vector<_float4>* pVecPos = m_pVirtualTerrain->Get_VecVertices_World();

	CCubeCollider* pCollider = { nullptr }; //벡터 저장용

	for(int i=0;i<VerticesZ-1;++i)
		for (int j = 0; j < VerticesX-1; ++j)
		{

			int iIndex = i * VerticesX + j;

			int iIndices[4] =
			{
				iIndex + VerticesX,
				iIndex + VerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_float fDist = { 1 };
			if (TriangleTests::Intersects(
				vRayPos, vRayDir,
				XMLoadFloat4(&(*pVecPos)[iIndices[1]]),
				XMLoadFloat4(&(*pVecPos)[iIndices[0]]),
				XMLoadFloat4(&(*pVecPos)[iIndices[2]]),
				fDist))
			{
				_vector PickingPos = fDist * vRayDir + vRayPos;
				
				_float4 PickingPosStore;
				XMStoreFloat4(&PickingPosStore, PickingPos);


				CCubeCollider::CUBE_DESC CubeDesc = {};
				CubeDesc.vPos = { PickingPosStore.x ,PickingPosStore.y,PickingPosStore.z ,1.f};
				CubeDesc.vRotAxis = { 0.f,1.f,0.f,0.f };
				CubeDesc.vScale = { 1.f,1.f,1.f };
				CubeDesc.index =m_vecCubeCollider.size();//빼는 기능 생기면 수정?
				CubeDesc.fAngle = 0.f;
				{
					m_vCollider_Position = PickingPosStore;
					m_vCollider_RotAxis = CubeDesc.vRotAxis;
					m_vCollider_Rotation = 0.f;
					m_vCollider_Scale = CubeDesc.vScale;

				}

				if (FAILED(m_pGameInstance->
					Add_GameObject_ToLayer(
						LEVEL_GAMEPLAY,
						TEXT("Prototype_GameObject_CubeCollider"),
						LEVEL_GAMEPLAY,
						L"Layer_GameObject",
						&CubeDesc,
						reinterpret_cast<CGameObject**>( &pCollider))))
					return E_FAIL;

				
				m_vecCubeCollider.push_back(static_cast<CCubeCollider*>(pCollider));
				return S_OK;
			};
			if (TriangleTests::Intersects(
				vRayPos, vRayDir,
				XMLoadFloat4(&(*pVecPos)[iIndices[2]]),
				XMLoadFloat4(&(*pVecPos)[iIndices[0]]),
				XMLoadFloat4(&(*pVecPos)[iIndices[3]]),
				fDist))
			{
				_vector PickingPos = fDist * vRayDir + vRayPos;
				

				_float4 PickingPosStore;
				XMStoreFloat4(&PickingPosStore, PickingPos);

				CCubeCollider::CUBE_DESC CubeDesc = {};
				CubeDesc.vPos = { PickingPosStore.x ,PickingPosStore.y,PickingPosStore.z,1.f };
				CubeDesc.vRotAxis = { 0.f,1.f,0.f,0.f };
				CubeDesc.vScale = { 1.f,1.f,1.f };
				CubeDesc.index = m_vecCubeCollider.size();//빼는 기능 생기면 수정?
				CubeDesc.fAngle = 0.f;
				{
					m_vCollider_Position = PickingPosStore;
					m_vCollider_RotAxis = CubeDesc.vRotAxis;
					m_vCollider_Rotation = 0.f;
					m_vCollider_Scale = CubeDesc.vScale;

				}
				if (FAILED(m_pGameInstance->
					Add_GameObject_ToLayer(
						LEVEL_GAMEPLAY,
						TEXT("Prototype_GameObject_CubeCollider"),
						LEVEL_GAMEPLAY,
						L"Layer_GameObject",
						&CubeDesc,
						reinterpret_cast<CGameObject**>(&pCollider))))
					return E_FAIL;
				
				m_vecCubeCollider.push_back(static_cast<CCubeCollider*>(pCollider));
				return S_OK;
			};


		}



	return S_OK;
}

CCubeCollider* CLevel_GamePlay::Picking_Collider_Box(int iIndex)
{
	pair<_vector, _vector> vRayInfo = Get_PickingPosInWorld();
	_vector vRayPos = vRayInfo.first;
	_vector vRayDir = vRayInfo.second;

	vRayDir = XMVector3Normalize(vRayDir);
	float fDist = { 0 };
	if (m_vecCubeCollider[iIndex]->m_Box.Intersects(vRayPos, vRayDir, fDist))
	{
		return m_vecCubeCollider[iIndex];
	}
	CCubeCollider* pCollider = { nullptr }; //벡터 저장용

	return nullptr;
}

void CLevel_GamePlay::IMGUI_Creating_Collider()
{
	ImGui::Begin("Creating Collider");

	if (ImGui::Button("CreateMode", ImVec2(100, 20)))
	{
		m_bCreateMode = true;
		m_eCreateMode = CREATE_OBJECT_MODE::COLLIDER;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancle CreateMode", ImVec2(200, 20)))
	{
		IMGUI_MODE ^= IMGUI_ColliderMode;
		m_bCreateMode = false;
		m_eCreateMode = CREATE_OBJECT_MODE::MODE_END;
	}


	if (m_iCubeColliderNowIndex + 1 <= m_vecCubeCollider.size())
	{
		m_vCollider_Position = m_vecCubeCollider[m_iCubeColliderNowIndex]->m_Desc.vPos;
		m_vCollider_Scale    = m_vecCubeCollider[m_iCubeColliderNowIndex]->m_Desc.vScale;
		m_vCollider_Rotation = m_vecCubeCollider[m_iCubeColliderNowIndex]->m_Desc.fAngle;
		m_vCollider_RotAxis  = m_vecCubeCollider[m_iCubeColliderNowIndex]->m_Desc.vRotAxis;
	}

	ImGui::SliderFloat3("SizeOfCollider", &m_vCollider_Scale.x,1.f,100.f);
	ImGui::SliderFloat3("Rotation_Axis",&m_vCollider_RotAxis.x,0.f,1.f);
	ImGui::SameLine();
	ImGui::SliderFloat("RotationRadian",&m_vCollider_Rotation,0.f,XMConvertToRadians(360.f));
	ImGui::SliderFloat("Position:X",&m_vCollider_Position.x, m_vCollider_Position.x-0.1f, m_vCollider_Position.x+0.1f);
	ImGui::SliderFloat("Position:Y", &m_vCollider_Position.y, m_vCollider_Position.y - 0.1f, m_vCollider_Position.y + 0.1f);
	ImGui::SliderFloat("Position:Z", &m_vCollider_Position.z, m_vCollider_Position.z - 0.1f, m_vCollider_Position.z + 0.1f);

	
	if (m_iCubeColliderNowIndex + 1 <= m_vecCubeCollider.size()) 
	{
		
		m_vecCubeCollider[m_iCubeColliderNowIndex]->Update_Transform_By_Desc(m_vecCubeCollider[m_iCubeColliderNowIndex]->m_Desc);
		m_vecCubeCollider[m_iCubeColliderNowIndex]->m_Desc.vPos = m_vCollider_Position;
		m_vecCubeCollider[m_iCubeColliderNowIndex]->m_Desc.vScale = m_vCollider_Scale;
		m_vecCubeCollider[m_iCubeColliderNowIndex]->m_Desc.fAngle = m_vCollider_Rotation;
		m_vecCubeCollider[m_iCubeColliderNowIndex]->m_Desc.vRotAxis = m_vCollider_RotAxis;

	}

	if (ImGui::Button("Picking Mode", ImVec2(100, 20)))
	{
		m_bCreateMode = false;
		m_bPickingMode = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancle Picking Mode", ImVec2(200, 20)))
	{
		m_bPickingMode = false;
	}

	for (int i = 0; i < m_vecCubeCollider.size(); ++i)
	{
		if (MOUSE_UP(MOUSE::DIM_LB) && m_bPickingMode && 0 < m_vecCubeCollider.size())
		{
			CCubeCollider* pInstance = nullptr;
			if (nullptr != (pInstance = Picking_Collider_Box(i)))
			{
				m_iCubeColliderNowIndex = pInstance->m_Desc.index;
				break;
			}
		}
	}
	int IndexRange = (m_vecCubeCollider.size() == 0) ? 0 : m_vecCubeCollider.size() - 1;
	ImGui::SliderInt("NowColliderIndex", &m_iCubeColliderNowIndex,0,IndexRange);

	if (ImGui::Button("NextObject", ImVec2(100, 20)))
	{
		if (m_iCubeColliderNowIndex < (int)m_vecCubeCollider.size() - 1)
		{
			m_iCubeColliderNowIndex++;
		}
	}

	if (ImGui::Button("Delete Now Collider"))
	{
		if (m_vecCubeCollider.size() == 0) {
			ImGui::End();
			return;
			
		}
		auto iter = m_vecCubeCollider.begin();
		for (int i = 0; i <m_iCubeColliderNowIndex; ++i)
		{
			++iter;
		}
		CGameObject* pGameObject = *iter;
		Safe_Release(*iter);
		m_vecCubeCollider.erase(iter);
		m_pGameInstance->Remove_Object(LEVEL_GAMEPLAY, L"Layer_GameObject", pGameObject);//지우는 것은 클리어 전에 미리 삭제하는것

		for (int i = 0; i < m_vecCubeCollider.size(); ++i)
		{
			m_vecCubeCollider[i]->m_Desc.index = i;
		}
		if (m_iCubeColliderNowIndex > 0)
			m_iCubeColliderNowIndex -= 1;
		

	}

	if (ImGui::Button("PrevObject", ImVec2(100, 20)))
	{
		if (m_iCubeColliderNowIndex > 0&& (int)m_vecCubeCollider.size()>0)
		{
			m_iCubeColliderNowIndex--;
		}
	}
	
	if (ImGui::Button("Save ALL Colliders", ImVec2(100, 20)))
	{  
		

		HANDLE hFile = CreateFile(m_iNowMapPaths[(_int)m_eCurStage][(_int)CREATE_OBJECT_MODE::COLLIDER], GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0); //저장하려면 폴더내가 생성해야됨 ( 최종경로에 파일만 생성)
		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		_int iNUmCubes = m_vecCubeCollider.size();
		WriteFile(hFile, &iNUmCubes, sizeof(_int), &dwByte, nullptr);
		for (int i = 0; i < m_vecCubeCollider.size(); ++i)
		{
			CCubeCollider::CUBE_DESC CubeDesc = {};
			CubeDesc = m_vecCubeCollider[i]->m_Desc;
			WriteFile(hFile, &CubeDesc, sizeof(CCubeCollider::CUBE_DESC), &dwByte, nullptr);
		}
		MSG_BOX("Completed Save!");
		CloseHandle(hFile);

	}
	if (ImGui::Button("Load Colliders", ImVec2(100, 20)))
	{
		HANDLE hFile = CreateFile(m_iNowMapPaths[(_int)m_eCurStage][(_int)CREATE_OBJECT_MODE::COLLIDER], GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		_int iNumCubes;
		ReadFile(hFile, &iNumCubes, sizeof(_int), &dwByte, nullptr);
		
		for (int i = 0; i < iNumCubes; ++i)
		{
			CCubeCollider* pCollider = nullptr;
			CCubeCollider::CUBE_DESC CubeDesc = {};
			ReadFile(hFile, &CubeDesc, sizeof(CCubeCollider::CUBE_DESC), &dwByte, nullptr);
			

			if (FAILED(m_pGameInstance->
				Add_GameObject_ToLayer(
					LEVEL_GAMEPLAY,
					TEXT("Prototype_GameObject_CubeCollider"),
					LEVEL_GAMEPLAY,
					L"Layer_GameObject",
					&CubeDesc,
					reinterpret_cast<CGameObject**>(&pCollider)))) {
				MSG_BOX("couldnt make collider while loading");
				return;
			}
			m_vecCubeCollider.push_back(pCollider);
		}
		CloseHandle(hFile);
		MSG_BOX("Completed load!");
	}

	ImGui::End();

}

HRESULT CLevel_GamePlay::Creating_Monsters()
{
	
	pair<_vector, _vector> vRayInfo = Get_PickingPosInWorld();
	_vector vRayPos = vRayInfo.first;
	_vector vRayDir = vRayInfo.second;

	vRayDir = XMVector3Normalize(vRayDir);//필수적으로 노말라이즈 시켜준다.

	CMonster* pMonster = { nullptr };
	_tchar   szBaseMonsterTag[MAX_PATH] = TEXT("Prototype_GameObject_Monster0%d");
	_tchar szNowMonsterTag[MAX_PATH] = TEXT("");
	wsprintf(szNowMonsterTag, szBaseMonsterTag, m_iNowMonster);


	for (size_t i = 0; i < m_TrianglesTo_Store_InClient_NaviMesh.size(); ++i)
	{

		_vector vPointA = DirectX::XMLoadFloat3(&m_TrianglesTo_Store_InClient_NaviMesh[i].vPointA);
		_vector vPointB = DirectX::XMLoadFloat3(&m_TrianglesTo_Store_InClient_NaviMesh[i].vPointB);
		_vector vPointC = DirectX::XMLoadFloat3(&m_TrianglesTo_Store_InClient_NaviMesh[i].vPointC);
		_float fDist = 0.f;
		if (TriangleTests::Intersects(
			vRayPos, vRayDir,
			XMVectorSetW(vPointA, 1.f),
			XMVectorSetW(vPointB, 1.f),
			XMVectorSetW(vPointC, 1.f),
			fDist))
		{
			_vector PickingPos = fDist * vRayDir + vRayPos;


			_float4 PickingPosStore;
			XMStoreFloat4(&PickingPosStore, PickingPos);

			CMonster::MONSTER_DESC MonDesc = {};
			/*For Store*/
			MonDesc.vPosition = { PickingPosStore.x ,PickingPosStore.y,PickingPosStore.z,1.f };
			MonDesc.vScale = { 1.f,1.f,1.f };
			MonDesc.iNaviIndex = i;
			/*For Maptool*/
			MonDesc.iIndex = m_iMonsterNowIndex[m_iNowMonster] = m_Monsters[m_iNowMonster].size();//빼는 기능 생기면 수정?
			MonDesc.fAngle = 0.f;
			MonDesc.fRotationPerSec = 3.f;
			MonDesc.fSpeedPerSec = 2.f;
			MonDesc.MonsterNo = m_iNowMonster;
			if (FAILED(m_pGameInstance->
				Add_GameObject_ToLayer(
					LEVEL_GAMEPLAY,
					szNowMonsterTag,
					LEVEL_GAMEPLAY,
					L"Layer_Monster",
					&MonDesc,
					reinterpret_cast<CGameObject**>(&pMonster))))
				return E_FAIL;


			m_Monsters[m_iNowMonster].push_back(static_cast<CMonster*>(pMonster));
			return S_OK;
		}

	}
	return S_OK;
}

void CLevel_GamePlay::IMGUI_Createing_Monsters()
{


	ImGui::Begin("Creating Monster");

	if (ImGui::Button("CreateMode", ImVec2(100, 20)))
	{
		m_bCreateMode = true;
		m_eCreateMode = CREATE_OBJECT_MODE::MONSTER;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancle CreateMode", ImVec2(200, 20)))
	{
		IMGUI_MODE ^= IMGUI_MonsterMode;
		m_bCreateMode = false;
		m_eCreateMode = CREATE_OBJECT_MODE::MODE_END;
	}
	ImGui::SliderInt("NowMonster", &m_iNowMonster, 1, m_iMonsterNum);

	if (m_Monsters[m_iNowMonster].size() > 0) 
	{
		CMonster* pNowMonster = m_Monsters[m_iNowMonster][m_iMonsterNowIndex[m_iNowMonster]];

		CMonster::MONSTER_DESC Desc = pNowMonster->m_Desc;

		ImGui::SliderFloat("Monster_Position:x",&(Desc.vPosition.x),-1.f+ (Desc.vPosition.x),1.f+ (Desc.vPosition.x));
		
		ImGui::SliderFloat("Monster_Position:y", &(Desc.vPosition.y), -1.f + (Desc.vPosition.y), 1.f + (Desc.vPosition.y));
	
		ImGui::SliderFloat("Monster_Position:z", &(Desc.vPosition.z), -1.f + (Desc.vPosition.z), 1.f + (Desc.vPosition.z));
		_float Go = 0;
		ImGui::SliderFloat("MonsterGoStraiht", &Go,-0.5f, 0.5f);
		ImGui::SliderFloat3("Monster_Scale:", &(Desc.vScale.x),1.f, 2.f);
		ImGui::SliderFloat("Monster_Angle:", &(Desc.fAngle), 0.f, XMConvertToRadians(360.f));
		ImGui::SliderInt("Now_Monster", &m_iMonsterNowIndex[m_iNowMonster],0,m_Monsters[m_iNowMonster].size()-1);
		ImGui::Text("MonsterNaviIndex: %d", Desc.iNaviIndex);
		
		pNowMonster->Update_Transform_By_Desc(Desc);
		pNowMonster->Update_Transform_Go_Straight(Go);
		Check_NowMonster_NaviIndex();

	}
	if (ImGui::Button("Picking Mode", ImVec2(100, 20)))
	{
		m_bCreateMode = false;
		m_bPickingMode = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancle Picking Mode", ImVec2(200, 20)))
	{
		m_bPickingMode = false;
	}

	for(int i=1;i<=m_iMonsterNum;++i)
	for (int j = 0; j < m_Monsters[i].size(); ++j)
	{
		if (MOUSE_UP(MOUSE::DIM_LB) && m_bPickingMode && 0 < m_Monsters[i].size())
		{
			CMonster* pInstance = nullptr;
			if (nullptr != (pInstance = picking_Monster(i,j)))
			{
				m_iNowMonster = i;
				m_iMonsterNowIndex[m_iNowMonster] = j;
				i = 100; j = 100;
				break;
			}
		}
	}


	if (ImGui::Button("Delete Now Monster"))
	{
		if (m_Monsters[m_iNowMonster].size() == 0)
			return;

		auto iter = m_Monsters[m_iNowMonster].begin();
		for (int i = 0; i < m_iMonsterNowIndex[m_iNowMonster]; ++i)
		{
			++iter;
		}
		if (iter == m_Monsters[m_iNowMonster].end())
			int a = 0;
		CGameObject* pGameObject = *iter;
		Safe_Release(*iter);
		m_Monsters[m_iNowMonster].erase(iter);
		m_pGameInstance->Remove_Object(LEVEL_GAMEPLAY, L"Layer_Monster", pGameObject);//지우는 것은 == 클리어 전에 미리 삭제하는것

		for (int i = 0; i < m_Monsters[m_iNowMonster].size(); ++i)
		{
			m_Monsters[m_iNowMonster][i]->m_Desc.iIndex = i;
		}
		if (m_iMonsterNowIndex[m_iNowMonster] > 0)
			m_iMonsterNowIndex[m_iNowMonster] -= 1;


	}
	ImGui::SameLine();
	if (ImGui::Button("ClearMosnters"))
	{
		ClearMonsters();
	}
	if (ImGui::Button("Save Monsters Data"))
	{
		HANDLE hFile = CreateFile(m_iNowMapPaths[(_int)m_eCurStage][(_int)CREATE_OBJECT_MODE::OBJECT], GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		
		WriteFile(hFile, &m_iMonsterNum, sizeof(_int),&dwByte,0);
		for (_int i = 1; i <= m_iMonsterNum; ++i)
		{
			_int iNowMonsterNum = m_Monsters[i].size();
			WriteFile(hFile, &iNowMonsterNum, sizeof(_int), &dwByte, 0);

		}
		
		for(int i=1; i<=m_iMonsterNum;++i)
			for (int j = 0; j < m_Monsters[i].size(); ++j)
			{
				WriteFile(hFile, &(m_Monsters[i][j]->m_Desc), sizeof(CMonster::MONSTER_DESC), &dwByte, 0);
			}
		
		MSG_BOX("Completed Save!");
		CloseHandle(hFile);
	}

	if (ImGui::Button("Save Monsters For Client Data"))
	{
		HANDLE hFile = CreateFile(m_iNowMapPaths_Client[(_int)m_eCurStage][(_int)CREATE_OBJECT_MODE::OBJECT], GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);


		WriteFile(hFile, &m_iMonsterNum, sizeof(_int), &dwByte, 0);
		for (_int i = 1; i <= m_iMonsterNum; ++i)
		{
			_int iNowMonsterNum = m_Monsters[i].size();
			WriteFile(hFile, &iNowMonsterNum, sizeof(_int), &dwByte, 0);

		}

		for (int i = 1; i <= m_iMonsterNum; ++i)
			for (int j = 0; j < m_Monsters[i].size(); ++j)
			{
				WriteFile(hFile, &(m_Monsters[i][j]->m_Desc.vScale), sizeof(_float3), &dwByte, 0);
				WriteFile(hFile, &(m_Monsters[i][j]->m_Desc.fAngle), sizeof(_float), &dwByte, 0);
				WriteFile(hFile, &(m_Monsters[i][j]->m_Desc.vPosition), sizeof(_float4), &dwByte, 0);
				WriteFile(hFile, &(m_Monsters[i][j]->m_Desc.iNaviIndex), sizeof(_int), &dwByte, 0);
			}

		MSG_BOX("Completed Save!");
		CloseHandle(hFile);
	}

	if (ImGui::Button("Load Monsters Data"))
	{



		HANDLE hFile = CreateFile(m_iNowMapPaths[(_int)m_eCurStage][(_int)CREATE_OBJECT_MODE::OBJECT], GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		_int iNumMonsterTypes = { 0 };
		ReadFile(hFile, &iNumMonsterTypes, sizeof(_int), &dwByte, 0);
		vector<_int> m_vecNumMonsters;
		m_vecNumMonsters.resize(iNumMonsterTypes + 1);

		for (int i = 1; i <= iNumMonsterTypes; ++i)
		{
			ReadFile(hFile, &m_vecNumMonsters[i], sizeof(_int), &dwByte, 0);
		}

		m_iNowMonster = 0;
		bool isFirstMon = true;
		for (int i = 1; i <= iNumMonsterTypes; ++i)
			for (int j = 0; j < m_vecNumMonsters[i]; ++j)
			{
				CMonster::MONSTER_DESC mDesc = {};
				ReadFile(hFile, &(mDesc), sizeof(CMonster::MONSTER_DESC), &dwByte, 0);
				if (isFirstMon) {
					isFirstMon = false;
					m_iNowMonster = i;
				}
				CMonster* pMonster = nullptr;
				_tchar   szBaseMonsterTag[MAX_PATH] = TEXT("Prototype_GameObject_Monster0%d");
				_tchar szNowMonsterTag[MAX_PATH] = TEXT("");
				wsprintf(szNowMonsterTag, szBaseMonsterTag, i);
				if (FAILED(m_pGameInstance->
					Add_GameObject_ToLayer(
						LEVEL_GAMEPLAY,
						szNowMonsterTag,
						LEVEL_GAMEPLAY,
						L"Layer_Monster",
						&mDesc,
						reinterpret_cast<CGameObject**>(&pMonster))))
				{
					MSG_BOX("MONSTER NO");
					return;
				}
				m_Monsters[i].push_back(static_cast<CMonster*>(pMonster));

			}

		MSG_BOX("Completed Load!");
		CloseHandle(hFile);

	}

	ImGui::End();


}

CMonster* CLevel_GamePlay::picking_Monster(int iNowMonster, int iNowIndex)
{

	pair<_vector, _vector> vRayInfo = Get_PickingPosInWorld();
	_vector vRayPos = vRayInfo.first;
	_vector vRayDir = vRayInfo.second;


	vRayDir = XMVector3Normalize(vRayDir);
	float fDist = { 0 };
	if (m_Monsters[iNowMonster][iNowIndex]->m_Box.Intersects(vRayPos, vRayDir, fDist))
	{
		return m_Monsters[iNowMonster][iNowIndex];
	}	

	return nullptr;
}

bool CLevel_GamePlay::Check_NowMonster_NaviIndex()
{

	for (size_t i = 0; i < m_TrianglesTo_Store_InClient_NaviMesh.size(); ++i)
	{
		_vector vPoints[3] = {};

		vPoints[0] = XMLoadFloat3(&m_TrianglesTo_Store_InClient_NaviMesh[i].vPointA);
		vPoints[1] = XMLoadFloat3(&m_TrianglesTo_Store_InClient_NaviMesh[i].vPointB);
		vPoints[2] = XMLoadFloat3(&m_TrianglesTo_Store_InClient_NaviMesh[i].vPointC);
		
		_vector vLines[3] = {};
		vLines[0] = vPoints[1] - vPoints[0];
		vLines[1] = vPoints[2] - vPoints[1];
		vLines[2] = vPoints[0] - vPoints[2];

		bool isIn = true;
		_vector vMonPos;

		for (int j = 0; j < 3; ++j)
		{
			//(법선 벡터)(x,0,z)-> (-z,0,x)(계산 대상)
			//(시계방향 선분의 노말 dot 내위치- 선분의 시작점)
			_vector vNormal = XMVectorSet(vLines[j].m128_f32[2] * -1.f, 0.f, vLines[j].m128_f32[0], 0.f);
			vMonPos = XMLoadFloat4(&m_Monsters[m_iNowMonster][m_iMonsterNowIndex[m_iNowMonster]]->m_Desc.vPosition);
			_vector vDir = vMonPos - vPoints[j];
			vDir = XMVectorSetW(vDir, 0.f);
			
			if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vNormal), XMVector3Normalize(vDir))))
			{
				isIn = false;
			}
		
		}
		if (true == isIn)
		{
			m_Monsters[m_iNowMonster][m_iMonsterNowIndex[m_iNowMonster]]->m_Desc.iNaviIndex = i;

			_vector vPlane = XMPlaneFromPoints(vPoints[0], vPoints[1], vPoints[2]);
			//_vectre(a,b,c,d) -> ax +by+cz+d =0


			/* ax + by + cz + d = 0*/

			/* y = (-ax - cz - d) / b */
			m_Monsters[m_iNowMonster][m_iMonsterNowIndex[m_iNowMonster]]->m_Desc.vPosition.y =
				(-XMVectorGetX(vPlane) * XMVectorGetX(vMonPos) - XMVectorGetZ(vPlane) * XMVectorGetZ(vMonPos) - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);

			return true;
		}
	}
	return false;
	
}


void CLevel_GamePlay::ClearMonsters()
{

	for (int i = 1; i <= m_iMonsterNum; ++i) {
		for (auto iter = m_Monsters[i].begin(); iter != m_Monsters[i].end();)
		{
			CGameObject* pGameObject = nullptr;
			
			if (iter != m_Monsters[i].end())
			{
				pGameObject = *iter;
				Safe_Release(*iter);
				iter = m_Monsters[i].erase(iter);
				m_pGameInstance->Remove_Object(LEVEL_GAMEPLAY, L"Layer_Monster", pGameObject);
			}
			
		}
		m_Monsters[i].clear();
	}
}

HRESULT CLevel_GamePlay::Creating_Map()
{
	return S_OK;
}

void CLevel_GamePlay::IMGUI_Moving_Map()
{

	if (nullptr == m_pBaseTerrain)
	{
		MSG_BOX("YOU DIDN'T LAOD STATGE MAP, MAKE AN MAP STAGE TERRAIN YOU WANT.");
		return;
	}

	CTerrain_Base::TERRAIN_BASE_DESC Map_Desc = m_pBaseTerrain->m_Desc;
	CTerrain_Base::TERRAIN_BASE_DESC VirtualDesc = m_pVirtualTerrain->m_Desc;

	ImGui::Begin("AdjustingMap");

	if (ImGui::Button("Adjusting Mode", ImVec2(100, 20)))
	{
		m_eCreateMode = CREATE_OBJECT_MODE::MODE_END;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancle Adjusting Mode", ImVec2(200, 20)))
	{
		IMGUI_MODE ^= IMGUI_MapAdjustingMode;
		m_bCreateMode = false;
		m_eCreateMode = CREATE_OBJECT_MODE::MODE_END;
	}
	{
		ImGui::SliderFloat("Terrain_Position:x", &(Map_Desc.vPosition.x), -1.f + (Map_Desc.vPosition.x), 1.f + (Map_Desc.vPosition.x));

		ImGui::SliderFloat("Terrain_Position:y", &(Map_Desc.vPosition.y), -1.f + (Map_Desc.vPosition.y), 1.f + (Map_Desc.vPosition.y));

		ImGui::SliderFloat("Terrain_Position:z", &(Map_Desc.vPosition.z), -1.f + (Map_Desc.vPosition.z), 1.f + (Map_Desc.vPosition.z));
		_float Go = 0;
		ImGui::SliderFloat("Terrain_GoStraiht", &Go, -0.5f, 0.5f);
		ImGui::SliderFloat3("Terrain_Scale:", &(Map_Desc.vScale.x), 1.f, 2.f);
		ImGui::SliderFloat("Terrain_Angle:", &(Map_Desc.fRotation), 0.f, XMConvertToRadians(360.f));
		ImGui::SliderFloat3("Terrain_RotationAxis:", &(Map_Desc.vAxis.x), 0.f, 1.f);
		XMStoreFloat4(&Map_Desc.vAxis, XMVector3Normalize(XMLoadFloat4(&Map_Desc.vAxis)));


		m_pBaseTerrain->m_Desc = Map_Desc;
		m_pBaseTerrain->Update_Transform_By_Desc(Map_Desc);
		m_pBaseTerrain->Update_Transform_Go_Straight(Go);
	}
	ImGui::Text("Save Terrain Position");
	if(ImGui::Button("Save Now Terrain", ImVec2(200, 20)))
	{

		HANDLE hFile = CreateFile(m_iNowMapPaths[(_int)m_eCurStage][(_int)CREATE_OBJECT_MODE::MAP], GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return;
		}
		CTerrain_Base::TERRAIN_BASE_DESC Desc = {};
		
		_ulong dwByte = 0;
		WriteFile(hFile, &Desc, sizeof(CTerrain_Base::TERRAIN_BASE_DESC), &dwByte, nullptr);
		CloseHandle(hFile);
	}
	if (ImGui::Button("Delete CurrentMap"))
	{
		if (m_pBaseTerrain)
		{
			Safe_Release(m_pBaseTerrain);
			m_pGameInstance->Remove_Object(LEVEL_GAMEPLAY, L"Layer_BackGround", m_pBaseTerrain);
		}
		m_pBaseTerrain = nullptr;
	}

	if (ImGui::Button("Load Selected Terrain", ImVec2(200, 20)))
	{
		if (m_pBaseTerrain)
		{
			Safe_Release(m_pBaseTerrain);
			m_pGameInstance->Remove_Object(LEVEL_GAMEPLAY, L"Layer_BackGround", m_pBaseTerrain);
			m_pBaseTerrain = nullptr;
		}

		HANDLE hFile = CreateFile(m_iNowMapPaths[(_int)m_eCurStage][(_int)CREATE_OBJECT_MODE::MAP], GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return;
		}
		_ulong dwByte = 0;
		CTerrain_Base::TERRAIN_BASE_DESC Desc = {};
		ReadFile(hFile, &Desc, sizeof(CTerrain_Base::TERRAIN_BASE_DESC), &dwByte, nullptr);

		//"Prototype_GameObject_Terrain_Stage01"

		TCHAR szMapProtoBase[MAX_PATH] = L"Prototype_GameObject_Terrain_Stage0";
		TCHAR szNowStage[MAX_PATH] = L"";
		wsprintf(szNowStage, szMapProtoBase, (_uint)m_eCurStage);
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY,
			szNowStage, LEVEL_GAMEPLAY, L"Layer_BackGround",
			&Desc, reinterpret_cast<CGameObject**>(m_pBaseTerrain)))) {
			MSG_BOX("COUDNT ADD MAP");
			return;
		}

	}
	ImGui::Text("Virtual_Terrain.");
	{
		ImGui::SliderFloat("Virtual_Position:x", &(VirtualDesc.vPosition.x), -1.f + (VirtualDesc.vPosition.x), 1.f + (VirtualDesc.vPosition.x));

		ImGui::SliderFloat("Virtual_Position:y", &(VirtualDesc.vPosition.y), -1.f + (VirtualDesc.vPosition.y), 1.f + (VirtualDesc.vPosition.y));

		ImGui::SliderFloat("Virtual_Position:z", &(VirtualDesc.vPosition.z), -1.f + (VirtualDesc.vPosition.z), 1.f + (VirtualDesc.vPosition.z));
		_float Go = 0;
		ImGui::SliderFloat("Virtual_GoStraiht", &Go, -0.5f, 0.5f);
		ImGui::SliderFloat3("Virtual_Scale:", &(VirtualDesc.vScale.x), 1.f, 20.f);
		ImGui::SliderFloat("Virtual_Angle:", &(VirtualDesc.fRotation), 0.f, XMConvertToRadians(360.f));
		ImGui::SliderFloat3("Virtual_RotationAxis:", &(VirtualDesc.vAxis.x), 0.f, 1.f);
		XMStoreFloat4(&VirtualDesc.vAxis, XMVector3Normalize(XMLoadFloat4(&VirtualDesc.vAxis)));


		m_pVirtualTerrain->m_Desc = VirtualDesc;
		m_pVirtualTerrain->Update_Transform_By_Desc(VirtualDesc);
		m_pVirtualTerrain->Update_Transform_Go_Straight(Go);


	}



	ImGui::End();

}
/*For Navi_Mesh*/
void CLevel_GamePlay::Adjusting_Triangle(TRIANGLE_VERTICES& Triangle)
{
	
	_vector vectorAB = XMLoadFloat3(&(Triangle.vPointB)) - XMLoadFloat3(&(Triangle.vPointA));
	_vector vectorBC = XMLoadFloat3(&(Triangle.vPointC)) - XMLoadFloat3(&(Triangle.vPointB));
	
	_vector vecCross = XMVector3Cross(vectorAB, vectorBC);

	/* 반시계방향이었다면 시계방향으로 바꿔준다.*/
	if (XMVectorGetY(vecCross) < 0)
	{
		_float3 vTempPoint;
		vTempPoint = Triangle.vPointB;
		Triangle.vPointB = Triangle.vPointC;
		Triangle.vPointC = vTempPoint;
	}



}
/* 찍은점이 일정거리 가깝다면 근사시킨다.*/
void CLevel_GamePlay::Mapping_Points(_float3& Point)
{

	for (auto Point_Stored : m_vecPoints_Navimesh)
	{

		_float distSquare =
			(Point_Stored.x - Point.x) * (Point_Stored.x - Point.x) +
			(Point_Stored.y - Point.y) * (Point_Stored.y - Point.y) +
			(Point_Stored.z - Point.z) * (Point_Stored.z - Point.z);

		if (distSquare<=1.f)
		{
			Point = Point_Stored;
			return;
		}
	}
	

	return;
}

void CLevel_GamePlay::IMGUI_NaviMesh()
{
	ImGui::Begin("NaviMesh");
	if (ImGui::Button("CreateMode", ImVec2(100, 20)))
	{
		m_bCreateMode = true;
		m_eCreateMode = CREATE_OBJECT_MODE::NAVI_MESH_VTX;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancle CreateMode", ImVec2(200, 20)))
	{
		IMGUI_MODE ^= IMGUI_NaviMeshMode;
		m_bCreateMode = false;
		m_eCreateMode = CREATE_OBJECT_MODE::MODE_END;
	}

	if (ImGui::Button("Create_On_Virtual_Terrain_Mode", ImVec2(100, 20)))
	{
		 m_bMake_NaviMeshOnVirtualTerrain = true;
		
		
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancle Create_On_Virtual_Terrain_Mode", ImVec2(200, 20)))
	{
		m_bMake_NaviMeshOnVirtualTerrain = false;
		
	}


	ImGui::SliderInt("Now_Mesh_Monster's Stage:", &m_iNowStageNumber, 2 , 10);
	ImGui::SliderInt("NowPoint:", &m_iNowTriangle_Point, 0, 2);
	if (3 <= m_iNowTriangle_Point)
	{
		if (ImGui::Button("Add_Vertices To Navigation as a Cell", ImVec2(200, 20)))
		{
			Adjusting_Triangle(m_NowTriangle);
			m_pBaseTerrain->Add_Triangle_To_Navigation(m_NowTriangle);
			m_TrianglesTo_Store_InClient_NaviMesh.push_back(m_NowTriangle);//저장용
			m_vecStage_Number.push_back(m_iNowStageNumber);

			m_vecPoints_Navimesh.push_back(m_NowTriangle.vPointA);
			m_vecPoints_Navimesh.push_back(m_NowTriangle.vPointB);
			m_vecPoints_Navimesh.push_back(m_NowTriangle.vPointC);
			m_NowTriangle = {};
			m_iNowTriangle_Point = 0;
			m_bCreateMode = false;
		}
	}


	ImGui::Text("Edit PointA:");
	ImGui::SliderFloat("PointA:x", &(m_NowTriangle.vPointA.x), -1.f + (m_NowTriangle.vPointA.x), 1.f + (m_NowTriangle.vPointA.x));
	ImGui::SliderFloat("PointA:y", &(m_NowTriangle.vPointA.y), -1.f + (m_NowTriangle.vPointA.y), 1.f + (m_NowTriangle.vPointA.y));
	ImGui::SliderFloat("PointA:z", &(m_NowTriangle.vPointA.z), -1.f + (m_NowTriangle.vPointA.z), 1.f + (m_NowTriangle.vPointA.z));

	ImGui::Text("Edit PointB:");
	ImGui::SliderFloat("PointB:x", &(m_NowTriangle.vPointB.x), -1.f + (m_NowTriangle.vPointB.x), 1.f + (m_NowTriangle.vPointB.x));
	ImGui::SliderFloat("PointB:y", &(m_NowTriangle.vPointB.y), -1.f + (m_NowTriangle.vPointB.y), 1.f + (m_NowTriangle.vPointB.y));
	ImGui::SliderFloat("PointB:z", &(m_NowTriangle.vPointB.z), -1.f + (m_NowTriangle.vPointB.z), 1.f + (m_NowTriangle.vPointB.z));


	ImGui::Text("Edit PointC:");
	ImGui::SliderFloat("PointC:x", &(m_NowTriangle.vPointC.x), -1.f + (m_NowTriangle.vPointC.x), 1.f + (m_NowTriangle.vPointC.x));
	ImGui::SliderFloat("PointC:y", &(m_NowTriangle.vPointC.y), -1.f + (m_NowTriangle.vPointC.y), 1.f + (m_NowTriangle.vPointC.y));
	ImGui::SliderFloat("PointC:z", &(m_NowTriangle.vPointC.z), -1.f + (m_NowTriangle.vPointC.z), 1.f + (m_NowTriangle.vPointC.z));
	if (ImGui::Button("Edit Cell", ImVec2(200, 20)))
	{
		m_pBaseTerrain->AdjustCurTriangle(m_NowTriangle);
	}


	ImGui::Text("Clear_NowTriangle Info.");
	if (ImGui::Button("Clear_NowTriangle_Info", ImVec2(200, 20)))
	{
		m_NowTriangle = {};
		m_iNowTriangle_Point = 0;
	}
	

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(500, 100), ImGuiWindowFlags_NoTitleBar);
	int iIndex = 0;
	for (auto& Triangle_Info_Stored : m_TrianglesTo_Store_InClient_NaviMesh)
	{

		
		ImGui::Text("  startIndex: %d  ", iIndex++);
		ImGui::Text("PointA: (%.3f, %.3f ,%.3f)", Triangle_Info_Stored.vPointA.x, Triangle_Info_Stored.vPointA.y, Triangle_Info_Stored.vPointA.z);
		ImGui::Text("PointB: (%.3f, %.3f ,%.3f)", Triangle_Info_Stored.vPointB.x, Triangle_Info_Stored.vPointB.y, Triangle_Info_Stored.vPointB.z);
		ImGui::Text("PointC: (%.3f, %.3f ,%.3f)", Triangle_Info_Stored.vPointC.x, Triangle_Info_Stored.vPointC.y, Triangle_Info_Stored.vPointC.z);
		ImGui::Text("  m_vecStage_Number: %d  ", m_vecStage_Number[iIndex-1]);
	}
	ImGui::EndChild();
	
	if (ImGui::Button("Decuct _NowTriangle From Navi", ImVec2(200, 20)))
	{

		m_TrianglesTo_Store_InClient_NaviMesh.pop_back();//저장용
		m_vecStage_Number.pop_back();

		m_vecPoints_Navimesh.pop_back();
		m_vecPoints_Navimesh.pop_back();
		m_vecPoints_Navimesh.pop_back();
		m_NowTriangle = {};
		m_iNowTriangle_Point = 0;
		m_pBaseTerrain->Pop_Triangle();
	}

	/*저장 */
	if (ImGui::Button("Save NaviMesh Data For Client"))
	{
		HANDLE hFile = CreateFile(L"../Bin/Resources/LoadData/Client_NaviMesh_Stage01.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		int iSize = m_TrianglesTo_Store_InClient_NaviMesh.size();

		WriteFile(hFile, &iSize, sizeof(_int), &dwByte, 0);
		for (_int i = 0; i < iSize ;  ++i)
		{
			WriteFile(hFile, &m_TrianglesTo_Store_InClient_NaviMesh[i], sizeof(TRIANGLE_VERTICES), &dwByte, 0);
			WriteFile(hFile, &m_vecStage_Number[i], sizeof(_int), &dwByte, 0);
		}

		MSG_BOX("Completed Save!");
		CloseHandle(hFile);
	}
	/*For Maptool*/
	if (ImGui::Button("Save NaviMesh Data For MapTool"))
	{
		HANDLE hFile = CreateFile(L"../Bin/Resources/LoadData/NaviMesh_Stage01.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		int iSize = m_TrianglesTo_Store_InClient_NaviMesh.size();

		WriteFile(hFile, &iSize, sizeof(_int), &dwByte, 0);
		for (_int i = 0; i < iSize; ++i)
		{
			WriteFile(hFile, &m_TrianglesTo_Store_InClient_NaviMesh[i], sizeof(TRIANGLE_VERTICES), &dwByte, 0);
			WriteFile(hFile, &m_vecStage_Number[i], sizeof(_int), &dwByte, 0);
		}

		iSize = m_vecPoints_Navimesh.size();
		WriteFile(hFile, &iSize, sizeof(_int), &dwByte, 0);
		for (_int i = 0; i < iSize; ++i)
		{
			WriteFile(hFile, &m_vecPoints_Navimesh[i], sizeof(_float3), &dwByte, 0);
		}

		MSG_BOX("Completed Save!");
		CloseHandle(hFile);
	}
	/* Load*/
	if (ImGui::Button("Load NaviMesh Data"))
	{
		HANDLE hFile = CreateFile(L"../Bin/Resources/LoadData/NaviMesh_Stage01.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		

		int iSize = 0;
		ReadFile(hFile, &iSize, sizeof(_int), &dwByte, 0);
		m_TrianglesTo_Store_InClient_NaviMesh.resize(iSize);
		
		for (_int i = 0; i < iSize; ++i)
		{
			TRIANGLE_VERTICES Triangle = {};
			_int iStageNumber = 0;
			ReadFile(hFile, &Triangle, sizeof(TRIANGLE_VERTICES), &dwByte, 0);
			m_pBaseTerrain->Add_Triangle_To_Navigation(Triangle);
			ReadFile(hFile, &iStageNumber, sizeof(_int), &dwByte, 0);
			m_TrianglesTo_Store_InClient_NaviMesh[i] = (Triangle);
			m_vecStage_Number.push_back(iStageNumber);

		}

		ReadFile(hFile, &iSize, sizeof(_int), &dwByte, 0);
		m_vecPoints_Navimesh.resize(iSize);
		for (_int i = 0; i < iSize; ++i)
		{
			_float3 fPoint = {};
			ReadFile(hFile, &fPoint, sizeof(_float3), &dwByte, 0);
			m_vecPoints_Navimesh[i] = (fPoint);
		}


		MSG_BOX("Completed Save!");
		CloseHandle(hFile);
	}



	ImGui::End();
}

void CLevel_GamePlay::Creating_Vertex()
{
	pair<_vector, _vector> Ray =Get_PickingPosInWorld();
	_vector vRayPos = Ray.first;
	_vector vRayDir = Ray.second;
	_float fDist = 0;
	for (auto& Triangle : m_Triangles)
	{
		_vector vPointA = DirectX::XMLoadFloat3(&Triangle.vPointA);
		_vector vPointB = DirectX::XMLoadFloat3(&Triangle.vPointB);
		_vector vPointC = DirectX::XMLoadFloat3(&Triangle.vPointC);

		if (TriangleTests::Intersects(
			vRayPos, vRayDir,
			XMVectorSetW(vPointA, 1.f),
			XMVectorSetW(vPointB, 1.f),
			XMVectorSetW(vPointC, 1.f),
			fDist))
		{

			_vector PickingPos = fDist * vRayDir + vRayPos;

			_float4 PickingPosStore;
			XMStoreFloat4(&PickingPosStore, PickingPos);
			
			if (0 == m_iNowTriangle_Point)
			{
				XMStoreFloat3(&m_NowTriangle.vPointA, PickingPos);
				Mapping_Points(m_NowTriangle.vPointA);
				m_iNowTriangle_Point += 1;
				return;
			}
			else if (1 == m_iNowTriangle_Point)
			{
				XMStoreFloat3(&m_NowTriangle.vPointB, PickingPos);
				Mapping_Points(m_NowTriangle.vPointB);
				m_iNowTriangle_Point += 1;
				return;
			}
			else if (2 == m_iNowTriangle_Point)
			{
				XMStoreFloat3(&m_NowTriangle.vPointC, PickingPos);
				Mapping_Points(m_NowTriangle.vPointC);
				m_iNowTriangle_Point += 1;
				return;
			}
			
			
			/*if (3 == m_iNowTriangle_Point)
			{
				m_iNowTriangle_Point = 0;
				m_NowTriangle = {};
			}*/
		}

	}
}

void CLevel_GamePlay::Creating_Vertex_On_Virtual_Terrain()
{

	pair<_vector, _vector> vRayInfo = Get_PickingPosInWorld();
	_vector vRayPos = vRayInfo.first;
	_vector vRayDir = vRayInfo.second;

	vRayDir = XMVector3Normalize(vRayDir);//필수적으로 노말라이즈 시켜준다.

	_int VerticesZ = m_pVirtualTerrain->m_Flat_Desc.iNumX;
	_int VerticesX = m_pVirtualTerrain->m_Flat_Desc.iNumZ;

	const vector<_float4>* pVecPos = m_pVirtualTerrain->Get_VecVertices_World();

	for (int i = 0; i < VerticesZ - 1; ++i)
		for (int j = 0; j < VerticesX - 1; ++j)
		{

			int iIndex = i * VerticesX + j;

			int iIndices[4] =
			{
				iIndex + VerticesX,
				iIndex + VerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_float fDist = { 1 };
			if (TriangleTests::Intersects(
				vRayPos, vRayDir,
				XMLoadFloat4(&(*pVecPos)[iIndices[1]]),
				XMLoadFloat4(&(*pVecPos)[iIndices[0]]),
				XMLoadFloat4(&(*pVecPos)[iIndices[2]]),
				fDist))
			{

				_vector PickingPos = fDist * vRayDir + vRayPos;

				_float4 PickingPosStore;
				XMStoreFloat4(&PickingPosStore, PickingPos);

				if (0 == m_iNowTriangle_Point)
				{
					XMStoreFloat3(&m_NowTriangle.vPointA, PickingPos);
					Mapping_Points(m_NowTriangle.vPointA);
					m_iNowTriangle_Point += 1;
					return;
				}
				else if (1 == m_iNowTriangle_Point)
				{
					XMStoreFloat3(&m_NowTriangle.vPointB, PickingPos);
					Mapping_Points(m_NowTriangle.vPointB);
					m_iNowTriangle_Point += 1;
					return;
				}
				else if (2 == m_iNowTriangle_Point)
				{
					XMStoreFloat3(&m_NowTriangle.vPointC, PickingPos);
					Mapping_Points(m_NowTriangle.vPointC);
					m_iNowTriangle_Point += 1;
					return;
				}

			}
			if (TriangleTests::Intersects(
				vRayPos, vRayDir,
				XMLoadFloat4(&(*pVecPos)[iIndices[2]]),
				XMLoadFloat4(&(*pVecPos)[iIndices[0]]),
				XMLoadFloat4(&(*pVecPos)[iIndices[3]]),
				fDist))
			{
				_vector PickingPos = fDist * vRayDir + vRayPos;

				_float4 PickingPosStore;
				XMStoreFloat4(&PickingPosStore, PickingPos);

				if (0 == m_iNowTriangle_Point)
				{
					XMStoreFloat3(&m_NowTriangle.vPointA, PickingPos);
					Mapping_Points(m_NowTriangle.vPointA);
					m_iNowTriangle_Point += 1;
					return;
				}
				else if (1 == m_iNowTriangle_Point)
				{
					XMStoreFloat3(&m_NowTriangle.vPointB, PickingPos);
					Mapping_Points(m_NowTriangle.vPointB);
					m_iNowTriangle_Point += 1;
					return;
				}
				else if (2 == m_iNowTriangle_Point)
				{
					XMStoreFloat3(&m_NowTriangle.vPointC, PickingPos);
					Mapping_Points(m_NowTriangle.vPointC);
					m_iNowTriangle_Point += 1;
					return;
				}
			}

		}
}

void CLevel_GamePlay::Clear_NowTriangle()
{
	m_iNowTriangle_Point = 0;
	m_NowTriangle = {};
}




CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{

	for (auto iter =m_vecCubeCollider.begin();iter!=m_vecCubeCollider.end();++iter)
	{
		Safe_Release(*iter);
	}

	for (int i = 0; i <= m_iMonsterNum; ++i)
	{
		for (auto monster : m_Monsters[i])
		{
			Safe_Release(monster);
		}
	}

	Safe_Release(m_pBaseTerrain);
	Safe_Release(m_pTerrain);
	Safe_Release(m_pVirtualTerrain);
	Safe_Release(m_pImguiMgr);
	__super::Free();
}
