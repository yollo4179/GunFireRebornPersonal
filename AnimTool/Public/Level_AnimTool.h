#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include"ImguiWindow.h"



const int m_iMonsterNum = 7;

const int IMGUI_MonsterMode =  2;
const int IMGUI_ObjectMode = 4;
const int IMGUI_PlayerMode = 8;


const int ANIM_MODE_LOOP = 1;
const int ANIM_MODE_SLERP = 2;
const int ANIM_MODE_CHECK = 4;

BEGIN(Engine)
class CCamera;
END

BEGIN(Client)

class CLevel_AnimTool final : public CLevel
{
private:
	CLevel_AnimTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_AnimTool() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_GameObject(const _wstring& strLayerTag);
	HRESULT Ready_Lights();
	HRESULT Ready_Monsters(const _wstring& strLayerTag);
	
	/*For STAGE Focusing*/
	
	
	/*For Picking*/
	pair<_vector,_vector> Get_PickingPosInWorld();

	/*For Editing Objects and Monsters or Player*/
	HRESULT IMGUI_SelectMode();
	

	/*for Monsters*/
	void IMGUI_Check_MonstersAnim();
	void ClearMonsters();

	/*For Saving Anim Data*/

	_tchar m_AnimPathes[m_iMonsterNum+1][MAX_PATH]=
	{
		{L""},
		{L"../Bin/Resources/AnimData/Monster_AnimData01.dat"},
		{L"../Bin/Resources/AnimData/Monster_AnimData02.dat"},
		{L"../Bin/Resources/AnimData/Monster_AnimData03.dat"},
		{L"../Bin/Resources/AnimData/Monster_AnimData04.dat"},
		{L"../Bin/Resources/AnimData/Monster_AnimData05.dat"},
		{L"../Bin/Resources/AnimData/Player_AnimData01.dat"},
		{L"../Bin/Resources/AnimData/Player_AnimData02.dat"},
	};

	_tchar m_AnimPathes_For_Tool[m_iMonsterNum + 1][MAX_PATH] =
	{
		{L""},
		{L"../Bin/Resources/AnimToolData/Monster_Tool_AnimData01.dat"},
		{L"../Bin/Resources/AnimToolData/Monster_Tool_AnimData02.dat"},
		{L"../Bin/Resources/AnimToolData/Monster_Tool_AnimData03.dat"},
		{L"../Bin/Resources/AnimToolData/Monster_Tool_AnimData04.dat"},
		{L"../Bin/Resources/AnimToolData/Monster_Tool_AnimData05.dat"},
		{L"../Bin/Resources/AnimToolData/Player_Tool_AnimData01.dat"},
		{L"../Bin/Resources/AnimToolData/Player_Tool_AnimData02.dat"},
	};

	_bool m_bModal_For_Save_Dat_Intool = { false };

public:
	static CLevel_AnimTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

private:

	
	int IMGUI_MODE = 0;

	CImguiWindow* m_pImguiMgr = { nullptr };
	
	

	/*for Monster*/
	int ANIM_MODE = 0;
	vector<class CMonster*>m_Monsters;
	int m_iNowMonster = {1};									//지금 배치할 몬스터의 번호
	
	class CCamera* m_pCamera = nullptr;
};
END