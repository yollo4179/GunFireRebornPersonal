#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"

enum M3_ANIM
{
	/*FAR*/
	M3_ANIM_FAR_IDLE, //= 4,
	M3_ANIM_FAR_SNIPE, //= 1,
	M3_ANIM_FAR_WALK_RIGHT,// = 8,
	M3_ANIM_FAR_WALK_LEFT, //= 10,
	M3_ANIM_FAR_WALK, //= 2,
	/* Close*/
	M3_ANIM_CLOSE_IDLE,// = 0,
	M3_ANIM_CLOSE_RUN,// = 3,
	M3_ANIM_CLOSE_MULTISHOT,// = 14,
	M3_ANIM_CLOSE_ROLL_LEFT,// = 12,
	M3_ANIM_CLOSE_ROLL_RIGHT,// = 13,
	M3_ANIM_CLOSE_WALK_BACK,// = 9,
	M3_ANIM_IN_THE_MIDDLE_WALK,// =11,

	M3_ANIM_LOAD,//=15,
	M3_ANIM_HIT,// =17,

	M3_ANIM_END
};


enum M3_STATE
{
	/*Far*/
	M3_STATE_FAR_IDLE				=0X00000001,
	M3_STATE_FAR_SNIPE				=0X00000002,
	M3_STATE_FAR_WALK_RIGHT			=0X00000004,
	M3_STATE_FAR_WALK_LEFT			=0X00000008,
	M3_STATE_FAR_WALK				=0X00000010,
	/*Close*/
	M3_STATE_CLOSE_IDLE				=0x00000020,
	M3_STATE_CLOSE_RUN				=0x00000040,
	M3_STATE_CLOSE_MULTISHOT		=0x00000080,
	M3_STATE_CLOSE_ROLL_LEFT		=0x00000100,
	M3_STATE_CLOSE_ROLL_RIGHT		=0x00000200,
	M3_STATE_CLOSE_WALK_BACK		=0x00000400,
	M3_STATE_IN_THE_MIDDLE_WALK		=0x00000800,

	M3_STATE_LOAD					=0X00001000,
	M3_STATE_HIT					=0X00002000,
	M3_STATE_LOOP					=0x00004000,


};


/*Condition LOOP */

/*LOOP  IDLE*/



/*LOOP FAR*/
#define CONDITION_FAR_IDLE  (M3_STATE::M3_STATE_FAR_IDLE	  &m_iState)
#define CONDITION_FAR_WALK_RIGHT (M3_STATE::M3_STATE_FAR_WALK_RIGHT &m_iState)
#define CONDITION_FAR_WALK_LEFT (M3_STATE::M3_STATE_FAR_WALK_LEFT	  &m_iState)
#define CONDITION_FAR_WALK		(M3_STATE::M3_STATE_FAR_WALK		  &m_iState)
#define CONDITION_FAR_LOOP (CONDITION_FAR_IDLE||CONDITION_FAR_WALK_RIGHT||CONDITION_FAR_WALK_LEFT||CONDITION_FAR_WALK)
/*LOOP CLOSE*/
#define CONDITION_CLOSE_IDLE    (M3_STATE::M3_STATE_CLOSE_IDLE	  &m_iState)
#define CONDITION_CLOSE_RUN     (M3_STATE::M3_STATE_CLOSE_RUN    &m_iState)
#define CONDITION_CLOSE_WALK_BACK (M3_STATE::M3_STATE_CLOSE_WALK_BACK &m_iState)
#define CONDITION_IN_THE_MIDDLE_WALK (M3_STATE::M3_STATE_IN_THE_MIDDLE_WALK &m_iState)
#define CONDITION_CLOSE_LOOP (CONDITION_CLOSE_IDLE||CONDITION_CLOSE_RUN||CONDITION_CLOSE_WALK_BACK||CONDITION_IN_THE_MIDDLE_WALK)

