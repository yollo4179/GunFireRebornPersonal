#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Monster.h"
#include"Npc.h"
BEGIN(Engine)
class CMonster;
END

BEGIN(Client)

const int M_SEMI_STAGE = 6;
const int M_MONSTER_TYPES = 7;

const int M_NPC_TYPES = 3;

class CLevel_GamePlay final : public CLevel
{


private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;	

public:
	void Update_Check_Stage_Clear();

private:
	HRESULT Ready_ShadowCamera();
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_UI(const _wstring& strLayerTag);
	HRESULT Ready_Layer_GameObject(const _wstring& strLayerTag);
	HRESULT Ready_Lights();
	HRESULT Ready_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Map(const _wstring& strLayerTag);
	HRESULT Ready_Player(const _wstring& strLayerTag);
	HRESULT Ready_DropItem(const _wstring& strLayerTag);
	HRESULT Ready_Npc(const _wstring& strLayerTag);
	HRESULT Ready_Trail(const _wstring strLayerTag);

	HRESULT Ready_Particle(const _wstring& strLayerTag);
	HRESULT Read_Monsters_Desc_From_File();
	HRESULT Read_Map_Desc_From_File();
	HRESULT Read_Npc_Desc_From_File();
	HRESULT Load_Objects_From_File();


	
	

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

private:
	//CImguiWindow* m_pImguiMgr = { nullptr };

	/*For Managing Monsters And Load Initial MonsterState*/
	vector<CMonster*>m_vecMonsters[M_SEMI_STAGE+1];
	vector<CMonster::MONSTER_DESC>m_vecMonsterDescs[M_SEMI_STAGE+1][M_MONSTER_TYPES+1];
	_uint m_iNowStage = { 0 };
	_bool m_bCanGo_NextStage = { false };
	_tchar m_DescPathes[M_SEMI_STAGE + 1][MAX_PATH] = {
		L"",
		L"../Bin/Resources/LoadData/Client_Monsters_Stage01_Layer01.dat",
		L"../Bin/Resources/LoadData/Client_Monsters_Stage01_Layer02.dat",
		L"../Bin/Resources/LoadData/Client_Monsters_Stage01_Layer03.dat",
		L"../Bin/Resources/LoadData/Client_Monsters_Stage01_Layer04.dat",
		L"../Bin/Resources/LoadData/Client_Monsters_Stage01_Layer05.dat",
		L"../Bin/Resources/LoadData/Client_Monsters_Stage01_Layer06.dat",
	};
	/*For Npc Paths*/

	vector <CNpc*> m_vecNpcs[M_SEMI_STAGE + 1];
	vector<CNpc::NPC_DESC>m_vecNpcDescs[M_SEMI_STAGE + 1][M_NPC_TYPES+1];
	_tchar m_DescPathes_Npc[M_SEMI_STAGE + 1][MAX_PATH] =
	{
		L"",
		L"../Bin/Resources/LoadData/Client_Objects_Stage01.dat",
		L"../Bin/Resources/LoadData/Client_Objects_Stage02.dat",
		L"../Bin/Resources/LoadData/Client_Objects_Stage03.dat",
		L"../Bin/Resources/LoadData/Client_Objects_Stage04.dat",
		L"../Bin/Resources/LoadData/Client_Objects_Stage05.dat",
		L"../Bin/Resources/LoadData/Client_Objects_Stage06.dat",
	};
	_bool m_bActiveNpcAllActive = { false };
	_bool m_bNpcActiveDone = { false };

	/*player matrix*/
	class CPlayer* m_pPlayer = { nullptr };
	const _float4x4* m_pPlayerFloat4x4 = { nullptr };

	/*Client Manger*/
	class CItem_Manager* m_pItem_Manager = { nullptr };

};

END