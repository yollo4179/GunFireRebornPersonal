#pragma once
#include"Client_Defines.h"
#include"DropItem.h"

BEGIN(Client)
class CDropItem_Grenade final: public CDropItem
{
public:
	typedef struct :public CDropItem::DROPITEM_DESC
	{

	}DROPITEM_GRENADE_DESC;


private:
	explicit CDropItem_Grenade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CDropItem_Grenade(const CDropItem_Grenade& Prototype);
	virtual ~CDropItem_Grenade() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Activate_DropItem()override;
	
private:
	_bool m_isAnimEnd = { false };
	_bool m_isChangingMotion = { false };
	_float m_fCurHeight = { 0.f };
	_float m_v0 = { 5.f };


private:
	HRESULT Ready_Components();
	HRESULT Ready_Particle();

public:
	static   CDropItem_Grenade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

