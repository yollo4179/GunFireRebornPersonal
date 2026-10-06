#pragma once
#include"Client_Defines.h"
#include"Hands.h"
const int g_iNumParticle_Player02_Hit = 10;
BEGIN(Engine)
class CCollider;
END
BEGIN(Client)
class CHands02_Bullet final :public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
	
	}PARTICLE_HANDS03_BULLET_DESC;

private:
	explicit CHands02_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CHands02_Bullet(const CHands02_Bullet& Prototype);
	virtual ~CHands02_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	/*For Particle Texture*/
	void Set_Pivot(_float3 fPivot);
	void Set_Particle_Matrix(_fmatrix vPosMatrix);
	void Reset_Vertices(_float fTimeDelta);
	/*For Bullet Collision*/
	void Set_Active(_bool isActive) { m_isActive = isActive; };
	_bool Get_Active() { return m_isActive; };
	void Reset_Bullets(_fmatrix matInitialShootingPos);


private:
	
	class CParticle_Player02_Bullet* m_pParticle_Bullet = { nullptr };

	class CCollider* m_pColliderCom = { nullptr };
	_float3	m_BulletSrc;
	_float3 m_BulletDir;
	_float m_fDistance;

	const _float m_fLifeTimeLimit = 10.f; //파티클도 라이프타임은 10초로 하자 ( 알파값 줄이는 것도 없애고 ...(플래이어 공격 패스 하나 따로 파자))
	_float m_fAccNowLifeTime = 0.f;
	_bool m_isActive = false;

	/* monster에게 터지는 파티클*/
	class CParticle_Player02_Hit* m_pHitParticles[g_iNumParticle_Player02_Hit] = {};
	_float m_fAccHitParticle_Time[g_iNumParticle_Player02_Hit] = {};
	const _float4x4* m_pFloat4x4sOfTargetPart[g_iNumParticle_Player02_Hit] = {};
	_float m_fHitParticleTime = 0.5f;
	_int m_iNowHitParticleIndex = 0;
	
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Particle();

public:
	static   CHands02_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
