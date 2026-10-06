#pragma once
#include"Client_Defines.h"
#include"GameObject.h"
BEGIN(Engine)
class CCollider;
END
BEGIN(Client)
class CMonster03_Bullet final : public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{

	}MONSTER03_BULLET_DESC;
private:
	CMonster03_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster03_Bullet(const CMonster03_Bullet& Prototype);
	virtual ~CMonster03_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_Active(_bool isActive); 
	_bool Get_Active() { return m_isActive; };
	void Reset_Bullets(_fmatrix matInitialShootingPos);

	void Set_Shot(_bool isShot) { isShot = m_bShot; };
private:
	class CCollider* m_pColliderCom = { nullptr };

	_float3	m_BulletSrc;
	_float3 m_BulletDir;
	_float m_fDistance;


	const _float m_fLifeTimeLimit = 10.f;
	_float m_fAccNowLifeTime = 0.f;

	_bool m_isActive = false;

	/*Effect*/
	class CTrail_Monster03* m_pTrail_Circle[3] = {nullptr};
	class CEffect_Monster03_Shot* m_pEffect_Shot = { nullptr };
	class CParticle_Monster03_Shot* m_pParticle_Shot = { nullptr };
	_bool m_bShot = { false };
private:
	HRESULT Ready_Components();
	HRESULT Ready_Effects();


public:
	static CMonster03_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
