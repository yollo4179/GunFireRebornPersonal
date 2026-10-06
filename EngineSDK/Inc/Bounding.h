#pragma once
#include"Base.h"
#include"DebugDraw.h"
#include"Collider.h"
/* 각 충돌체 클래스들의 부모 클래스 : directxCollision이 제공하는 구조체는 부모 타입이 없어 */
/* 충돌체 끼리 상속을 사용할수 없다 aabb, obb sphehre */
/* 원래 구조체를 쓰면 되는 구조체의 부모 구조체가 없다 */
BEGIN(Engine)
class CBounding abstract: public CBase
{
public:
	typedef struct
	{
		//-> 모든 종류의 바운딩박스가 센터를 가지므로 이거는 부모가 공통으로 가짐 
		_float3 vCenter; 

	} BOUND_DESC;
	
protected:
	//부모니까 접근 지정자는 protected로 
	explicit CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_fmatrix WorldMatrix) = 0;  //실제 구현 객체들이 맞게 골라이더가 따라가도록, draw 하도록 override
	virtual void Update(pair<_float3, _float3>pairRay) = 0;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) = 0;
	virtual _bool Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding) = 0;


protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	
	/* 그렸을때 충돌했으면 색갈을 바꾸기위한 불 변수*/
	_bool				m_isColl = { false };

public:
	virtual void Free() override;

};
END
