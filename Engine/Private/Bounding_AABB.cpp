#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"
#include"Bounding_Ray.h"

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding{pDevice, pContext}
{
}

_vector CBounding_AABB::Compute_RayPos()
{
	return _vector();
}

HRESULT CBounding_AABB::Initialize(const BOUND_DESC* pBoundDesc)
{
	const BOUND_AABB_DESC* pDesc = static_cast< const BOUND_AABB_DESC*> (pBoundDesc);
	/* 툴킷에서 제공하는 aabb바운딩박스 구조체  center와 extents를 채워준다*/
	m_pOriginalBoundBox = new BoundingBox( pDesc->vCenter, pDesc->vExtents);
	m_pBoundBox = new BoundingBox(*m_pOriginalBoundBox);
	
//Bouding box의 트랜스폼함수는 나의 로컬 행렬에 월드 행렬을 곱한값을 
//인자에 넣은 다른 bounding box 에 넣는다. ->트랜스폼에 자기 자신을 넣으먄 
//월드를 곱한값이 다시 로컬행렬로 취급해서 월드를 중복해서 곱하므로 
//나의 로컬은 original에 저장-> 월드를 곱한값을 받아오는 콜라이더 구조체 -> m_pBoundBox 를만든다


	return S_OK;
}

void CBounding_AABB::Update(_fmatrix WorldMatrix)
{
	//트랜스폼 회전 들어가면 스케일이 같이 커졌다가 작아지는 현상이 생긴다 -> 항등으로 맞워서  회전을 막자.

	//처음에 세팅한 스케일도 그대로 저장해야 하므로 저장전에 미리 스케일을 곱한 후에 원래 스케일을 그대로
	// 그대로 적용한 항등행렬로 맞춘 콜라이더 월드를 갱신한다.
	// (위치만 바뀐다는 ... 
	_matrix		TransformMatrix = WorldMatrix;
	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[0]);
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[1]);
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(TransformMatrix.r[2]);


	m_pOriginalBoundBox->Transform(*m_pBoundBox, WorldMatrix);
	//BoundBox에 실제 월드에 배치할 행렬을 original의 로컬에 곱하여  boundBox를 매번 갱신한다 
}

void CBounding_AABB::Update(pair<_float3, _float3> pairRay)
{
}

void CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	//갱신된 월드로 채운 바인딩박스를 그린다 (Collider 컴포넌트가 이 aabb 구조체를 가지고 있으니까 ) -> 거기서 ia 병합 다하고 
	//이걸로 그림
	DX::Draw(pBatch, *m_pBoundBox ,m_isColl ==true ? XMVectorSet(1.f,0.f,0.f,1.f):XMVectorSet(0.f,1.f,0.f,1.f) );

}

_bool CBounding_AABB::Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding)
{
	m_isColl = false;

	switch (eType)
	{
	case CCollider::TYPE_AABB:
		m_isColl = m_pBoundBox->Intersects(*static_cast<CBounding_AABB*>(pTargetBounding)->Get_Desc());
		break;

	case CCollider::TYPE_OBB:
		m_isColl = m_pBoundBox->Intersects(*static_cast<CBounding_OBB*>(pTargetBounding)->Get_Desc());
		break;

	case CCollider::TYPE_SPHERE:
		m_isColl = m_pBoundBox->Intersects(*static_cast<CBounding_Sphere*>(pTargetBounding)->Get_Desc());
		break;
	case CCollider::TYPE_RAY:
		CBounding_Ray::BOUND_RAY_DESC RayDesc = {};
		RayDesc = static_cast<CBounding_Ray*>(pTargetBounding)->Get_Desc();
		m_isColl = m_pBoundBox ->Intersects(
			XMVectorSetW(XMLoadFloat3(&RayDesc.vRaySrc), 0.f),
			XMVectorSetW(XMLoadFloat3(&RayDesc.vRayDir), 0.f),
			RayDesc.fDist
		);
		break;
	}

	return m_isColl;
}

_bool CBounding_AABB::Intersect_to_AABB(CBounding_AABB* pTarget)
{
	_float3 vSourMin, vSourMax;
	_float3 vDestMin, vDestMax;


	vSourMin = _float3(
		m_pBoundBox->Center.x - m_pBoundBox->Extents.x,
		m_pBoundBox->Center.y - m_pBoundBox->Extents.y,
		m_pBoundBox->Center.z - m_pBoundBox->Extents.z
	);
	vSourMax = _float3(
		m_pBoundBox->Center.x + m_pBoundBox->Extents.x,
		m_pBoundBox->Center.y + m_pBoundBox->Extents.y,
		m_pBoundBox->Center.z + m_pBoundBox->Extents.z
	);
	vDestMin = _float3(
		pTarget->m_pBoundBox->Center.x - pTarget->m_pBoundBox->Extents.x,
		pTarget->m_pBoundBox->Center.y - pTarget->m_pBoundBox->Extents.y,
		pTarget->m_pBoundBox->Center.z - pTarget->m_pBoundBox->Extents.z
	);
	vDestMax = _float3(
		pTarget->m_pBoundBox->Center.x + pTarget->m_pBoundBox->Extents.x,
		pTarget->m_pBoundBox->Center.y + pTarget->m_pBoundBox->Extents.y,
		pTarget->m_pBoundBox->Center.z + pTarget->m_pBoundBox->Extents.z
	);

	//x 축에 대한 비교 
	// 겹치지 않을 조건들을 모두 통과하면 겹치지 않는것이다 .
	// 
	
	// 두 네모가 yz 평면상에서 떨어져 있다 충돌 아니다 -> false
	if (max(vSourMin.x, vDestMin.x) > min(vDestMax.x, vDestMax.x))
		return false;
	// 두 네모가 xz 평면상에서 떨어져 있다 충돌 아니다 -> false
	if (max(vSourMin.y, vDestMin.y) > min(vDestMax.y, vDestMax.y))
		return false;
	// 두 네모가 xy 평면상에서 떨어져 있다 충돌 아니다 -> false
	if (max(vSourMin.z, vDestMin.z) > min(vDestMax.z, vDestMax.z))
		return false;


	//겹치지 않는 3경우만 모두 통과 하면 충돌 판정 
	return true;
}

CBounding_AABB* CBounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUND_DESC* pBoundDesc)
{
	CBounding_AABB* pInstance = new CBounding_AABB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pBoundDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_AABB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBounding_AABB::Free()
{

	__super::Free();
	Safe_Delete(m_pOriginalBoundBox);
	Safe_Delete(m_pBoundBox);

}
