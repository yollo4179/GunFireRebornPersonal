#pragma once
#include"Bounding.h"
BEGIN(Engine)
class CBounding_AABB final :public CBounding
{

public:
	typedef struct :public CBounding::BOUND_DESC
	{
		/*AABB BoundingBox 는 extents를 우너소로 가짐*/
		_float3 vExtents;
	}BOUND_AABB_DESC;

private:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_AABB() =default;


public: 
	const BoundingBox* Get_Desc() const {
		return m_pBoundBox;
	}
	_vector Compute_RayPos();

	/* Extents 만 채우고 center는 자식에 넘겨주기 위해 구조체 상속을 통한 정보전달*/
	virtual HRESULT Initialize(const BOUND_DESC* pBoundDesc);
	virtual void Update(_fmatrix WorldMatrix)override;  //콜라이더 월드로 이동 
	virtual void Update(pair<_float3, _float3>pairRay)override;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch)override;//콜라이더 그리기 
	virtual _bool Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding)override;

	//tk 에서 그리는 함수 제공 effect 등 기본 셰이더 , 인풋 레이아웃 정보들 vertexPositionColor 에서 얻을 수 있다 (정점 정보 정점 크기 , element 개수와  버텍스 크기 ...)
private:
	BoundingBox* m_pOriginalBoundBox = {};
	BoundingBox* m_pBoundBox = {};


private:
	_bool Intersect_to_AABB(CBounding_AABB* pTarget);

public:
	static CBounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUND_DESC* pBoundDesc);
	virtual void Free() override;

};
END
