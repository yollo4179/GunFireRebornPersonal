#pragma once
#include"Client_Defines.h"
#include"GameObject.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class CTexture;
class CCollider;
END
BEGIN(Client)
class CHands_Skill_Granade_Bomb final :public CGameObject 
{
	public:
	typedef struct: public CGameObject::GAMEOBJECT_DESC
	{

	}HANDS_SKILL_GRANADE_BOMB_DESC;

private:
	explicit CHands_Skill_Granade_Bomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CHands_Skill_Granade_Bomb(const CHands_Skill_Granade_Bomb& Prototype);
	virtual ~CHands_Skill_Granade_Bomb() = default;

public:
	/*For Bullet Collision*/
	void Set_Active(_bool isActive);
	_bool Get_Active() { return m_isActive; };
	void Reset_Bullets(_fmatrix matInitialShootingPos);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void Set_BombRange(_float fBombRange) { m_fBombRange = fBombRange; };

private:
	
	CTexture* m_pTextureCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	class CParticle_Player_Skill_Poison* m_pParticle = { nullptr };//Prototype_GameObject_Particle_Player_Skill_Poison
	
	_float2 m_vRange = {5.f,5.f};
	_float4 m_vLook = {};
	_float4 m_vPosition = {};
	_bool m_isActive = {false};
	_float m_fAccNowLifeTime = { 0.f };
	_float m_fLifeTime = { 4.f };

	_float m_fBombRange = { 3.f};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources_ToTerrain();
	HRESULT Ready_Particle();

public:
	static   CHands_Skill_Granade_Bomb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};
END

