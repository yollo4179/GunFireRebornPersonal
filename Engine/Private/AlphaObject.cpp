#include "..\Public\AlphaObject.h"
#include "Transform.h"
#include "GameInstance.h"

CAlphaObject::CAlphaObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CAlphaObject::CAlphaObject(const CAlphaObject& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CAlphaObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAlphaObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CAlphaObject::Priority_Update(_float fTimeDelta)
{
}

void CAlphaObject::Update(_float fTimeDelta)
{
}

void CAlphaObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CAlphaObject::Render()
{
	return S_OK;
}

void CAlphaObject::Compute_ViewZ()
{
	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//내 월드에다가 카메라의 역행렬을 곱한값의 z (뷰z 계산한다) -> alpha그룹이므로  소팅을 위한 COMPUTE VIEWZ 값이 필요하다.
	vPosition = XMVector3TransformCoord(vPosition, m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));

	m_fViewZ = XMVectorGetZ(vPosition);
}


void CAlphaObject::Free()
{
	__super::Free();


}
