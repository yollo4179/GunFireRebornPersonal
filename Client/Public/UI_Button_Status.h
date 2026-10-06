#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class  CUI_Button_Status final :public CUIObject
{
	typedef struct : public CUIObject::UIOBJECT_DESC
	{

	}UI_BUTTTON_STATUS_DESC;

private:
	 CUI_Button_Status(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	 CUI_Button_Status(const   CUI_Button_Status& Prototype);
	virtual ~ CUI_Button_Status() = default;

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
	static   CUI_Button_Status* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
