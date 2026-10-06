#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END
BEGIN(Client)
class CNpc abstract : public CGameObject
{

public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		_float4 vPosition;
		_float3 vScale;
		_float fAngle;
		_int iNaviIndex;
		_int NPCNo;

	}NPC_DESC;

	explicit CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNpc(const CNpc& Prototype);
	virtual ~CNpc() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Activate_Npc() = 0;

public:
	/*For Bullet Collision*/
	virtual void Set_Active(_bool isActive) { m_isActive = isActive; };
	virtual void Set_InActive();
	_bool Get_Active() { return m_isActive; };

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	_bool m_isActive = { false };
	/*ForAnim*/
	_bool m_isChangingMotion = { false };

	_uint m_NpcNo = {};
	/* common*/
	_bool m_bActiveTalkBox = { false };
	_bool m_bPlayerInFirst = { false };
	_bool m_bPlayerOutFirst = { false };

	_bool m_isAnimEnd = {false};
protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	virtual void Free()override;
};
END
