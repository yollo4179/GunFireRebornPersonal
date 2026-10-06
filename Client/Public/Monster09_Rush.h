#pragma once
#include"Client_Defines.h"
#include"GameObject.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Flat;
class CTexture;
END

BEGIN(Client)
class CMonster09_Rush final :public CGameObject
{
public:
	typedef struct :public CGameObject :: GAMEOBJECT_DESC
	{

	}MONSTER09_RUSH_DESC;

private:
	CMonster09_Rush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster09_Rush(const CMonster09_Rush& Prototype);
	virtual ~CMonster09_Rush() = default;

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
	
	_float m_fSpeed = 10.f;

	const _float m_fLifeTimeLimit = 10.f;
	_float m_fAccNowLifeTime = 0.f;

	_bool m_isActive = false;
	


	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_Noise = { nullptr };
	CTexture* m_pTextureCom_Water = { nullptr };
	CTexture* m_pTextureCom_Diffuse = { nullptr };

	CTexture* m_pTextureCom_Glow_Noise = { nullptr };
	CVIBuffer_Flat* m_pVIBufferCom = { nullptr };
	_float m_fTime = { 0.f };
	_float m_fNumWaves = { 30.f };
	_float2 m_vNumTerrainFlat_Vertices;

	/*Sound*/
	_int m_iSoundID = { -1 };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();

public:
	static CMonster09_Rush* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END

