#include"VIBuffer_Particle_Point.h"
#include"GameInstance.h"

CVIBuffer_Particle_Point::CVIBuffer_Particle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer_Particle_Instance{pDevice,pContext}
{
}

CVIBuffer_Particle_Point::CVIBuffer_Particle_Point(const CVIBuffer_Particle_Point& Prototype)
	:CVIBuffer_Particle_Instance(Prototype)
{
}

HRESULT CVIBuffer_Particle_Point::Initialize_Prototype(const PARTICLE_DESC& ParticleDesc)
{
	/*파티클이나 렉트,큐브같은 간단한 도형은 기하셰이더에서 정점을 만들어서 픽셀셰이더로 보내는 작업을 추가하는것이 퍼포먼스적으로 더 낫다*/
	/*UI작업도 포인트와 인덱스 버퍼 그리고 인스턴스 버퍼를 통해서 기하셰이더로 도형을 만드는게 더 낫다*/
	
	m_iNumVertexBuffers = 2;//인스턴스 +정점 버퍼

	
	m_iNumInstances = ParticleDesc.iNumInstances;//인스턴스(만들 사각형의 개수)

	m_iVertexStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;

	m_iIndexStride = sizeof(_ushort);
	/*하나의 점으로 사각형 하나를 만들기 때문에 정점의 인덱스는 인스턴스 개수 만큼 필요하다.*/
	m_iNumIndices = m_iNumInstances;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST; //기본 기하는 라인리스트

#pragma region VERTEX_BUFFER	
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	/*버텍스는 사실 한개*/
	VTXPOINT* pVertices = new VTXPOINT[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOINT) * m_iNumVertices);
	//어차피 버텍스는 한개니까 대표주소로 바로 접근 -> 버텍스는 기본으로 원점에, 사이즈는 1,1 로 할것임->트랜스폼으로 2차 조정
	pVertices->vPosition = XMFLOAT3(0.0f, 0.0f, 0.f);
	pVertices->vSize = XMFLOAT2(1.f, 1.f);

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
	/*인스턴스 개수만큼(*포인트 1개)인덱스 할당 */
	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);
	/*모든 인덱스는 다 0으로 체워준다 -> 기하 셰이더에서 정점 하나로 카메라를 바라보도록 월드 상의 점을 찍을것임*/
	/*기하 셰이더 -> 점 한개 전달 기하 셰이더로 점 6개로 더 만들음 */
	/*버텍스에서 instancedml 크기는 다 지운 후 따로저장하고 로컬 배치까지하고 월드행렬도 곱해서 월드로 이동시킴 */
	/* 버텍스 셰이더에서 점에대한 스케일을 적용하여 크기를 적용(점이니까 이동 후 스케일 , 스케일 후 이동 상관 x )*/
	/*그리고 나서 카메라를 바라보도록 점을 4개 찍고 4개의 점으로 6개의 인덱스 매핑을 해준다 *///사이즈의 반만큼 이동시켜서 4점을 찾자
	/*인스턴스에서는 인덱스 개수만큼 셰이더 호출 (인덱스하나는 도형 하나)*/

	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceDesc.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

#pragma region INSTANCE_BUFFER
	m_iInstanceStride = sizeof(VTXINSTANCE);
	/* 사각형 그리니까 인스턴스당 매핑할(인덱스 개수)는 6개로 할거다*/
	m_iNumIndexPerInstance = 6;

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstances;
	m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;
	m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_InstanceBufferDesc.MiscFlags = 0;

	/*인스턴스 개수만큼 인스턴스를위한 행렬정보를 가지도록 한다(6개의 인덱스매핑된 정점들(파티클인스턴스))에대한*/
	m_pInstanceVertices = new VTXINSTANCE[m_iNumInstances];
	ZeroMemory(m_pInstanceVertices, m_iInstanceStride * m_iNumInstances);

	m_isLoop = ParticleDesc.isLoop;
	m_pSpeeds = new _float[m_iNumInstances];
	

	m_vPivot = ParticleDesc.vPivot;
	m_vRange = ParticleDesc.vRange;
	m_vCenter = ParticleDesc.vCenter;

	for (size_t i = 0; i < m_iNumInstances; i++)
	{
		m_pSpeeds[i] = m_pGameInstance->Compute_Random(ParticleDesc.vSpeed.x, ParticleDesc.vSpeed.y);

		_float		fSize = m_pGameInstance->Compute_Random(ParticleDesc.vSize.x, ParticleDesc.vSize.y);

		m_pInstanceVertices[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		m_pInstanceVertices[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, fSize, 0.f);
		m_pInstanceVertices[i].vTranslation =
			_float4(
				m_pGameInstance->Compute_Random(ParticleDesc.vCenter.x - ParticleDesc.vRange.x * 0.5f, ParticleDesc.vCenter.x + ParticleDesc.vRange.x * 0.5f),
				m_pGameInstance->Compute_Random(ParticleDesc.vCenter.y - ParticleDesc.vRange.y * 0.5f, ParticleDesc.vCenter.y + ParticleDesc.vRange.y * 0.5f),
				m_pGameInstance->Compute_Random(ParticleDesc.vCenter.z - ParticleDesc.vRange.z * 0.5f, ParticleDesc.vCenter.z + ParticleDesc.vRange.z * 0.5f),
				1.f);

		m_pInstanceVertices[i].vLifeTime = _float2(
			m_pGameInstance->Compute_Random(ParticleDesc.vLifeTime.x, ParticleDesc.vLifeTime.y),
			0.f
		);
	}

	ZeroMemory(&m_InstanceInitialDesc, sizeof m_InstanceInitialDesc);
	m_InstanceInitialDesc.pSysMem = m_pInstanceVertices;

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Particle_Point* CVIBuffer_Particle_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_DESC& ParticleDesc)
{
	CVIBuffer_Particle_Point* pInstance = new CVIBuffer_Particle_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(ParticleDesc)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Particle_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Particle_Point::Clone(void* pArg)
{
	CVIBuffer_Particle_Point* pInstance = new CVIBuffer_Particle_Point(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Particle_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Particle_Point::Free()
{
	__super::Free();
}
