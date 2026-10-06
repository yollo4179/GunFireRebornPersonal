#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUI_Dash final :public CUI_Parent
{

public:

	typedef struct : public CUIObject::UIOBJECT_DESC
	{


	}UI_DASH_DESC;
private:
	explicit CUI_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Dash(const CUI_Dash& Prototype);
	virtual ~CUI_Dash() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Ready_Components();
	
	HRESULT BindAndRender_Icon();
	HRESULT BindAndRender_Icon_Back();
	HRESULT BindAndRender_BG();
	HRESULT BindAndRender_Shift();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_BG = { nullptr };
	CTexture* m_pTextureCom_Icon = { nullptr };
	CTexture* m_pTextureCom_Icon_Back = { nullptr };
	CTexture* m_pTextureCom_Shift = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float m_fBombCoolTimeRatio = { 0.f };
private:

public:
	static  CUI_Dash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
