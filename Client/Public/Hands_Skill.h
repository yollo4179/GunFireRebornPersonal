#pragma once
#include"Client_Defines.h"
#include"Hands.h"
BEGIN(Engine)
class CShader;
class CModel;
END
BEGIN(Client)
class CHands_Skill final :public CHands
{

public: 
	typedef struct :CPartObject::PARTOBJECT_DESC{
		_uint* pState;
		_int iNowMapStage;
	}HANDS_SKILL_DESC;

private:
	explicit CHands_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CHands_Skill(const CHands_Skill& Prototype);
	virtual ~CHands_Skill() = default;
public:
	const _float4x4* Get_BoneMatrix(const _char* pBoneName)const;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	HRESULT			Init_AnimSpeed();

	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void SetActive(_bool bActiveState);

public:
	void Set_BombRange(_float fBombRange);


private:
	_uint* m_pState = { nullptr };
	_int m_iNowMapStage = {};
	/*Shooting pos ±¸ÇÏ±â  */

	class CHands_Skill_Granade* m_pGranades[3] = {nullptr};
	int m_iNowGranadeIndex = {0};
	const _float4x4* m_pMuzzle_Socket_Matrix = { nullptr };
	_bool  m_bShootSign = false;
	_float m_fAccShootingTiming = 0.f;
	_float m_fShootingTiming = 0.3f;

	_bool m_isAnimEnd = false;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Bullets();

public:
	static CHands_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
