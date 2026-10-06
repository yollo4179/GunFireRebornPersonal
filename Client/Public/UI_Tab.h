#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUI_Tab final :public CUI_Parent
{

public:

	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		

	}UI_TAB_DESC;
private:
	explicit CUI_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Tab(const CUI_Tab& Prototype);
	virtual ~CUI_Tab() = default;


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
	HRESULT BindAndRender_Tab();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_Icon= { nullptr };
	CTexture* m_pTextureCom_Tab = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	
public:
	static  CUI_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
