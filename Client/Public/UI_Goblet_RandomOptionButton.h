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
class CUI_Goblet_RandomOptionButton final : public CUI_Parent
{
public:
	enum REINFORCE_TYPE
	{
		GRENADE_CRI_DEMAGE,
		GRENADE_CRI_CHANCE,
		GRENADE_RANGE,
		GRENADE_LOADPLUS,
		GRENADE_DEMAGE,
		HP_PLUS,
		REINFORCE_END,
	};
#define GOBLET_GRENADE_CRI_DMG (REINFORCE_TYPE::GRENADE_CRI_DEMAGE==m_iGobletType)
#define GOBLET_GRENADE_CRI_CHANCE (REINFORCE_TYPE::GRENADE_CRI_CHANCE==m_iGobletType)
#define GOBLET_GRENADE_RANGE (REINFORCE_TYPE::GRENADE_RANGE==m_iGobletType)
#define GOBLET_GRENADE_LOAD (REINFORCE_TYPE::GRENADE_LOADPLUS==m_iGobletType)
#define GOBLET_GRENADE_DMG (REINFORCE_TYPE::GRENADE_DEMAGE==m_iGobletType)
#define GOBLET_GRENADE (GOBLET_GRENADE_CRI_DMG||GOBLET_GRENADE_CRI_CHANCE||GOBLET_GRENADE_RANGE||GOBLET_GRENADE_LOAD||GOBLET_GRENADE_DMG)

public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
	}UI_GOBLET_RANDOM_OPTION_BUTTON_DESC;

private:
	CUI_Goblet_RandomOptionButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Goblet_RandomOptionButton(const  CUI_Goblet_RandomOptionButton& Prototype);
	virtual ~CUI_Goblet_RandomOptionButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool Get_Button() { return m_bOptionSelected; };
	void Set_Button(_bool isActive) { m_bOptionSelected = isActive; };

	void Set_GobletType(_uint iGoblet) { m_iGobletType = iGoblet; };

private:
	void Event_MouseOn();
	void Event_MouseAway();
	void Event_MouseUp();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom[10] = {nullptr};
	CTexture* m_pTextureCom_BG[2] = { nullptr };
	CTexture* m_pTextureCom_Kind = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_bool m_bOptionSelected = { false };
	_bool m_isMouseOn = { false };
	_uint m_iGobletType = {};

	_float m_fSizefOnMouse = {1.F};
	_tchar m_szText[REINFORCE_END][MAX_PATH];
private:
	HRESULT Ready_Components();
	HRESULT Ready_Text();
	HRESULT Render_ShaderResources_BG();
	HRESULT Render_ShaderResources_Icon();
	HRESULT Render_ShaderResources_Kind();

public:
	static  CUI_Goblet_RandomOptionButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};
END
