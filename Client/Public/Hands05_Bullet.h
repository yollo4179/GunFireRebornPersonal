#pragma once
#include"Client_Defines.h"
#include"GameObject.h"
BEGIN(Engine)
class CCollider;
class CModel;
class CShader;
END
BEGIN(Client)
class CHands05_Bullet final :public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		
	}HANDS05_BULLET_DESC;

private:
	explicit CHands05_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CHands05_Bullet(const CHands05_Bullet& Prototype);
	virtual ~CHands05_Bullet() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:

	/*For Bullet Collision*/
	void Set_Active(_bool isActive); 
	_bool Get_Active() { return m_isActive; };
	void Reset_Bullets(_fmatrix matInitialShootingPos);


private:

	class CCollider* m_pColliderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };
	class CShader* m_pShaderCom = { nullptr };
	_bool m_isAnimEnd = {false};
	_bool m_isChangingMotion = { false };


	_float3	m_BulletSrc;
	_float3 m_BulletDir;
	_float m_fDistance;

	const _float m_fLifeTimeLimit = 10.f; //파티클도 라이프타임은 10초로 하자 ( 알파값 줄이는 것도 없애고 ...(플래이어 공격 패스 하나 따로 파자))
	_float m_fAccNowLifeTime = 0.f;
	_bool m_isActive = false;

	_float4 m_vInitRight = {};
	_float4 m_vInitUp = {};
	_float4 m_vInitLook = {};

	_bool m_bColMoving = false;
	_bool m_bRowMoving = false;
	

	class CParticle_Player05_Bullet_Chasing* m_pParticle_Chasing = { nullptr };
	class CParticle_Player05_Bomb* m_pParticle_Bomb = { nullptr };


	_float m_fAccTimeDelta = 0.f;


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();
	HRESULT Ready_Particle();

public:
	static   CHands05_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};
END
