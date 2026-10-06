#include "Cell.h"
#include"VIBuffer_Cell.h"
CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice{pDevice},
	m_pContext{pContext}
{
	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex,int iStage)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);
	m_iIndex = iIndex; //나의 인덱스
	m_iStage = iStage;

	Set_Normal();

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

	

	return S_OK;
}

_bool CCell::isIn(_fvector vLocalPos,_int& iNeghborIndex)
{

	for (size_t i = 0; i < LINE_END; ++i)
	{
		_vector vLine = XMLoadFloat3(&m_vPoints[(i + 1) % POINT_END]) - XMLoadFloat3(&m_vPoints[i]);
		//(법선 벡터)(x,0,z)-> (-z,0,x)(계산 대상)
		_vector vNormal = XMVectorSet(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0], 0.f);
		//(시계방향 선분의 노말 dot 내위치- 선분의 시작점)
		_vector vDir = vLocalPos - XMLoadFloat3(&m_vPoints[i]); //내위치에서 시작점을 뺀다
		//하나라도 양수면 안에 없는거다
		// (나간 방향의 선분쪽의 외부에 있어야 양수가 나옴) 
		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vNormal), XMVector3Normalize(vDir))))
		{
			
			/* 네비에서 밖으로 나갈수 있는지 판단하기 위해 라인에 등록된 셀의 인덱스도 함께 보내준다*/
			/* 지금 셀 밖으로 나갈수 있으면 iCurIndex를 갱신해야되기 때문에 ,-1 이라면 밖으로 나가면 안되니까 인자로 받아옴*/
			iNeghborIndex = m_iNeighborIndices[i];
			return false;
		}
	}
	/* 위 조건 다 통과하면 안에 있는거 */
	return true;
}
/*src 시작점이 겹치는지 -> 겹쳤다면desc 겹치는지*/
_bool CCell::Compare_Points(const _float3& SourPoint, const _float3& DestPoint)
{
	/*XMVectorEqual -> xyzw 성분 각각에 대해 따로따로 비교 -> 결과는 성분 당 true(1) false(0) 값을 담은 벡터*/
	/*XMVector3Equal()-> 같은 값을 가졌으면  true bool값 반환*/
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&SourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&DestPoint)))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&DestPoint)))
			return true;
	}
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&SourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&DestPoint)))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&DestPoint)))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&SourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&DestPoint)))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&DestPoint)))
			return true;
	}


	return false;
}

