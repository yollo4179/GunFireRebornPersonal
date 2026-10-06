#pragma once
#pragma once
#include"Client_Defines.h"
#include"Base.h"

BEGIN(Client)

class CAsyncLoader final :public CBase
{

private:
	CAsyncLoader();
	virtual ~CAsyncLoader() = default;


public:
	HRESULT Initialize();
	HRESULT  Loading();

	HRESULT Begin_Thread(std::function<HRESULT()>);


public:
	_bool isFinished() const {
		return m_IsFinished;
	}


private:
	HANDLE	m_hThread = {};
	CRITICAL_SECTION	m_CriticalSection = {};
	std::function<HRESULT()> m_pFunction = {};

private:
	_bool m_IsFinished = {};
	_tchar m_szLoadingText = {};
public:
	static CAsyncLoader* Create();
	virtual void Free() override;
};
END

