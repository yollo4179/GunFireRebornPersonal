#pragma once
#include"Client_Defines.h"
#include"GameObject.h"
#include"Hands_Skill_Granade.h"
BEGIN(Engine)
class CCollider;
class CModel;
class CShader;
class CNavigation;
END
BEGIN(Client)
class CHands_Skill_Granade final :public CGameObject
{

public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		_int iNowMapStage;
	}HANDS_SKILL_GRANADE_DESC;
private:
	explicit CHands_Skill_Granade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CHands_Skill_Granade(const CHands_Skill_Granade& Prototype);
	virtual ~CHands_Skill_Granade() = default;
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
	void Set_BombRange(_float fBombRange);

private:

	
	class CModel* m_pModelCom = { nullptr };
	class CShader* m_pShaderCom = { nullptr };
	class CNavigation* m_pNavigationCom = {nullptr};
	class CCollider* m_pColliderCom = { nullptr };
	int m_iNowMapStage = {};

	_bool m_isAnimEnd = { false };
	_bool m_isChangingMotion = { false };


	_float3	m_BulletSrc;
	_float3 m_BulletDir;
	_float m_fDistance;
	_bool m_isActive;
	
	_float m_fGravity   = { 9.8f };
	_float m_fCurHeight = { 100.f };
	_float m_v0			= { 1.f };
	

	_float m_fAccTimeDelta = 0.f;

	class CHands_Skill_Granade_Bomb* m_pBomb = { nullptr };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Particle();
	HRESULT Ready_Bullet();
public:
	static   CHands_Skill_Granade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
