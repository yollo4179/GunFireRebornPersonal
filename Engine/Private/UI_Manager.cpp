#include "UI_Manager.h"
#include<queue>
CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Initialize()
{
    return S_OK;
}

void CUI_Manager::Update_UIs()
{

    _uint m_iCurIndex = m_iMinPopUpUIDepth;

    priority_queue< pair< _uint, CUIObject*> , vector<pair<_uint,CUIObject*>> , greater<pair<_uint, CUIObject*>>> pq;
    for (auto iter = m_mapUIs.begin(); iter != m_mapUIs.end(); ++iter)
    {
        pq.push({ iter->second->Get_SortDepth(), iter->second });
    }

    while (!pq.empty())
    {
        pair<_uint, CUIObject*> pObjectInfo =pq.top();
        if(pObjectInfo.second->Get_Type() == UI_TYPE::UI_POP)
        {
            if (pObjectInfo.first != m_iCurIndex)
            {
                pObjectInfo.second->Set_SortDepth(m_iCurIndex);
            }
            
            ++m_iCurIndex;
            
        }
       
        pq.pop();
    }
    if(m_iMinPopUpUIDepth != m_iCurIndex)
    m_iMaxPopUpUIDepth = m_iCurIndex - 1;

}

HRESULT CUI_Manager::Add_UI_To_UIMap(const _wstring UI_Tag, CUIObject* pUIObject, UI_TYPE eType)
{
    if (nullptr != Find_UIObject(UI_Tag))
    {
        MSG_BOX("Already have this UI Object");
        return E_FAIL;
    }

    m_mapUIs.emplace(UI_Tag,pUIObject);

    if (UI_TYPE::UI_FIXED == eType)
    {
        pUIObject->Set_SortDepth(10);
    }
    /*else if (UI_TYPE::UI_POP == eType)
    {
        pUIObject->Set_SortDepth(m_iMinPopUpUIDepth);
    }*/

    //Safe_AddRef안할거임 
    // 오브젝트 매니저에 추가시 사본 생성하고 주소 받아오면 addref 하니까
    // ui받아와서 여기서만 relese하면 딱맞음 
    return S_OK;
}

HRESULT CUI_Manager::Set_Active_A_UI(const _wstring UI_Tag)
{
    CUIObject* pInstance = { nullptr };
    if (nullptr == (pInstance =Find_UIObject(UI_Tag)) )
    {
        MSG_BOX("There's no UI you named like that");
    }
    pInstance->Set_Active();

    if (UI_TYPE::UI_POP == pInstance->Get_Type())
    {
        pInstance->Set_SortDepth(++m_iMaxPopUpUIDepth);
 
    }

    return S_OK;
}

HRESULT CUI_Manager::Set_Active_All_UIs()
{

    for (auto el : m_mapUIs)
    {
       (el.second)->Set_Active();
    }
    return S_OK;
}

HRESULT CUI_Manager::Set_InActive_A_UI(const _wstring UI_Tag)
{
    CUIObject* pInstance = { nullptr };
    if (nullptr == (pInstance = Find_UIObject(UI_Tag)))
    {
        MSG_BOX("There's no UI you named like that");
    }
    pInstance->Set_InActive();
    UI_TYPE eType = pInstance->Get_Type();
    if (UI_TYPE::UI_POP == eType)
    {
        pInstance->Set_SortDepth(m_iMinPopUpUIDepth);
    }


    return S_OK;
}

HRESULT CUI_Manager::Set_InActive_All_UIs()
{
    for (auto el : m_mapUIs)
    {
        (el.second)->Set_InActive();
    }
    return S_OK;
}

HRESULT CUI_Manager::Clear_UIObjects()
{
    for (auto el : m_mapUIs)
    {
        Safe_Release(el.second);
    }
    m_mapUIs.clear();

    for (auto pDemageFont : m_pDemageFonts)
    {
        Safe_Release(pDemageFont);
    }
    m_pDemageFonts.clear();

    return S_OK;
}

HRESULT CUI_Manager::Change_A_UI_SortOrder(const _wstring UI_Tag, _uint iSortOrder, UI_TYPE eType)
{
    CUIObject* pInstance = { nullptr };
    if (nullptr == (pInstance = Find_UIObject(UI_Tag)))
    {
        MSG_BOX("There's no UI you named like that");
    }
    pInstance->Set_SortDepth(iSortOrder);

    return S_OK;
}

CUIObject* CUI_Manager::Find_UIObject(const _wstring UI_Tag)
{
    auto iter = m_mapUIs.find(UI_Tag);

    if (iter == m_mapUIs.end())
        return nullptr;

    return iter->second;
}
//마우스와 영역이 겹치는 최우선 순위 ui를 가져와 select 상태로 만든 후 마우스 상태를 변경한다.-> 
//마우스에서 클릭(down)했을때 호출 (좌클릭) 마우스는 선택된 유아이를 멤버로 가지고 있으며, 마우스 상태에 따라 마우스 스테이트를 변경(마우스는 체크 스테이트를 업데이트 시마다 호출해서 ui 상태 변경
CUIObject* CUI_Manager::Select_Object(_float2 MousePos) 
{

    priority_queue<pair<int, CUIObject*>>pq;


    for (auto UI : m_mapUIs)
    {
        pq.emplace( UI.second->Get_SortDepth(), UI.second);
    }
    
    CUIObject* SelectedUI = nullptr;
    pair<int, CUIObject*> UISelectedInfo = {};
    
    /*무우스에서 널포인터가 아니라면 -> 마우스 상태를 세팅해서 ....*/
    if (pq.empty())
        return nullptr;

    UISelectedInfo = pq.top();
    SelectedUI = UISelectedInfo.second;
    
    
    if (UI_TYPE::UI_POP == SelectedUI->Get_Type())
    {
        SelectedUI->Set_SortDepth(++m_iMaxPopUpUIDepth);
        SelectedUI->Set_Is_Selected();
        pq.pop();
    }
    else
    {
        return nullptr;
    }
        while (!pq.empty())
        {
            if (UI_TYPE::UI_POP == pq.top().second->Get_Type()) {
                pq.top().second->Set_Is_Not_Selected();
                
            }

        }
    
    return SelectedUI;

}

CUIObject* CUI_Manager::Get_Now_UI_DemageFont()
{
    if (m_iNowDemageFontIndex >= m_iNumDemageFonts || -1>=m_iNowDemageFontIndex)
    {
        MSG_BOX("Failed  to GET DemageFont :Index OUT RANGE");
        return nullptr;
    }

    CUIObject* pNowDemageFont = m_pDemageFonts[m_iNowDemageFontIndex];
    m_iNowDemageFontIndex = (m_iNowDemageFontIndex + 1) % m_iNumDemageFonts;
    return pNowDemageFont;

    return nullptr;
}

HRESULT CUI_Manager::Add_DemageFonts(CUIObject* pDemageFont)
{
    if (nullptr == pDemageFont)
    {
        MSG_BOX("Failed  to Add DemageFont :nullptr");
        return E_FAIL;
    }
    m_pDemageFonts.push_back(pDemageFont);

    ++m_iNumDemageFonts;
    return S_OK;
}

CUI_Manager* CUI_Manager::Create()
{
    CUI_Manager* pInstance = new CUI_Manager();
    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Create UIManager");
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CUI_Manager::Free()
{
    for (auto el : m_mapUIs)
    {
        Safe_Release(el.second);
    }
    m_mapUIs.clear();

    for (auto pDemageFont : m_pDemageFonts)
    {
        Safe_Release(pDemageFont);
    }
    m_pDemageFonts.clear();

    __super::Free();
}
