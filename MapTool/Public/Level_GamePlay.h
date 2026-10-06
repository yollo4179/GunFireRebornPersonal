#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include"ImguiWindow.h"

enum class CREATE_OBJECT_MODE
{
	COLLIDER,
	MONSTER,
	OBJECT,
	MAP,
	NAVI_MESH_VTX,
	MODE_END,

};
enum class STAGE
{
	STAGE01=1,
	STAGE02,
	STAGE03,
	STAGE_END,
};
const int m_iMonsterNum = 3;//FOR Monsters:: 7;

const int IMGUI_ColliderMode = 1;
const int IMGUI_MonsterMode =  2;
const int IMGUI_MapAdjustingMode = 4;
const int IMGUI_NaviMeshMode = 8;

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_GameObject(const _wstring& strLayerTag);
	HRESULT Ready_Lights();
	HRESULT Ready_Monster(const _wstring& strLayerTag);
	
	/*For STAGE Focusing*/
	void IMGUI_SELCT_STAGE();
	
	/*For Picking*/
	pair<_vector,_vector> Get_PickingPosInWorld();

	/*ForColliders*/
	HRESULT IMGUI_SelectMode();
	HRESULT Picking_And_Create_Collider();
	class CCubeCollider* Picking_Collider_Box(int iIndex = 0);
	void	IMGUI_Creating_Collider();

	/*for Monsters*/
	HRESULT Creating_Monsters();
	void IMGUI_Createing_Monsters();
	class CMonster* picking_Monster(int i,int j);
	bool Check_NowMonster_NaviIndex();
	void ClearMonsters();

	/*For Map_Stage*/
	HRESULT Creating_Map();
	void IMGUI_Moving_Map();
	
	/*For Navi Mesh*/
	void Adjusting_Triangle(TRIANGLE_VERTICES& Triangle);
	void Mapping_Points(_float3& Point);
	void IMGUI_NaviMesh();
	void Creating_Vertex();
	void Creating_Vertex_On_Virtual_Terrain();
	void Clear_NowTriangle();

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

private:

	STAGE m_eCurStage = STAGE:: STAGE01;

	_tchar m_iNowMapPaths[(_int)STAGE::STAGE_END][(_int)CREATE_OBJECT_MODE::MODE_END][MAX_PATH] =
	{	{{L""} ,{L"" } ,{L"" }, {L"" }},
		{{L"../Bin/Resources/LoadData/Colliders_Stage01.dat"},{L"../Bin/Resources/LoadData/Monsters_Stage01_Layer06.dat"},{L"../Bin/Resources/LoadData/Objects_Stage06.dat"},{L"../Bin/Resources/LoadData/Map_Stage01.dat"}},
		{{L"../Bin/Resources/LoadData/Colliders_Stage02.dat"},{L"../Bin/Resources/LoadData/Monsters_Stage02.dat"},{L"../Bin/Resources/LoadData/Objects_Stage02.dat"},{L"../Bin/Resources/LoadData/Map_Stage02.dat"}},
		{{L"../Bin/Resources/LoadData/Colliders_Stage03.dat"},{L"../Bin/Resources/LoadData/Monsters_Stage03.dat"},{L"../Bin/Resources/LoadData/Objects_Stage03.dat"},{L"../Bin/Resources/LoadData/Map_Stage03.dat"}},
	};
	_tchar m_iNowMapPaths_Client[(_int)STAGE::STAGE_END][(_int)CREATE_OBJECT_MODE::MODE_END][MAX_PATH] =
	{ {{L""} ,{L"" } ,{L"" }, {L"" }},
		{{L"../Bin/Resources/LoadData/Colliders_Stage01.dat"},{L"../Bin/Resources/LoadData/Client_Monsters_Stage01_Layer06.dat"},{L"../Bin/Resources/LoadData/Client_Objects_Stage06.dat"},{L"../Bin/Resources/LoadData/Map_Stage01.dat"}},
		{{L"../Bin/Resources/LoadData/Colliders_Stage02.dat"},{L"../Bin/Resources/LoadData/Monsters_Stage02.dat"},{L"../Bin/Resources/LoadData/Objects_Stage02.dat"},{L"../Bin/Resources/LoadData/Map_Stage02.dat"}},
		{{L"../Bin/Resources/LoadData/Colliders_Stage03.dat"},{L"../Bin/Resources/LoadData/Monsters_Stage03.dat"},{L"../Bin/Resources/LoadData/Objects_Stage03.dat"},{L"../Bin/Resources/LoadData/Map_Stage03.dat"}},
	};
	int IMGUI_MODE = 0;

	CImguiWindow* m_pImguiMgr = { nullptr };

	CREATE_OBJECT_MODE m_eCreateMode = { CREATE_OBJECT_MODE::MODE_END };//콜라이더, 몬스터, 오브젝트 생성 모드

	//for Terrain_Setting
	class CTerrain* m_pTerrain = { nullptr };
	const _float4* m_pPos = { nullptr };
	_uint		   m_VerticesX = { 0 };
	_uint		   m_VerticesZ = { 0 };
	bool		  m_bCreateMode = false;
	bool		  m_bPickingMode = false;

	/* for Manageming Cube Created*/
	vector<class CCubeCollider*> m_vecCubeCollider;
	_float3						 m_vCollider_Scale = { 1.f,1.f,1.f };
	_float4						 m_vCollider_RotAxis = { 0.f,1.f,0.f,0.f };
	_float						 m_vCollider_Rotation = { 0.f };
	_float4						 m_vCollider_Position = { 0.f,0.f,0.f,1.f };
	_int						 m_iCubeColliderNowIndex = { 0 };
	_bool						 m_bCreatingCubeMode = { false };

	/*for Monster*/
	vector<class CMonster*>m_Monsters[m_iMonsterNum+1];
	int m_iMonsterNowIndex[m_iMonsterNum+1] = { };		//지금 보고 있는 몬스터의 인덱스
	int m_iNowMonster = {1};									//지금 배치할 몬스터의 번호


	/*for Stage_Map*/
	class CTerrain_Base* m_pBaseTerrain = {nullptr};
	class CFlat_Virtual_Terrain* m_pVirtualTerrain = { nullptr };
	
	TRIANGLE_VERTICES m_NowTriangle = {};
	_int m_iNowTriangle_Point = 0;

	vector<TRIANGLE_VERTICES> m_Triangles;/* navi mesh*/
	vector<TRIANGLE_VERTICES> m_TrianglesTo_Store_InClient_NaviMesh; /*ToStore*/
	vector< _uint>m_vecStage_Number;
	_int m_iNowStageNumber = 0;
	vector<_float3> m_vecPoints_Navimesh;

	_bool m_bMake_NaviMeshOnVirtualTerrain =false;




};
END