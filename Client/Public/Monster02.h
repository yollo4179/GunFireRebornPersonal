#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"
enum class M2_ANIM
{
	/*BATTLE RANGE */
	M2_ANIM_BATTLE_IDLE,				//0
	M2_ANIM_BATTLE_WALK_BACK,				//3
	M2_ANIM_BATTLE_DOUBLE_SLASH,		//18  
	M2_ANIM_BATTLE_SLASH,				//15

	/*RUSH_SLASH_RANGE */
	M2_ANIM_RUSH_SLASH,					//14
	/*Middle Range*/
	M2_ANIM_MIDDLE_RUSH,				//8


	/*FAR RANGE*/
	M2_ANIM_FAR_WALK_RIGHT,     //2
	M2_ANIM_FAR_WALK_LEFT,     //4
	M2_ANIM_FAR_WALK,  //5
	M2_ANIM_FAR_IDLE,   //6

	
	/*Roll_*/
	M2_ANIM_ROLL_RIGHT,		//11
	M2_ANIM_ROLL_LEFT,			//12
	/*Hit*/
	M2_ANIM_HIT,             //7
	
	M2_ANIM_END,
};
enum M2_STATE
{
	/*Battle_Range*/
	M2_STATE_BATTLE_IDLE			=0X00000001,
	M2_STATE_BATTLE_WALK_BACK		=0X00000002,
	M2_STATE_BATTLE_DOUBLE_SLASH	=0X00000004,
	M2_STATE_BATTLE_SLASH			=0X00000008,
	/*Rush_Slash_Range*/
	M2_STATE_RUSH_SLASH				=0X00000010,
	/*Middle_Range*/
	M2_STATE_MIDDLE_RUSH			=0X00000020,
	/*FAR _RANGE*/
	M2_STATE_FAR_WALK_RIGHT			=0X00000040, 
	M2_STATE_FAR_WALK_LEFT			=0X00000080,     
	M2_STATE_FAR_WALK				=0X00000100,  
	M2_STATE_FAR_IDLE				=0X00000200,
	
	/*HIT*/
	M2_STATE_HIT					=0X00000400,
	/*Roll*/
	M2_STATE_ROLL_RIGHT				=0X00000800,
	M2_STATE_ROLL_LEFT				=0X00001000,
	
	M2_STATE_LOOP					=0x00002000
};


/*Condition Idle lOOP*/
#define CONDITION_BATTLE_IDLE  ( (_uint)M2_STATE::M2_STATE_BATTLE_IDLE & m_iState)    
#define CONDITION_FAR_IDLE  ((_uint)M2_STATE::M2_STATE_FAR_IDLE & m_iState)
#define CONDITION_IDLE			  CONDITION_BATTLE_IDLE|| CONDITION_FAR_IDLE
/*Condition  Transition loop*/
#define CONDITION_WALK_LEFT		((_uint)M2_STATE::M2_STATE_FAR_WALK_LEFT & m_iState )
#define CONDITION_WALK_RIGHT	((_uint)M2_STATE::M2_STATE_FAR_WALK_RIGHT & m_iState )
#define CONDITION_WALK_BACK		((_uint)M2_STATE::M2_STATE_BATTLE_WALK_BACK & m_iState )
#define CONDITION_WALK			((_uint)M2_STATE::M2_STATE_FAR_WALK & m_iState )
#define CONDITION_WALK_STATE     (CONDITION_WALK_LEFT||CONDITION_WALK_RIGHT|| CONDITION_WALK_BACK||CONDITION_WALK)

