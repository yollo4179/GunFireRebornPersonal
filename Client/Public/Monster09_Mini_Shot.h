#pragma once
#include"Client_Defines.h"
#include"GameObject.h"
BEGIN(Engine)
class CCollider;
class CModel; 
class CShader;
class CTexture;
END

BEGIN(Client)
class CMonster09_Mini_Shot final: public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		_float fSpeed;
	}MONSTER09_MINI_SHOT_DESC;

private:
	CMonster09_Mini_Shot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster09_Mini_Shot(const CMonster09_Mini_Shot& Prototype);
	virtual ~CMonster09_Mini_Shot() = default;



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

private:
	class CCollider* m_pColliderCom = { nullptr };

	CModel* m_pModelCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_Noise = { nullptr };


	_float3	m_BulletSrc;
	_float3 m_BulletDir;
	_float m_fDistance;

	_float m_fSpeed;

	const _float m_fLifeTimeLimit = 10.f;
	_float m_fAccNowLifeTime = 0.f;

	_bool m_isActive = false;

	/*For ShotMoving*/
	_float m_fGravity = { 9.8f };
	_float m_fCurHeight = { 0.f };
	_float m_v0 = { 5.f };



private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_SHaderResources_Glow();


public:
	static CMonster09_Mini_Shot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
