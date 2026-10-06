#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
class CUI_Numbers final :public CUIObject
{
public:
	/*percent로 나타낼것인지 그냥 숫자로 나타낼것인지  precentage 가 true라면 어떤 숫자가 들어갈 것인지 */
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		_float fRadian;
		_bool isFraction;
		_bool isDenominatorInfinity;
		_uint iNumerator;
		_uint iDenominator;
		_float3 vColor;

	}UI_NUMBERS_DESC;

private:
	explicit CUI_Numbers(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Numbers(const CUI_Numbers& Prototype);
	virtual ~CUI_Numbers() =default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT BunchToRender(_uint iSymbol);

public:
	void Set_Numerator(_uint iNumber) { m_iNumerator = iNumber; };
	void Set_MaxNumber(_uint iMaxNumber) { m_iDenominator = iMaxNumber; };
	void Set_Infinity(_bool _isInfinity) { m_isDenominatorInfinity = _isInfinity; };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources(_uint iSymbol);
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
private:
	
	/*For Number*/
	_float m_fRadian = { 0 };

	_bool m_isFraction = {};
	_bool m_isDenominatorInfinity={};
	_uint m_iDenominator = {};
	_uint m_iNumerator = {};
	_float3 m_vColor = {};

	_float m_fOffsetY = { 0.f };
	_float m_fOffsetX = { 0.f };

public:
	static  CUI_Numbers* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

