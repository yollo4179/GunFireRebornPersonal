#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END
BEGIN(Client)
class CUI_Inventory_Weapons final :public CUI_Parent
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		//1280 720 -> 100, 100, -- 1180 620
	}UI_INVENTORY_WEAPON_DESC;

private:
	explicit  CUI_Inventory_Weapons(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Inventory_Weapons(const CUI_Inventory_Weapons& Prototype);
	virtual ~CUI_Inventory_Weapons() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT  Ready_Components();
	HRESULT  Ready_ChildUIs();
	HRESULT  Bind_ShaderResources();
private:

	vector<class CUI_WeaponBox*> m_pWeaponBoxes;
	_uint     m_bShowSelection = { 0 };
	_uint m_iMinWeaponBoxIndex = 0;
	_uint m_iMaxWeaponBoxIndex = 2;
public:
	static   CUI_Inventory_Weapons* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

