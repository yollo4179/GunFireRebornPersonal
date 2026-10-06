#pragma once
#include"Base.h"
#include"Collider.h"
#include"GameObject.h"

class CCollider_Manager final :public CBase
{
public:
	enum COLLISION_GROUP
	{
		COLLISION_MONSTER,
		COLLISION_MONSTER_RANGE,
		COLLISION_PLAYER,
		COLLISION_CHECK_RAY,
		COLLISION_MONSTER_ATTACK,
		COLLISION_PLAYER_ATTACK,
		COLLISION_ITEM,
		COLLISION_NPC,
		COLLISION_END
	};

private:
	explicit CCollider_Manager();
	virtual ~CCollider_Manager()=default;
	HRESULT Initialize();
public:
	HRESULT		Add_Collision_Group(COLLISION_GROUP eMyGroup,CGameObject* pGameObject,CCollider* pCollider);
	void		Clear_Collision_Group();
	_bool		Check_Collision(CCollider* pCollider, COLLISION_GROUP eTargetGroup, CGameObject** ppOutTargetObject);
private:
	list<pair<CGameObject*, CCollider*>>m_CollisionPairList[COLLISION_END];

public:
	static CCollider_Manager* Create();
	virtual void Free()override;

};

