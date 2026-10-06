#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"
enum class M9_ANIM
{
	/*IDLE*/
	M9_ANIM_IDLE, //1

	/* Attack */
	M9_ANIM_ATTACK1_SHOT,   //0 :올라와서 물방울 발사
	 

	/*ATTACK_RUSH1*/
	
	M9_ANIM_ATTACK2_RUSH1,//5
	M9_ANIM_ATTACK2_RUSH2,//12
	/*UNDER WATER*/
	M9_ANIM_DIVE_AFTER_RUSH_ATTACK, //13
	

	/*WAVE Attack*/
	M9_ANIM_ATTACK3_WAVE_RIGHT, //14
	M9_ANIM_ATTACK3_WAVE_LEFT, //16
	/*Under Water*/
	M9_ANIM_DIVE_AFTER_ATTACK_WAVE_RIGHT,//_15 OR SHOT
	M9_ANIM_DIVE_AFTER_ATTACK_WAVE_LEFT , //17 OR SHOT

	//Dead,
	M9_ANIM_DEAD1,		//6
	M9_ANIM_DEAD2,		//7
	M9_ANIM_DEAD3,		//8


	//On Battle
	M9_ANIM_POP_OUT1,//0
	M9_ANIM_POP_OUT2,//4
	

	M9_ANIM_END,
};
enum M9_STATE
{
	/*잠수중->잠수중일때만 움직임 , 잠수중일때는 렌더하지 않는다(혹은 ㅁ파동만 렌더한다.)*/
	M9_STATE_SUBMERGE						=0x00000001,
	/*위로 올라옴*/
	M9_STATE_POPOUT				    		=0x00000002,//루프 아니다
	/*처치 , 죽음*/
	M9_STATE_DEAD							=0x00000004,//루프 아니다.
	/*물방울 발사*/
	M9_STATE_ATTACK_SHOT					=0x00000008,//루프 아니다.
	M9_STATE_ATTACK_MULTISHOT				=0x00000010,
	/*돌진*/
	M9_STATE_ATTACK_RUSH					=0x00000020,//루프 아니다
	M9_STATE_DIVE_AFTER_ATTACK				=0x00000040,
	
	/*웨이브*/
	M9_STATE_ATTACK_WAVE_RIGHT				=0x00000080,//루프 아니다
	M9_STATE_ATTACK_WAVE_LEFT				=0X00000100,
	M9_STATE_DIVE_AFTER_WAVE_RIGHT 			=0X00000200,
	M9_STATE_DIVE_AFTER_WAVE_LEFT			=0X00000400,
	M9_STATE_ATTACK_WAVE					=0x00000800, //메시지큐에 넣기 위한 용도
	/*루프 - idle*/
	M9_STATE_IDLE							=0x00001000,
	M9_STATE_LOOP							=0X00002000,
};
enum M9_DIRECTION
{
	M9_DIRECTION_LEFT,
	M9_DIRECTION_RIGHT,
	M9_DIRECTION_END,
};

/*Condition NON lOOP*/
#define CONDITION_POPOUT  (m_iState &M9_STATE::M9_STATE_POPOUT)
#define CONDITION_DEAD (m_iState& M9_STATE::M9_STATE_DEAD)

#define CONDITION_ATTACK_SHOT (m_iState& M9_STATE::M9_STATE_ATTACK_SHOT)
#define CONDITION_ATTACK_MULTISHOT (m_iState& M9_STATE::M9_STATE_ATTACK_MULTISHOT)
#define CONDITION_ATTACK_RUSH (m_iState& M9_STATE::M9_STATE_ATTACK_RUSH)
#define CONDITION_ATTACK_WAVE_LEFT (m_iState& M9_STATE::M9_STATE_ATTACK_WAVE_LEFT)
#define CONDITION_ATTACK_WAVE_RIGHT (m_iState& M9_STATE::M9_STATE_ATTACK_WAVE_RIGHT)
#define CONDITION_ATTACK_WAVE (CONDITION_ATTACK_WAVE_LEFT||CONDITION_ATTACK_WAVE_RIGHT)

