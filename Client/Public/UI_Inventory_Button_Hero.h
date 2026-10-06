#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Inventory_Button_Hero final: public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{

	}UI_BUTTTON_INVENTORY_HERO_DESC;

private:
	CUI_Inventory_Button_Hero(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Inventory_Button_Hero(const  CUI_Inventory_Button_Hero& Prototype);
	virtual ~CUI_Inventory_Button_Hero() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool Get_Button() { return m_bActiveHero; };
	void Set_Button(_bool isActive) { m_bActiveHero = isActive; };
	void Set_Selected(_bool isSelected) { m_isSelected = isSelected; };
private:
	void Event_MouseOn();
	void Event_MouseAway();
	void Event_MouseUp();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureCom_CheckGlow = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_bool m_bActiveHero = { false };
	_bool m_isSelected = { false };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();

public:
	static  CUI_Inventory_Button_Hero* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
