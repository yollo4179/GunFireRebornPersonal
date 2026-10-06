#pragma once
#include"Client_Defines.h"
#include "Hands.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
class CTexture;
END
#define CONDITION_IDLE (CPlayer::STATE_IDLE & *m_pState)
#define CONDITION_LOOP (CPlayer::STATE_IDLE  & *m_pState)
#define CONDITION_LOAD (CPlayer::STATE_LOAD & *m_pState)
#define CONDITION_ATTACK_DOWN (CPlayer::STATE_ATTACK_DOWN & *m_pState)
#define CONDITION_ATTACK_PRESSING (CPlayer::STATE_ATTACK_PRESSING & *m_pState)
#define CONDITION_ATTACK  (CONDITION_ATTACK_DOWN||CONDITION_ATTACK_PRESSING) 
#define CONDITION_ATTACK_UP (CPlayer::STATE_ATTACK_UP & *m_pState)
#define CONDITION_POP_OUT (CPlayer::STATE_POP_OUT& *m_pState)

enum class HANDS04_STATE
{
	STATE_IDLE,
	STATE_ATTACK_PRESSING,
	STATE_ATTACK_DOWN,
	STATE_LOAD,
	STATE_POP_OUT,
	STATE_END,
};
BEGIN(Client)
class CHands04 final :public CHands
{

public:
	typedef struct : CPartObject::PARTOBJECT_DESC
	{
		_uint* pState;
	}HANDS04_DESC;

private:
	explicit CHands04(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CHands04(const CHands04& Prototype);
	virtual ~CHands04() = default;
public:
	const _float4x4* Get_BoneMatrix(const _char* pBoneName)const;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	HRESULT			Init_Mapping_Index_To_State();
	HRESULT			Init_AnimSpeed();

	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Glow();

private:

private:
	_uint* m_pState = { nullptr };


	/* for Storing States*/
	vector<_uint> m_vecStates;

	_bool m_isAnimEnd = { false };
	_bool m_bStartSetUp = { false };

	_float m_fAccAttackDownTime = { 1.f };

	_bool m_isCahngingMotion = false;

	_bool m_bCall_Idle = { true };

	/*For Bullet*/
	class CCollider* m_pColliderCom = { nullptr };
	/*Shooting pos ±¸ÇÏ±â  */
	const _float4x4* m_pMuzzle_Socket_Matrix = { nullptr };
	/*Particle*/
	_float4x4 m_pBulletTransform = {};
	class CParticle_Player04_Rasor* m_pParticle_Ray = {};
	_float m_fAccRayLength = 0.f;
	_bool m_bShootSign = { false };
	_float m_fRayDist = { 0.f };
	
	class CHands04_Rasor* m_pRasor = { nullptr };
	/*ForGlow*/
	CShader* m_pShaderCom_Glow = { nullptr };
	CTexture* m_pTextureCom_Glow = { nullptr };

	/*ForSound*/
	_float m_fAccAttackSoundTime = { 0.f };
	_uint m_iAttackID = {};
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();

	HRESULT Ready_Particle();
	HRESULT Ready_Rasor();

public:
	static CHands04* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};
END
