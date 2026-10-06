#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Particle_Point;
class CTexture;
END
BEGIN(Client)
class CParticle_Player05_Bomb :public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{

	}PARTICLE_PLAYER05_BULLET_BOMB_DESC;

private:
	explicit CParticle_Player05_Bomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CParticle_Player05_Bomb(const CParticle_Player05_Bomb& Prototype);
	virtual ~CParticle_Player05_Bomb() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void Set_Pivot(_float3 fPivot);
	void Set_Position(_fvector matrix);
	void Reset_Vertices(_float fTimeDelta);
	void Set_Matrix(_fmatrix fPosMatrix);
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Particle_Point* m_pVIBufferCom = { nullptr };


	_int m_iTextureIndex = { 0 };
	_float m_fAccTimeDelta = { 0.f };

	_bool m_bActive = {false};
	_float m_fAccParticleTime = { 0.f };
	_float m_fParticleTIme = 1.f;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static   CParticle_Player05_Bomb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};
END
