#pragma once
#include"VIBuffer.h"
/* 파티클 인스턴스들의 공통 기능을 모아놓을 부모 클래스 */
BEGIN(Engine)
class ENGINE_DLL  CVIBuffer_Particle_Instance abstract : public CVIBuffer
{

public:
	/* 생성할 파티클 인스턴스의 개수*/
	typedef struct
	{
		_uint			iNumInstances = {};//파티클인스턴스의 개수
		_float3			vCenter; //파티클 인스턴스들의중심
		_float3			vRange;  // 파티클이 퍼질 범위
		_float3			vPivot;  //explosion용이다  파티클은 피봇에서 뺀 방향벡터를 기반으로 움직인다(매번 피봇기준으로 갱신)
		_float2         vSpeed;// x성분은 speed의 최소 y는 최댓값
		_float2         vLifeTime; //x 는 파티클당 라이프 타임 최솟값 y성분은 최대 라이프타임 제한값
		_float2         vSize;		//사이즈의 최소소값과 최댓값을 저장
		_bool			isLoop;    //파티클을 루프로 돌려서 다시 재생할 것인지를 결정	
	}PARTICLE_DESC;


protected:
	CVIBuffer_Particle_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Particle_Instance(const CVIBuffer_Particle_Instance& Prototype);
	virtual ~CVIBuffer_Particle_Instance() = default;
public:

	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render();
	virtual HRESULT Bind_BufferDesc();

public:
	void Set_Pivot(_float3 vPivot) { m_vPivot = vPivot; };
	void Set_Range(_float3 vRange) { m_vRange = vRange; };
public:
	void Drop(_float fTimeDleta);
	void Spread(_float fTimeDelta);
	void Spread_Gravity(_float fTimeDelta);
	void Hit_Random_Show(_float fTimeDleta);
	void Go_Up_Random(_float fTImeDelta);
	void Absorb(_float fTimeDelta);
	void Rotation_Horizon(_float fTimeDelta);
	void Reset_Vertices(_float fTimeDelta);
	void Reset_Vertices_Random(_float fTimeDelta);
	void Reset_Particle_Transform(_fmatrix matParticle);

protected:
	ID3D11Buffer*					m_pVBInstance = { nullptr };
	D3D11_BUFFER_DESC				m_InstanceBufferDesc = {}; //파티클 인스턴스의 버텍스 정보 
	D3D11_SUBRESOURCE_DATA			m_InstanceInitialDesc = {};// 파티클 인스턴스 초기 세팅 데이터 -> 원형만들때 한번 만들고 저장해놓는다 .사본 만들때 얕은 복사
	_uint							m_iNumInstances = {};      //인스턴스(파티클)의 개수
	_uint							m_iNumIndexPerInstance = {};  //파티클 하나당 인덱스의 개수 (drawindexed Instance 함수가 요구하는 인자 )
	_uint							m_iInstanceStride = {};			//인스턴스 구조체의 크기
	VTXINSTANCE*					m_pInstanceVertices = {};		///사본 마다 가지고 있는 버텍스 행렬을 저장할 배열을가리킬 포인터
	//파티클 제어 
	_float3							m_vPivot = {};
	_bool							m_isLoop = {};
	_float*							m_pSpeeds = {};
	_float*							m_pSpeeds_Gravity = {};
	_float3							m_vCenter = {};//For Random
	_float3							m_vRange = {}; //For Random
	_float							m_fGravity = { 9.8f };


public:

	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};
END

