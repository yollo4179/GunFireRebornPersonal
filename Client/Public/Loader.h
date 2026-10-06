#pragma once

#include "Client_Defines.h"
#include "Base.h"


#define GAMEPLAY2_LOADING_BEGIN(K) \
m_AsyncLoaders_GamePlay2[K]->Begin_Thread([&]()->HRESULT {

#define GAMEPLAY_LOADING_BEGIN(K) \
m_AsyncLoaders_GamePlay1[K]->Begin_Thread([&]()->HRESULT {

#define LOGO_LOADING_BEGIN(K) \
m_AsyncLoaders_Logo[K]->Begin_Thread([&]()->HRESULT {

#define LOADING_END return S_OK;});

/* 서브 스레드를 생성한다.  */
/* 다음 레벨에 필요한 자원을 로드하는 역활 */
BEGIN(Engine)
class CGameInstance;
END



BEGIN(Client)
class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LEVELID eNextLevelID);
	HRESULT Loading();

#ifdef _DEBUG
public:
	void Show_Debug();
#endif

public:
	_bool isFinished() const {
		return m_isFinished;
	}

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	LEVELID					m_eNextLevelID = { LEVEL_END };
	HANDLE					m_hThread = {};
	CRITICAL_SECTION		m_Critical_Section = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };

private:
	_bool					m_isFinished = { false };
	_tchar					m_szLoadingText[MAX_PATH] = {};	
private:
	vector< class CAsyncLoader*>m_AsyncLoaders_Logo;
	vector< class CAsyncLoader*>m_AsyncLoaders_GamePlay1;
private:
	HRESULT Loading_Level_Logo();
	HRESULT Loading_Level_GamePlay();
	HRESULT Loading_Level_GamePlay2();

	HRESULT Load_UI();
	HRESULT Load_UI_Tool();
	HRESULT Load_UI_MonsterHP();
	HRESULT Load_UI_BossHP();
	HRESULT Load_DemageFont();
	HRESULT Load_UI_MiniMap();
	HRESULT Load_UI_PlayerHP();
	HRESULT Load_UI_Invectory();
	HRESULT Load_UI_Shop();
	HRESULT Load_UI_Money();
	HRESULT Load_UI_Talk();
	HRESULT Load_UI_Dialogue();
	HRESULT Load_UI_Goblet();
	HRESULT Load_UI_Keys();
	HRESULT Load_UI_Dash();
	HRESULT Load_UI_Victory();

public:
	void AddUpdateCnt_Static();
	void Add_UpdateCnt_Stage_1();
	void Add_UpdateCnt_Stage_2();
public:
	_float m_fNowRatio = { 0.f };
	_uint m_iMaxStatic_Cnt = { 18 };
	_int m_iMaxStage1_Cnt = { 11 };
	_uint m_iMaxStage2_Cnt = { 8 };
	
	
public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END