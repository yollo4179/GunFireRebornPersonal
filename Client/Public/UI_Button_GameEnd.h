#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class  CUI_Button_GameEnd final :public CUIObject
{
	typedef struct : public CUIObject::UIOBJECT_DESC
	{

	}UI_BUTTTON_GAMEEND_DESC;

private:
	 CUI_Button_GameEnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	 CUI_Button_GameEnd(const   CUI_Button_GameEnd& Prototype);
	virtual ~ CUI_Button_GameEnd() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Event_MouseOn();
	void Event_MouseAway();
	void Event_MouseUp();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };



private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static   CUI_Button_GameEnd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END

