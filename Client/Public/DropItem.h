#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END
BEGIN(Client)
class CDropItem abstract: public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		_uint iNowMapStage;
	}DROPITEM_DESC;

protected:
	explicit CDropItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CDropItem(const CDropItem& Prototype);
	virtual ~CDropItem() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	virtual void Activate_DropItem()=0;

public:
	/*For Bullet Collision*/
	void Set_Active(_bool isActive) { m_isActive = isActive; };
	_bool Get_Active() { return m_isActive; };
	virtual void Reset_Item(_fmatrix matInitialItemPos ,CNavigation* pMonsterNavigation);
protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

	_bool m_isActive = { false };

	/*ForAnim*/
	_bool m_isChangingMotion = { false };

	_float m_fGravity = { 9.8f };
	_float m_fCurHeight = { 100.f };
	_float m_v0 = { 0.001f };
	_bool m_bOnFround = { false };
	
	_uint m_iNowMapStage = {};


protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	virtual void Free()override;


};
END
