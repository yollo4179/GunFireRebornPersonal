#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUI_Player_HP final :public CUI_Parent
{
public:
	
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		_float4* pPlayerHPInfo;

	}UI_PLAYER_HP_DESC;
private:
	explicit CUI_Player_HP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Player_HP(const CUI_Player_HP& Prototype);
	virtual ~CUI_Player_HP() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Ready_Components();
	HRESULT BindAndRender_ShaderResources_BG();
	HRESULT BindAndRender_ShaderResources_Player();
	HRESULT BindAndRender_ShaderResources_HP_Red();
	HRESULT Bind_ShaderResources_HP_Green();
	HRESULT Ready_ChildUIs();


public:
	void Init_HPInfo(_float4* pPlayerHPInfo) { m_pPlayerHPInfo = pPlayerHPInfo; };
	
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_BG = { nullptr };
	CTexture* m_pTextureCom_Player = { nullptr };
	CTexture* m_pTextureCom_Red = { nullptr };
	CTexture* m_pTextureCom_Green = { nullptr };
	CTexture* m_pTextureCom_Back = { nullptr };

	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float4* m_pPlayerHPInfo;
public:
	static  CUI_Player_HP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
