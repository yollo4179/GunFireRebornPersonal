#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CUI_Talk final : public CUIObject
{

public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		_float2 vFontOffset;
	}UI_TALK_DESC;

private:
	explicit CUI_Talk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Talk(const CUI_Talk& Prototype);
	virtual ~CUI_Talk() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void Set_WorldMatrix(_fmatrix DemageMatrix);
	void Find_XY_ForText();
	
private:
	_float2 m_vFontPos = {};
	_float2 m_vFontOffset = {};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float2 m_vSize;
	_float3 m_vColor = { 1.f,1.f,1.f };
	
	_float m_fLifeTime = 0.6f;
	_float m_fAccLifeTime = 0.f;
	

public:
	static  CUI_Talk* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END

