#pragma once
#include"ContainerObject.h"
#include"Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
class CNavigation;
class CUIObject;
class CCollider;
END


BEGIN(Client)
class CMonster : public CContainerObject
{
public:
	typedef struct :public CContainerObject::CONTAINEROBJECT_DESC{
		_float4 vPosition;
		_float3 vScale;
		_float fAngle;
		_int iNaviIndex;
		_int MonsterNo;
	}MONSTER_DESC;

protected:
	explicit CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	const _float4x4* Get_BoneMatrix(const _char* pBoneName)const;

protected:
	/*For Util*/
	_bool Util_isPlayer_InRange(_float fRange);
	
	_int  Util_TurnTo_Player(_float fTimeDelta);
	void  Util_Go_Right(_float fTimeDelta , _float fWalkSpeed = 1.f);
	void  Util_Go_Left(_float fTimeDelta, _float fWalkSpeed = 1.f);
	void  Util_Go_Back(_float fTimeDelta, _float fWalkSpeed = 1.f);
	void  Util_Go_Straight(_float fTImeDelta, _float fWalkSpeed = 1.f);
	
	/*For PriUpdate*/
	void Pri_Update_NaviMesh();

	/*For Update_HpBarUpdate*/
	void Pri_Update_UI_HpBarMatrix();
	void Update_Get_ViewZDistance();
	void Update_Get_isFocused();

public:
	void Set_PlayerFloat4x4(const _float4x4* pPlayerFloat4x4);
	CNavigation* Get_Navigation() { return m_pNavigationCom; };

public:
	virtual void Set_Active() { m_bActive = true; };
	virtual void Set_InActive() { m_bActive = false; };
	void Set_Dead() { m_bDead = true; };
	_bool Get_Dead() { return m_bDead; };
	_bool Get_Active() { return m_bActive;};

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	/*AVODING COUPING*/
	CCollider* m_pColliderCom_Range = { nullptr };
	_float4 m_vPrevPushedDir = {};
	_float m_fAccPushingCoolTime = 0.f;
	_bool m_bWaitPusing = { false };
protected:
	/*For Navi*/
	_int m_iInitial_NaviIndex = { 0 };
	_bool m_bCan_GoStraight = {true};
	_bool m_bCan_GoRight = { true };
	_bool m_bCan_GoLeft = { true };

	/* For Test */
	_float m_fAccMotionChangeTime = { 0.f};


	/* For Anim*/
	_uint m_iCurAnimIndex = 0;
	_bool m_isChangingMotion = { false };
	_bool m_isAnimEnd = { false };

	/*For UI*/
	/*For_UI_HP_BAR*/
	_float4x4 m_HPBarMatrix = {};
	_float4x4 m_HPBarSocketMatrix = {};
	 CUIObject* m_pUIHpBar = { nullptr };
	 _float		m_fViewZ_Distance = {};
	 _bool		m_isMonsterPicked = { false };
	 _float     m_fShowHPBarTime = { 1.f };
	 _float		m_fAccShowHPBarTime = { 2.f };

	
	//HP   ->  Blue MaxBlue Red MaxRed
	 _float4 m_HPInfo = {};
	 /* Controlling HP */
	 _bool m_bHeadShot = { false };
	 _float m_fHitTime = { 0.2f };
	 _float m_fAccHitTime = { 0.f };
	 
	 _bool m_bHit_NormalMonster = { false };
	 _float m_fAccShowHPBarTime_GotHit_NormalMonster = { 0.f };
	 _float m_fShowpBarTime_GotHit_NormalMonster = { 2.f };
	


	 /*Dead*/
	 _bool m_bDead = {false};
	 _bool m_bActive = { false };

	 _bool m_isRemoving  = { false };
	 _bool m_bDoneRemoving = { false };
	 _float m_fAccDesolveTime = { 0.f };
	 _float m_fDesolveTime = { 1.5f };
	 CTexture* m_pTextureCom_Desolve = { nullptr };


	 /*Client_Manger */
	 class CItem_Manager* m_pItem_Manager = { nullptr };


public:
	/*For Battle */
	const _float4x4* m_pPlayerFloat4x4 = { nullptr };


protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Desolve();
	HRESULT Bind_ShaderResources_Desolve_Glow();
	HRESULT Ready_UI_HPBar_LEVEL_GAMEPLAY();
protected:
	HRESULT Bind_ShaderResources_For_Collider();

public:
	static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END