#define CONDITION_DIVE_AFTER_ATTACK (m_iState& M9_STATE::M9_STATE_DIVE_AFTER_ATTACK)
#define CONDITION_DIVE_AFTER_WAVE_LEFT (m_iState& M9_STATE::M9_STATE_DIVE_AFTER_WAVE_LEFT)
#define CONDITION_DIVE_AFTER_WAVE_RIGHT (m_iState& M9_STATE::M9_STATE_DIVE_AFTER_WAVE_RIGHT)
#define CONDITION_DIVE_AFTER_WAVE (CONDITION_DIVE_AFTER_WAVE_LEFT||CONDITION_DIVE_AFTER_WAVE_RIGHT)

#define CONDITION_ATTACK (CONDITION_ATTACK_SHOT ||CONDITION_ATTACK_MULTISHOT|| CONDITION_ATTACK_RUSH||CONDITION_ATTACK_WAVE||CONDITION_DIVE_AFTER_WAVE||CONDITION_DIVE_AFTER_ATTACK)
#define CONDITION_NON_LOOP (CONDITION_ATTACK|| CONDITION_POPOUT||CONDITION_DEAD)

/*CONDITION LOOP*/
#define CONDITION_SUBMERGE    (m_iState& M9_STATE::M9_STATE_SUBMERGE)
#define CONDITION_IDLE		  (m_iState& M9_STATE::M9_STATE_IDLE)
#define CONDITION_STATE_LOOP	(m_iState& M9_STATE::M9_STATE_LOOP)

#define CONDITION_LOOP (CONDITION_SUBMERGE|| CONDITION_IDLE||CONDITION_STATE_LOOP)




/*Condition  Transition loop*/



/* For Loop */

/*For Non Loop*/
/* For Attack */

