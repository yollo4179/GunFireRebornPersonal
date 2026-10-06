#include "..\Public\VIBuffer_Particle_Rect.h"

#include "GameInstance.h"


CVIBuffer_Particle_Rect::CVIBuffer_Particle_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Particle_Instance{ pDevice, pContext }
{
}

CVIBuffer_Particle_Rect::CVIBuffer_Particle_Rect(const CVIBuffer_Particle_Rect& Prototype)
	: CVIBuffer_Particle_Instance( Prototype )
{
}

HRESULT CVIBuffer_Particle_Rect::Initialize_Prototype(const PARTICLE_DESC& ParticleDesc)
{
	m_iNumVertexBuffers = 2; //버텍스 버퍼의 개수는 2개 (사각형을 여러개 그림->  각각의 사각형을 제어할 행렬(혹은 필요한 추가 정보)을 저장할 버텍스 버퍼를 하나더  사용 ->(인스턴싱) )
	m_iNumInstances = ParticleDesc.iNumInstances; //외부에서 몇개의 파티클 만들것인지 정보 전해줌 

	m_iVertexStride = sizeof(VTXPOSTEX);  //내 정점 버퍼는 텍스처 (텍스처르 파티클로)
	m_iNumVertices = 4;

	m_iIndexStride = sizeof(_ushort);
	m_iNumIndices = 6 * m_iNumInstances;// 인스턴스 개수만큼 6개의 인덱스가 필요 (각각을 제어하기위해서 -> 6개의 인덱스는 1개의 사각형 -> 인스턴수 개수마늠 같은 0,0 로컬용욕에 파티클을 만들음) 
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/*버텍스 버퍼 할당은 똑같다.*/
#pragma region VERTEX_BUFFER	
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	pVertices[0].vPosition = XMFLOAT3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = XMFLOAT2(0.f, 0.f);

	pVertices[1].vPosition = XMFLOAT3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = XMFLOAT2(1.f, 0.f);

	pVertices[2].vPosition = XMFLOAT3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = XMFLOAT2(1.f, 1.f);

	pVertices[3].vPosition = XMFLOAT3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = XMFLOAT2(0.f, 1.f);

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion


	//인스턴스 (파티클 ) 개수만큼 인덱스 6개 할당 
#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */ //바이트 크기는 인덱스 개수 * 인덱스 의 크기
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	_uint			iNumIndices = { 0 };

	for (size_t i = 0; i < m_iNumInstances; i++)
	{
		pIndices[iNumIndices++] = 0;
		pIndices[iNumIndices++] = 1;
		pIndices[iNumIndices++] = 2;

		pIndices[iNumIndices++] = 0;
		pIndices[iNumIndices++] = 2;
		pIndices[iNumIndices++] = 3;
	}



	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceDesc.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion


	//인스턴스 ( 행렬을 저장하는 버퍼)
#pragma region INSTANCE_BUFFER
	m_iInstanceStride = sizeof(VTXINSTANCE);
	m_iNumIndexPerInstance = 6;

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstances;//인스턴스 정보 크기(셰이더로 던질))
	m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  //형식은 버텍스 버퍼
	m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;   ///맵 업맵으로 행렬을 변화시킴 -> 다이나믹 
	m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;
	m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//타이나믹이니까 cpu에서 동적으로 접근할수 있게 허용 
	m_InstanceBufferDesc.MiscFlags = 0;

	m_pInstanceVertices = new VTXINSTANCE[m_iNumInstances];
	ZeroMemory(m_pInstanceVertices, m_iInstanceStride * m_iNumInstances);

	m_isLoop = ParticleDesc.isLoop;
	m_pSpeeds = new _float[m_iNumInstances];
	m_vPivot = ParticleDesc.vPivot;



	for (size_t i = 0; i < m_iNumInstances; i++)
	{
		
		m_pSpeeds[i] = m_pGameInstance->Compute_Random(ParticleDesc.vSpeed.x, ParticleDesc.vSpeed.y);
		_float fSize = m_pGameInstance->Compute_Random(ParticleDesc.vSize.x, ParticleDesc.vSize.y);

		
		m_pInstanceVertices[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		m_pInstanceVertices[i].vUp = _float4(0.f,fSize, 0.f, 0.f);
		m_pInstanceVertices[i].vLook = _float4(0.f, 0.f,fSize, 0.f);
		m_pInstanceVertices[i].vTranslation =
			_float4(
				m_pGameInstance->Compute_Random(ParticleDesc.vCenter.x - ParticleDesc.vRange.x*0.5f, ParticleDesc.vCenter.x + ParticleDesc.vRange.x*0.5f),
				m_pGameInstance->Compute_Random(ParticleDesc.vCenter.y - ParticleDesc.vRange.y*0.5f, ParticleDesc.vCenter.y + ParticleDesc.vRange.y*0.5f),
				m_pGameInstance->Compute_Random(ParticleDesc.vCenter.z - ParticleDesc.vRange.z*0.5f, ParticleDesc.vCenter.z + ParticleDesc.vRange.z*0.5f),
				1.f);

		m_pInstanceVertices[i].vLifeTime = _float2(
			m_pGameInstance->Compute_Random(ParticleDesc.vLifeTime.x, ParticleDesc.vLifeTime.y),
			0.f
		);
	}

	ZeroMemory(&m_InstanceInitialDesc, sizeof m_InstanceInitialDesc);
	m_InstanceInitialDesc.pSysMem = m_pInstanceVertices;//이거로 정보만 채워놓고 사본 만뜰때 이거 바탕으로 만들음

	//프로토에서는 정보만 채워놓고 초기값 세팅 (정보를 들고있고 )이니셜라이즈 에서 실제 정보를 기반으로 버퍼를 할당 (항상 초기값은 가지고 있다 ) 
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Particle_Rect::Initialize(void* pArg)
{
	//기본 정보는 프로토타입에서 하고, 인스턴스 버퍼는 사본에서따로 만들자 파티클 위치정보는  타티클마다 달라야하니까 ( 동시에 같은 파티클 -> 모양이 같으면 어색함-> 애니메이션처처럼 가각의 키프레임 인덱스는 따로 저장햇던것 기억)
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Particle_Rect* CVIBuffer_Particle_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_DESC& ParticleDesc)
{
	CVIBuffer_Particle_Rect* pInstance = new CVIBuffer_Particle_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(ParticleDesc)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Particle_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Particle_Rect::Clone(void* pArg)
{
	CVIBuffer_Particle_Rect* pInstance = new CVIBuffer_Particle_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Particle_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CVIBuffer_Particle_Rect::Free()
{
	__super::Free();
}
