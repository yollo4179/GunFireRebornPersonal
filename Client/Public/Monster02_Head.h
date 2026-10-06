#pragma once
#include"Client_Defines.h"
#include"PartObject.h"
BEGIN(Engine)
class CCollider;
END
BEGIN(Client)
class CMonster02_Head final :public CPartObject
{

public:
	typedef struct : CPartObject::PARTOBJECT_DESC
	{
		_uint* pState;
		_bool* pHeadShot;
		_bool* pisMonsterPicked;
		const _float4x4* pSocketMatrix;
		
		_float*  pHitTime;
		_float*  pAccHitTime;
		_float4* pHPInfo;

		_bool* pHit_NormalMonster;
	}MONSTER02_HEAD_DESC;




private:
	CMonster02_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster02_Head(const CMonster02_Head& Prototype);
	virtual ~CMonster02_Head() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CCollider* m_pColliderCom = { nullptr };

private:
	_uint* m_pState = { nullptr };
	_bool* m_pHeadShot = { nullptr };
	_bool* m_pisMonsterPicked = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };


	_float* m_pHitTime;
	_float* m_pAccHitTime;
	_float4* m_pHPInfo;

	_bool* m_pHit_NormalMonster = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
public:
	static CMonster02_Head* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END