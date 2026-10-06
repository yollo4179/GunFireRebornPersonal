#pragma once
#include"GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CPartObject abstract :public CGameObject
{

public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
		//나의 로컬이 배치되고 나서 ( (0,0)로컬 -> *본에 붙인다 (본행렬)-> *(컨테이너월드에 배치한다)
	{//부모의 월드행렬을 일단 가지고 있는다  부모가 배치된 자리를 따라다녀야되니까
		const _float4x4* pParentMatrix;
	}PARTOBJECT_DESC;

protected:
	CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartObject(const CPartObject& Prototype);
	virtual ~CPartObject() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;
public:
	_matrix Get_WorldMatrixOfPartObject() { return XMLoadFloat4x4(&m_WorldMatrix); };
	 const _float4x4* Get_Float4x4_Ptr_OfPartObject(){ return &m_WorldMatrix; };
protected:
	//부모행렬의 변화에 따라  파트오브젝트의 전체 행렬도 재배치해줘야 되므로 일단 가지고 있는다 . 
	const _float4x4* m_pParentMatrix = {}; 
	
	/* 얘는 파트 오브젝트의 전체 콤바인드 메트릭스 개념이다*/
	/* 나의 로컬상태(항등) * 본 행렬 (뼈에 장착 (배치))* world 에 재배치-> 최종 행렬 저장 */
	_float4x4		m_WorldMatrix = {};


public:
	virtual void Free() override;
};
END

