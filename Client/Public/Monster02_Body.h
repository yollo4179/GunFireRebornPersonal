#pragma once
#include"Client_Defines.h"
#include"PartObject.h"
BEGIN(Engine)
class CCollider;
END
BEGIN(Client)
class CMonster02_Body final :public CPartObject
{

public: 
	typedef struct : CPartObject::PARTOBJECT_DESC
	{
		_uint* pState;
		const _float4x4* pSocketMatrix;
		_bool* pisMonsterPicked;

		_float* pHitTime;
		_float* pAccHitTime;
		_float4* pHPInfo;
		_bool* pHit_NormalMonster;

	}MONSTER02_BODY_DESC;




private:
	CMonster02_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster02_Body(const CMonster02_Body& Prototype);
	virtual ~CMonster02_Body() = default;
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
	const _float4x4* m_pSocketMatrix = { nullptr };
	_bool* m_pisMonsterPicked = { nullptr };

	_float* m_pHitTime = {nullptr};
	_float* m_pAccHitTime={ nullptr };
	_float4* m_pHPInfo={ nullptr };

	_bool* m_pHit_NormalMonster = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
public:
	static CMonster02_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