#define CONDITION_STATE_LOOP (M3_STATE::M3_STATE_LOOP & m_iState)
#define CONDITION_LOOP  (CONDITION_CLOSE_LOOP||CONDITION_FAR_LOOP)||CONDITION_STATE_LOOP
/* NON LOOP*/
#define CONDITION_FAR_SNIPE (M3_STATE::M3_STATE_FAR_SNIPE &m_iState)
#define CONDITION_CLOSE_MULTISHOT (M3_STATE::M3_STATE_CLOSE_MULTISHOT  & m_iState)
#define CONDITION_CLOSE_ROLL_LEFT (M3_STATE::M3_STATE_CLOSE_ROLL_LEFT  & m_iState)
#define CONDITION_CLOSE_ROLL_RIGHT (M3_STATE::M3_STATE_CLOSE_ROLL_RIGHT & m_iState)

#define CONDITION_ROLL (CONDITION_CLOSE_ROLL_LEFT||CONDITION_CLOSE_ROLL_RIGHT)
#define CONDITION_LOAD (M3_STATE::M3_STATE_LOAD  & m_iState)
#define CONDITION_HIT  (M3_STATE::M3_STATE_HIT   & m_iState)

#define CONDITION_NON_LOOP   CONDITION_FAR_SNIPE||CONDITION_CLOSE_MULTISHOT||CONDITION_CLOSE_ROLL_LEFT||CONDITION_CLOSE_ROLL_RIGHT||CONDITION_LOAD||CONDITION_HIT
BEGIN(Client)
class CMonster03 final : public CMonster
{
public:
	enum PARTOBJECT
	{
		PART_HEAD,
		PART_BODY,
		PART_GUN,
		PART_END
	};

private:
	explicit CMonster03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster03(const CMonster03& Prototype);
	virtual ~CMonster03() = default;


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
	void PriUpdate_Non_LoopCall_State();

private:
	vector<_uint> m_vecAnimStates;
	M3_STATE m_eCurState = M3_STATE::M3_STATE_FAR_IDLE;

	/*Ranges*/
	_float m_fFarRange = 40.f;
	_float m_fCloseRange = 25.f;
	_float m_fMultiShotRange = 5.f;
	/*AccTime*/
	_float m_fAcc_Far_Walk_Left_Time = 0.F;
	_float m_fAcc_Far_Walk_Right_Time = 0.F;
	_float m_fAcc_Far_Walk_Time = 0.F;
	_float m_fAcc_Close_Run_Time = 0.f;
	_float m_fAcc_Close_Walk_Back_Time = 0.f;
	/*<-------->*/
	_float m_fAcc_Random_WalkTime = 0.f;
	_float m_fAcc_Close_Idle_Time = 0.f;

	_float m_fAcc_WalkBack_CoolTime = 5.f;

	_float m_fAcc_Snipe_Time = 0.f;
	_float m_fAcc_MultiShot_Time = 0.f;
	_float m_fAcc_Multishot_Duration = 5.f;
	_float m_fAcc_Multishot_TrackPosition = 0.f;
	_float m_fAcc_MultiShot_Timing =0.f;

	_float m_fAcc_StartRollTime = 0.f;

	/*Bullet_Count*/
	_uint m_iNumBullets = 3.f;
	_bool m_bMultiShotSign = false;
	_int m_iMultiShot_Count = 0;
	/*LoopCall*/
	bool m_bCall_Far_Idle = true;
	bool m_bCall_Far_Walk_Right = true;
	bool m_bCall_Far_Walk_Left = true;
	bool m_bCall_Far_Walk = true;

	bool m_bCall_Close_Idle = true;
	bool m_bCall_Close_Run = true;
	bool m_bCall_Close_Walk_Back = true;
	bool m_bCall_In_The_Middle_Walk = true;

	/*NonAnim Loop Call*/
	bool m_bCall_Close_Roll_Right = true;
	bool m_bCall_Close_Roll_Left = true;

	bool m_bCall_Close_MultiShot = true; 
	bool m_bCall_Load = true;
	bool m_bCall_Hit = false;
	


	_uint m_iState = 0;
	_bool m_bHeadShot = false;
	
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_PartObjects();

public:
	static CMonster03* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END