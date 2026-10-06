#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CUI_DemageFont final : public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		
	}UI_DEMAGE_FONT_DESC;

private:
	explicit CUI_DemageFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_DemageFont(const CUI_DemageFont& Prototype);
	virtual ~CUI_DemageFont() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT NumberToRender(_uint iSymbol,_int iNumberCount);

public:
	void Set_Demage(_uint iNumber, _bool isCritical);
	void Set_WorldMatrix(_fmatrix DemageMatrix);
	void Set_Active(_bool isActive) { m_bActive = isActive; m_fAccLifeTime = 0.f; };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources(_uint iSymbol,_uint iNumberIndex);


private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float2 m_vSize;
	_float3 m_vColor = {1.f,1.f,0.f};
	_int m_iDemage = { 0 };

	_float m_fLifeTime = 0.6f;
	_float m_fAccLifeTime = 0.f;

public:
	static  CUI_DemageFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};
END

