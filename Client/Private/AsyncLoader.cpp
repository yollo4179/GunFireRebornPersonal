#include"stdafx.h"
#include "AsyncLoader.h"

CAsyncLoader::CAsyncLoader()
{
}

HRESULT CAsyncLoader::Initialize()
{
    return S_OK;
}
_uint APIENTRY ALoadingMain(void* pArg)
{
    CAsyncLoader* pLoader = static_cast<CAsyncLoader*>(pArg);
    if (FAILED(pLoader->Loading())) {
        return 1;
    }
    return 0;
}
HRESULT CAsyncLoader::Begin_Thread(std::function<HRESULT()> pEntryPoint)
{
    m_pFunction = std::bind(pEntryPoint);
    InitializeCriticalSection(&m_CriticalSection);
    m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ALoadingMain, this, 0, nullptr);
    
    if (0 == m_hThread)
        return E_FAIL;
    return S_OK;
}
HRESULT  CAsyncLoader::Loading()
{
    EnterCriticalSection(&m_CriticalSection);
    CoInitializeEx(nullptr, 0);
    HRESULT		hr = {};
    hr = m_pFunction();
    m_IsFinished = true;
   

    if (E_FAIL == hr)    
        return E_FAIL;
    return S_OK;
    LeaveCriticalSection(&m_CriticalSection);
}


CAsyncLoader* CAsyncLoader::Create()
{
    CAsyncLoader* pInstance = new CAsyncLoader();
    if (nullptr == pInstance) return nullptr;
    
    return pInstance;
}

void CAsyncLoader::Free()
{
    WaitForSingleObject(m_hThread, INFINITE);
    DeleteObject(m_hThread);
    DeleteCriticalSection(&m_CriticalSection);
    __super::Free();
}
