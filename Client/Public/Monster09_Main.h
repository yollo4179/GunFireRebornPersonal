#pragma once
#include"Monster.h"
#include"Monster09.h"

BEGIN(Client)
class CMonster09_Main final :public CMonster
{
public:

	typedef struct : public CMonster::MONSTER_DESC
	{
		_float4 _vInitPos;
		_float fDomainX;
		_float fDomainZ;
		_float2 fHPInfo;
		vector<CMonster*>* pMonsters;
	}MONSTER09_MAIN_DESC;

private:
	explicit CMonster09_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster09_Main(const CMonster09_Main& Prototype);
	virtual ~CMonster09_Main() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	_bool Compute_DistanceWithPlayer();

public:
	vector<CMonster09*>* Get_Monsters() { return &m_pMonster09s; };
private:
	vector<class CMonster09*> m_pMonster09s;
	_uint m_iNow_Monter09_Index = {0};
	const int  m_iMonster09s_Num = {5};

	vector<class CMonster09_Mini*> m_pMinis;	
	_uint m_iNowMiniIndex = {0};
	const int m_iNumMinis = {10};
	_float m_fAccMiniSummonTime = 0.f;
	_float m_fMiniSummonTime = 20.f;

	vector<class CMonster*>*m_pMonsters;

	_float4 m_vInitPos = {};
	_float m_fDomainX = {};
	_float m_fDomainZ = {};
	_float2 m_vBossHPInfo = { };
	_bool m_bActiveCondition[5] = {true,true,true,true,true  };


	class CUI_BossHP* m_pUIBossHP = { nullptr };

	_bool m_isCutSceneDone = { false };


	
	

private:

	// 새끼 몬스터 10마리 그냥 몬스터
	// 3마리 미리 세팅해 놓고,
	// 다 인액티브 시켜놓자 	
	
	//HRESULT Ready_Components();
	//HRESULT Ready_PartObjects();
	HRESULT Ready_Monsters();
	HRESULT Ready_UI();

public:
	static CMonster09_Main* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};
END
