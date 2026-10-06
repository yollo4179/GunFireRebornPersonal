#include"Bounding_OBB.h"
#include "Bounding_AABB.h"
#include "Bounding_Sphere.h"
#include"Bounding_Ray.h"
CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBounding{ pDevice, pContext }
{
}



_vector CBounding_OBB::Compute_RayPos()
{
	return _vector();
}

HRESULT CBounding_OBB::Initialize(const BOUND_DESC* pBoundDesc)
{
	const BOUND_OBB_DESC* pDesc = static_cast<const BOUND_OBB_DESC*>(pBoundDesc);

	//쿼터니언 정보로 바꿔주는 함수가 존재한다.
	_float4		vRotation;
	XMStoreFloat4(&vRotation, XMQuaternionRotationRollPitchYaw(pDesc->vRotation.x, pDesc->vRotation.y, pDesc->vRotation.z));

	//obb박스의 rotation정보는 쿼터니언 기반이므로 _float4 정보로 바꿔서 전달한다.(처음 회전 각도 )
	m_pOriginalBoundBox = new BoundingOrientedBox(pDesc->vCenter, pDesc->vExtents, vRotation);
	m_pBoundBox = new BoundingOrientedBox(*m_pOriginalBoundBox);

	return S_OK;
}

void CBounding_OBB::Update(_fmatrix WorldMatrix)
{
	m_pOriginalBoundBox->Transform(*m_pBoundBox, WorldMatrix);
}

void CBounding_OBB::Update(pair<_float3, _float3> pairRay)
{
}

void CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundBox, m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));
}

_bool CBounding_OBB::Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding)
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
		m_isColl = m_pBoundBox->Intersects(
			XMVectorSetW(XMLoadFloat3(&RayDesc.vRaySrc), 0.f),
			XMVectorSetW(XMLoadFloat3(&RayDesc.vRayDir), 0.f),
			RayDesc.fDist
		);
		break;
	}

	return m_isColl;
}

_bool CBounding_OBB::Intersect_To_OBB(CBounding_OBB* pTarget)
{
	OBBDESC		OBBDesc[2];

	OBBDesc[0] = Compute_OBB();
	OBBDesc[1] = pTarget->Compute_OBB();



	return _bool();

}

CBounding_OBB::OBBDESC CBounding_OBB::Compute_OBB()
{
	OBBDESC OBBDesc{};
	_float3 vPoints[8];

	m_pBoundBox->GetCorners(vPoints);
	OBBDesc.vCenter = m_pBoundBox->Center;
	_float3 vAxis[3] = {};
	/* 사실상 평면을  직선 취급*/
	XMStoreFloat3(&vAxis[0], XMLoadFloat3(&vPoints[5]) - XMLoadFloat3(&vPoints[4]));
	XMStoreFloat3(&vAxis[1], XMLoadFloat3(&vPoints[7]) - XMLoadFloat3(&vPoints[4]));
	XMStoreFloat3(&vAxis[2], XMLoadFloat3(&vPoints[0]) - XMLoadFloat3(&vPoints[4]));

	for (size_t i = 0; i < 3; i++)
		XMStoreFloat3(&OBBDesc.vExtentDir[i], XMLoadFloat3(&vAxis[i]) * 0.5f); //법선 벡터(정사영내릴것들)

	for (size_t i = 0; i < 3; i++)
		XMStoreFloat3(&OBBDesc.vAlignDir[i], XMVector3Normalize(XMLoadFloat3(&vAxis[i])));//평면 벡터(정사영 대상)

	return OBBDesc;


}

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUND_DESC* pBoundDesc)
{
	CBounding_OBB* pInstance = new CBounding_OBB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pBoundDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_OBB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBounding_OBB::Free()
{
	__super::Free();
	Safe_Delete(m_pOriginalBoundBox);
	Safe_Delete(m_pBoundBox);
}
