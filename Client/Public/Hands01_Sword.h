#pragma once
#include"Client_Defines.h"
#include"PartObject.h"

const int g_iNumParticle_Player01_Hit = 10;
BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END
BEGIN(Client)
class CHands01_Sword final :public CPartObject
{
public:
	typedef struct : CPartObject::PARTOBJECT_DESC
	{
		
		_uint* pState;
		const _float4x4* pSocketMatrix[5];  
		//소켓 매트릭스는 무기의 본메트릭스이다-> 이걸로 무기를 로컬의 모델에 배치후 , 모델의 월드를 곱해 찐 으로 월드에 배치한다  
	}HANDS01_SWORD_DESC;


private:
	CHands01_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHands01_Sword(const CHands01_Sword& Prototype);
	virtual ~CHands01_Sword() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void Set_Active(bool isValid) { m_isValid = isValid;}

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom[5] = {nullptr,nullptr,nullptr,nullptr,nullptr};

private:
	_uint* m_pState = { nullptr };
	const _float4x4* m_pSocketMatrix[5] = {nullptr,nullptr ,nullptr,nullptr,nullptr };
	/*Particle-on*/
	class CParticle_Player01_On* m_pParticles_On[20] = {nullptr};
	/*Particle-Hit*/
	class CParticle_Player01_Hit* m_pParticle_Hit[g_iNumParticle_Player01_Hit] = {nullptr};
	_float m_fAccParticleTime[g_iNumParticle_Player01_Hit] = {};
	const _float4x4* m_matFloat4x4OfTargetPart[g_iNumParticle_Player01_Hit] = {nullptr};
	_float m_fParticelTime = 0.5f;
	_int m_iNowParticleIndex = 0;

	_bool m_isAttackChance = { false };
	_float m_fAccNowAttackTime = { 0.f };
	_float m_fNowAttackTime = { 0.6f };
	_bool m_isHandsAnimEnd = { false };

private:
	bool m_isValid = { false };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Particles();

public:
	static CHands01_Sword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END