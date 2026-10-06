#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUI_Bomb final :public CUI_Parent
{

public:

	typedef struct : public CUIObject::UIOBJECT_DESC
	{


	}UI_BOMB_DESC;
private:
	explicit CUI_Bomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Bomb(const CUI_Bomb& Prototype);
	virtual ~CUI_Bomb() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Ready_Components();
	HRESULT Ready_Numbers();
	HRESULT BindAndRender_Icon();
	HRESULT BindAndRender_Icon_Back();
	HRESULT BindAndRender_BG();
	HRESULT BindAndRender_Q();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_BG = { nullptr };
	CTexture* m_pTextureCom_Icon = { nullptr };
	CTexture* m_pTextureCom_Icon_Back = { nullptr };
	CTexture* m_pTextureCom_Q = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float m_fBombCoolTimeRatio = { 0.f };
private:

public:
	static  CUI_Bomb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
