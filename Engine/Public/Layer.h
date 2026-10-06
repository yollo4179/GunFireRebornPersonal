#pragma once

#include "Base.h"

/* 객체들을 레이어 별로 묶어서 다수 보관한다. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	// 컴포넌트 태그를 통해 컴포넌트를 가져온다 ,레이어 리스트의 index 를 알면 인덱스도 넘겨줌( 플레이어때문에 사용할 일이 많을 것)
	//자주 사용하는것들은 독립적인 레이어 를 만들어 걔만 불러오자, 복합적이더라도 0번 만 사용할 수 있게 세팅
	class CComponent* Get_Component(const _wstring strComponentTag, _uint iObjectIndex = 0);// 걍 플레이어 꺼 
	class CComponent* Get_Component(_uint iPartObjectIndex, const _wstring& strPartComponentTag, _uint iObjectIndex = 0);//플레이어의 파트까
	class CGameObject* Get_GameObject(_uint iObjectIndex);
	class CGameObject* Get_GameObject(_uint iPartObjectIndex, _uint iObjectIndex);
public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Remove_From_Layer(CGameObject* pGameObject);
	

private:
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END