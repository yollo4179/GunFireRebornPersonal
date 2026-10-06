#include"VIBuffer_Trail.h"

CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer{pDevice, pContext}
{
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& Prototype)
	:CVIBuffer(Prototype)
	, m_iNumMaxVertices{Prototype.m_iNumMaxVertices }
{
}

void CVIBuffer_Trail::Add_Points(_fmatrix vtxTrail_High_Matrix, _fmatrix vtxTrail_Low_Matrix)
{
	if (m_iNumMaxVertices < m_TrailVertices.size() + 2)
	{
		/*_uint RemoveCnt = m_TrailVertices.size() / 4;
		
		for (int i = 0; i < RemoveCnt; ++i) {
			m_TrailVertices.pop_back();
			m_TrailVertices.pop_back();
		}*/
		m_TrailVertices.pop_back();
		m_TrailVertices.pop_back();

	}

	

	_vector vLocalTrailLow = XMLoadFloat3(&m_vLocalTrailLow);
	_vector vLocalTrailHigh = XMLoadFloat3(&m_vLocalTrailHigh);
	VTXPOSTEX vtxTrail_High= {};
	VTXPOSTEX vtxTrail_Low = {};
	_vector vVtxTrailLowPos = {};
	_vector vVtxTrailHighPos = {};
	vVtxTrailHighPos = XMVector3TransformCoord(vLocalTrailHigh, vtxTrail_High_Matrix);
	vVtxTrailLowPos = XMVector3TransformCoord(vLocalTrailLow, vtxTrail_Low_Matrix);
	
	XMStoreFloat3(&vtxTrail_High.vPosition,vVtxTrailHighPos);
	XMStoreFloat3(&vtxTrail_Low.vPosition, vVtxTrailLowPos);
	m_TrailVertices.push_front(vtxTrail_High);
	m_TrailVertices.push_front(vtxTrail_Low);

	D3D11_MAPPED_SUBRESOURCE subResource;
	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	VTXPOSTEX* pVertices = (VTXPOSTEX*)subResource.pData;
	for (_uint i = 0;i < m_TrailVertices.size(); i+=2)
	{
		pVertices[i].vPosition = m_TrailVertices[i].vPosition;
		pVertices[i].vTexcoord = { (_float)i / (_float)m_TrailVertices.size()/2 * m_fTrailLength,0.f };
		
		pVertices[i+1].vPosition = m_TrailVertices[i+1].vPosition;
		pVertices[i + 1].vTexcoord = { (_float)i / (_float)m_TrailVertices.size()/2 * m_fTrailLength,1.f };
	}
	m_pContext->Unmap(m_pVB, 0);

	m_iNumIndices = 3 * 2 * (m_TrailVertices.size() / 2 - 1);

}

void CVIBuffer_Trail::Sub_Points()
{
	if (m_TrailVertices.empty())
		return ;
	m_TrailVertices.pop_back();
	m_TrailVertices.pop_back();

	D3D11_MAPPED_SUBRESOURCE subResource;
	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	VTXPOSTEX* pVertices = (VTXPOSTEX*)subResource.pData;
	for (_uint i = 0; i < m_TrailVertices.size(); i += 2)
	{
		pVertices[i].vPosition = m_TrailVertices[i].vPosition;
		pVertices[i].vTexcoord = { (_float)i / (_float)m_TrailVertices.size() *m_fTrailLength,0.f };

		pVertices[i + 1].vPosition = m_TrailVertices[i + 1].vPosition;
		pVertices[i + 1].vTexcoord = { (_float)i / (_float)m_TrailVertices.size() * m_fTrailLength,1.f };
	}
	m_pContext->Unmap(m_pVB, 0);
	m_iNumIndices = 3 * 2 * (m_TrailVertices.size() / 2 - 1);

}

