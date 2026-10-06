#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Particle_Point;
class CTexture;
END
BEGIN(Client)
class CParticle_Monster09_Mini final : public CGameObject
{
public:
	typedef struct :public CGameObject :: GAMEOBJECT_DESC
	{

	}PARTICLE_MONSTER09_MINI_DESC;

	explicit CParticle_Monster09_Mini(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CParticle_Monster09_Mini(const CParticle_Monster09_Mini& Prototype);
	virtual ~CParticle_Monster09_Mini() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Position(_fvector vPos);
	void Reset_Vertices(_float fTimeDelta);
	void Set_Matrix(_fmatrix PosMatrix);

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Particle_Point* m_pVIBufferCom = { nullptr };

	_float m_fAccTimeDelta = { 0.f };

	_bool m_bActive = { false };
	_float m_fAccParticleTime = { 0.f };
	_float m_fParticleTIme = 1.f;


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static   CParticle_Monster09_Mini* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};
END
