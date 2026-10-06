#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"
BEGIN(Engine)
class CShader;
class CModel;
END
enum M1_ANIM
{
	M1_ANIM_BATTLE_IDLE,		//0
	M1_ANIM_BATTLE_ATTACK1,		//7
	M1_ANIM_BATTLE_ATTACK2,		//8

	M1_ANIM_FAR_WALK,			//1

	M1_ANIM_MIDDLE_RUN,			//2

	M1_ANIM_HIT,					//4
	M1_ANIM_BREATHING,			//9	
	M1_ANIM_END
};
enum M1_STATE
{
	M1_STATE_BATTLE_IDLE		=0X00000001,
	M1_STATE_BATTLE_ATTACK1		=0X00000002,
	M1_STATE_BATTLE_ATTACK2		=0x00000004,

	M1_STATE_FAR_WALK			=0x00000008,

	M1_STATE_MIDDLE_RUN			=0x00000010,

	M1_STATE_HIT				=0x00000020,
	M1_STATE_BREATHING			=0x00000040,
	M1_STATE_LOOP				=0x00000080,

};

/*lOOP*/
#define CONDITION_BATTLE_IDLE (M1_STATE::M1_STATE_BATTLE_IDLE&m_iState)

#define CONDITION_FAR_WALK  (M1_STATE::M1_STATE_FAR_WALK&m_iState)

#define CONDITION_MIDDLE_RUN (M1_STATE::M1_STATE_MIDDLE_RUN&m_iState)

#define CONDITION_STATE_LOOP (M1_STATE::M1_STATE_LOOP&m_iState)

#define CONDITION_LOOP (CONDITION_BATTLE_IDLE||CONDITION_FAR_WALK||CONDITION_MIDDLE_RUN||CONDITION_STATE_LOOP)

/*Non Loop*/
#define CONDITION_BATTLE_ATTACK1 (M1_STATE::M1_STATE_BATTLE_ATTACK1&m_iState)
#define CONDITION_BATTLE_ATTACK2 (M1_STATE::M1_STATE_BATTLE_ATTACK2&m_iState)
#define CONDITION_HIT (M1_STATE::M1_STATE_HIT&m_iState)
#define CONDITION_BREATHING (M1_STATE::M1_STATE_BREATHING&m_iState)

#define CONDITION_NON_LOOP (CONDITION_BATTLE_ATTACK1||CONDITION_BATTLE_ATTACK2||CONDITION_HIT||CONDITION_BREATHING)

BEGIN(Client)
class CMonster01 final : public CMonster
{
public:
	enum PARTOBJECT
	{
		PART_HEAD,
		PART_BODY,
		PART_END
	};
private:
	explicit CMonster01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster01(const CMonster01& Prototype);
	virtual ~CMonster01() = default;


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

private:
	vector<_uint> m_vecAnimStates;
	M1_STATE m_eCurStae = M1_STATE_BATTLE_IDLE;

	/*Ranges*/
	_float m_fFarRange = 40.f;
	_float m_fMiddleRange = 25.f;
	_float m_fBattleRange = 5.f;
	/*accTime*/
	/*moving time*/
	
	_float m_fAcc_Far_Walk_Time = 0.f;
	_float m_fAcc_Middle_Run_Time = 0.f;
	

	_float m_fAcc_Random_WalkTime = 0.f;
	_float m_fAcc_Battle_Idle_Time = 0.f;

	_float m_fAcc_Attack1_Time = 0.f;
	_float m_fAcc_Attack2_Time = 0.f;





	/*BulletCount*/
	_uint m_iNumBullets = 5.f;



	/*LoopCall*/
	bool m_bCall_Far_Idle = true;
	bool m_bCall_Far_Walk = true;
	
	bool m_bCall_Battle_Idle = true;
	bool m_bCall_Middle_Run = true;
	bool m_bCall_State_loop = true;
	/*NonLoopCall*/
	
	bool m_bCall_Hit = true;
	_uint m_iState = 0;


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_PartObjects();

public:
	static CMonster01* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END