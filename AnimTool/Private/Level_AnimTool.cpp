#include "stdafx.h"
#include "..\Public\Level_AnimTool.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include"CubeCollider.h"
#include"Terrain.h"
#include"Monster.h"
#include"Terrain_Base.h"
#include"Flat_Virtual_Terrain.h"


CLevel_AnimTool::CLevel_AnimTool(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_AnimTool::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camara"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_GameObject(TEXT("Layer_GameObject"))))
		return E_FAIL;*/


	if (FAILED(Ready_Monsters(TEXT("Layer_Monster"))))
		return E_FAIL;

	m_pImguiMgr = CImguiWindow::Create(m_pDevice, m_pContext);



	return S_OK;
}

void CLevel_AnimTool::Update(_float fTimeDelta)
{

 	

	/*if(MOUSE_UP(MOUSE::DIM_LB) && m_bCreateMode && CREATE_OBJECT_MODE::MONSTER == m_eCreateMode) {
		Creating_Monsters();
	}*/
	
}

HRESULT CLevel_AnimTool::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("맵툴 레벨입니다."));
#endif
	
	if (nullptr == m_pImguiMgr)return E_FAIL;
	if (nullptr == m_pGameInstance)return E_FAIL;


	if (FAILED(m_pImguiMgr->Start_Render()))return E_FAIL;

	IMGUI_SelectMode();
	
	if (FAILED(m_pImguiMgr->End_Render()))return E_FAIL;

	return S_OK;
}

HRESULT CLevel_AnimTool::Ready_Layer_Camera(const _wstring & strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC		Desc{};

	Desc.fSpeedPerSec = 5.f;
	Desc.fRotationPerSec = XMConvertToRadians(180.f);
	Desc.fMouseSensor = 0.1f;
	Desc.fFovy = XMConvertToRadians(60.f);
	Desc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.vEye = _float3(0.f, 1.5f, 4.f);
	Desc.vAt = _float3(0.f, 1.5f, 0.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_GAMEPLAY, strLayerTag, &Desc,reinterpret_cast<CGameObject**>(&m_pCamera))))
		return E_FAIL;

	return S_OK;
}




HRESULT CLevel_AnimTool::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(-1.f, -1.f, -1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_AnimTool::Ready_Layer_GameObject(const _wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_AnimTool::Ready_Monsters(const _wstring& strLayerTag)
{
	
	TCHAR szBaseMonsterProro[MAX_PATH] = L"Prototype_GameObject_Monster0%d";
	m_Monsters.resize(m_iMonsterNum + 1);

	for (int i = 1; i <= m_iMonsterNum; ++i)
	{
		CMonster* pMonster = nullptr;
		TCHAR szNowMonsterProto[MAX_PATH] = L"";
		wsprintf(szNowMonsterProto, szBaseMonsterProro,i);

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY,
			szNowMonsterProto, LEVEL_GAMEPLAY
			,strLayerTag
			, nullptr
			, reinterpret_cast<CGameObject**>(&pMonster)
		)));
		m_Monsters[i] = pMonster;
	}


	m_iNowMonster = 1;

	return S_OK;
}



pair<_vector, _vector> CLevel_AnimTool::Get_PickingPosInWorld()
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

HRESULT CLevel_AnimTool::IMGUI_SelectMode()
{

	ImGui::Begin("Select_Option");

	if (ImGui::Button("CreateCollider", ImVec2(100, 20)))
	{
		IMGUI_MODE |= IMGUI_ObjectMode;
		
	}

	if (ImGui::Button("Monsters Anim", ImVec2(100, 20)))
	{
		IMGUI_MODE |= IMGUI_MonsterMode;

	}

	
	ImGui::End();




	
	if (IMGUI_MonsterMode & IMGUI_MODE)
		IMGUI_Check_MonstersAnim();

	if (IMGUI_ObjectMode & IMGUI_MODE)
	{

	}
	return S_OK;
}





