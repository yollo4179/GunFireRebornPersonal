#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"

BEGIN(Client)
class CMonster04 final : public CMonster
{

private:
	explicit CMonster04(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster04(const CMonster04& Prototype);
	virtual ~CMonster04() = default;


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
	static CMonster04* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END