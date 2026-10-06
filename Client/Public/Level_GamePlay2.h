#pragma once
#include"Client_Defines.h"
#include"Level.h"

BEGIN (Client)
class CLevel_GamePlay2 final :public CLevel
{

private:
	CLevel_GamePlay2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay2()=default;


public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
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


	HRESULT Ready_Particle(const _wstring& strLayerTag);
	HRESULT Read_Monsters_Desc_From_File();
	HRESULT Read_Map_Desc_From_File();
	HRESULT Load_Objects_From_File();


public:
	static CLevel_GamePlay2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

private:
	/*player matrix*/

	vector<class CMonster*>m_vecMonsters;
	class CPlayer* m_pPlayer = { nullptr };
	const _float4x4* m_pPlayerFloat4x4 = { nullptr };

	/*BossMatrix*/
	class CMonster* m_pBoss = { nullptr };
	
	vector<CMonster*> m_pMonsters;
};
END
