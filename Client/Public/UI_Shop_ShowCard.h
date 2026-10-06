#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
#include"UI_Shop.h"
#include"UI_Shop.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Shop_ShowCard final : public CUI_Parent
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
	}UI_SHOP_SHOWCARD_DESC;

private:
	CUI_Shop_ShowCard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Shop_ShowCard(const  CUI_Shop_ShowCard& Prototype);
	virtual ~CUI_Shop_ShowCard() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_ItemInfo(CUI_Shop::ItemInfo eItemInfo);

private:
	CShader*  m_pShaderCom             = { nullptr };
	CTexture* m_pTextureCom_ShowCard    = { nullptr };
	CTexture* m_pTextureCom_Item       = { nullptr };
	CTexture* m_pTextureCom_Glow       = { nullptr };
	CTexture* m_pTextureCom_Frame      = { nullptr };
	CTexture* m_pTextureCom_Click      = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom     = { nullptr };
	
	CUI_Shop::ItemInfo m_eItemInfo = {};


private:
	HRESULT Ready_Components();
	HRESULT Render_ShaderResources_ShowCard();
	HRESULT Render_ShaderResources_Frame();
	HRESULT Render_ShaderResources_Item();
	HRESULT Render_ShaderResources_Glow();
	HRESULT Render_ShaderResources_Click();

public:
	static  CUI_Shop_ShowCard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
