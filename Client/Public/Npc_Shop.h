#pragma once
#include"Client_Defines.h"
#include"Npc.h"
BEGIN(Client)
class CNpc_Shop  final :public CNpc
{
public:
	typedef struct :public CNpc::NPC_DESC
	{

	}NPC_SHOP_DESC;

private:
	explicit CNpc_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNpc_Shop(const CNpc_Shop& Prototype);
	virtual ~CNpc_Shop() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Activate_Npc()override;
	virtual void Set_Active(_bool isActive); 

private:
	_bool m_isAnimEnd = { false };
	_bool m_isChangingMotion = { false };
	

private:
	HRESULT Ready_Components();
	HRESULT Ready_Particle();

public:
	static   CNpc_Shop* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};END
