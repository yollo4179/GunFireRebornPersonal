#pragma once
#include"Base.h"
BEGIN(Engine)
class CLight_Manager final :public CBase
{
private:
	explicit CLight_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(_uint iIndex)const;

	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
public:

	void Set_CurLevel(_uint iLevel) { m_iCurLevel = iLevel; };
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = {nullptr};
	list<class CLight*>*		m_pLights;

	typedef list<class CLight*> LIGHTS;
	_uint m_iNumLevels = {};
	_uint m_iCurLevel = {};
public:
	static CLight_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,_uint iNumLevels);
	virtual void Free() override;
};


END

