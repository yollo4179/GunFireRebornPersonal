#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Particle_Point;
class CTexture;
END
BEGIN(Client)
class CParticle_Player05_Bullet_Chasing :public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{

	}PARTICLE_PLAYER05_BULLET_CHASING_DESC;
private:
	explicit CParticle_Player05_Bullet_Chasing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CParticle_Player05_Bullet_Chasing(const CParticle_Player05_Bullet_Chasing& Prototype);
	virtual ~CParticle_Player05_Bullet_Chasing() = default;

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

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static   CParticle_Player05_Bullet_Chasing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};
END

