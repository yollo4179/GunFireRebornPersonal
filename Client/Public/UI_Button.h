#pragma once
#include"UIObject.h"
class CUI_Button abstract :public CUIObject 
{
protected: 
	explicit CUI_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Button(const CUI_Button& Protorype);
	virtual ~CUI_Button() = default;



public:
	virtual bool isClicked();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Add_Child_UIs() = 0;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

