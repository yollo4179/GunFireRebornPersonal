#pragma once
#include"Client_Defines.h"
#include"PartObject.h"
BEGIN(Engine)
class CCollider;
END
BEGIN(Client)
class CMonster09_Body final :public CPartObject
{

public:
	typedef struct : CPartObject::PARTOBJECT_DESC
	{
		_uint* pState;
		const _float4x4* pSocketMatrix[10];
		_float2* pBossHPInfo;
		_float* pHitTime;
		_float* pAccHitTime;
		const _float4x4* pSocketMatrixFor_AngelRing;

	}MONSTER09_BODY_DESC;




private:
	CMonster09_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster09_Body(const CMonster09_Body& Prototype);
	virtual ~CMonster09_Body() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CCollider* m_pColliderCom[10] = {nullptr};

private:
	_uint* m_pState = { nullptr };
	const _float4x4* m_pSocketMatrix[10] = {nullptr};

	_float2* m_pBossHPInfo = { nullptr };
	_float* m_pHitTime = { nullptr };
	_float* m_pAccHitTime = { nullptr };


	const _float4x4* m_pSocketMatrixFor_AngelRing = { nullptr };
	_float4x4  m_pRealMatrixFporAngelRing = {};
	class Effect_Monster09_AngelRing*  m_pAngelRing= { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_AngelRing();
public:
	static CMonster09_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