_float CCell::Compute_Height(_fvector vPosition)
{
	/* 시계방향순서로 정점을 널어주먄  위를 향하는 방향 벡터를 가진  평면 벡터를 반환 , 반시계로 넣으먄 아래 향하는 법선을 기반으로 평면을 구함*/
	_vector vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&m_vPoints[POINT_C]));
	//_vectre(a,b,c,d) -> ax +by+cz+d =0
	

	/* ax + by + cz + d = 0*/

	/* y = (-ax - cz - d) / b */

	return(-XMVectorGetX(vPlane) * XMVectorGetX(vPosition) - XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition) - XMVectorGetW(vPlane))/ XMVectorGetY(vPlane);


}
void CCell::AdjustPoints(TRIANGLE_VERTICES vTriangle)
{
	memcpy(m_vPoints, &vTriangle, sizeof(_float3) * POINT_END);

}
_vector CCell::Get_Normal()
{

	return XMLoadFloat3(&m_Normal);
	
}
void CCell::Set_Normal()
{
	_vector vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&m_vPoints[POINT_C]));
	m_Normal.x = XMVectorGetX(vPlane);
	m_Normal.y = XMVectorGetY(vPlane);
	m_Normal.z = XMVectorGetZ(vPlane);

}
/*objPos 는 로컬 pos*/
pair<_vector,_vector> CCell::Get_Elements_For_Sliding(_fvector vObjPos)
{
	_vector vCenter = ( XMLoadFloat3(&m_vPoints[POINT_A]) + XMLoadFloat3(&m_vPoints[POINT_B])+ XMLoadFloat3(&m_vPoints[POINT_C]) )/3.f;
	_float3 vCenterFloat3;
	XMStoreFloat3(&vCenterFloat3,vCenter);

	/*삼각형을 삼각형의 중심 한 정점으로 가지는 3개의 삼각형으로 나눈다. (영역을 통해서 어떤 선분에 슬라이딩 벡터를 투영할지 정한다. */
	_float3 vTriangles[3][3];
	vTriangles[0][0] = m_vPoints[POINT_A];
	vTriangles[0][1] = m_vPoints[POINT_B];
	vTriangles[0][2] = vCenterFloat3;

	vTriangles[1][0] = m_vPoints[POINT_B];
	vTriangles[1][1] = m_vPoints[POINT_C];
	vTriangles[1][2] = vCenterFloat3;

	vTriangles[2][0] = m_vPoints[POINT_C];
	vTriangles[2][1] = m_vPoints[POINT_A];
	vTriangles[2][2] = vCenterFloat3;

	int b = 0;
	_bool Is = isIn(vObjPos,b);

	for (int i = 0; i < 3; ++i)
	{

		/*if (-1 != m_iNeighborIndices[i])
			continue;*/

		_bool bIsIn = true;
		_vector vLine = {};

		for (int j = 0; j < 3; ++j) 
		{

			vLine = XMLoadFloat3(&vTriangles[i][(j + 1) % 3]) - XMLoadFloat3(&vTriangles[i][j]);
			//(법선 벡터)(x,0,z)-> (-z,0,x)(계산 대상)
			_vector vNormal = XMVectorSet(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0], 0.f);
			//(시계방향 선분의 노말 dot 내위치- 선분의 시작점)
			_vector vDir = vObjPos - XMLoadFloat3(&vTriangles[i][j]); //내위치에서 시작점을 뺀다
			//하나라도 양수면 안에 없는거다
			// (나간 방향의 선분쪽의 외부에 있어야 양수가 나옴) 
			if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vNormal), XMVector3Normalize(vDir))))
			{
				bIsIn = false;
			}

		}
		if(true ==bIsIn)
		{		//선분을 포함하는 삼각형을 반환
			switch (i) {
			case 0:
				m_vSrcOfSlidingVector = m_vPoints[POINT_A];
				break;
			case 1 :
				m_vSrcOfSlidingVector = m_vPoints[POINT_B];
				break;
			case 2:
				m_vSrcOfSlidingVector = m_vPoints[POINT_C];
				break;
			}
			
			vLine=XMVector3Normalize(XMLoadFloat3(&vTriangles[i][1]) - XMLoadFloat3(&vTriangles[i][0]));
			return { XMVectorSetW(XMLoadFloat3(&m_vSrcOfSlidingVector),1.f), vLine };
		}
	}
	  
	int a = 0;
	return { _vector(),_vector()};

}
_vector CCell::Get_Src_Of_SlidingVector()
{
	return XMVectorSetW(XMLoadFloat3(&m_vSrcOfSlidingVector),1.f);
}
_bool CCell::Check_Near_To_Points(_fvector vObjPos)
{
	_float fDist = 0.f;
	for (int i = 0; i < 3;++i)
	{
		_vector vDistVec =( XMVectorSetW(vObjPos,0.f) - XMLoadFloat3(&m_vPoints[i]));
		
		if (0.1f > (fDist =XMVectorGetX(XMVector3Length(vDistVec))) )
			return true;
	}

	return false;
}
#ifdef _DEBUG
HRESULT CCell::Render()
{

	m_pVIBuffer->Bind_BufferDesc();
	m_pVIBuffer->Render();

	return S_OK;
}
#endif
CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex, int iStage)
{
	CCell* pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex,iStage)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCell::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

}
