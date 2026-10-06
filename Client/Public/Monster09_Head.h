#pragma once
#include"Client_Defines.h"
#include"PartObject.h"
BEGIN(Engine)
class CCollider;
END
BEGIN(Client)
class CMonster09_Head final :public CPartObject
{

public:
	typedef struct : CPartObject::PARTOBJECT_DESC
	{
		_uint* pState;
		_float* pNowShotTiming;
		const _float4x4* pSocketMatrix;
		const _float4x4* pSocketMatrix_Eyes[3];

	}MONSTER09_HEAD_DESC;




private:
	CMonster09_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster09_Head(const CMonster09_Head& Prototype);
	virtual ~CMonster09_Head() = default;
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
	_float* m_pNowShotTiming = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	_bool m_bShotSign = { false };
	_bool m_bShotEnd = { false };
	const _float4x4* m_pSocketMatrix_Eyes[3] = {nullptr};
	_float4x4 m_EyeMatrix[3] = {};
	class CTrail_Monster09* m_pTrail_Eyes[3] = { nullptr };
	_float m_fAccAddTrailTiming = {0.f};
	_float m_fAddTrail_Timing = { 0.0005f };

private:
	vector<class CMonster09_Shot*>m_pWaterShots;
	_uint m_iNowShotIndex = { 0 };
	const _uint m_iNumShots = { 2 };
	/*Particle*/
	class CParticle_Splash* m_pParticle= { nullptr };
	class  CParticle_Monster09_Shot_Charging*	m_pCharging = { nullptr };
	
private:
	HRESULT Ready_Components();
	HRESULT Ready_Bullets();
	HRESULT Ready_Particle();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Trails();
public:
	static CMonster09_Head* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END