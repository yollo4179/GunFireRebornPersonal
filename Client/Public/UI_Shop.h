#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END
BEGIN(Client)
class CUI_Shop final :public CUI_Parent
{
public:
	/*¸ÅÄª */
	enum UI_ITEM_LIST
	{
		ITEM_RICE=0,
		ITEM_BULLET=1,
		ITEM_GRENADE=2,
		ITEN_SWORD=3,
		ITEM_THUNDER_GAUNTLET=4,
		ITEM_DEFAULT_GUN=5,
		ITEM_RASORCANON=6,
		ITEM_LIVING_GUN=7,
		ITEM_END,
	};
	typedef struct
	{
		_uint iValue;
		_uint iItemNo;
		_tchar sz_Name[MAX_PATH];
		_tchar sz_Explain_Text[MAX_PATH];
		_uint iIsRare;
		_uint iReinforceType;
	}ItemInfo;

public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		//1280 720 -> 100, 100, -- 1180 620
	}UI_SHOP_DESC;
private:
	explicit  CUI_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Shop(const CUI_Shop& Prototype);
	virtual ~CUI_Shop() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void ResetItems();
	virtual void Set_Active()override;
private:
	HRESULT  Ready_Components();
	HRESULT  Ready_ChildUIs();
	HRESULT  Bind_ShaderResources();
	HRESULT BindAndRender_ShaderResources_Coin();
	HRESULT BindAndRender_ShaderResources_bullet();

private:

	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom_BackGround = { nullptr };
	CTexture* m_pTextureCom_Coins = { nullptr };
	CTexture* m_pTextureCom_Bullets = { nullptr };
	ItemInfo m_eItemInfo[UI_ITEM_LIST::ITEM_END] = {};
	_int m_ShowCardNo = -1;

	
public:
	static   CUI_Shop* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};
END
