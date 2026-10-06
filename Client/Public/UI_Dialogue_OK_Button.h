#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUI_Dialogue_OK_Button final : public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
	}UI_DIALOGUE_OK_BUTTON_DESC;
private:
	CUI_Dialogue_OK_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Dialogue_OK_Button(const  CUI_Dialogue_OK_Button& Prototype);
	virtual ~CUI_Dialogue_OK_Button() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool Get_Button() { return m_isClicked; };
	void Set_Button(_bool isActive) { m_isClicked = isActive; };
private:
	void Event_MouseOn();
	void Event_MouseAway();
	void Event_MouseUp();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr }; 
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_bool m_isClicked = { false };

private:
	HRESULT Ready_Components();
	HRESULT Render_ShaderResources();

public:
	static  CUI_Dialogue_OK_Button* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
