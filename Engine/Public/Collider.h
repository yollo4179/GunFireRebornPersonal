#pragma once
#include"Component.h"
/* 객체에 씌워야 할 충돌체를 의미한다 tk의 boundingbox 구조체를 가진   box 자식 구조체 들중 하나를 가짐 */

BEGIN(Engine)
class ENGINE_DLL  CCollider final :public CComponent 
{

public:
	enum COLLIDERTYPE{TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_RAY, TYPE_END};

private:

	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLIDERTYPE eColliderType);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void    Update(_fmatrix WorldMatrix);
	virtual void	Update(pair<_float3, _float3> pairRay);
	
#ifdef _DEBUG
public:
	virtual HRESULT Render();
#endif

public:
	_bool Intersect(const CCollider* pTargetCollider);
public:
	_matrix Get_WorldMatrix() { return XMLoadFloat4x4(&m_matColliderTransform); };
	_vector Compute_RayPos();

private: 
	COLLIDERTYPE			m_eType = { TYPE_END };
	class CBounding*		m_pBounding = { nullptr }; //자식 클래스로부터 실제 충돌 구조체를 가져온다

#ifdef _DEBUG
	PrimitiveBatch<VertexPositionColor>*		m_pBatch = { nullptr };
	BasicEffect*								m_pEffect = { nullptr };
	ID3D11InputLayout*							m_pInputLayout = { nullptr };
#endif

	/*콜라이더 위치 검출*/
protected:
	_float4x4 m_matColliderTransform;

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDERTYPE eColliderType);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;

};
END

