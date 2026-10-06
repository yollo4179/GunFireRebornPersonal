#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CUI_Inventory_WeaponPicture final :public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		

	}UI_INVENTORY_WEAPON_PICTURE_DESC;
private:
	explicit CUI_Inventory_WeaponPicture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Inventory_WeaponPicture(const CUI_Inventory_WeaponPicture& Prototype);
	virtual ~CUI_Inventory_WeaponPicture() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: 
	void Set_PlayerToolNo(_uint iPlayerToolNo) { m_iPlayerToolNo = iPlayerToolNo; };

	
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr};
	CTexture* m_pTextureComBG = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	_uint m_iPlayerToolNo = {};
	map<_uint, _uint>mapPlayerArmID;



private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources( _uint iNowArmTexture = 0);
	HRESULT Bind_ShaderResources_BG();

public:
	static   CUI_Inventory_WeaponPicture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END

