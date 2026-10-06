#include "VIBuffer_Collider.h"

CVIBuffer_Collider::CVIBuffer_Collider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Collider::CVIBuffer_Collider(const CVIBuffer_Collider& Prototype)
	:CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Collider::Initialize(void* pArg)
{

	return S_OK;
}

HRESULT CVIBuffer_Collider::Initialize_Prototype()
{
	m_iNumVertexBuffers = 1;

	m_iVertexStride = sizeof(VTXCUBECOL);
	m_iNumVertices = 8;

	m_iIndexStride = sizeof(_ushort);
	m_iNumIndices = 36;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER	
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXCUBECOL* pVertices = new VTXCUBECOL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXCUBECOL) * m_iNumVertices);

	//전면
	pVertices[0].vPosition = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vColor = XMFLOAT3(1.f, 1.f, 1.f);
	

	pVertices[1].vPosition = XMFLOAT3(0.5f, 0.5f, -0.5f);
	pVertices[1].vColor = XMFLOAT3(1.f, 1.f, 1.f);

	pVertices[2].vPosition = XMFLOAT3(0.5f, -0.5f, -0.5f);
	pVertices[2].vColor = XMFLOAT3(1.f, 1.f, 1.f);

	pVertices[3].vPosition = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vColor = XMFLOAT3(1.f, 1.f, 1.f);

	//후면
	pVertices[4].vPosition = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vColor = XMFLOAT3(1.f, 1.f, 1.f);

	pVertices[5].vPosition = XMFLOAT3(0.5f, 0.5f, 0.5f);
	pVertices[5].vColor = XMFLOAT3(1.f, 1.f, 1.f);

	pVertices[6].vPosition = XMFLOAT3(0.5f, -0.5f, 0.5f);
	pVertices[6].vColor = XMFLOAT3(1.f, 1.f, 1.f);

	pVertices[7].vPosition = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vColor = XMFLOAT3(1.f, 1.f, 1.f);

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
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
	_int iIndex = 0;
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);
	//전면
	{
		pIndices[iIndex++] = 0;
		pIndices[iIndex++] = 1;
		pIndices[iIndex++] = 2;

		pIndices[iIndex++] = 0;
		pIndices[iIndex++] = 2;
		pIndices[iIndex++] = 3;
	}
	//후면
	{
		pIndices[iIndex++] = 5;
		pIndices[iIndex++] = 4;
		pIndices[iIndex++] = 7;

		pIndices[iIndex++] = 5;
		pIndices[iIndex++] = 7;
		pIndices[iIndex++] = 6;

	}
	//왼쪽
	{
		pIndices[iIndex++] = 4;
		pIndices[iIndex++] = 0;
		pIndices[iIndex++] = 3;

		pIndices[iIndex++] = 4;
		pIndices[iIndex++] = 3;
		pIndices[iIndex++] = 7;
	}
	//오른쪽
	{
		pIndices[iIndex++] = 1;
		pIndices[iIndex++] = 5;
		pIndices[iIndex++] = 6;

		pIndices[iIndex++] = 1;
		pIndices[iIndex++] = 6;
		pIndices[iIndex++] = 2;
	}
	//위
	{
		pIndices[iIndex++] = 4;
		pIndices[iIndex++] = 5;
		pIndices[iIndex++] = 1;

		pIndices[iIndex++] = 4;
		pIndices[iIndex++] = 1;
		pIndices[iIndex++] = 0;
	}
	//아래
	{
		pIndices[iIndex++] = 3;
		pIndices[iIndex++] = 2;
		pIndices[iIndex++] = 6;

		pIndices[iIndex++] = 3;
		pIndices[iIndex++] = 6;
		pIndices[iIndex++] = 7;
	}
	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceDesc.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	return S_OK;
}

CVIBuffer_Collider* CVIBuffer_Collider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Collider* pInstance = new CVIBuffer_Collider(pDevice, pContext);
	if (pInstance->Initialize_Prototype())
	{
		MSG_BOX("Created Failed :Cube");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent* CVIBuffer_Collider::Clone(void* pArg)
{
	CVIBuffer_Collider* pInstance = new CVIBuffer_Collider(*this);
	if (pInstance->Initialize(pArg))
	{
		MSG_BOX("Clone Failed :Cube");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVIBuffer_Collider::Free()
{
	__super::Free();
}
