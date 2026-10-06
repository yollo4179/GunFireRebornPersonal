#pragma once
#include"Client_Defines.h"
#include"DropItem.h"

BEGIN(Client)
class CDropItem_Bullet_Standard final : public CDropItem
{
public:
	typedef struct :public CDropItem::DROPITEM_DESC
	{

	}DROPITEM_BULLET_STANDARD_DESC;


private:
	explicit CDropItem_Bullet_Standard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CDropItem_Bullet_Standard(const CDropItem_Bullet_Standard& Prototype);
	virtual ~CDropItem_Bullet_Standard() = default;
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



private:
	HRESULT Ready_Components();
	HRESULT Ready_Particle();

public:
	static   CDropItem_Bullet_Standard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
