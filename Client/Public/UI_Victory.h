#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END
BEGIN(Client)
class CUI_Victory final :public CUI_Parent
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		//1280 720 -> 100, 100, -- 1180 620
	}UI_VICTORY_DESC;
public:
	enum LOGO_STATE
	{
		STATE_BIGGER,
		STATE_SMALLER,
		STATE_INACTIVE,
		STATE_END,
	};
private:
	explicit  CUI_Victory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Victory(const CUI_Victory& Prototype);
	virtual ~CUI_Victory() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Set_Active()override;
private:
	HRESULT  Ready_Components();
	HRESULT  Ready_ChildUIs();
	HRESULT  Bind_ShaderResources();



private:


	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };


	_uint m_iState = { };
	_float m_fAccSmallerTime = {};
	_float m_fSmallerTime = { 0.65f };

	_float m_fAccBiggerAgainTime = {};
	_float m_fBiggerTime = { 1.f };

	_bool m_bStart_Bigger = { false };

public:
	static   CUI_Victory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