void CVIBuffer_Trail::Add_Points_CatMullRom(_fmatrix vtxTrail_High_Matrix, _fmatrix vtxTrail_Low_Matrix)
{
	


	m_iEndIndex = m_iCatmullRomCnt * 2 + m_iVerticesCnt;
	

	m_iCatmullRomIndex[2] = m_iEndIndex - 2;
	m_iCatmullRomIndex[3] = m_iEndIndex;


	if (m_iVerticesCnt <= 6)
	{
		_vector vLocalTrailLow = XMLoadFloat3(&m_vLocalTrailLow);
		_vector vLocalTrailHigh = XMLoadFloat3(&m_vLocalTrailHigh);

		  
		XMStoreFloat3(&m_TrailVertices[m_iVerticesCnt].vPosition, XMVector3TransformCoord(vLocalTrailLow, vtxTrail_Low_Matrix));
		XMStoreFloat3(&m_TrailVertices[m_iVerticesCnt + 1].vPosition, XMVector3TransformCoord(vLocalTrailHigh, vtxTrail_High_Matrix));
		m_iVerticesCnt += 2;
	}



	else {
		
		
		
		
		if (true ==m_bStartSpline)
		{
			m_iCatmullRomIndex[0] = m_iVerticesCnt - 6;
			m_iCatmullRomIndex[1] = m_iVerticesCnt -4;
			m_bStartSpline = false;
		}

		if (m_iEndIndex + 2 >= m_iNumMaxVertices)
		{
			_uint iRemoveCnt = m_iVerticesCnt / 4;
			if (0 != iRemoveCnt % 2)iRemoveCnt -= 1;
			m_iVerticesCnt -= iRemoveCnt;
			for (_uint i = 0; i < m_iVerticesCnt; i += 2)
			{
				m_TrailVertices[i].vPosition = m_TrailVertices[iRemoveCnt + i].vPosition;
				m_TrailVertices[i + 1].vPosition = m_TrailVertices[iRemoveCnt + i + 1].vPosition;
			}
			m_bStartSpline = true;
			return;

		}

		m_TrailVertices[m_iEndIndex - 2].vPosition = m_TrailVertices[m_iVerticesCnt - 2].vPosition;
		m_TrailVertices[m_iEndIndex - 1].vPosition = m_TrailVertices[m_iVerticesCnt - 1].vPosition;

		_vector vLocalTrailLow = XMLoadFloat3(&m_vLocalTrailLow);
		_vector vLocalTrailHigh = XMLoadFloat3(&m_vLocalTrailHigh);


		XMStoreFloat3(&m_TrailVertices[m_iEndIndex].vPosition, XMVector3TransformCoord(vLocalTrailLow, vtxTrail_Low_Matrix));
		XMStoreFloat3(&m_TrailVertices[m_iEndIndex + 1].vPosition, XMVector3TransformCoord(vLocalTrailHigh, vtxTrail_High_Matrix));

		

		for (_uint i = 0; i < m_iCatmullRomCnt; ++i)
		{
			_uint index = i * 2 +m_iVerticesCnt- 2;
			_float fWeight = (_float)(i + 1) / (m_iCatmullRomCnt + 1);
			_vector vPosHigh = XMVectorCatmullRom
			(
				XMVectorSetW(XMLoadFloat3(&m_TrailVertices[m_iCatmullRomIndex[0]].vPosition), 1.f),
				XMVectorSetW(XMLoadFloat3(&m_TrailVertices[m_iCatmullRomIndex[1]].vPosition), 1.f),
				XMVectorSetW(XMLoadFloat3(&m_TrailVertices[m_iCatmullRomIndex[2]].vPosition), 1.f),
				XMVectorSetW(XMLoadFloat3(&m_TrailVertices[m_iCatmullRomIndex[3]].vPosition), 1.f),
				fWeight
			);
			XMStoreFloat3(&m_TrailVertices[index].vPosition, vPosHigh);

			_vector vPosLow = XMVectorCatmullRom
			(
				XMVectorSetW(XMLoadFloat3(&m_TrailVertices[m_iCatmullRomIndex[0] + 1].vPosition), 1.f),
				XMVectorSetW(XMLoadFloat3(&m_TrailVertices[m_iCatmullRomIndex[1] + 1].vPosition), 1.f),
				XMVectorSetW(XMLoadFloat3(&m_TrailVertices[m_iCatmullRomIndex[2] + 1].vPosition), 1.f),
				XMVectorSetW(XMLoadFloat3(&m_TrailVertices[m_iCatmullRomIndex[3] + 1].vPosition), 1.f),
				fWeight
			);
			XMStoreFloat3(&m_TrailVertices[index + 1].vPosition, vPosLow);
		}

		m_iVerticesCnt = m_iEndIndex + 2;
	}

	
		m_iCatmullRomIndex[0] = m_iCatmullRomIndex[1];
		m_iCatmullRomIndex[1] = m_iCatmullRomIndex[2];
	

	
	for (_uint i = 0; i < m_iVerticesCnt; i += 2)
	{
		m_TrailVertices[i].vTexcoord = { (_float)i / (_float)(m_iVerticesCnt - 2),1.f }; //low
		m_TrailVertices[i+1].vTexcoord = { (_float)i / (_float)(m_iVerticesCnt - 2),0.f };//high
	}
	for (_uint i = m_iVerticesCnt; i < m_iNumMaxVertices-2; i += 2)
	{
		m_TrailVertices[i].vTexcoord = { };
		m_TrailVertices[i + 1].vTexcoord = {};
	}


	D3D11_MAPPED_SUBRESOURCE subResource;
	m_pContext->Map(this->m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	VTXPOSTEX* pVertices = (VTXPOSTEX*)subResource.pData;
	for (_uint i = 0; i < m_iVerticesCnt; ++i)
	{
		pVertices[i].vPosition = m_TrailVertices[i].vPosition;
		pVertices[i].vTexcoord = m_TrailVertices[i].vTexcoord;
	}

	m_pContext->Unmap(this->m_pVB, 0);

	if(m_iVerticesCnt>=6)
		m_iNumIndices = 3 * 2 * (m_iVerticesCnt / 2 - 1);

}

void CVIBuffer_Trail::Clear_Points()
{
	m_TrailVertices.clear();
}

void CVIBuffer_Trail::Resize_Vertices()
{
	
		m_TrailVertices.resize(m_iNumMaxVertices);
		
}

void CVIBuffer_Trail::Reset_VerticesCnt()
{
	/*m_iVerticesCnt = 0;
	m_iNumIndices = 0;
	D3D11_MAPPED_SUBRESOURCE subResource;
	m_pContext->Map(this->m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	VTXPOSTEX* pVertices = (VTXPOSTEX*)subResource.pData;
 	memset(subResource.pData, 0, sizeof(VTXPOSTEX) * m_iNumMaxVertices);
	m_pContext->Unmap(this->m_pVB, 0);
	m_iCatmullRomIndex[0] = 0;
	m_iCatmullRomIndex[1] = 0;
	m_iCatmullRomIndex[2] = 0;
	m_iCatmullRomIndex[3] = 0;
	m_iEndIndex = 0;
	m_TrailVertices.clear();
	m_TrailVertices.resize(m_iNumMaxVertices);
	m_bStartSpline = true;*/
	m_iVerticesCnt = 0;
	m_bStartSpline = true;
	m_iCatmullRomIndex[0] = 0;
	m_iCatmullRomIndex[1] = 0;
	m_iCatmullRomIndex[2] = 0;
	m_iCatmullRomIndex[3] = 0;
	m_iEndIndex = 0;
	m_iNumIndices = 0;

}

HRESULT CVIBuffer_Trail::Render()
{
	if (0 < m_iNumIndices)
		m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer_Trail::Bind_BufferDesc()
{
	__super::m_pVB;

	ID3D11Buffer* pVertexBuffers[] = {
		this->m_pVB,
	};

	_uint					iVertexStrides[] = {
		m_iVertexStride,
	};

	_uint					iOffsets[] = {
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(this->m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}


HRESULT CVIBuffer_Trail::Initialize_Prototype()
{
	return S_OK;
}
/* 모델에서 버퍼 생성할때 _Trail_Wave 넣어주자 */
HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{


	TRAIL_DESC* pDesc = static_cast<TRAIL_DESC*>(pArg);
	m_fTrailLength = pDesc->fTrailLength;

	m_iNumVertexBuffers = 1;

	m_iVertexStride = sizeof(VTXPOSTEX);
	
	m_iNumVertices = m_iNumMaxVertices = pDesc->iNumMaxVertices;
	m_iIndexStride = sizeof(_ushort);
	m_iNumIndices = 3 * 2 * (m_iNumVertices / 2 - 1);
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER	
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&(this->m_pVB))))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	_uint iNowIndex = 0;
	_uint iNum_TriCnt = m_iNumIndices/3;
	for (_uint i = 0; i < iNum_TriCnt; i += 2)
	{
		pIndices[iNowIndex++] = i; 
		pIndices[iNowIndex++] = i + 2; 
		pIndices[iNowIndex++] = i + 3;

		pIndices[iNowIndex++] = i;
		pIndices[iNowIndex++] = i + 3;
		pIndices[iNowIndex++] = i + 1; 
	}

	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceDesc.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&(this->m_pIB))))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;

	
}


CVIBuffer_Trail* CVIBuffer_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Trail::Clone(void* pArg)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();
	Safe_Release(this->m_pVB);
	Safe_Release(this->m_pIB);
}