/*For Non Loop*/
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CMonster09 final : public CMonster
{
public:
	typedef struct :public CMonster::MONSTER_DESC
	{
		vector<CMonster09*>* pMonster09s;
		_float2* pBossHPInfo;
		_bool* pDead;
		_int iSplattingIndex;
	}MONSTER09_DESC;

public:
	enum PARTOBJECT
	{
		PART_HEAD,
		PART_BODY,
		PART_END
	};
public:
	enum TEXTURE_TYPE { TEXTURE_TYPE_MASK, TEXTURE_TYPE_BRUSH, TEXTURE_TYPE_MONING_BRUSH, TEXTURE_TYPE_END};
	enum SKILL_TYPE { SKILL_TYPE_RUSH, SKILL_TYPE_SHOT, SKILL_TYPE_END };
private:
	explicit CMonster09(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster09(const CMonster09& Prototype);
	virtual ~CMonster09() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Glow();
private:
	HRESULT Mapping_Anim_States();
	void Pri_Update_Cancle_LoopState();
	void Pri_Update_LoopCall_State();
	void Pri_Update_Non_LoopCall_State();

public: 
	
	/*안겹치게 하기위한 장치-> 나올지 안나올지 결정 짓는 요소1 */
	_bool Get_Submerge_State() { return CONDITION_SUBMERGE; };
	_bool Get_Rush_State() { return CONDITION_ATTACK_RUSH; }
	_bool Can_PopOut();


	/*Render RushSign*/
	HRESULT Render_RushSign();

	void CutSceneMoving();
	void CutSceneStart();

	virtual void Set_Active()override;
public:
	_matrix Get_RushMatrix();
private:
	vector< _uint> m_vecAnimStates;
	//M9_STATE m_eCurState = M9_STATE::M9_STATE_FAR_IDLE;


private:

	/*Ranges*/
	_float m_fBattleRange = { 40.f };

	/*_float m_fFarRange = { 40.f };
	_float m_fMiddleRange = { 25.f };
	_float m_fRush_SlashRange = { 7.f };
	_float m_fBattleRange = { 4.f };*/
	
	/*AccTimes 충돌시 구현하자*/
	
	/*지속시간& 모션 제한시간 -idle*/
	_float m_fAcc_Idle_Time = { 0.f };
	_float m_fRandom_Idle_Duration = { 0.f };
	_float m_fAcc_Submerge_Time = { 0.f };
	_float m_fRandom_Submerge_Duration = { 0.f };

	/*현재 쿨타임 상황 & 쿨타임 제한   -공격 */
	
	_float m_fAcc_Shot_Time = { 0.f };
	_float m_fAcc_Shot_Timing = { 0.f };
	_float m_fShot_Cool_Time = { 3.f };

	
	_float m_fAcc_Rush_Time = { 0.f };
	_float m_fAcc_Rush_Timing = { 0.f };
	_float m_fRush_Cool_Time = { 3.f };
	_bool m_bRushStart = false;

	_float m_fAcc_Wave_Time = { 0.f };
	_float m_fAcc_Wave_Timing = { 0.f };
	_float m_fWave_Cool_Time = { 3.f };

	queue<M9_STATE>Skill_Order_Queue; //비어있으면 서브머지

	

	/*m_bCall 은 애니메이션 제어를 위해 -한번만 호출-> 애니메이션끝났을때 다음으로 넘어감 */
	/*fAccTime은-> 쿨타임  루프call 에서 일정 시간동안 유지할때 쓰고 , */
	/*fAcc_StartTime은 데미지 타이밍 잡을때 -> 일정 시간일때만 콜라이더 불변수로 활성화*/

	/*loop Call*/
	_bool m_bCall_Submerge = {true};
	_bool m_bCall_Idle = { true };

	/*non loop call*/
	_bool m_bCall_Popout = { true };

	/*Attack*/
	_bool m_bCall_Attack_Shot = { true };
	
	_bool m_bCall_Attack_Rush= { true };

	_bool m_bCall_Attack_Wave_Left= { true };
	_bool m_bCall_Attack_Wave_Right = { true };
	/*SubMerge*/
	_bool m_bCall_Dive_After_Wave_Left = { true };
	_bool m_bCall_Dive_After_Wave_Right = { true };
	_bool m_bCall_Dive_After_Attack = { true };

	/*Dead*/
	_bool m_bCall_Dead = { true };

	_uint m_iState = 0;
	
	/*For Battle Domain*/
	_float m_fDomainMinX=-30.f;
	_float m_fDomainMaxX =30.f;
	_float3 m_vInitPosition = {};
	_float m_fDomainMinZ = -10.f;
	_float m_fDomainMaxZ = 20.f;
	/*CurPos ByDomain*/
	_float3 m_vNowPosition = {};
	_uint m_iNowDir = {};

	/*Boss HP*/
	_float2* m_pBossHPInfo = { nullptr };
	_float* m_pHitTime = { nullptr };
	_float* m_pAccHitTime = { nullptr };
	_bool* m_pDead = { nullptr };
private:
	vector<class CMonster09_Wave*> m_vecAttackWaves;
	_uint m_iNowWaveIndex = {0};
	_bool m_bWaveSign = {true};
	/*For Moving->  머리끼리 안겹치게 하기위한 로직- 플레이어와 내 머리 사이에 나와 있는 머리가 있으면 안된다- submerge시간이 끝났어도*/
	vector<class CMonster09*>* m_pMonster09s;


	CTexture* m_pTextureCom[SKILL_TYPE_END][TEXTURE_TYPE_END] = {};
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom_VtxPos = { nullptr };
	CShader* m_pTerrainShaderCom = { nullptr };
	_int m_iSplattingIndex{};
	_float m_fAccMovingTextureLifeTime = 0.f;


	/*MonsterEffect*/
	CTexture* m_pTextureCom_Emissive_Eye = { nullptr };
	CTexture* m_pTextureCom_Emissive_Body = { nullptr };
	CShader* m_pShaderCom_Monster09 = { nullptr };

	class CParticle_Monster09_Attack* m_pParticle_Attack = { nullptr };
	class CParticle_Monster09_Moving* m_pParticle_Moving = {nullptr};
	_float m_fAccParticleTiming = {};

	class CEffect_Monster09_Moving* m_pEffect_Moving = { nullptr };

	class CMonster09_Rush* pMonster09_Rush = { nullptr };


	_bool m_bStartCutScene = { false };
	_bool m_isCutSceneNow = { false };



	/*Sound*/
	_float m_fAccCutScene_RoarStart = { 0.f };
	_bool m_bRoarStart_Sound = { true };


	_float m_fAccShoutTime_Sound = { 0.f };
	_bool m_bShoutStart_Sound = { true };

	_float m_fAccVisibleTime = 0.f;
	_float m_fVisibleTime = 2.f;
	_bool m_bVisible = false;

	_int iPreCondition = M9_STATE_SUBMERGE;
	
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();
	HRESULT Ready_PartObjects();
	HRESULT Ready_Attack_Waves();
	HRESULT Ready_Attack_Rush();

	HRESULT Ready_Particles();
public:
	static CMonster09* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END