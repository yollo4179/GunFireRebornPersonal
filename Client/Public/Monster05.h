#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"

enum M5_ANIM
{
	/*Close_Range && Far_Range*/
	M5_ANIM_BATTLE_IDLE,  //0
	/*Close_Range*/
	M5_ANIM_CLOSE_FIRE, //15
	M5_ANIM_CLOSE_WALK_BACK,//11	
	/*Middle_Range*/
	M5_ANIM_MIDDLLE_RUN,//3
	/*Close_Range && Middle_Range*/
	M5_ANIM_ROLL_RIGHT,//13
	M5_ANIM_ROLL_LEFT,//14
	/*Far_Range*/
	M5_ANIM_FAR_WALK,//9
	M5_ANIM_FAR_WALK_LEFT,//12
	M5_ANIM_FAR_WALK_RIGHT,//10
	M5_ANIM_FAR_SNIPE,//5
	MS_ANIM_FAR_IDLE,//4
	/*All_RANGE*/
	M5_ANIM_HIT,//6
	M5_ANIM_LOAD,//16
	M5_ANIM_END

};
enum M5_STATE
{
	/*Close_Range && Far_Range*/
	M5_STATE_BATTLE_IDLE		=0x00000001,  
	/*Close_Range*/
	M5_STATE_CLOSE_FIRE			=0x00000002,
	M5_STATE_CLOSE_WALK_BACK	=0x00000004,
	/*Middle_Range*/
	M5_STATE_MIDDLE_RUN			=0x00000008,
	/*Close_Range && Middle_Range*/
	M5_STATE_ROLL_RIGHT			=0x00000010,
	M5_STATE_ROLL_LEFT			=0x00000020,
	/*Far_Range*/
	M5_STATE_FAR_WALK			=0x00000040,
	M5_STATE_FAR_WALK_LEFT		=0x00000080,
	M5_STATE_FAR_WALK_RIGHT		=0x00000100,
	M5_STATE_FAR_SNIPE			=0x00000200,
	M5_STATE_FAR_IDLE			=0x00000400,
	/*All_RANGE*/
	M5_STATE_HIT				=0x00000800,
	M5_STATE_LOAD				=0x00001000,
	M5_STATE_LOOP				=0x00002000,
};
/*CONDITION_LOOP*/
#define CONDITION_BATTLE_IDLE (M5_STATE::M5_STATE_BATTLE_IDLE &m_iState )
	/*Close_Range*/
#define CONDITION_CLOSE_WALK_BACK (M5_STATE::M5_STATE_CLOSE_WALK_BACK & m_iState)
	/*Middle_Range*/
#define CONDITION_MIDDLE_RUN (M5_STATE::M5_STATE_MIDDLE_RUN & m_iState)
	/*Far-Range*/
#define CONDITION_FAR_WALK (M5_STATE::M5_STATE_FAR_WALK & m_iState)
#define CONDITION_FAR_WALK_LEFT (M5_STATE::M5_STATE_FAR_WALK_LEFT & m_iState)
#define CONDITION_FAR_WALK_RIGHT (M5_STATE::M5_STATE_FAR_WALK_RIGHT & m_iState)
#define CONDITION_FAR_IDLE (M5_STATE::M5_STATE_FAR_IDLE & m_iState)
#define CONDITION_FAR_LOOP (CONDITION_FAR_WALK||CONDITION_FAR_WALK_LEFT||CONDITION_FAR_WALK_RIGHT||CONDITION_FAR_IDLE)
	/*IS LOOP*/
#define CONDITION_STATE_LOOP (M5_STATE::M5_STATE_LOOP & m_iState)
#define CONDITION_LOOP (CONDITION_FAR_LOOP||CONDITION_BATTLE_IDLE||CONDITION_CLOSE_WALK_BACK||CONDITION_MIDDLE_RUN||CONDITION_STATE_LOOP)
/*NON_LOOP*/
#define CONDITION_ROLL_LEFT (M5_STATE::M5_STATE_ROLL_LEFT & m_iState)
#define CONDITION_ROLL_RIGHT (M5_STATE::M5_STATE_ROLL_RIGHT & m_iState)
#define CONDITION_ROLL (CONDITION_ROLL_LEFT|| CONDITION_ROLL_RIGHT)

#define CONDITION_LOAD (M5_STATE::M5_STATE_LOAD & m_iState)
#define CONDITION_HIT (M5_STATE::M5_STATE_HIT & m_iState)

#define CONDITION_CLOSE_FIRE (M5_STATE::M5_STATE_CLOSE_FIRE & m_iState)
#define CONDITION_FAR_SNIPE (M5_STATE::M5_STATE_FAR_SNIPE & m_iState)
#define CONDITION_ATTACK (CONDITION_CLOSE_FIRE|| CONDITION_FAR_SNIPE)
#define CONDITION_NON_LOOP (CONDITION_ATTACK||CONDITION_LOAD||CONDITION_HIT||CONDITION_ROLL)


BEGIN(Client)
class CMonster05 final : public CMonster
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
	explicit CMonster05(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster05(const CMonster05& Prototype);
	virtual ~CMonster05() = default;


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
	M5_STATE m_eCurStae = M5_STATE_FAR_IDLE;

	/*Ranges*/
	_float m_fFarRange = 40.f;
	_float m_fMiddleRange = 25.f;
	_float m_fCloseRange = 10.f;
	/*accTime*/
	/*moving time*/
	_float m_fAcc_Far_Walk_Left_Time = 0.f;
	_float m_fAcc_Far_Walk_Right_Time = 0.f;
	_float m_fAcc_Far_Walk_Time = 0.f;
	_float m_fAcc_Middle_Run_Time = 0.f;
	_float m_fAcc_Close_Walk_Back_Time = 0.f;
	
	_float m_fAcc_Random_WalkTime = 0.f;
	_float m_fAcc_Battle_Idle_Time = 0.f;
	_float m_fAcc_StartRollTime = 0.f;


	_float m_fAcc_Snipe_Time = 0.f;				//쿨타임
	_float m_fAcc_Snipe_Duration = 3.f;			//지속시간
	_float m_fAcc_Snipe_TrackPosition = 0.f;   //현재 컬린시간
	_float m_fAcc_CloseFire_Time = 0.f;			//쿨타임





	/*BulletCount*/
	_uint m_iNumBullets = 5.f;
	_bool m_bSniperShootSign = false;


	/*LoopCall*/
	bool m_bCall_Far_Idle = true;
	bool m_bCall_Far_Walk = true;
	bool m_bCall_Far_Walk_Left = true;
	bool m_bCall_Far_Walk_Right = true;
	bool m_bCall_Close_Walk_Back = true;
	bool m_bCall_Battle_Idle = true;
	bool m_bCall_Middle_Run = true;

	/*NonLoopCall*/
	bool m_bCall_Roll_Right = true;
	bool m_bCall_Roll_Left = true;
	bool m_bCall_Far_Snipe = true;//얘는 따로뺴서 관리
	bool m_bCall_Load = true;
	bool m_bCall_Hit = true;


	/*Hit*/
	_bool m_bHeadShot = false;
	_uint m_iState = 0;
	
private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Bind_ShaderResources();

public:
	static CMonster05* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END