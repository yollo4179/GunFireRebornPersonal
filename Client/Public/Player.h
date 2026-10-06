#pragma once
#include"ContainerObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CNavigation;
class CCollider;
END

BEGIN(Client)
/* 플레이어 컨테이너 오브젝트*/
class CPlayer final: public  CContainerObject
{

public:
	typedef struct:public CContainerObject::CONTAINEROBJECT_DESC {

		_float fInitial_Height;
		_float4 InitialPos;
		_uint iNaviMeshIndex;
		_uint iNowMapStage;

	} PLAYER_DESC ;
public:
	enum PARTOBJECT {
		PART_HANDS01,
		PART_HANDS01_SWORD,
		PART_HANDS02,
		PART_HANDS03,
		PART_HANDS04,
		PART_HANDS05,
		PART_HANDS_SKILL,
		PART_EFFECT,
		PART_END};
	enum STATE
	{
		STATE_RESET				= 0X00000000,
		STATE_POP_OUT			= 0X00000001,
		STATE_IDLE				= 0X00000002,
		STATE_RUN				= 0x00000004,
		STATE_WALK				= 0X00000008,
		STATE_ATTACK_PRESSING	= 0X00000010,
		STATE_ATTACK_DOWN		= 0x00000020,
		STATE_ATTACK_UP			= 0x00000040,
		STATE_LOAD				= 0x00000080,
		STATE_DASH				= 0x00000100,
		STATE_GRANADE			= 0x00000200,
		STATE_GRANADE_END		= 0x00000400,
		STATE_JUMP				= 0x00000800,
		STATE_END				= 0x00000200,
	};
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	const _float4x4* Get_PlayerFloat4x4_ptr();

public:
	CCollider* Get_Collider() { return m_pColliderCom; };
	CNavigation* Get_Navigation() { return m_pNavigationCom; };
protected:
	void Init_UIS();
	void Init_UI_TOOL();
	void Init_UI_HPBar();

protected:
	void  Pri_Update_KeyInput(_float fTimeDelta);
	_bool Pri_Update_KeyInput_GoStraight_GoBackward(_float fTimeDelta);
	_bool Pri_Update_KeyInput_GoLeft_GoRight(_float fTimeDelta);
	_bool Pri_Update_KeyInput_Turn_Horize_Vertical(_float fTimeDelta);
	void Pri_Update_KeyInput_ChangeState(_float fTimeDelta);
	void Key_Input_UI();

public:
	_uint* Get_ToolInfo() { return m_MyTools;};
	_uint Get_CurrentTool() { return m_iCurTool; };
	void Set_CurrentTool(_uint iCurTool) { m_iCurTool = iCurTool; };
	void Set_ToolNoByIndex(_uint iIndex, _uint iPlayerToolIndex); 
	_uint Get_ToolNoByIndex(_uint iIndex); 
	void Set_State_PopOut() { m_iState = STATE_POP_OUT; };

public: /* Item and Money*/
	PlayerItemInfo Get_PlayerItemInfo() { return m_ePlayerItemInfo; };
	void Set_PlayerItemInfo(PlayerItemInfo eInfo) { m_ePlayerItemInfo = eInfo; };

	void Set_ExtraMoney(_int iNumExtraCoins) { m_ePlayerItemInfo.iCoins += iNumExtraCoins; };
	void Set_ExtraGrenades(_int iNumExtraGrenades);
	void Set_ExtraBullets(_int iNumExtraBullets) { m_ePlayerItemInfo.iNumBullets += iNumExtraBullets; };


public:
	/*Upgrade*/
	_float4 Get_HPInfo() { return m_vHPInfo; };
	void Set_HPInfo(_float4 vHPInfo) { m_vHPInfo = vHPInfo; };
	_float Get_BombCoolTimeRatio() { return (m_fAccBombCoolTime / m_fBombCoolTime); };
	_float Get_DashCoolTimeRatio() { return (m_fAccDash_CoolTime / m_fDash_CoolTime); };
private:
	_uint			m_iState = {};						//플레이어의 현재 상태이다 .
	_uint			m_iPrevState = {};					//플ㄹ이어의 이전 상태이다 .
	_float			m_fAccAttackTerm = {};
	const _bool*			m_pAnimEnd[PART_END] = {false};						//바디가 애니메이션이 끝나지 않았다면 재생하지 말자. 


/*KeyInput*/
	_float			m_fMouseSensor = {0.1f};			//마우스 감도
	_float			m_fDeceleration = 0.05f;			//감속
	_float			m_fVelocityX =0.f;					//속력
	_float			m_fVelocityZ = 0.f;
	_float			m_fSpeed = 1.f;						//속도

	_float			m_fGravity = { 9.8f };
	_float			m_fCurHeight = { 0.f };
	_float			m_v0 = { 5.f };
	_bool			m_isjumping = { false };

/*GamePlay*/

	_uint		m_iCurTool = { 0 };						//내가 현재 착용하고 있는 총의 인덱스 0,1,2
	_uint		m_MyTools[g_iNumTools] = {};						//내가 현재 가지고 있는 총들의 enum 키값

	_bool			m_bFix = { true };

	/*For UIs*/
	class CUI_Arms* m_pArms = { nullptr };
	class CUI_Player_HP* m_pPlayerHP = { nullptr };
	_bool m_bInvenActive = { false };
	_bool m_bShopActive = { false };
	/* Transporm*/
	_float m_fInitialHeight = { 0.f };

	/* For Navigation */
	CNavigation* m_pNavigationCom = { nullptr };
	_uint m_iNaviMeshIndex{};
	_uint m_iNowMapStage = {};
	/*For Collider*/
	CCollider* m_pColliderCom = { nullptr };

	/*Ray Collider -> For Check Ray */
	class CCollider* m_pColliderCom_CheckRay = { nullptr };


	/*Managing HP*/
	_float4 m_vHPInfo = {}; //->전역 
	_float m_fAccHitTime = {0.f};
	_float m_fHitTime = {0.3f};
	
	/*Player Item Info -money grenades -bullets for reinforce */
	PlayerItemInfo m_ePlayerItemInfo; //-> 전역

	/*Bomb CoolTime*/
	_float m_fBombCoolTime = { 10.f };
	_float m_fAccBombCoolTime = { 10.f };

	/*Dash Cool Time*/
	_float m_fDash_CoolTime = { 3.f };
	_float m_fAccDash_CoolTime = { 3.f };
	_float m_fDashTime = { 0.3f };
	_float m_fAccDashTime = { 0.f };
	_bool m_bDashStart = { false };

	_bool m_bSound_StartDash = { false };
	_bool m_bSound_StartJump = { false };

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;




};
END;

