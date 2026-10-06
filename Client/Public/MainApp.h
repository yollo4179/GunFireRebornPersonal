#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();
	

private:
	CGameInstance*				m_pGameInstance = { nullptr };
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	/*Client Manger*/
	class CItem_Manager* m_pItem_Manager = { nullptr };

	_float m_fAccFrame = { 0.f };
	_uint m_iFrame = 0;
	_uint m_iNowFrame = { 0 };

private:
	HRESULT SetUp_StartLevel(LEVELID eLevelID);
	HRESULT Ready_Test();
	HRESULT Ready_UI_LOGO();
	HRESULT Ready_LoadingBar();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END