void CLevel_AnimTool::IMGUI_Check_MonstersAnim()
{


	ImGui::Begin("Editing Monster");

	if (ImGui::Button("Fix Camera", ImVec2 (200, 50)))
	{
		if (nullptr == m_pCamera)
		{
			ImGui::End();
			return;
		}
		static_cast<CCamera_Free*>(m_pCamera)->Set_CameraActive(false);
	}
	if (ImGui::Button("Active Camera", ImVec2(200, 50)))
	{
		if (nullptr == m_pCamera)
		{
			ImGui::End();
			return;
		}
		static_cast<CCamera_Free*>(m_pCamera)->Set_CameraActive(true);
	}


	ImGui::SameLine();
	if (ImGui::Button("Cancle Editing Mode", ImVec2(200, 20)))
	{
		IMGUI_MODE ^= IMGUI_MonsterMode;
		
	}
	ImGui::SliderInt("Now Monster", &m_iNowMonster, 1, m_iMonsterNum);

	for (int i = 1; i <= m_iMonsterNum; ++i) {
		if (i == m_iNowMonster)
			m_Monsters[m_iNowMonster]->m_isVisible = true;
		else
			m_Monsters[i]->m_isVisible = false;
	}
		
	ImGui::SliderInt("NowAnimMotion",&m_Monsters[m_iNowMonster]->m_iCurAnimIndex, 0, m_Monsters[m_iNowMonster]->m_iNumAnims-1);
	
	if (ImGui::Button("Show Specific Anim Infinite", ImVec2(200, 20)))
	{
		ANIM_MODE = 0;
		m_Monsters[m_iNowMonster]->PlayNowAnim(m_Monsters[m_iNowMonster]->m_iCurAnimIndex,true);
	}

	
	ImGui::SliderFloat("Temp Distance", &m_Monsters[m_iNowMonster]->m_fDistanceForStore, 0.f, 2.f);
	ImGui::SliderInt("Start MotionIndex", &m_Monsters[m_iNowMonster]->m_StartAnimIndex, 0, m_Monsters[m_iNowMonster]->m_iNumAnims - 1);
	ImGui::SliderInt("End MotionIndeex", &m_Monsters[m_iNowMonster]->m_EndAnimIndex, 0, m_Monsters[m_iNowMonster]->m_iNumAnims - 1);
	ImGui::InputText("AnimName", m_Monsters[m_iNowMonster]->m_AnimName, MAX_PATH);


	if (ImGui::Button("Mode :Play AnimToAnim"))
	{
		m_Monsters[m_iNowMonster]->StartAnimToAnim();
		ANIM_MODE = 0;
		ANIM_MODE |= ANIM_MODE_SLERP;
	}
	ImGui::SameLine();
	if (ImGui::Button("Mode:Cancle AnimToAnim"))
	{
		ANIM_MODE ^= ANIM_MODE_SLERP;
	}
	
	if (ANIM_MODE & ANIM_MODE_SLERP)
	{
		m_Monsters[m_iNowMonster]->Play_AnimToAnim(
			m_Monsters[m_iNowMonster]->m_StartAnimIndex,
			m_Monsters[m_iNowMonster]->m_EndAnimIndex,
			m_Monsters[m_iNowMonster]->m_fDistanceForStore
		);
	}
	
	if (ImGui::Button("AddAnimToAnim", ImVec2(200, 20)))
	{

		_uint iStartAnimIndex = m_Monsters[m_iNowMonster]->m_StartAnimIndex;
		_uint iEndAnimIndex = m_Monsters[m_iNowMonster]->m_EndAnimIndex;
		_float fDistance = m_Monsters[m_iNowMonster]->m_fDistanceForStore;
		char AnimName[MAX_PATH]="";
		strcpy_s(AnimName, m_Monsters[m_iNowMonster]->m_AnimName);
		
		
		CMonster::Name name;
		strcpy_s(name.Name, AnimName);

		m_Monsters[m_iNowMonster]->
			m_eAnim_Desc.m_mapDistances.emplace(make_pair(iStartAnimIndex, iEndAnimIndex), make_pair(name,fDistance));
	
	}
	ImGui::SameLine();
	if (ImGui::Button("Mode :Check Added Anims", ImVec2(200, 20)))
	{
		m_Monsters[m_iNowMonster]->StartAnimToAnim();
		ANIM_MODE = 0;
		ANIM_MODE |= ANIM_MODE_CHECK;
	}

	if (ANIM_MODE & ANIM_MODE_CHECK) {
		int iMaxSize = m_Monsters[m_iNowMonster]->m_eAnim_Desc.m_mapDistances.size();
		bool isEmpty = false;
		if (iMaxSize >= 1)
			--iMaxSize;
		else
			isEmpty = true;
		/* 현재 볼 애니메이션*/
		ImGui::SliderInt("Now Animation", &m_Monsters[m_iNowMonster]->m_iCurAnimToAnimIndex, 0, iMaxSize);
		
		//현재 보고 있는 애니메이션의 이터레이터 
		int NowAnimation = m_Monsters[m_iNowMonster]->m_iCurAnimToAnimIndex;
		int iIndex = 0;
		
		map<pair< _uint, _uint>, pair<CMonster::Name, _float>>::iterator iter = m_Monsters[m_iNowMonster]->m_eAnim_Desc.m_mapDistances.begin();
		for(int i=0;i<NowAnimation;++i)
		{
			++iter;
		};
		
		
		/*모든 애니메이션 */
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(500, 100), ImGuiWindowFlags_NoTitleBar);
			for (auto& Desc : m_Monsters[m_iNowMonster]->m_eAnim_Desc.m_mapDistances)
			{
				//모션순서를 바꾸지는 않는다
				int StartMotionIndex = (Desc.first.first);
				int EndMotionIndex = (Desc.first.second);
				_float Distance = Desc.second.second;
				//인덱스 변경
	
				string strNo =to_string(iIndex);
				if (ImGui::Button(strNo.c_str(), ImVec2(20, 20)))
				{
					m_Monsters[m_iNowMonster]->m_iCurAnimToAnimIndex=iIndex;
				}
				ImGui::SameLine();
				ImGui::Text(Desc.second.first.Name);
				ImGui::SameLine();
				ImGui::Text("  startIndex: %d  ",StartMotionIndex);
				ImGui::SameLine();
				ImGui::Text("EndIndex: %d  ", EndMotionIndex);
				if (NowAnimation == iIndex) {
					
					ImGui::SameLine(); 
					ImGui::SliderFloat("Distance: ", &Distance, 0.f, 2.f);
				}
				else {
					ImGui::SameLine();
					ImGui::Text("Distance:%f",Distance);
				}
				Desc.second.second = Distance;
				++iIndex;
			}
			ImGui::EndChild();
			if (!isEmpty)
			{
				if (ImGui::Button("Start AnimToAnim ", ImVec2(200, 20)))
				{
					m_Monsters[m_iNowMonster]->StartAnimToAnim();
				}
				m_Monsters[m_iNowMonster]->Play_AnimToAnim(
					(*iter).first.first,
					(*iter).first.second,
					(*iter).second.second
				);
			}
		if (ImGui::Button("Delete NowAnimation"))
		{
			
			m_Monsters[m_iNowMonster]->m_eAnim_Desc.m_mapDistances.erase(iter);
			
			if (m_Monsters[m_iNowMonster]->m_iCurAnimToAnimIndex == m_Monsters[m_iNowMonster]->m_eAnim_Desc.m_mapDistances.size())
				m_Monsters[m_iNowMonster]->m_iCurAnimToAnimIndex--;
		}
	}

	

	bool Modal = false;
	if (Modal = ImGui::Button("Save Now AnimData For Tool", ImVec2(200, 30)))
	{
		if (true == Modal)
		{
			m_bModal_For_Save_Dat_Intool = { true };
		}

	}

	if (m_bModal_For_Save_Dat_Intool) {
		ImGui::Begin("Confirm Save");
		if (ImGui::Button("Yes", ImVec2(200, 20)))
		{
			HANDLE hFile = CreateFile(m_AnimPathes_For_Tool[m_iNowMonster], GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, nullptr);
			if (INVALID_HANDLE_VALUE == hFile)
			{
				MSG_BOX("Coudnt Find File ");
				ImGui::End();
				return;
			}
			_ulong dwSize = 0;
			_ulong dwByte = 0;

			const CMonster::ANIM_DESC& Desc = m_Monsters[m_iNowMonster]->m_eAnim_Desc;


			_uint iSize = Desc.m_mapDistances.size();
			WriteFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);
			for (auto el : Desc.m_mapDistances)
			{
				_uint iIndex1 = el.first.first;
				_uint iIndex2 = el.first.second;
				_float fDistance = el.second.second;
				_char szAnimName[MAX_PATH]="";
				_uint iStrlen = 0;
				memcpy(szAnimName, el.second.first.Name,strlen(el.second.first.Name));
				iStrlen = strlen(szAnimName)+1;

				WriteFile(hFile, &iIndex1, sizeof(_uint), &dwByte, nullptr);
				WriteFile(hFile, &iIndex2, sizeof(_uint), &dwByte, nullptr);
				WriteFile(hFile, &fDistance, sizeof(_float), &dwByte, nullptr);
				WriteFile(hFile, &iStrlen, sizeof(_uint), &dwByte, nullptr);
				WriteFile(hFile, &szAnimName, iStrlen, &dwByte, nullptr);


			}

			CloseHandle(hFile);
			m_bModal_For_Save_Dat_Intool = { true };
		}
		ImGui::End();
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Load Now AnimData For Tool", ImVec2(200, 30)))
	{

		m_Monsters[m_iNowMonster]->m_eAnim_Desc.m_mapDistances.clear();
		HANDLE hFile = CreateFile(m_AnimPathes_For_Tool[m_iNowMonster], GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			MSG_BOX("rename File Path");
			ImGui::End();
			return;
		}
		_ulong dwByte = 0;
		_ulong dwSize = 0;
		_uint iAnimInfoSize = { 0 };
		ReadFile(hFile, &iAnimInfoSize, sizeof(_uint), &dwByte, nullptr);
	
		for (int i = 0; i < iAnimInfoSize;++i)
		{
			_uint iIndex1 = { 0 };
			_uint iIndex2 = { 0 };
			_float fDistance = { 0.f };
			_char szAnimName[MAX_PATH]="";
			_uint iStrlen = 0;
			ReadFile(hFile,&iIndex1,sizeof(_uint),&dwByte,nullptr);
			ReadFile(hFile, &iIndex2, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &fDistance, sizeof(_float),&dwByte,nullptr);
			ReadFile(hFile, &iStrlen, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &szAnimName,iStrlen, &dwByte,nullptr);
			CMonster::Name eName;
			memcpy(eName.Name, szAnimName,iStrlen);
			m_Monsters[m_iNowMonster]->m_eAnim_Desc.m_mapDistances.emplace(make_pair(iIndex1, iIndex2), make_pair(eName,fDistance));
		}
		CloseHandle(hFile);

	}
	
	if (ImGui::Button("Save Now AnimData For Client", ImVec2(200, 30)))
	{
		
			HANDLE hFile = CreateFile(m_AnimPathes[m_iNowMonster], GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, nullptr);
			if (INVALID_HANDLE_VALUE == hFile)
			{
				MSG_BOX("Coudnt Find File ");
				ImGui::End();
				return;
			}
			_ulong dwSize = 0;
			_ulong dwByte = 0;

			const CMonster::ANIM_DESC& Desc = m_Monsters[m_iNowMonster]->m_eAnim_Desc;


			_uint iSize = Desc.m_mapDistances.size();
			WriteFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);
			for (auto el : Desc.m_mapDistances)
			{
				_uint iIndex1 = el.first.first;
				_uint iIndex2 = el.first.second;
				_float fDistance = el.second.second;

				WriteFile(hFile, &iIndex1, sizeof(_uint), &dwByte, nullptr);
				WriteFile(hFile, &iIndex2, sizeof(_uint), &dwByte, nullptr);
				WriteFile(hFile, &fDistance, sizeof(_float), &dwByte, nullptr);

			}

			CloseHandle(hFile);

		}

	ImGui::End();


}


void CLevel_AnimTool::ClearMonsters()
{



	for (int i = 1; i <= m_iMonsterNum; ++i) {
		Safe_Release(m_Monsters[i]);
		m_pGameInstance->Remove_Object(LEVEL_GAMEPLAY, L"Layer_Monster", m_Monsters[i]);
	}
	m_Monsters.clear();
	
}




CLevel_AnimTool * CLevel_AnimTool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_AnimTool*	pInstance = new CLevel_AnimTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_AnimTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_AnimTool::Free()
{


	for (int i = 1; i <= m_iMonsterNum; ++i)
	{
		Safe_Release(m_Monsters[i]);
	}
	m_Monsters.clear();

	Safe_Release(m_pCamera);
	Safe_Release(m_pImguiMgr);
	__super::Free();
}
