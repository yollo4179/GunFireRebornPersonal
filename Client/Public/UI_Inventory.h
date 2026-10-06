#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END
BEGIN(Client)
class CUI_Inventory final :public CUI_Parent
{

public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		//1280 720 -> 100, 100, -- 1180 620
	}UI_INVENTORY_DESC;
public:
	enum BUTTON_STATE 
	{
		STATE_WEAPON=0x00000001,
		STATE_HERO =0x00000002,
	};
private:
	explicit  CUI_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Inventory(const CUI_Inventory& Prototype);
	virtual ~CUI_Inventory() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	virtual void Set_Active()override;
	virtual void Set_InActive()override;

private:
	HRESULT  Ready_Components();
	HRESULT  Ready_ChildUIs();
	HRESULT  Bind_ShaderResources();

public:
	void Update_HandsInfo(_uint iIndex, HandsInfo eHandsInfo);
	
private:

	vector<class CUI_WeaponBox*> m_pWeaponBoxes;
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom_BackGround = { nullptr };
	_uint     m_ShowSelection = STATE_WEAPON;
	
	HandsInfo m_HandsInfo[5];

public:
	static   CUI_Inventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

