#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
#include"Player.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END
BEGIN(Client)
class CUI_Inventory_Weapon_Explain final :public CUI_Parent
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		//1280 720 -> 100, 100, -- 1180 620
	}UI_INVENTORY_WEAPON_EXPLAIN_DESC;

	typedef struct
	{
		_tchar sz_Name[MAX_PATH];
		_tchar sz_Feature[MAX_PATH];
	}Arm_InfoText;

private:
	explicit  CUI_Inventory_Weapon_Explain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Inventory_Weapon_Explain(const CUI_Inventory_Weapon_Explain& Prototype);
	virtual ~CUI_Inventory_Weapon_Explain() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void Set_PlayerToolNo(_uint iPlayerToolNo, _uint iPlayerToolIndex);

	_uint Get_SwappedCard() { return m_iSwappedCard; };
	void Reset_SwappedCard() { m_iSwappedCard = -1; };
private:
	HRESULT  Ready_Components();
	HRESULT  Ready_ChildUIs();
	HRESULT  Bind_ShaderResources();
	HRESULT  Bind_And_Render_ToolNumber();
private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom= { nullptr };

	CTexture* m_pTextureCom_ToolNumber = { nullptr };

	map<_uint, _uint>mapPlayerArmID;
	_uint m_iPlayerToolIndex = {};
	_int m_iSwappedCard = {};
	HandsInfo m_eHandsInfo = {};
	_uint m_iMyArmNo = {};
	map<_uint, Arm_InfoText>m_mapArmInfo;



public:
	static   CUI_Inventory_Weapon_Explain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
