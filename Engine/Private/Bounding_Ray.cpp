#include "Bounding_Ray.h"
#include "Bounding_Sphere.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
CBounding_Ray::CBounding_Ray(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CBounding{pDevice, pContext}
{
}

_vector CBounding_Ray::Compute_RayPos()
{
	m_BoundRay.vRaySrc;
	m_BoundRay.vRayDir;
	m_BoundRay.fDist;
	_vector vSrc =XMVectorSetW(XMLoadFloat3(&m_BoundRay.vRaySrc),1.f);
	_vector vRayDir = XMLoadFloat3(&m_BoundRay.vRayDir);
	return vSrc + vRayDir * m_BoundRay.fDist;
}

HRESULT CBounding_Ray::Initialize()
{
    return S_OK;
}

void CBounding_Ray::Update(_fmatrix WorldMatrix)
{
}

void CBounding_Ray::Update(pair<_float3, _float3> pairRay)
{
	m_BoundRay.vRaySrc = pairRay.first;
	m_BoundRay.vRayDir = pairRay.second;
}

void CBounding_Ray::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
    DX::DrawRay(pBatch,
        XMVectorSetW(XMLoadFloat3(&m_BoundRay.vRaySrc), 0.f),
        XMVectorSetW(XMLoadFloat3(&m_BoundRay.vRayDir), 0.f)*m_BoundRay.fDist,
        false,
        m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f)
    );
}

_bool CBounding_Ray::Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding)
{
	m_isColl = false;

	switch (eType)
	{
	case CCollider::TYPE_AABB:
		m_isColl = static_cast<CBounding_AABB*>(pTargetBounding)->Get_Desc()
			->Intersects(
				XMVectorSetW(XMLoadFloat3(&m_BoundRay.vRaySrc), 0.f),
				XMVectorSetW(XMLoadFloat3(&m_BoundRay.vRayDir), 0.f),
				m_BoundRay.fDist
			);
		break;

	case CCollider::TYPE_OBB:
		m_isColl = static_cast<CBounding_OBB*>(pTargetBounding)->Get_Desc()
			->Intersects(
				XMVectorSetW(XMLoadFloat3(&m_BoundRay.vRaySrc), 0.f),
				XMVectorSetW(XMLoadFloat3(&m_BoundRay.vRayDir), 0.f),
				m_BoundRay.fDist
			);
		break;

	case CCollider::TYPE_SPHERE:
		m_isColl = static_cast<CBounding_Sphere*>(pTargetBounding)->Get_Desc()
		->Intersects(
			XMVectorSetW(XMLoadFloat3(&m_BoundRay.vRaySrc), 0.f),
			XMVectorSetW(XMLoadFloat3(&m_BoundRay.vRayDir), 0.f),
			m_BoundRay.fDist
		);
		break;
	}

	return m_isColl;
}

CBounding_Ray* CBounding_Ray::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBounding_Ray* pInstance = new CBounding_Ray(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created: CBounding_Ray");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBounding_Ray::Free()
{
	__super::Free();
}
