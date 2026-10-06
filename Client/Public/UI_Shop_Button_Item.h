#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
#include"UI_Shop.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Shop_Button_Item final : public CUI_Parent
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
	}UI_BUTTTON_SHOP_ITEM_DESC;
private:
	CUI_Shop_Button_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Shop_Button_Item(const  CUI_Shop_Button_Item& Prototype);
	virtual ~CUI_Shop_Button_Item() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool Get_Button() { return m_bSoldOutButton; };
	void Set_Button(_bool isActive) { m_bSoldOutButton = isActive; };
	_bool IsMouseOn() { return m_bMouseOn; };

	void Set_Item(_uint iItemNo) { m_ItemNo = iItemNo; };
	void Set_SoldOut() { m_bSoldOut = true; };
	void Reset_SoldOut() { m_bSoldOut = false; };
	_bool Get_SoldOut() { return m_bSoldOut; }

	void Set_ItemInfo(CUI_Shop::ItemInfo eItemInfo);
	CUI_Shop::ItemInfo Get_ItemInfo() { return m_eItemInfo; };
private:
	void Event_MouseOn();
	void Event_MouseAway();
	void Event_MouseUp();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureCom_Glow = { nullptr };
	CTexture* m_pTextureCom_Item = { nullptr };
	CTexture* m_pTextureCom_SoldOut = { nullptr };
	CTexture* m_pTextureCom_Money = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };


	_bool m_bMouseOn = { false };
	_bool m_bSoldOutButton = { false };
	_bool m_bSoldOut = { false };
	_uint m_ItemNo = {};

	CUI_Shop::ItemInfo m_eItemInfo = {};
private:
	HRESULT Ready_Components();
	HRESULT Ready_Number();
	HRESULT Render_ShaderResources_Frame();
	HRESULT Render_ShaderResources_Glow();
	HRESULT Render_ShaderResources_Item();
	HRESULT Render_ShaderResources_SoldOut();
	HRESULT Render_ShaderResources_Money();

public:
	static  CUI_Shop_Button_Item* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END