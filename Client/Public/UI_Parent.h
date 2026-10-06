#pragma once
#include"UIObject.h"
#include"Client_Defines.h"

BEGIN(Client)
class CUI_Parent abstract :public CUIObject
{
protected:
	explicit CUI_Parent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Parent(const CUI_Parent& Prototype);
	virtual ~CUI_Parent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Add_Child_UI(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& LayerTag,const _wstring& strChild_UI_Tag, _uint iLevelIndex,CUIObject** ppOut, void* pArg = nullptr);
	virtual CUIObject* Find_Child_UI(const _wstring& strChild_UI_Tag);
	virtual HRESULT Ready_Child_UI();
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	map<const _wstring, CUIObject*>m_ChildUIs;

};
END

