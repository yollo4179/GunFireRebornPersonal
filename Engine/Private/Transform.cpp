#include "..\Public\Transform.h"

#include "Shader.h"
#include"Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent { pDevice, pContext }
{
}

CTransform::CTransform(const CTransform & Prototype)
	: CComponent(Prototype )
{
}

_float3 CTransform::Compute_Scaled()
{
	return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))), 
		XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
		XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if (nullptr == pArg)
	{
		m_fSpeedPerSec = 1.f;
		m_fRotationPerSec = 1.f;
	}
	if (nullptr == pArg)
		return S_OK;

	TRANSFORM_DESC*		pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	m_fSpeedPerSec = pDesc->fSpeedPerSec;


	m_fRotationPerSec = pDesc->fRotationPerSec;

	
	

	return S_OK;
}


void CTransform::Scaling(_float fX, _float fY, _float fZ)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * fX); 
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * fY);
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * fZ);
}

_bool CTransform::Go_Straight(_float fTimeDelta, _bool bRemoveYElement, class CNavigation* pNavigation, _bool isJumping, _float CurHeight)
{
	_vector		vPosition =Get_State(STATE_POSITION); 
	_vector		vLook = Get_State(STATE_LOOK);
	

	_vector vPositionBySliding = {};
	_vector vPositionDefault = vPosition + XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;
	
	

	_bool b_isMoveResult = { true };
	if (pNavigation) {

		_vector		vPosition = pNavigation->SetUp_HeightOnNavigation(Get_State(CTransform::STATE_POSITION));
		/*점에 가깝다면 1칸 뒤로 +옆으로 */
		//if (m_iPrevIndex == pNavigation->Get_Index() && pNavigation->Check_Near_To_Points(vPosition))
		//{
		//	_vector vLine = XMLoadFloat3(&m_PrevLine);
		//	_vector vNormal = XMVectorSet(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0], 0.f);
		//	vLine = XMVector3Normalize(vLine);
		//	vNormal = XMVector3Normalize(vNormal);
		//	/*For Checking if Can Go*/
		//	if (m_iPrevSign < 0)
		//		vNormal *= -1;
		//	vPositionDefault = vPosition + (vLine - vNormal) * fTimeDelta * 30;
		//}


		

		
		/*슬라이딩 벡터 구하기*/
		/*C= Position Of Objects*/


		pNavigation->isMove(vPosition);
		pair<_vector , _vector> SlidingPairs = pNavigation->Get_Elements_For_Sliding_FromCurCell(vPosition);
		_vector vLineAB = SlidingPairs.second;														
		_vector vPointA= SlidingPairs.first;															
		_vector vCenter = (XMLoadFloat3(&(pNavigation->Get_A_Point_From_CurCell(CCell::POINT_A))) +
			XMLoadFloat3(&(pNavigation->Get_A_Point_From_CurCell(CCell::POINT_B))) +
			XMLoadFloat3(&(pNavigation->Get_A_Point_From_CurCell(CCell::POINT_C)))
			) / 3.f;
		vCenter = XMVectorSetW(vCenter, 1.f);

		_vector vLineAC = vCenter -vPointA;
		

		_vector vLineAD = XMVector3Normalize(vLineAB) * XMVector3Dot(XMVector3Normalize(vLineAB),vLineAC);//(XMVector3Dot(vLineAB,vLineAC) / XMVector3Dot(vLineAB, vLineAB));
		_vector vPointD = vPointA + vLineAD;

		_vector vNormalForSliding = XMVector3Normalize(vCenter - vPointD);
		_vector vSlidingVector = {};

		_vector vRes = XMVector3Dot(vNormalForSliding, vLineAB);
		

		if (!XMVectorEqual(vLineAB, _vector()).m128_f32[0]) {
			vSlidingVector = vLook + vNormalForSliding * (XMVector3Dot(vNormalForSliding, -vLook));
			vSlidingVector = XMVector3Normalize(vLineAB) * XMVector3Dot(XMVector3Normalize(vLineAB), vSlidingVector);
		}
		
		vPositionBySliding =
			vPosition + XMVector3Normalize(vSlidingVector) * fTimeDelta * 10;
			
			/*vPosition + XMVector3Dot(pNavigation->Get_Elements_For_Sliding_FromCurCell(vPosition).second, Get_SlidingFromLookVector(vLook, pNavigation))
			* pNavigation->Get_Elements_For_Sliding_FromCurCell(vPosition).second * m_fSpeedPerSec / 1.5f * fTimeDelta;*/
		/*슬라이딩 벡터 유효한지 확인*/
		_bool isSlidingOK = pNavigation->isMove(vPositionBySliding);

		

		/* 다음 프레임을 위한 준비 (이전 프레임 정보를 넘겨주는 구조)*/
		_float fSignDecision = { XMVectorGetX(XMVector3Dot(vLineAB, vLook)) };
		if (0 < fSignDecision)
		{
			m_iPrevSign = 1;
		}
		else if (0 == fSignDecision)
		{
		}
		else
		{
			m_iPrevSign = -1; 
		}

		/* 이전 라인 저장*/
		m_iPrevIndex = pNavigation->Get_Index();
	
	/*네비게이션을 안쓰거나 삼각형 안에 있을때, 삼각형 밖에 있지만 다음 인덱스셀이 이어져있을때 */
		

		/*길이 가로막혔다면 슬라이딩 벡터를 타고 가자*/
		if ( false == (b_isMoveResult = pNavigation->isMove(vPositionDefault)))
		{
			if (!isSlidingOK) {
				//슬라이딩 벡터도 막혔다 ( 정점에 도달)
				Go_Backward(fTimeDelta,pNavigation);

				if (fSignDecision < 0)
					Go_Right(fTimeDelta * 3.f, pNavigation);
				else
					Go_Left(fTimeDelta * 3.f, pNavigation);

				return b_isMoveResult;
			}
			if(isJumping)
				vPositionBySliding = XMVectorSetY(vPositionBySliding,CurHeight);
			 
			Set_State(STATE_POSITION, vPositionBySliding);
		}
		

		else if (true == b_isMoveResult)
		{
			if (isJumping)
				vPositionDefault = XMVectorSetY(vPositionDefault, CurHeight);
			Set_State(STATE_POSITION, vPositionDefault);
		}
	}
	
	/*네비게이션을 안쓰는 경우*/
	if(!pNavigation)
		Set_State(STATE_POSITION, vPositionDefault);
	
	

	return b_isMoveResult;
}

