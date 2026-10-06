#pragma once
#include"Base.h"
#include"UIObject.h"



BEGIN(Engine)
class CUI_Manager final:public CBase
{
private:
	explicit CUI_Manager();
	virtual ~CUI_Manager()=default;


public:
	HRESULT Initialize();
	void Update_UIs();
	
	HRESULT Add_UI_To_UIMap(const _wstring UI_Tag, CUIObject* pUIObject,UI_TYPE eType= UI_TYPE::UI_FIXED);
	HRESULT Set_Active_A_UI(const _wstring UI_Tag);
	HRESULT Set_Active_All_UIs();
	HRESULT Set_InActive_A_UI(const _wstring UI_Tag);
	HRESULT Set_InActive_All_UIs();
	HRESULT Clear_UIObjects();
	HRESULT Change_A_UI_SortOrder(const _wstring UI_Tag, _uint iSortOrder,UI_TYPE eType=UI_TYPE::UI_FIXED);
	CUIObject* Find_UIObject(const _wstring UI_Tag);
	CUIObject* Select_Object(_float2 MousePos);

	/*ForManaging DemageFont*/
	CUIObject* Get_Now_UI_DemageFont();
	HRESULT Add_DemageFonts(CUIObject* pDemageFont);
	void Reset_NumDemageFonts() { m_iNumDemageFonts = 0; };
private:
	map<const _wstring ,CUIObject*>m_mapUIs;

	/*For Managing DemageFont*/
	vector <CUIObject*> m_pDemageFonts;
	_int m_iNowDemageFontIndex = {0};
	_uint m_iNumDemageFonts = {0};

public:

	static CUI_Manager* Create();
	virtual void Free();

	const _int m_iMinFixdUIDepth = { 10 };
		 _int m_iMaxFixedUIDepth = { 10 };
	
	const _int m_iMinPopUpUIDepth = { 100 };
	_int m_iMaxPopUpUIDepth = { 100 };


};
END
