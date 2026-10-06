#pragma once

#include"Client_Defines.h"
#include"Hands.h"
#include"Player.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END


#define CONDITION_IDLE ((CPlayer::STATE_IDLE & *m_pState))
#define CONDITION_LOOP ((CPlayer::STATE_IDLE & *m_pState))
#define CONDITION_LOAD ((CPlayer::STATE_LOAD & *m_pState))
#define CONDITION_ATTACK ((CPlayer::STATE_ATTACK_DOWN & *m_pState) ||(CPlayer::STATE_ATTACK_PRESSING & *m_pState))
#define CONDITION_ATTACK_DOWN (CPlayer::STATE_ATTACK_DOWN & *m_pState)
#define CONDITION_ATTACK_PRESSING (CPlayer::STATE_ATTACK_PRESSING & *m_pState)
#define CONDITION_ATTACK_UP		(CPlayer::STATE_ATTACK_UP&*m_pState)
enum class HANDS01_STATE
{
	STATE_ATTACK01,
	STATE_ATTACK02,
	STATE_ATTACK03,
	STATE_IDLE,
	STATE_POP_OUT,
	STATE_LOAD,
	STATE_END,
};


BEGIN(Client)
class CHands01 final: public CHands
{

public:
	typedef struct : CPartObject::PARTOBJECT_DESC
	{

		_uint* pState;
		vector<CGameObject*>* pPartObjects;
		// 컨테이너(부모, 플레이어)의 상태에따라 이 파츠는 어떻게 움직일지 결정한다 .
	} HANDS01_DESC;

private:
	CHands01(ID3D11Device* pDeviec, ID3D11DeviceContext* pContext);
	CHands01(const CHands01& Prototype);
	virtual ~CHands01() = default;
public:
	// 이 바디가따라갈뼈의 이름을 알고 있어야된다 .( 뼈의 이름은 하드코딩으로 박음)
	const _float4x4* Get_BoneMatrix(const _char* pBoneName)const;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT			Init_Mapping_Index_To_State();
	HRESULT			Init_AnimSpeed();

	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Glow();

public:
	void SetActive(_bool bActiveState);
	_bool Get_AttackChance() { return m_bAttackChance; };
	void Reset_AttackChance() { m_bAttackChance = false; };

public:
	const _bool* Get_PartAnimEndState() { return &m_isAnimEnd; };
private:
	/*CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };*/

private:
	_uint* m_pState = { nullptr };
	
	
	
	/* for Storing States*/
	vector<_uint>m_vecStates;
	/* Ready For Next Anim */
	_bool m_isAnimEnd = { false };
	_bool m_bStartSetUp = { false };


	_uint m_iAttackCount = { 0 };
	_float m_fAttackDuration = { 0.f };
	_uint m_DebugCount = { 0 };


	
	/* for contro;; other part object*/
	vector<CGameObject*>* m_pPartObjects = { nullptr };

	/*ForGlow*/
	CShader* m_pShaderCom_Glow = {nullptr};
	CTexture* m_pTextureCom_Glow = {nullptr};

	_bool m_bAttackChance = { false };
	_bool m_bAttackStart = { false };
	_bool m_bAttackEnd = { false };
	_float m_fAccAttackTime = { 0.f };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();
public:
	static CHands01* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};
END;