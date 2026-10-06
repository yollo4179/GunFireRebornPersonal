#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"


enum M4_ANIM
{
	/*Battle_Range + R*/
	M4_ANIM_BATTLE_IDLE,		//0
	M4_ANIM_BATTLE_WALK_BACK,//3		
	M4_ANIM_BATTLE_SLASH,    //15	
	/*Rush_Slash_Range*/
	M4_ANIM_RUSH_SLASH,		//14
	/*Middle_Range +R*/
	M4_ANIM_MIDDLE_RUN,		//8
	/*Far Range*/
	M4_ANIM_FAR_WALK_LEFT,	//4
	M4_ANIM_FAR_WALK_RIGHT,	//2
	M4_ANIM_WALK,			//5
	M4_ANIM_FAR_IDLE,		//6
	/*Roll*/
	M4_ANIM_ROLL_LEFT,		//12
	M4_ANIM_ROLL_RIGHT,		//11

	/*Hit*/
	M4_ANIM_HIT,			//7
	M4_ANIM_END
};

enum M4_STATE
{
	M4_STATE_BATTLE_IDLE = 0x00000001,
	M4_STATE_BATTLE_WALK_BACK = 0x00000002,
	M4_STATE_BATTLE_SLASH = 0x00000004,

	M4_STATE_RUSH_SLASH = 0x00000008,

	M4_STATE_MIDDLE_RUN = 0x00000010,

	M4_STATE_FAR_WALK_LEFT = 0x00000020,
	M4_STATE_FAR_WALK_RIGHT = 0x00000040,
	M4_STATE_FAR_WALK = 0x00000080,
	M4_STATE_FAR_IDLE = 0x00000100,

	M4_STATE_ROLL_LEFT = 0x00000200,
	M4_STATE_ROLL_RIGHT = 0x00000400,

	M4_STATE_HIT = 0x00000800,
	M4_STATE_LOOP = 0X00001000,
};
/*CONDITION_LOOP*/
#define CONDITION_BATTLE_IDLE (M4_STATE::M4_STATE_BATTLE_IDLE & m_iState)
#define CONDITION_BATTLE_WALK_BACK (M4_STATE::M4_STATE_BATTLE_WALK_BACK & m_iState)
#define CONDITION_BATTLE_LOOP (CONDITION_BATTLE_IDLE || CONDITION_BATTLE_WALK_BACK)

#define CONDITION_MIDDLE_RUN (M4_STATE::M4_STATE_MIDDLE_RUN &m_iState)

#define CONDITION_FAR_WALK_LEFT (M4_STATE::M4_STATE_FAR_WALK_LEFT & m_iState)
#define CONDITION_FAR_WALK_RIGHT (M4_STATE::M4_STATE_FAR_WALK_RIGHT& m_iState)
#define CONDITION_FAR_WALK (M4_STATE::M4_STATE_FAR_WALK &m_iState)
#define CONDITION_FAR_IDLE (M4_STATE::M4_STATE_FAR_IDLE & m_iState)
#define CONDITION_FAR_LOOP (CONDITION_FAR_WALK_LEFT||CONDITION_FAR_WALK_RIGHT||CONDITION_FAR_WALK||CONDITION_FAR_IDLE)

#define CONDITION_STATE_LOOP (M4_STATE::M4_STATE_LOOP& m_iState)
#define CONDITION_LOOP (CONDITION_BATTLE_LOOP||CONDITION_MIDDLE_RUN||CONDITION_FAR_LOOP||CONDITION_STATE_LOOP)

/*CONDITION_ NON_LOOP*/

#define CONDITION_BATTLE_SLASH (M4_STATE::M4_STATE_BATTLE_SLASH &m_iState)
#define CONDITION_RUSH_SLASH (M4_STATE::M4_STATE_RUSH_SLASH & m_iState)
#define CONDITION_ATTACK (CONDITION_BATTLE_SLASH||CONDITION_RUSH_SLASH)
#define CONDITION_ROLL_LEFT (M4_STATE::M4_STATE_ROLL_LEFT&m_iState)
#define CONDITION_ROLL_RIGHT (M4_STATE::M4_STATE_ROLL_RIGHT& m_iState)
#define CONDITION_ROLL (CONDITION_ROLL_LEFT||CONDITION_ROLL_RIGHT)
#define CONDITION_HIT (M4_STATE::M4_STATE_HIT& m_iState)

#define CONDITION_NON_LOOP (CONDITION_ATTACK||CONDITION_ROLL||CONDITION_HIT)

#define CONDITION_ATTACK_SWORD_M4  ((M4_STATE::M4_STATE_BATTLE_SLASH & *m_pState)||(M4_STATE::M4_STATE_RUSH_SLASH & *m_pState))
BEGIN(Engine)
class CCollider;
END
BEGIN(Client)
class CMonster04 final : public CMonster
{
public:
	enum PARTOBJECT
	{
		PART_HEAD,
		PART_BODY,
		PART_SWORD,
		PART_END
	};
private:
	explicit CMonster04(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster04(const CMonster04& Prototype);
	virtual ~CMonster04() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;
private:
	HRESULT Mapping_Anim_States();
	void Pri_Update_Cancle_LoopState();
	void Pri_Update_LoopCall_State();
	void Pri_Update_Non_LoopCall_State();
private:
	
private:
	vector<_uint> m_vecAnimStates;
	M4_STATE m_eCurState = M4_STATE_FAR_IDLE;


	/*Ranges*/
	_float m_fFarRange = 40.f;
	_float m_fMiddleRange = 25.f;
	_float m_fRush_SlashRange = 7.f;
	_float m_fBattleRange = 4.f;
	/*accTime*/
	_float m_fAcc_Far_Walk_Left_Time = 0.f;
	_float m_fAcc_Far_Walk_Right_Time = 0.f;
	_float m_fAcc_Far_Walk_Time = 0.f;
	_float m_fAcc_Middle_Run_Time = 0.f;
	_float m_fAcc_Battle_Walk_Back_Time = 0.f;

	_float m_fAcc_Random_WalkTime = 0.f;
	_float m_fAcc_Battle_Idle_Time = 0.f;
	_float m_fAcc_StartRollTime = 0.f;;
	_float m_fAcc_StartRushSlashTime = 0.f;


	_float m_fAcc_Slash_Time=0.f;
	_float m_fAcc_Rush_Slash_Time = 0.f;

	_float m_fAccRushSlash_Timing = 0.f;
	_bool m_bSound_RushSlash = true;


	/*LoopCall*/
	bool m_bCall_Far_Idle = true;
	bool m_bCall_Far_Walk = true;
	bool m_bCall_Far_Walk_Left = true;
	bool m_bCall_Far_Walk_Right = true;
	bool m_bCall_Middle_Run = true;
	bool m_bCall_Battle_Walk_Back = true;
	bool m_bCall_Battle_Idle = true;

	/*Non loop Call*/
	bool m_bCall_Roll_Right = true;
	bool m_bCall_Roll_Left = true;
	bool m_bCall_Hit = true;



	_uint m_iState = 0;
	/*Demage*/
	_bool m_bHeadShot = { false };

	

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_PartObjects();

public:
	static CMonster04* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END