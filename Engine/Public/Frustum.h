#pragma once
#include "Base.h"
BEGIN(Engine)

class CFrustum final : public CBase
{
private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT Initialize();
	void Update();
	_bool isIn_InWorldSpace(_fvector vWorldPos, _float fRange = 0.f);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

	_float3								m_vPoints[8] = {};
	_float3								m_vPoints_InWorld[8] = {};

	_float4								m_vPlanes_InWorld[6] = {};

private:
	void Make_Planes(const _float3* pPoints, _Out_ _float4* pPlanes);

public:
	static CFrustum* Create();
	virtual void Free() override;
};

END