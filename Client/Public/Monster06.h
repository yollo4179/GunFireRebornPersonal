#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"


BEGIN(Client)
class CMonster06 final : public CMonster
{

private:
	explicit CMonster06(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster06(const CMonster06& Prototype);
	virtual ~CMonster06() = default;


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
	HRESULT Ready_PartObjects();
public:
	static CMonster06* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END