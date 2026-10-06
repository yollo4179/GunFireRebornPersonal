#pragma once
#include"GameObject.h"
BEGIN(Engine)
/* 컨테이너 object는 part 오브젝트를 가지고있다, 각각의  part object는 무기(뼈에붙는), 이펙트 (뼈에붙는),바디, 헤드 ,사운드 객첵가 될수있다*/
class ENGINE_DLL CContainerObject abstract : public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		_uint		iNumPartObject;
	} CONTAINEROBJECT_DESC;

protected:
	CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CContainerObject(const CContainerObject& Prototype);
	virtual ~CContainerObject()=default;

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	/* 레이어에서 컨테이너 오브젝트를 찾았으면 필요한 컴포넌트 (콜라이더 찾기)*/
	class CComponent* Find_Component(_uint iPartObjectIndex, const _wstring& strPartComponentTag);
	class CGameObject* Find_GameObject(_uint iPartObjectIndex);
	/* 이 객체를 구성하기위한 부품(몸, 헤드, 무기, 이펙트, 사운드) 객체들 */
protected:
	vector<CGameObject*>			m_PartObjects;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END