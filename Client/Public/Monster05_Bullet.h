#pragma once
#include"Client_Defines.h"
#include"GameObject.h"
BEGIN(Engine)
class CCollider;

END
BEGIN(Client)
class CMonster05_Bullet final : public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{

	}MONSTER05_BULLET_DESC;
private:
	CMonster05_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster05_Bullet(const CMonster05_Bullet& Prototype);
	virtual ~CMonster05_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_Active(_bool isActive) { m_isActive = isActive; };
	_bool Get_Active() { return m_isActive; };
	void Reset_Bullets(_fmatrix matInitialShootingPos);
	void Set_TracingMode(_bool TracingMode) { m_bTracingMode = TracingMode; };

private:
	class CCollider* m_pColliderCom = { nullptr };

	_float3	m_BulletSrc;
	_float3 m_BulletDir;
	_float m_fDistance;


	const _float m_fLifeTimeLimit = 10.f;
	_float m_fAccNowLifeTime = 0.f;

	_bool m_isActive = false;

	/*Effect*/
	class CEffect_Monster05_Shot* m_pEffect_Shot = { nullptr };
	class CTrail_Monster05* m_pTrail_Shot = { nullptr };
	_float m_fAccTrailTime = { 0.f };
	_float m_fTrailTiming = { 0.05f };
	_bool m_bTracingMode = { false };



private:
	HRESULT Ready_Components();
	HRESULT Ready_Effects();


public:
	static CMonster05_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

