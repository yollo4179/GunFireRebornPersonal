#pragma once

#include "Client_Defines.h"
#include "UI_Parent.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END


BEGIN(Client)

class CBackGround final : public CUI_Parent
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{

	}BACKGROUND_DESC;
private:
	CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBackGround(const CBackGround& Prototype);
	virtual ~CBackGround() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void Set_BackGroundType(_bool isLogoBackGround) { m_isLogoBackGround = isLogoBackGround; };

	void Set_Ratio(_float _fRatio) { m_fRatio = _fRatio; };

public:
	void Set_Static();

private:
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };

	_bool					m_isLogoBackGround = { true };
	_float					m_fAccTime = 5.f;
	_uint m_iCurPaint = { 0 };
	_float m_fRatio = { 0.f };

	_bool m_isStatic = {false};
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_ChildUIs();


public:
	static CBackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END

