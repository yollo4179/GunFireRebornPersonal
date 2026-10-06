#pragma once
#include"Monster.h"
#include"Client_Defines.h"
enum class M9_MINI_ANIM
{
	M9_MINI_ANIM_IDLE,//0
	M9_MINI_ANIM_DEAD1,//2
	M9_MINI_ANIM_DEAD2,//4
	M9_MINI_ANIM_SHOT, //3
	M9_MINI_ANIM_POPOUT,//1
	M9_MINI_ANIM_END
};
enum M9_MINI_STATE
{
	//IDLE 
	M9_MINI_STATE_IDLE		=0x00000001,
	//POPOUT 
	M9_MINI_STATE_POPOUT	=0x00000002,
	//DEAD ,
	M9_NIMI_STATE_DEAD		=0x00000004,
	//ATTACK
	M9_MINI_STATE_ATTACK_SHOT		=0x00000008,
	M9_MINI_STATE_LOOP				=0x00000010,
};

/* Non Loop*/
#define CONDITION_POPOUT (m_iState&M9_MINI_STATE::M9_MINI_STATE_POPOUT )
#define CONDITION_ATTACK_SHOT (m_iState&M9_MINI_STATE::M9_MINI_STATE_ATTACK_SHOT)
#define CONDITION_DEAD (m_iState&M9_MINI_STATE::M9_NIMI_STATE_DEAD)
#define CONDITION_NON_LOOP (CONDITION_POPOUT|| CONDITION_ATTACK_SHOT||CONDITION_DEAD)
/*Loop */
#define CONDITION_IDLE  (m_iState&M9_MINI_STATE::M9_MINI_STATE_IDLE)
#define CONDITION_LOOP_STATE (m_iState& M9_MINI_STATE::M9_MINI_STATE_LOOP)
#define CONDITION_LOOP (CONDITION_IDLE||CONDITION_LOOP_STATE)
BEGIN(Engine)
class CCollider;
END

BEGIN(Client)
class CMonster09_Mini final :public CMonster
{
private:
	CMonster09_Mini(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster09_Mini(const CMonster09_Mini& Prototype);
	virtual ~CMonster09_Mini() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Mapping_Anim_States();
	void Pri_Update_Cancle_LoopState();
	void Pri_Update_LoopCall_State();
	void Pri_Update_Non_LoopCall_State();

public:
	void Reset_Position(_fmatrix Mat);
public:
	void Set_Active(_bool isActive); 
	_bool Get_Active() { return  m_bActive; };
	

private:
	_uint m_iState = 0;
	vector< _uint> m_vecAnimStates;

	_bool m_isActive = { false };
	
	/*SocketMatrix For Attack*/
	/* 필요한것 " 콜라이더  모델, */


	/*m_fAccTime */
	
	/*Shot Time*/
	_float m_fAcc_Attack_Shot_Time = 0.f;
	_float m_fAcc_Attack_Shot_Timing = 0.f;
	_float m_fAttack_Shot_Cool_Time =5.f;


	/*m_bCall*/
	_bool m_bCall_Dead = { true };
	_bool m_bCall_PopOut = { true };
	_bool m_bCall_Attack_Shot = { true };
	_bool m_bCall_Idle = { true };

private:
	CCollider* m_pColliderCom = { nullptr};
	/*For Attack SokcetMatrix*/
	
	const _float4x4* m_pSocketMatrix_For_Attack = { nullptr };
	_float4x4 m_WorldMatrix_For_Attack = {};
	vector<class CMonster09_Mini_Shot*>m_pMiniShots;
	_uint m_iNowMiniShotIndex = 0;
	const int m_iNumMiniShots = 10;
	_bool m_bShotSign = { true };
	_bool m_bShotSignEnd = { false };

	queue<_uint>Skill_Sign_Queue;
	
	class CParticle_Monster09_Mini* m_pParticle_Mini = { nullptr };
	_float m_fAccParticleTime = 0.f;
	_float m_fParticleTime = { 3.f };
	_bool m_isParticle_Working = { false };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_MiniShots();
	HRESULT Ready_Particle();
	

public:
	static CMonster09_Mini* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;



};
END
