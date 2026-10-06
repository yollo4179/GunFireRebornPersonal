#pragma once
#include"Client_Defines.h"
#include"AlphaObject.h"
BEGIN(Engine)
class CCollider;
class CModel;
class CShader;
class CTexture;
END
BEGIN(Client)
class CMonster09_Shot final :public CAlphaObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		float fSpeed;

	}MONSTER09_SHOT_DESC;
private:
	CMonster09_Shot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster09_Shot(const CMonster09_Shot& Prototype);
	virtual ~CMonster09_Shot() = default;

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

private:
	class CCollider* m_pColliderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };
	class CShader* m_pShaderCom = { nullptr };

	_float3	m_BulletSrc;
	_float3 m_BulletDir;
	_float m_fDistance;

	_float m_fSpeed;

	const _float m_fLifeTimeLimit = 10.f;
	_float m_fAccNowLifeTime = 0.f;

	_bool m_isActive = false;

	CTexture* m_pTextureCom_Noise = { nullptr };

	class CParticle_Monster09_Shot* m_pParticle_Shot_Down = { nullptr };
	_float m_fAccParticle_Shot_DownTime = 0.f;
	_float m_fParticle_Shot_DownTime = 3.f;
	_bool m_bParticle_ShotDown = { false };

	class CEffect_Monster09_ShotDown* m_pEffect_ShotDown = { nullptr };
	_float m_fAccEffect_ShotDownTime = { 0.f };
	_float m_fEffect_ShotDownTime = { 0.15f };
	_bool m_bEffectShotDown = { false };

	class CMonster09_WarningSign* m_pWarningSign = {};

private:
	HRESULT Ready_Components();
	HRESULT Readt_Particles();
	HRESULT Ready_WarningSign();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_SHaderResources_Glow();


public:
	static CMonster09_Shot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