void CTransform::Go_Backward(_float fTimeDelta, class CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition))
	Set_State(STATE_POSITION, vPosition);
}

_bool CTransform::Go_Left(_float fTimeDelta, class CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	_bool b_isMoveResult = { true };
	if (nullptr == pNavigation || true == (b_isMoveResult = pNavigation->isMove(vPosition)))
	Set_State(STATE_POSITION, vPosition);
	return b_isMoveResult;
}

_bool CTransform::Go_Right(_float fTimeDelta, class CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;


	_bool b_isMoveResult = { true };
	if (nullptr == pNavigation || true == (b_isMoveResult=pNavigation->isMove(vPosition)))
		Set_State(STATE_POSITION, vPosition);
	return b_isMoveResult;
}

_bool CTransform::Go_Pushed(_float fTimeDelta, _fvector vPushedDir, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vPushedDir) * m_fSpeedPerSec * fTimeDelta;

	_bool b_isMoveResult = { true };
	if (nullptr == pNavigation || true == (b_isMoveResult = pNavigation->isMove(vPosition)))
		Set_State(STATE_POSITION, vPosition);
	return b_isMoveResult;
}



void CTransform::LookAt(_fvector vAt)
{
	_float3		vScale = Compute_Scaled();

	_vector		vPosition = Get_State(STATE_POSITION);	
	_vector		vLook = vAt - vPosition;
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_float3		vScale = Compute_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

_vector CTransform::Get_SlidingFromLookVector(_fvector vLook, CNavigation* pNavigation)
{
	
	/*Normal*/
	_vector vCellNormal = pNavigation->Get_Normal_From_CurCell();
	vCellNormal = XMVector3Normalize(vCellNormal);
	/*LookVector*/
	_vector vObjLook = XMVector3Normalize(vLook);

	
	

	_vector vSlidingVector= vObjLook - XMVector3Dot(vCellNormal, vObjLook) * vCellNormal;

	vSlidingVector = XMVector3Normalize(vSlidingVector);

	return vSlidingVector;
}

HRESULT CTransform::Bind_ShaderResource(CShader * pShader, const _char * pConstantName)

 
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}

