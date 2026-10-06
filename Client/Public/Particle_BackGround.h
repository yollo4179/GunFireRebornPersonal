#pragma once
#include"GameObject.h"
#include"Client_Defines.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Particle_Rect;
class CTexture;
END

BEGIN(Client)
class CParticle_BackGround final :public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
	}PARTICLE_BACKGROUND_DESC;
private:
	explicit CParticle_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CParticle_BackGround(const CParticle_BackGround& Prototype);
	virtual ~CParticle_BackGround() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Particle_Rect* m_pVIBufferCom = { nullptr };

	_uint* m_pNowEquipedArmIndex = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static   CParticle_BackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
