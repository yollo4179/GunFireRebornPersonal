#include "Collider.h"
#include"GameInstance.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent{pDevice, pContext}
{
}

CCollider::CCollider(const CCollider& Prototype)
    :CComponent(Prototype)
    ,m_eType(Prototype.m_eType)
#ifdef _DEBUG
	,m_pBatch(Prototype.m_pBatch)// ㅅ에이더 파일과 배치는 공유한다
	,m_pEffect(Prototype.m_pEffect)
	,m_pInputLayout(Prototype.m_pInputLayout)
#endif 
{
	Safe_AddRef(m_pInputLayout);
}

HRESULT CCollider::Initialize_Prototype(COLLIDERTYPE eColliderType)
{
    m_eType = eColliderType;

#ifdef _DEBUG
	/* 박스, 구를 그려내기위한 객체들을 선언해놓는다. */
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext); //바운딩 박스 옵션 색깔,정점 인덱스 정보
	m_pEffect = new BasicEffect(m_pDevice); //제공되는 기본 셰이더 생성

	//레이아웃 정보 
	const void* pShaderByteCode = { nullptr };//
	size_t		iShaderByteCodeLength = { 0 };//정정 크기

	m_pEffect->SetVertexColorEnabled(true); 
	//이펙트는 컬러 값을줘도 기본으로 텍스처를 우선으로 하기때문에 색상옵션을 넣는다.  

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;
//셰이더 ia  layout 을 체크하기위한 정보 채운다 
#endif
	return S_OK;

}

HRESULT CCollider::Initialize(void* pArg)
{
	CBounding::BOUND_DESC* pDesc = static_cast<CBounding::BOUND_DESC*>(pArg);

	switch (m_eType)
	{
	case  TYPE_AABB:
		m_pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, pDesc);
		break;	
	case TYPE_OBB:
		m_pBounding = CBounding_OBB::Create(m_pDevice, m_pContext, pDesc);
		break;
	case TYPE_SPHERE:
		m_pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, pDesc);
		break;
	case TYPE_RAY:
		m_pBounding = CBounding_Ray::Create(m_pDevice , m_pContext);
	}
	
	return S_OK;
}

void CCollider::Update(_fmatrix WorldMatrix)
{

	if (nullptr == m_pBounding) {
		MSG_BOX("m_PbOUDING IS NULL : ccollider");
		return;
	}

	XMStoreFloat4x4(&m_matColliderTransform, WorldMatrix);
	m_pBounding->Update(WorldMatrix);
	
}

void CCollider::Update(pair<_float3, _float3> pairRay)
{
	m_pBounding->Update(pairRay);
}
#ifdef _DEBUG
HRESULT CCollider::Render()
{
	m_pContext->GSSetShader(nullptr, nullptr, 0);//기하셰이더 안쓰니까 무시하는 옵션 적용 안되게

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));
	
	//기본 셰이더 이펙트는 어차피 패스가 1개밖에 없다 걍 apply만 하면 됨 
	m_pEffect->Apply(m_pContext);					//1
	m_pContext->IASetInputLayout(m_pInputLayout);    //2 순서 지켜야됨
	
	m_pBatch->Begin();
	m_pBounding->Render(m_pBatch);
	m_pBatch->End();
	return S_OK;

}
#endif

_bool CCollider::Intersect(const CCollider* pTargetCollider)
{
	return m_pBounding->Intersect(pTargetCollider->m_eType, pTargetCollider->m_pBounding);
}

_vector CCollider::Compute_RayPos()
{
	return static_cast<CBounding_Ray*>(m_pBounding)->Compute_RayPos();	
}

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDERTYPE eColliderType)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eColliderType)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{

	__super::Free();
	Safe_Release(m_pBounding);

	if (false == m_isCloned)  ///클론된게 아니면 )원본이면) -> 프로토타입에 등록된것을 지울때 지워준다 ( 얕은 복사로 공유하는 문제 
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}

	Safe_Release(m_pInputLayout);

}
