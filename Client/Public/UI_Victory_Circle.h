#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Victory_Circle final : public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{

	}UI_VICTORY_CIRCLE_DESC;

private:
	CUI_Victory_Circle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Victory_Circle(const  CUI_Victory_Circle& Prototype);
	virtual ~CUI_Victory_Circle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Bigger(_bool _bBigger);
	virtual void Set_Active()override;
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float m_fAccBiggerTime = {};
	_float m_fBiggerTime = { 1.f };
	_bool m_bBigger = {false};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	

public:
	static  CUI_Victory_Circle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};
END
