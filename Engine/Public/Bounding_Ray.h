#pragma once
#include"Bounding.h"
BEGIN(Engine)
class CBounding_Ray final :public CBounding
{
public:
	typedef struct
	{
		_float3 vRayDir;
		_float3 vRaySrc;
		_float fDist;

	}BOUND_RAY_DESC;


private:
	CBounding_Ray(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_Ray()=default;


public:
	const BOUND_RAY_DESC Get_Desc()const {
		return m_BoundRay;
	}
	void SetUp_RayInfo(BOUND_RAY_DESC Desc) { m_BoundRay = Desc; };
	_vector Compute_RayPos();
public:
	virtual HRESULT Initialize();
	virtual void Update(_fmatrix WorldMatrix) override;
	virtual void Update(pair<_float3, _float3>pairRay)override;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	virtual _bool Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding)override;


	static CBounding_Ray* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

	BOUND_RAY_DESC m_BoundRay = {};

};
END
