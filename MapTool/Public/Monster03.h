#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"


BEGIN(Client)
class CMonster03 final : public CMonster
{


private:
	explicit CMonster03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster03(const CMonster03& Prototype);
	virtual ~CMonster03() = default;


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
	static CMonster03* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END