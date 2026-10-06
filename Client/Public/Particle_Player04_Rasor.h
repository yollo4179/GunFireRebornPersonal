#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Particle_Point;
class CTexture;
END
BEGIN(Client)
class CParticle_Player04_Rasor :public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{

	}PARTICLE_PLAYER02_DESC;

private:
	explicit CParticle_Player04_Rasor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CParticle_Player04_Rasor(const CParticle_Player04_Rasor& Prototype);
	virtual ~CParticle_Player04_Rasor() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void Set_RayLength(_float fRayLength) { m_fRayDist = fRayLength; };

public:
	void Set_Pivot(_float3 fPivot);
	void Set_Position(_fvector matrix);
	void Reset_Vertices(_float fTimeDelta);
	void Set_Matrix(_fmatrix fPosMatrix);
	void Set_ParticleMatrix(_fmatrix ParticleMatrix);
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Particle_Point* m_pVIBufferCom = { nullptr };

	_float m_fRayDist = 0;
	_float4x4 m_ParticleMatrix = {};
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static   CParticle_Player04_Rasor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
