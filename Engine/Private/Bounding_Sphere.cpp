#include "Bounding_Sphere.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include"Bounding_Ray.h"
CBounding_Sphere::CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBounding{ pDevice, pContext }
{
}

_vector CBounding_Sphere::Compute_RayPos()
{
	return _vector();
}

HRESULT CBounding_Sphere::Initialize(const BOUND_DESC* pBoundDesc)
{
	const BOUND_SPHERE_DESC* pDesc = static_cast<const BOUND_SPHERE_DESC*>(pBoundDesc);

	m_pOriginalBoundSphere = new BoundingSphere(pDesc->vCenter, pDesc->fRadius);
	m_pBoundSphere = new BoundingSphere(*m_pOriginalBoundSphere);

	return S_OK;
}

void CBounding_Sphere::Update(_fmatrix WorldMatrix)
{

	m_pOriginalBoundSphere->Transform(*m_pBoundSphere, WorldMatrix);
}

void CBounding_Sphere::Update(pair<_float3, _float3> pairRay)
{
}


void CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundSphere, m_isColl ==true ? XMVectorSet(1.f, 0.f,0.f,1.f): XMVectorSet(0.f,1.f,0.f,1.f));

}

_bool CBounding_Sphere::Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding)
{
	m_isColl = false;

	switch (eType)
	{
	case CCollider::TYPE_AABB:
		m_isColl = m_pBoundSphere->Intersects(*static_cast<CBounding_AABB*>(pTargetBounding)->Get_Desc());
		break;

	case CCollider::TYPE_OBB:
		m_isColl = m_pBoundSphere->Intersects(*static_cast<CBounding_OBB*>(pTargetBounding)->Get_Desc());
		break;

	case CCollider::TYPE_SPHERE:
		m_isColl = m_pBoundSphere->Intersects(*static_cast<CBounding_Sphere*>(pTargetBounding)->Get_Desc());
		break;

	case CCollider::TYPE_RAY:
		CBounding_Ray::BOUND_RAY_DESC RayDesc = {};
		RayDesc = static_cast<CBounding_Ray*>(pTargetBounding)->Get_Desc();
		m_isColl = m_pBoundSphere->Intersects(
			XMVectorSetW(XMLoadFloat3(&RayDesc.vRaySrc), 0.f),
			XMVectorSetW(XMLoadFloat3(&RayDesc.vRayDir), 0.f),
			RayDesc.fDist
		);
		break;
	}

	return m_isColl;
}

CBounding_Sphere* CBounding_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUND_DESC* pBoundDesc)
{
	CBounding_Sphere* pInstance = new CBounding_Sphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pBoundDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_Sphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBounding_Sphere::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalBoundSphere);
	Safe_Delete(m_pBoundSphere);
}
