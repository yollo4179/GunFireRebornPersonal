#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END
BEGIN(Client)
class CUI_Loading final :public CUI_Parent
{

public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		//1280 720 -> 100, 100, -- 1180 620
	}UI_LOADING_DESC;


private:
	explicit  CUI_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Loading(const CUI_Loading& Prototype);
	virtual ~CUI_Loading() = default;
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
	HRESULT  Bind_ShaderResources_BG();
	HRESULT  Bind_ShaderResources_Fill();
	HRESULT  Bind_ShaderResources_Boss();
	HRESULT  Bind_ShaderResources_Middle1();
	HRESULT  Bind_ShaderResources_Middle2();
	HRESULT  Bind_ShaderResources_Middle3();
	HRESULT  Bind_ShaderResources_Begin();

	HRESULT Bind_ShaderResources_Bar_Static();
	HRESULT Bind_ShaderResources_Fill_Static();

public:
	void Set_Ratio(_float fRatio) { m_fRatio = fRatio; };

public:
	void Set_Static_Loading() { m_isStatic = true; }
private:

	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	CTexture* m_pTextureCom_BG = { nullptr };
	CTexture* m_pTextureCom_Fill = { nullptr };
	
	CTexture* m_pTextureCom_Begin = { nullptr };
	CTexture* m_pTextureCom_Middle = { nullptr };
	CTexture* m_pTextureCom_Boss = { nullptr };

	CTexture* m_pTextureCom_Static_Loading_Bar = { nullptr };
	CTexture* m_pTextureCom_Static_Loading_Fill = { nullptr };

private:
	_float m_fRatio = {};
	_bool m_isStatic = { false };

public:
	static   CUI_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END;
