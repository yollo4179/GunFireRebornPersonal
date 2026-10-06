#pragma once
#include"Client_Defines.h"
#include"PartObject.h"

BEGIN(Engine)
class CCollider;
END
BEGIN(Client)
class CMonster04_Sword final :public CPartObject
{

public:
	typedef struct :CPartObject::PARTOBJECT_DESC
	{
		_uint* pState;
		const _float4x4* pSocketMatrix;
	}MONSTER04_SWORD_DESC;
private:
	CMonster04_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster04_Sword(const CMonster04_Sword& Prototype);

	virtual ~CMonster04_Sword()=default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	CCollider* m_pColliderCom[4] = {nullptr,nullptr,nullptr,nullptr };
private:
	_uint* m_pState = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	 
	class CTrail_Monster04* m_pTrail = { nullptr };
	_bool m_isAttackDoing = { false };
	_bool m_bAttackDone = { false };
	_float m_fAccVerticesAddTiming = {};
	_float m_fVerticesAddingTerm = {0.01f};
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Trail();

public:
	static CMonster04_Sword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};
END

