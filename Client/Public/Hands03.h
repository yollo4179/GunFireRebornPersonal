#pragma once
#include"Client_Defines.h"
#include "Hands.h"


const int g_iNumParticle_Player03_Hit = 10;
BEGIN(Engine)
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


	enum class HANDS03_STATE
	{
		STATE_IDLE,
		STATE_ATTACK_PRESSING,
		STATE_ATTACK_DOWN,
		STATE_LOAD,
		STATE_POP_OUT,
		STATE_END,
	};
	BEGIN(Client)
class CHands03 final :public CHands
{
	public:
		typedef struct : CPartObject::PARTOBJECT_DESC
		{
			_uint* pState;
		}HANDS03_DESC;

	private:
		explicit CHands03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		explicit CHands03(const CHands03& Prototype);
		virtual ~CHands03() = default;
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

		/*Ray Collider*/
		class CCollider* m_pColliderCom = { nullptr };
		

		/*Shooting Particle && Texture */
		_float4x4* m_pSocketMatrix_Muzzle = { nullptr };
		
		/*Particle*/
		class CParticle_Player03_Hit* m_pParticle_Hit[g_iNumParticle_Player03_Hit] = {};
		_float m_fAccParticleTime[g_iNumParticle_Player03_Hit]={};
		_float m_fParticelTime = 0.5f;
		_int m_iNowParticleIndex = 0;
		/*ForGlow*/
		CShader* m_pShaderCom_Glow = { nullptr };
		CTexture* m_pTextureCom_Glow = { nullptr };

	private:
		HRESULT Ready_Components();
		HRESULT Bind_ShaderResources();
		HRESULT Bind_ShaderResources_Glow();
		HRESULT Ready_Particle();
	public:
		static CHands03* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		virtual CGameObject* Clone(void* pArg);
		virtual void Free() override;
};
END;
