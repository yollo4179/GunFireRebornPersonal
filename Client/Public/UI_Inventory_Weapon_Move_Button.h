#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Inventory_Weapon_Move_Button final : public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		_uint iButton_Dir;
	}UI_BUTTTON_INVENTORY_WEAPON_MOVE_BUTTON_DESC;


private:
	CUI_Inventory_Weapon_Move_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Inventory_Weapon_Move_Button(const  CUI_Inventory_Weapon_Move_Button& Prototype);
	virtual ~CUI_Inventory_Weapon_Move_Button() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool Get_Button() { return m_bMoveButton; };
	void Set_Button(_bool isActive) { m_bMoveButton = isActive; };
private:
	void Event_MouseOn();
	void Event_MouseAway();
	void Event_MouseUp();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureCom_CheckGlow = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_bool m_bMoveButton = { true };
	_int m_iButton_Dir = {};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static  CUI_Inventory_Weapon_Move_Button* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
