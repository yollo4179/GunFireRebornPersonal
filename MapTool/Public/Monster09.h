#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"


BEGIN(Client)
class CMonster09 final : public CMonster
{

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




private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CMonster09* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END