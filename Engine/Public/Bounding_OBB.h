#pragma once
#include"Bounding.h"
BEGIN(Engine)
class CBounding_OBB final :public CBounding
{

public:
	typedef struct :public  CBounding::BOUND_DESC
	{
		_float3 vExtents;
		_float3 vRotation;
	}BOUND_OBB_DESC;

	typedef struct
	{
		//obb 충돌을 위한 준비물 1.  -> 3개의 평면을 대표하는 3개의 길이가 1인 축벡터( 정사영을 내려서 비교할 대상)
		//-> 평면은 8개의 정점을 통해 구한 수직을 이루는 각 3개 축으로 대체한다. 
		//aabb 충돌을 위한 준비물 2. -> 중심으로부터 면에 수직인 벡터 (정사영을 내릴 대상) 
		//상대방의 2번 축들의 정사영값의 합과 
		// 과 내축들의 정사영값의 합을 더한 값이 
		//  2개 물체사이의 센터를 연결한 벡터의 정사영 한것보다 크면 충돌한것으로 한다 작으면 충돌 안한것 
		
		// 각 육변체의 평행한 3개의 축
		// ( 정사영을 할 벡터들(길이는 1로 맞춘다) 과
		//  정사영을 내릴 나의 벡터들 
		_float3 vCenter;
		_float3 vExtentDir[3];
		_float3 vAlignDir[3];
	}OBBDESC;

private:
	CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_OBB() = default;


public:
	const BoundingOrientedBox* Get_Desc() const {
		return m_pBoundBox;
	}
	_vector Compute_RayPos();
public:
	virtual HRESULT Initialize(const BOUND_DESC* pBoundDesc);
	virtual void Update(_fmatrix WorldMatrix) override;
	virtual void Update(pair<_float3, _float3>pairRay)override;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	virtual _bool Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding)override;


private:
	BoundingOrientedBox* m_pOriginalBoundBox = {};
	BoundingOrientedBox* m_pBoundBox = {};

private:
	_bool Intersect_To_OBB(CBounding_OBB* pTarget);
	OBBDESC Compute_OBB();

public:
	static CBounding_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUND_DESC* pBoundDesc);
	virtual void Free() override;
};
END