#define CONDITION_RUN			((_uint)M2_STATE::M2_STATE_MIDDLE_RUSH &m_iState )
#define CONDITION_TRANSITION    CONDITION_WALK_STATE||CONDITION_RUN
/* For Loop */
#define CONDITION_STATE_LOOP    ((_uint)M2_STATE::M2_STATE_LOOP& m_iState)
#define CONDITION_LOOP         (CONDITION_IDLE || CONDITION_TRANSITION)||CONDITION_STATE_LOOP
/*For Non Loop*/
#define CONDITION_ROLL_RIGHT	((_uint)M2_STATE::M2_STATE_ROLL_RIGHT & m_iState) 
#define CONDITION_ROLL_LEFT		((_uint)M2_STATE::M2_STATE_ROLL_LEFT & m_iState )
#define CONDITION_ROLL			(CONDITION_ROLL_RIGHT||CONDITION_ROLL_LEFT)
#define CONDITION_HIT			((_uint)M2_STATE::M2_STATE_HIT & m_iState)
/* For Attack */
#define CONDITION_DOUBLE_SLASH		((_uint)M2_STATE::M2_STATE_BATTLE_DOUBLE_SLASH & m_iState)
#define CONDITION_SLASH				((_uint)M2_STATE::M2_STATE_BATTLE_SLASH & m_iState)
#define CONDITION_RUSH_SLASH		((_uint)M2_STATE::M2_STATE_RUSH_SLASH & m_iState)
#define CONDITION_ATTACK		(CONDITION_DOUBLE_SLASH||CONDITION_SLASH||CONDITION_RUSH_SLASH)
/*For Non Loop*/
#define CONDITION_NON_LOOP  ((CONDITION_ROLL)||(CONDITION_HIT)||(CONDITION_ATTACK))

#define CONDITION_ATTACK_SWORD  (((_uint)M2_STATE::M2_STATE_BATTLE_DOUBLE_SLASH & *m_pState)||((_uint)M2_STATE::M2_STATE_BATTLE_SLASH & *m_pState)||((_uint)M2_STATE::M2_STATE_RUSH_SLASH & *m_pState))

BEGIN(Client)
class CMonster02 final : public CMonster
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
	explicit CMonster02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster02(const CMonster02& Prototype);
	virtual ~CMonster02() = default;


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
	vector< _uint> m_vecAnimStates;
	M2_STATE m_eCurState= M2_STATE::M2_STATE_FAR_IDLE;
	

private:
	
	/*Ranges*/
	_float m_fFarRange = { 40.f };
	_float m_fMiddleRange = { 25.f };
	_float m_fRush_SlashRange =	{ 7.f };
	_float m_fBattleRange = { 4.f };
	/*AccTimes*/
	

	_float m_fAcc_Far_Walk_Left_Time = { 0.f };
	_float m_fAcc_Far_Walk_Right_Time = { 0.f };
	_float m_fAcc_Far_Walk_Time = { 0.f };
	_float m_fAcc_Middle_Run_Time = { 0.f };
	_float m_fAcc_Battle_Walk_Back_Time = { 0.f };


	_float m_fAcc_StartRollTime = { 0.f };
	_float m_fAcc_StartRushSlashTime = { 0.f };
	_float m_fAcc_Random_WalkTime = { 0.f };
	_float m_fAcc_Battle_Idle_Time = { 0.f };

	
	_float m_fAcc_DoubleSlash_Time = { 0.f };
	_float m_fAcc_Slash_Time = { 0.f };
	_float m_fAcc_Rush_Slash_Time = { 0.f };

	/*Loop Call*/
	_bool m_bCall_Battle_Idle = true;
	_bool m_bCall_Far_Idle = true;
	_bool m_bCall_Walk_Left = true;
	_bool m_bCall_Walk_Right = true;
	_bool m_bCall_Walk_Slow = true;
	_bool m_bCall_Rush = true;
	_bool m_bCall_Walk_Back = true;

	/*non loop call*/
	_bool m_bCall_Roll_Left = true;
	_bool m_bCall_Roll_Right = true;
	_bool m_bCall_Hit = true;

	_uint m_iState = 0;

	_bool m_bHeadShot = false;

	/*Sound*/
	_float m_fAccRushSlashTiming = { 0.f };
	_bool m_bRushSlashSound = { true };

	_float m_fAccDoubleSlashTiming = { 0.f };
	_bool m_bDoubleSlashSound = { true };
	

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_PartObjects();
public:
	static CMonster02* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END