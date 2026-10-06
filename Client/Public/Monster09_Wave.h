#pragma once
#include"Client_Defines.h"
#include"GameObject.h"
BEGIN(Engine)
class CCollider;
class CShader;
class CVIBuffer_Flat;
class CTexture;
END
BEGIN(Client)
class CMonster09_Wave final :public CGameObject
{
public:
	enum WAVE_DIRECTION
	{
		WAVE_RIGHT,
		WAVE_LEFT,
	};
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		float fSpeed;

	}MONSTER09_SHOT_DESC;
private:
	CMonster09_Wave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster09_Wave(const CMonster09_Wave& Prototype);
	virtual ~CMonster09_Wave() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_Active(_bool isActive, WAVE_DIRECTION eDir) { m_isActive = isActive;    m_eDirection = eDir; };
	_bool Get_Active() { return m_isActive; };
	void Reset_Bullets(_fmatrix matInitialShootingPos);

private:
	vector<class CCollider*> m_pColliderComs;

	_float3	m_BulletSrc;
	_float3 m_BulletDir;
	_float m_fDistance;

	_float m_fSpeed = 10.f;

	const _float m_fLifeTimeLimit = 4.f;
	_float m_fAccNowLifeTime = 0.f;

	_bool m_isActive = false;
	WAVE_DIRECTION m_eDirection = {};


	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_Noise = { nullptr };
	CTexture* m_pTextureCom_Water = { nullptr };
	CTexture* m_pTextureCom_Diffuse = { nullptr };

	CTexture* m_pTextureCom_Event_Noise = { nullptr };
	_bool m_bNoiseEvent_FinalTarget = { false };
	_float m_fAccEventNoiseTime = { 0.f };
	_float m_fEventNoiseTime = { 5.f };

	CTexture* m_pTextureCom_Glow_Noise = { nullptr };
	CVIBuffer_Flat* m_pVIBufferCom = { nullptr };
	_float m_fTime ={ 0.f};
	_float m_fNumWaves = { 50.f };
	_float2 m_vNumTerrainFlat_Vertices;
	/*Sound*/
	_int m_iStopWaveSoundID= {-1};

	_int m_iEventNoiseSoundID = { -1 };
	_bool m_bStartEventNoise_Sound = { true };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();


public:
	static CMonster09_Wave* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
