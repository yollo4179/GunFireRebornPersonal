#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END

BEGIN(Client)
class CUI_Inventory_Weapon_StatusText final :public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		//1280 720 -> 100, 100, -- 1180 620
	}UI_INVENTORY_WEAPON_STATUSTEXT;

private:
	explicit  CUI_Inventory_Weapon_StatusText(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Inventory_Weapon_StatusText(const CUI_Inventory_Weapon_StatusText& Prototype);
	virtual ~CUI_Inventory_Weapon_StatusText() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void Set_Status(_tchar* sz_Status) { memcpy(m_szStatusText, sz_Status, sizeof(m_szStatusText)); };



private:
	HRESULT  Ready_Components();
	HRESULT  Bind_ShaderResources();


private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_tchar m_szStatusText[MAX_PATH] = {};

public:
	static   CUI_Inventory_Weapon_StatusText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};
END
