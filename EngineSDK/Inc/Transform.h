#pragma once

#include "Component.h"

/* 다른 컴포넌트와는 좀 다르게 객체 생성 시 강제로 트랜스폼 컴포넌트를 가지고 있게 할것이다. */
/* 객체의 월드 상태를 표현한다.(m_WorldMatrix) */
/* 객체의 월드 상태 변환의 기능을 수행한다. 앞으로 간다, 회전한다. 바라본다.  */
/* 월드행렬을 정점에게 적용할 수 있도록 쉐이더로 월드행렬을 던지는 기능을 수행한다. */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

	typedef struct
	{
		_float		fSpeedPerSec{};
		_float		fRotationPerSec{};

	}TRANSFORM_DESC;
private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState], vState);
	}

	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	const _float4x4* Get_WorldFloat4x4_Ptr()
	{
		/* 컨테이너 object에서 part object로 매프레임 변하는 행렬값 을 주기위해 참조로
		파트가 컨테이너의 월드 행렬을 가지고 있게 한다.
		*/
		return &m_WorldMatrix;
	}

	_matrix Get_WorldMatrix_Inverse()
	{
			return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	_float3 Compute_Scaled();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void Scaling(_float fX, _float fY, _float fZ);
	_bool Go_Straight(_float fTimeDelta, _bool bRemoveYElement=false, class CNavigation* pNavigation = nullptr,_bool isJumping=false ,_float BaseHeight=0.f);


	void Go_Backward(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	_bool Go_Left(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	_bool Go_Right(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	_bool Go_Pushed(_float fTimeDelta, _fvector vPushedDir,class CNavigation* pNavigation=nullptr);

	void LookAt(_fvector vAt);

	/* 기존 회전을 기준으로 추가로 정해진 속도로 회전한다. */
	void Turn(_fvector vAxis, _float fTimeDelta);
	/* 항등상태를 기준으로 지정한 각도로 회전한다. */
	void Rotation(_fvector vAxis, _float fRadian);


	/*For Navigation Sliding Vector*/
	_vector Get_SlidingFromLookVector(_fvector vLook, class CNavigation* pNavigation);
	
public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);


private:
	/* rowmajor*/
	_float4x4				m_WorldMatrix = {};

	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};


	/*ForSliding ->ToNext Cell*/
	_float3 m_PrevLine = {};
	_uint m_iPrevIndex = {};
	_int m_iPrevSign = { 0 };
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END